"""*WIKI* 
    Compute I(q) for reduced SANS data
*WIKI*"""
from mantid.api import *
from mantid.kernel import *
import math

class SANSAzimuthalAverage1D(PythonAlgorithm):

    def category(self):
        return "Workflow\\SANS;PythonAlgorithms"

    def name(self):
        return "SANSAzimuthalAverage1D"
    
    def PyInit(self):
        self.declareProperty(MatrixWorkspaceProperty("InputWorkspace", "", 
                                                     direction=Direction.Input))

        self.declareProperty(FloatArrayProperty("Binning", values=[0.,0.,0.],
                             direction=Direction.Input), "Positive is linear bins, negative is logorithmic")
        
        self.declareProperty("NumberOfBins", 100, validator=IntBoundedValidator(lower=1),
                             doc="Number of Q bins to use if binning is not supplied")
        self.declareProperty("LogBinning", False, "Produce log binning in Q when true and binning wasn't supplied")
        self.declareProperty("NumberOfSubpixels", 1, "Number of sub-pixels per side of a detector pixel: use with care")
        self.declareProperty("ErrorWeighting", False, "Backward compatibility option: use with care")
        
        self.declareProperty("ReductionProperties", "__sans_reduction_properties", 
                             validator=StringMandatoryValidator(),
                             doc="Property manager name for the reduction")

        self.declareProperty(MatrixWorkspaceProperty("OutputWorkspace", "", 
                                                     direction = Direction.Output),
                             "I(q) workspace")
        self.declareProperty("OutputMessage", "", 
                             direction=Direction.Output, doc = "Output message")
        
    def PyExec(self):
        # Warn user if error-weighting was turned on
        error_weighting = self.getProperty("ErrorWeighting").value
        if error_weighting:
            msg = "The ErrorWeighting option is turned ON. "
            msg += "This option is NOT RECOMMENDED"
            Logger.get("SANSAzimuthalAverage").warning(msg)
            
        # Warn against sub-pixels
        n_subpix = self.getProperty("NumberOfSubpixels").value
        if n_subpix != 1:
            msg = "NumberOfSubpixels was set to %s: " % str(n_subpix)
            msg += "The recommended value is 1"
            Logger.get("SANSAzimuthalAverage").warning(msg)
        
        # Q binning options
        binning = self.getProperty("Binning").value
                
        input_ws_name = self.getPropertyValue("InputWorkspace")
        output_ws_name = self.getPropertyValue("OutputWorkspace")

        if not AnalysisDataService.doesExist(input_ws_name):
            Logger.get("SANSAzimuthalAverage").error("Could not find input workspace")
        workspace = AnalysisDataService.retrieve(input_ws_name)
        
            
        # Q range                        
        pixel_size_x = workspace.getInstrument().getNumberParameter("x-pixel-size")[0]
        pixel_size_y = workspace.getInstrument().getNumberParameter("y-pixel-size")[0]
        
        if len(binning)==0 or (binning[0]==0 and binning[1]==0 and binning[2]==0):
            # Wavelength. Read in the wavelength bins. Skip the first one which is not set up properly for EQ-SANS
            x = workspace.dataX(1)
            x_length = len(x)
            if x_length < 2:
                raise RuntimeError, "Azimuthal averaging expects at least one wavelength bin"
            wavelength_max = (x[x_length-2]+x[x_length-1])/2.0
            wavelength_min = (x[0]+x[1])/2.0
            if wavelength_min==0 or wavelength_max==0:
                raise RuntimeError, "Azimuthal averaging needs positive wavelengths"
            qmin, qstep, qmax = self._get_binning(workspace, wavelength_min, wavelength_max)
            binning = [qmin, qstep, qmax]
        else:
            qmin = binning[0]
            qmax = binning[2]
            
        #output_ws_name = "%_Iq" % output_ws_name

        # If we kept the events this far, we need to convert the input workspace
        # to a histogram here
        if workspace.id()=="EventWorkspace":
            input_workspace = '__'+input_ws_name
            alg = AlgorithmManager.create("ConvertToMatrixWorkspace")
            alg.initialize()
            alg.setPropertyValue("InputWorkspace", input_ws_name)
            alg.setPropertyValue("OutputWorkspace", input_ws_name) 
            alg.execute()
            
        alg = AlgorithmManager.create("Q1DWeighted")
        alg.initialize()
        alg.setPropertyValue("InputWorkspace", input_ws_name)
        alg.setProperty("OutputBinning", binning)
        alg.setProperty("NPixelDivision", n_subpix) 
        alg.setProperty("PixelSizeX", pixel_size_x) 
        alg.setProperty("PixelSizeY", pixel_size_y) 
        alg.setProperty("ErrorWeighting", error_weighting) 
        alg.setPropertyValue("OutputWorkspace", output_ws_name) 
        alg.execute()
        
        alg = AlgorithmManager.create("ReplaceSpecialValues")
        alg.initialize()
        alg.setPropertyValue("InputWorkspace", output_ws_name)
        alg.setPropertyValue("OutputWorkspace", output_ws_name)
        alg.setProperty("NaNValue", 0.0)
        alg.setProperty("NaNError", 0.0)
        alg.setProperty("InfinityValue", 0.0)
        alg.setProperty("InfinityError", 0.0)
        alg.execute()

        # Q resolution
        #if reducer._resolution_calculator is not None:
        #    reducer._resolution_calculator(InputWorkspace=output_ws, 
        #                                   OutputWorkspace=output_ws)
            
        # Add output workspace to the list of important output workspaces
        #reducer.output_workspaces.append(output_ws)
        
        msg = "Performed radial averaging between Q=%g and Q=%g" % (qmin, qmax)
        self.setProperty("OutputMessage", msg)        
        self.setPropertyValue("OutputWorkspace", output_ws_name)


    def _get_binning(self, workspace, wavelength_min, wavelength_max):    
        log_binning = self.getProperty("LogBinning").value
        nbins = self.getProperty("NumberOfBins").value
        beam_ctr_x = workspace.getRun().getProperty("beam_center_x").value
        beam_ctr_y = workspace.getRun().getProperty("beam_center_y").value
        sample_detector_distance = workspace.getRun().getProperty("sample_detector_distance").value
        nx_pixels = int(workspace.getInstrument().getNumberParameter("number-of-x-pixels")[0])
        ny_pixels = int(workspace.getInstrument().getNumberParameter("number-of-y-pixels")[0])
        pixel_size_x = workspace.getInstrument().getNumberParameter("x-pixel-size")[0]
        pixel_size_y = workspace.getInstrument().getNumberParameter("y-pixel-size")[0]

        # Q min is one pixel from the center, unless we have the beam trap size
        if workspace.getRun().hasProperty("beam-trap-diameter"):
            mindist = workspace.getRun().getProperty("beam-trap-diameter").value/2.0
        else:
            mindist = min(pixel_size_x, pixel_size_y)
        qmin = 4*math.pi/wavelength_max*math.sin(0.5*math.atan(mindist/sample_detector_distance))
        
        dxmax = pixel_size_x*max(beam_ctr_x,nx_pixels-beam_ctr_x)
        dymax = pixel_size_y*max(beam_ctr_y,ny_pixels-beam_ctr_y)
        maxdist = math.sqrt(dxmax*dxmax+dymax*dymax)
        qmax = 4*math.pi/wavelength_min*math.sin(0.5*math.atan(maxdist/sample_detector_distance))
        
        if not log_binning:
            qstep = (qmax-qmin)/nbins
            f_step = (qmax-qmin)/qstep
            n_step = math.floor(f_step)
            if f_step-n_step>10e-10:
                qmax = qmin+qstep*n_step
            return qmin, qstep, qmax
        else:
            # Note: the log binning in Mantid is x_i+1 = x_i * ( 1 + dx )
            qstep = (math.log10(qmax)-math.log10(qmin))/nbins
            f_step = (math.log10(qmax)-math.log10(qmin))/qstep
            n_step = math.floor(f_step)
            if f_step-n_step>10e-10:
                qmax = math.pow(10.0, math.log10(qmin)+qstep*n_step)
            return qmin, -(math.pow(10.0,qstep)-1.0), qmax
#############################################################################################

registerAlgorithm(SANSAzimuthalAverage1D)
