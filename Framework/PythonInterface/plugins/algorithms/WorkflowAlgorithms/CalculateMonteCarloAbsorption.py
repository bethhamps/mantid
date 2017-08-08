from __future__ import (absolute_import, division, print_function)
import mantid.simpleapi as s_api
from mantid.api import (DataProcessorAlgorithm, AlgorithmFactory, PropertyMode, MatrixWorkspaceProperty,
                        WorkspaceGroupProperty, InstrumentValidator, WorkspaceUnitValidator, Progress)
from mantid.kernel import (VisibleWhenProperty, PropertyCriterion, StringListValidator, StringMandatoryValidator, IntBoundedValidator,
                           FloatBoundedValidator, Direction, logger, CompositeValidator)


class CalculateMonteCarloAbsorption(DataProcessorAlgorithm):
    # General variables
    _beam_height = None
    _beam_width = None
    _unit = None
    _emode = None
    _efixed = None
    _number_wavelengths = None
    _events = None

    # Sample variables
    _sample_ws_name = None
    _sample_chemical_formula = None
    _sample_density_type = None
    _sample_density = None

    # Container variables
    _can_ws_name = None
    _can_chemical_formula = None
    _can_density_type = None
    _can_density = None

    # Output workspaces
    _ass_ws = None
    _acc_ws = None
    _output_ws = None

    def category(self):
        return "Workflow\\Inelastic;CorrectionFunctions\\AbsorptionCorrections;Workflow\\MIDAS"

    def summary(self):
        return "Calculates indirect absorption corrections for a given sample shape."

    def PyInit(self):
        # General properties

        self.declareProperty(name='BeamHeight', defaultValue=1.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Height of the beam (cm)')
        self.declareProperty(name='BeamWidth', defaultValue=1.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Width of the beam (cm)')

        self.setPropertyGroup('BeamHeight', 'Beam Options')
        self.setPropertyGroup('BeamWidth', 'Beam Options')

        # Monte Carlo options
        self.declareProperty(name='NumberOfWavelengthPoints', defaultValue=10,
                             validator=IntBoundedValidator(1),
                             doc='Number of wavelengths for calculation')
        self.declareProperty(name='EventsPerPoint', defaultValue=1000,
                             validator=IntBoundedValidator(0),
                             doc='Number of neutron events')
        self.declareProperty(name='Interpolation', defaultValue='Linear',
                             validator=StringListValidator(
                                 ['Linear', 'CSpline']),
                             doc='Type of interpolation')

        self.setPropertyGroup('NumberOfWavelengthPoints', 'Monte Carlo Options')
        self.setPropertyGroup('EventsPerPoint', 'Monte Carlo Options')
        self.setPropertyGroup('Interpolation', 'Monte Carlo Options')

        # Sample options
        self.declareProperty(MatrixWorkspaceProperty('SampleWorkspace', '', direction=Direction.Input),
                             doc='Sample Workspace')
        self.declareProperty(name='SampleChemicalFormula', defaultValue='',
                             doc='Chemical formula for the sample material')
        self.declareProperty(name='SampleDensityType', defaultValue='Mass Density',
                             validator=StringListValidator(['Mass Density', 'Number Density']),
                             doc='Sample density type')
        self.declareProperty(name='SampleDensity', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Sample density')

        self.setPropertyGroup('SampleWorkspace', 'Sample Options')
        self.setPropertyGroup('SampleChemicalFormula', 'Sample Options')
        self.setPropertyGroup('SampleDensityType', 'Sample Options')
        self.setPropertyGroup('SampleDensity', 'Sample Options')

        # Container options
        self.declareProperty(MatrixWorkspaceProperty('ContainerWorkspace', '', direction=Direction.Input),
                             doc='Container Workspace')
        self.declareProperty(name='ContainerChemicalFormula', defaultValue='',
                             doc='Chemical formula for the container material')
        self.declareProperty(name='ContainerDensityType', defaultValue='Mass Density',
                             validator=StringListValidator(['Mass Density', 'Number Density']),
                             doc='Container density type')
        self.declareProperty(name='ContainerDensity', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Container density')

        self.setPropertyGroup('ContainerWorkspace', 'Container Options')
        self.setPropertyGroup('ContainerChemicalFormula', 'Container Options')
        self.setPropertyGroup('ContainerDensityType', 'Container Options')
        self.setPropertyGroup('ContainerDensity', 'Container Options')

        # Shape options
        self.declareProperty(name='Shape', defaultValue='FlatPlate', validator=StringListValidator(['FlatPlate', 'Cylinder', 'Annulus']),
                             doc='Geometric shape of the sample environment')

        flatPlateCondition = VisibleWhenProperty('Shape', PropertyCriterion.IsEqualTo, 'FlatPlate')
        cylinderCondition = VisibleWhenProperty('Shape', PropertyCriterion.IsEqualTo, 'Cylinder')
        annulusCondition = VisibleWhenProperty('Shape', PropertyCriterion.IsEqualTo, 'Annulus')

        # height is common to all, and should be the same for sample and container
        self.declareProperty('Height', defaultValue=0.0, validator=FloatBoundedValidator(0.0),
                             doc='Height of the sample environment (cm)')

        self.setPropertyGroup('Shape', 'Shape Options')
        self.setPropertyGroup('Height', 'Shape Options')

        # ---------------------------Sample---------------------------
        # Flat Plate
        self.declareProperty(name='SampleWidth', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Width of the sample environment (cm)')
        self.declareProperty(name='SampleThickness', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Thickness of the sample environment (cm)')
        self.declareProperty(name='SampleCenter', defaultValue=0.0,
                             doc='Center of the sample environment')
        self.declareProperty(name='SampleAngle', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Angle of the sample environment with respect to the beam (degrees)')

        self.setPropertySettings('SampleWidth', flatPlateCondition)
        self.setPropertySettings('SampleThickness', flatPlateCondition)
        self.setPropertySettings('SampleCenter', flatPlateCondition)
        self.setPropertySettings('SampleAngle', flatPlateCondition)

        self.setPropertyGroup('SampleWidth', 'Sample Shape Options')
        self.setPropertyGroup('SampleThickness', 'Sample Shape Options')
        self.setPropertyGroup('SampleCenter', 'Sample Shape Options')
        self.setPropertyGroup('SampleAngle', 'Sample Shape Options')

        # Cylinder
        self.declareProperty(name='SampleRadius', defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Radius of the sample environment (cm)')

        self.setPropertySettings('SampleRadius', cylinderCondition)
        self.setPropertyGroup('SampleRadius', 'Sample Shape Options')

        # Annulus
        self.declareProperty(name='SampleInnerRadius',defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Inner radius of the sample environment (cm)')
        self.declareProperty(name='SampleOuterRadius',defaultValue=0.0,
                             validator=FloatBoundedValidator(0.0),
                             doc='Outer radius of the sample environment (cm)')

        self.setPropertySettings('SampleInnerRadius', annulusCondition)
        self.setPropertySettings('SampleOuterRadius', annulusCondition)

        self.setPropertyGroup('SampleInnerRadius', 'Sample Shape Options')
        self.setPropertyGroup('SampleOuterRadius', 'Sample Shape Options')




    def pyExec(self):
        pass


# Register algorithm with Mantid
AlgorithmFactory.subscribe(CalculateMonteCarloAbsorption)
