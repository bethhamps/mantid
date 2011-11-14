#ifndef DIMENSIONSLICEWIDGET_H
#define DIMENSIONSLICEWIDGET_H

#include <QtGui/QWidget>
#include "ui_DimensionSliceWidget.h"
#include "MantidGeometry/MDGeometry/IMDDimension.h"
#include "DllOption.h"

namespace MantidQt
{
namespace SliceViewer
{


/** Widget for the 2D slice viewer. Select whether the dimension
 * is X or Y, or if it is not one to be shown, where is the
 * slice.
 * Shows the dimension name and units
 *
 * @author Janik Zikovsky
 * @date Oct 3, 2011
 */

class EXPORT_OPT_MANTIDQT_SLICEVIEWER DimensionSliceWidget : public QWidget
{
  Q_OBJECT

//  /** Enum for each shown dimension */
//  enum eShownDim
//  {
//    None=-1,
//    X=0,
//    Y=1
//  };


public:
  DimensionSliceWidget(QWidget *parent = 0);
  ~DimensionSliceWidget();

  void setDimension(int index, Mantid::Geometry::IMDDimension_const_sptr dim);
  void setMinMax(double min, double max);
  void setShownDim(int dim);

  double getSlicePoint() const
  { return m_slicePoint; }

  /// @return the shown dimension, 0=X, 1=Y, -1=None
  int getShownDim() const
  { return m_shownDim; }


public slots:
  void sliderMoved();
  void btnXYChanged();
  void spinBoxChanged();

signals:
  void changedShownDim(int index, int dim, int oldDim);
  void changedSlicePoint(int index, double value);

public:
  /// Auto-gen UI class
  Ui::DimensionSliceWidgetClass ui;

private:
  /// Sptr to the dimensions being displayed
  Mantid::Geometry::IMDDimension_const_sptr m_dim;

  /// The index of the dimension into the workspace
  int m_dimIndex;

  /// Which dimension is being shown. -1 = None, 0 = X, 1 = Y. 2+ reserved for higher dimensions
  int m_shownDim;

  /// If the dimensions is not shown, where is the slice point?
  double m_slicePoint;

  bool m_insideSetShownDim;
  bool m_insideSpinBoxChanged;
};

} // namespace SliceViewer
} // namespace Mantid

#endif // DIMENSIONSLICEWIDGET_H
