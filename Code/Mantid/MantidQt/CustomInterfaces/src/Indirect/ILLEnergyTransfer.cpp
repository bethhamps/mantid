#include "MantidQtCustomInterfaces/Indirect/ILLEnergyTransfer.h"

#include "MantidQtCustomInterfaces/Background.h"
#include "MantidQtCustomInterfaces/UserInputValidator.h"

#include <QFileInfo>
#include <QInputDialog>

using namespace Mantid::API;

namespace MantidQt
{
namespace CustomInterfaces
{
  //----------------------------------------------------------------------------------------------
  /** Constructor
   */
  ILLEnergyTransfer::ILLEnergyTransfer(IndirectDataReduction * idrUI, QWidget * parent) :
      IndirectDataReductionTab(idrUI, parent)
  {
    m_uiForm.setupUi(parent);

    connect(this, SIGNAL(newInstrumentConfiguration()), this, SLOT(setInstrumentDefault()));
    connect(m_batchAlgoRunner, SIGNAL(batchComplete(bool)), this, SLOT(algorithmComplete(bool)));

    // Validate to remove invalid markers
    validateTab();
  }


  //----------------------------------------------------------------------------------------------
  /** Destructor
   */
  ILLEnergyTransfer::~ILLEnergyTransfer()
  {
  }


  void ILLEnergyTransfer::setup()
  {
  }


  bool ILLEnergyTransfer::validate()
  {
    UserInputValidator uiv;

    // Validate run file
    if(!m_uiForm.rfInput->isValid())
      uiv.addErrorMessage("Run File is invalid.");

    // Validate map file if it is being used
    bool useMapFile = m_uiForm.cbGroupingType->currentText() == "Map File";
    if(useMapFile && !m_uiForm.rfInput->isValid())
      uiv.addErrorMessage("Map File is invalid.");

    // Show error message for errors
    if(!uiv.isAllInputValid())
      showMessageBox(uiv.generateErrorMessage());

    return uiv.isAllInputValid();
  }


  void ILLEnergyTransfer::run()
  {
    IAlgorithm_sptr reductionAlg = AlgorithmManager::Instance().create("IndirectILLReduction");
    reductionAlg->initialize();

    reductionAlg->setProperty("Analyser", getInstrumentConfiguration()->getAnalyserName().toStdString());
    reductionAlg->setProperty("Reflection", getInstrumentConfiguration()->getReflectionName().toStdString());

    // Handle einput files
    QString filename = m_uiForm.rfInput->getFirstFilename();
    reductionAlg->setProperty("Run", filename.toStdString());

    // Handle mapping file
    bool useMapFile = m_uiForm.cbGroupingType->currentText() == "Map File";
    if(useMapFile)
    {
      QString mapFilename = m_uiForm.rfMapFile->getFirstFilename();
      reductionAlg->setProperty("MapFile", mapFilename.toStdString());
    }

    // Set mirror mode option
    bool mirrorMode = m_uiForm.ckMirrorMode->isChecked();
    reductionAlg->setProperty("MirrorMode", mirrorMode);

    // Set left and right workspaces when using mirror mode
    if(mirrorMode)
    {
      reductionAlg->setProperty("LeftWorkspace", "l"); //TODO
      reductionAlg->setProperty("RightWorkspace", "r"); //TODO
    }

    // Set output workspace properties
    reductionAlg->setProperty("RawWorkspace", "raw"); //TODO
    reductionAlg->setProperty("ReducedWorkspace", "red"); //TODO

    // Set output options
    reductionAlg->setProperty("Plot", m_uiForm.ckPlot->isChecked());
    reductionAlg->setProperty("Save", m_uiForm.ckSave->isChecked());

    m_batchAlgoRunner->addAlgorithm(reductionAlg);
    m_batchAlgoRunner->executeBatchAsync();
  }


  /**
   * Handles completion of the algorithm.
   *
   * @param error True if the algorithm was stopped due to error, false otherwise
   */
  void ILLEnergyTransfer::algorithmComplete(bool error)
  {
    if(error)
      return;

    // Nothing to do here
  }


  /**
   * Called when the instrument has changed, used to update default values.
   */
  void ILLEnergyTransfer::setInstrumentDefault()
  {
    std::map<QString, QString> instDetails = getInstrumentDetails();

    // Set instrument in run file widgets
    m_uiForm.rfInput->setInstrumentOverride(instDetails["instrument"]);
    m_uiForm.rfMapFile->setInstrumentOverride(instDetails["instrument"]);
  }


} // namespace CustomInterfaces
} // namespace Mantid
