#include "EnggDiffGSASFittingPresenter.h"
#include "EnggDiffGSASRefinementMethod.h"
#include "MantidQtWidgets/LegacyQwt/QwtHelper.h"

namespace MantidQt {
namespace CustomInterfaces {

EnggDiffGSASFittingPresenter::EnggDiffGSASFittingPresenter(
    std::unique_ptr<IEnggDiffGSASFittingModel> model,
    IEnggDiffGSASFittingView *view,
    boost::shared_ptr<IEnggDiffMultiRunFittingWidgetPresenter> multiRunWidget)
    : m_model(std::move(model)), m_multiRunWidget(multiRunWidget), m_view(view),
      m_viewHasClosed(false) {}

EnggDiffGSASFittingPresenter::~EnggDiffGSASFittingPresenter() {}

void EnggDiffGSASFittingPresenter::notify(
    IEnggDiffGSASFittingPresenter::Notification notif) {

  if (m_viewHasClosed) {
    return;
  }

  switch (notif) {

  case IEnggDiffGSASFittingPresenter::DoRefinement:
    processDoRefinement();
    break;

  case IEnggDiffGSASFittingPresenter::LoadRun:
    processLoadRun();
    break;

  case IEnggDiffGSASFittingPresenter::SelectRun:
    processSelectRun();
    break;

  case IEnggDiffGSASFittingPresenter::Start:
    processStart();
    break;

  case IEnggDiffGSASFittingPresenter::ShutDown:
    processShutDown();
    break;
  }
}

GSASIIRefineFitPeaksParameters
EnggDiffGSASFittingPresenter::collectInputParameters(
    const RunLabel &runLabel, const Mantid::API::MatrixWorkspace_sptr inputWS,
    const GSASRefinementMethod refinementMethod) const {
  const auto instParamFile = m_view->getInstrumentFileName();
  const auto phaseFiles = m_view->getPhaseFileNames();
  const auto pathToGSASII = m_view->getPathToGSASII();
  const auto GSASIIProjectFile = m_view->getGSASIIProjectPath();

  const auto dMin = m_view->getPawleyDMin();
  const auto negativeWeight = m_view->getPawleyNegativeWeight();
  const auto xMin = m_view->getXMin();
  const auto xMax = m_view->getXMax();
  const auto refineSigma = m_view->getRefineSigma();
  const auto refineGamma = m_view->getRefineGamma();

  return GSASIIRefineFitPeaksParameters(inputWS, runLabel, refinementMethod,
                                        instParamFile, phaseFiles, pathToGSASII,
                                        GSASIIProjectFile, dMin, negativeWeight,
                                        xMin, xMax, refineSigma, refineGamma);
}

void EnggDiffGSASFittingPresenter::displayFitResults(const RunLabel &runLabel) {
  const auto latticeParams = m_model->getLatticeParams(runLabel);
  const auto rwp = m_model->getRwp(runLabel);

  if (!latticeParams || !rwp) {
    m_view->userError("Invalid run identifier",
                      "Unexpectedly tried to display fit results for invalid "
                      "run, run number = " +
                          std::to_string(runLabel.runNumber) + ", bank ID = " +
                          std::to_string(runLabel.bank) +
                          ". Please contact the development team");
    return;
  }

  m_view->displayLatticeParams(*latticeParams);
  m_view->displayRwp(*rwp);
}

Mantid::API::MatrixWorkspace_sptr
EnggDiffGSASFittingPresenter::doPawleyRefinement(
    const GSASIIRefineFitPeaksParameters &params) {
  return m_model->doPawleyRefinement(params);
}

Mantid::API::MatrixWorkspace_sptr
EnggDiffGSASFittingPresenter::doRietveldRefinement(
    const GSASIIRefineFitPeaksParameters &params) {
  return m_model->doRietveldRefinement(params);
}

void EnggDiffGSASFittingPresenter::processDoRefinement() {
  if (!m_multiRunWidget->hasSelectedRunLabel()) {
    m_view->userWarning("No run selected",
                        "Please select a run to do refinement on");
    return;
  }
  const auto runLabel = m_multiRunWidget->getSelectedRunLabel();
  const auto inputWSOptional = m_multiRunWidget->getFocusedRun(runLabel);

  if (!inputWSOptional) {
    m_view->userError(
        "Invalid run selected for refinement",
        "Tried to run refinement on invalid focused run, run number " +
            std::to_string(runLabel.runNumber) + " and bank ID " +
            std::to_string(runLabel.bank) +
            ". Please contact the development team with this message");
    return;
  }

  const auto refinementMethod = m_view->getRefinementMethod();
  const auto refinementParams =
      collectInputParameters(runLabel, *inputWSOptional, refinementMethod);

  try {
    Mantid::API::MatrixWorkspace_sptr fittedPeaks;

    switch (refinementMethod) {

    case GSASRefinementMethod::PAWLEY:
      fittedPeaks = doPawleyRefinement(refinementParams);
      break;

    case GSASRefinementMethod::RIETVELD:
      fittedPeaks = doRietveldRefinement(refinementParams);
      break;
    }

    m_multiRunWidget->addFittedPeaks(runLabel, fittedPeaks);
  } catch (const std::runtime_error &ex) {
    m_view->userError("Refinement failed", ex.what());
  }
}

void EnggDiffGSASFittingPresenter::processLoadRun() {
  const auto focusedFileNames = m_view->getFocusedFileNames();

  try {
    for (const auto fileName : focusedFileNames) {
      const auto focusedRun = m_model->loadFocusedRun(fileName);
      m_multiRunWidget->addFocusedRun(focusedRun);
    }
  } catch (const std::exception &ex) {
    m_view->userWarning("Could not load file", ex.what());
  }
}

void EnggDiffGSASFittingPresenter::processSelectRun() {
  if (m_multiRunWidget->showFitResultsSelected()) {
    const auto runLabel = m_multiRunWidget->getSelectedRunLabel();
    const auto rwp = m_model->getRwp(runLabel);
    const auto latticeParams = m_model->getLatticeParams(runLabel);

    if (!rwp || !latticeParams) {
      m_view->userError(
          "Invalid run label",
          "Tried to display fit results for run number " +
              std::to_string(runLabel.runNumber) + " and bank ID " +
              std::to_string(runLabel.bank) +
              ". Please contact the development team with this message");
      return;
    }
    m_view->displayRwp(*rwp);
    m_view->displayLatticeParams(*latticeParams);
  }
}

void EnggDiffGSASFittingPresenter::processStart() {
  auto addMultiRunWidget = m_multiRunWidget->getWidgetAdder();
  (*addMultiRunWidget)(*m_view);
}

void EnggDiffGSASFittingPresenter::processShutDown() { m_viewHasClosed = true; }

} // MantidQt
} // CustomInterfaces
