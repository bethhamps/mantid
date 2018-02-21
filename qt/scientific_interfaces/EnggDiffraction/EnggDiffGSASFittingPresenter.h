#ifndef MANTIDQTCUSTOMINTERFACES_ENGGDIFFRACTION_ENGGDIFFGSASFITTINGPRESENTER_H_
#define MANTIDQTCUSTOMINTERFACES_ENGGDIFFRACTION_ENGGDIFFGSASFITTINGPRESENTER_H_

#include "DllConfig.h"
#include "IEnggDiffGSASFittingModel.h"
#include "IEnggDiffGSASFittingPresenter.h"
#include "IEnggDiffGSASFittingView.h"

#include "MantidAPI/MatrixWorkspace_fwd.h"

#include <memory>

namespace MantidQt {
namespace CustomInterfaces {

// needs to be dll-exported for the tests
class MANTIDQT_ENGGDIFFRACTION_DLL EnggDiffGSASFittingPresenter
    : public IEnggDiffGSASFittingPresenter {

public:
  EnggDiffGSASFittingPresenter(std::unique_ptr<IEnggDiffGSASFittingModel> model,
                               IEnggDiffGSASFittingView *view);

  EnggDiffGSASFittingPresenter(EnggDiffGSASFittingPresenter &&other);

  EnggDiffGSASFittingPresenter &operator=(EnggDiffGSASFittingPresenter &&other);

  ~EnggDiffGSASFittingPresenter() override;

  void notify(IEnggDiffGSASFittingPresenter::Notification notif) override;

private:
  void processDoRefinement();
  void processLoadRun();
  void processSelectRun();
  void processShutDown();
  void processStart();

  /**
   Perform a Pawley refinement on a run
   @param runLabel Run number and bank ID of the workspace to refine
   @param instParamFile The instrument parameter file name (.prm) to use for
   refinement
   @param phaseFiles Vector of file paths to phases to use in refinement
   @param pathToGSASII Location of the directory containing GSASIIscriptable.py
   (and GSAS-II executables)
   @param GSASIIProjectFile Location to save the .gpx project to
   @return String describing any failures, empty optional if refinement was
   successful
   */
  boost::optional<std::string>
  doPawleyRefinement(const RunLabel &runLabel, const std::string &instParamFile,
                     const std::vector<std::string> &phaseFiles,
                     const std::string &pathToGSASII,
                     const std::string &GSASIIProjectFile);

  /**
   Perform a Rietveld refinement on a run
   @param runLabel Run number and bank ID of the workspace to refine
   @param instParamFile The instrument parameter file name (.prm) to use for
   refinement
   @param phaseFiles Vector of file paths to phases to use in refinement
   @param pathToGSASII Location of the directory containing GSASIIscriptable.py
   (and GSAS-II executables)
   @param GSASIIProjectFile Location to save the .gpx project to
   @return String describing any failures, empty optional if refinement was
   successful
   */
  boost::optional<std::string> doRietveldRefinement(
      const RunLabel &runLabel, const std::string &instParamFile,
      const std::vector<std::string> &phaseFiles,
      const std::string &pathToGSASII, const std::string &GSASIIProjectFile);

  /**
   Overplot fitted peaks for a run, and display lattice parameters and Rwp in
   the view
   @param runLabel Run number and bank ID of the run to display
  */
  void displayFitResults(const RunLabel &runLabel);

  /**
   Update the view with the data from a run, and refinement results if they are
   available and the user has selected to show them
   @param runLabel Run number and bank ID of the run
  */
  void updatePlot(const RunLabel &runLabel);

  std::unique_ptr<IEnggDiffGSASFittingModel> m_model;

  IEnggDiffGSASFittingView *m_view;

  bool m_viewHasClosed;
};

} // namespace CustomInterfaces
} // namespace MantidQt

#endif // MANTIDQTCUSTOMINTERFACES_ENGGDIFFRACTION_ENGGDIFFGSASFITTINGPRESENTER_H_
