# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui/inelastic/dgs_sample_setup.ui'
#
# Created: Mon Jul 23 16:41:16 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Frame(object):
    def setupUi(self, Frame):
        Frame.setObjectName(_fromUtf8("Frame"))
        Frame.resize(1020, 483)
        Frame.setFrameShape(QtGui.QFrame.StyledPanel)
        Frame.setFrameShadow(QtGui.QFrame.Raised)
        self.verticalLayout_2 = QtGui.QVBoxLayout(Frame)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.sample_label = QtGui.QLabel(Frame)
        self.sample_label.setMinimumSize(QtCore.QSize(110, 0))
        self.sample_label.setObjectName(_fromUtf8("sample_label"))
        self.horizontalLayout.addWidget(self.sample_label)
        self.sample_edit = QtGui.QLineEdit(Frame)
        self.sample_edit.setObjectName(_fromUtf8("sample_edit"))
        self.horizontalLayout.addWidget(self.sample_edit)
        self.sample_browse = QtGui.QPushButton(Frame)
        self.sample_browse.setObjectName(_fromUtf8("sample_browse"))
        self.horizontalLayout.addWidget(self.sample_browse)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.ei_guess_label = QtGui.QLabel(Frame)
        self.ei_guess_label.setMinimumSize(QtCore.QSize(110, 0))
        self.ei_guess_label.setObjectName(_fromUtf8("ei_guess_label"))
        self.horizontalLayout_2.addWidget(self.ei_guess_label)
        self.ei_guess_edit = QtGui.QLineEdit(Frame)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ei_guess_edit.sizePolicy().hasHeightForWidth())
        self.ei_guess_edit.setSizePolicy(sizePolicy)
        self.ei_guess_edit.setObjectName(_fromUtf8("ei_guess_edit"))
        self.horizontalLayout_2.addWidget(self.ei_guess_edit)
        self.ei_units_label = QtGui.QLabel(Frame)
        self.ei_units_label.setObjectName(_fromUtf8("ei_units_label"))
        self.horizontalLayout_2.addWidget(self.ei_units_label)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.use_ei_guess_chkbox = QtGui.QCheckBox(Frame)
        self.use_ei_guess_chkbox.setObjectName(_fromUtf8("use_ei_guess_chkbox"))
        self.horizontalLayout_2.addWidget(self.use_ei_guess_chkbox)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem2)
        self.verticalLayout_2.addLayout(self.horizontalLayout_2)
        self.et_range_box = QtGui.QGroupBox(Frame)
        self.et_range_box.setCheckable(True)
        self.et_range_box.setChecked(False)
        self.et_range_box.setObjectName(_fromUtf8("et_range_box"))
        self.verticalLayout = QtGui.QVBoxLayout(self.et_range_box)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.etr_low_label = QtGui.QLabel(self.et_range_box)
        self.etr_low_label.setMinimumSize(QtCore.QSize(93, 0))
        self.etr_low_label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.etr_low_label.setObjectName(_fromUtf8("etr_low_label"))
        self.horizontalLayout_6.addWidget(self.etr_low_label)
        self.etr_low_edit = QtGui.QLineEdit(self.et_range_box)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.etr_low_edit.sizePolicy().hasHeightForWidth())
        self.etr_low_edit.setSizePolicy(sizePolicy)
        self.etr_low_edit.setObjectName(_fromUtf8("etr_low_edit"))
        self.horizontalLayout_6.addWidget(self.etr_low_edit)
        self.etr_width_label = QtGui.QLabel(self.et_range_box)
        self.etr_width_label.setObjectName(_fromUtf8("etr_width_label"))
        self.horizontalLayout_6.addWidget(self.etr_width_label)
        self.etr_width_edit = QtGui.QLineEdit(self.et_range_box)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.etr_width_edit.sizePolicy().hasHeightForWidth())
        self.etr_width_edit.setSizePolicy(sizePolicy)
        self.etr_width_edit.setObjectName(_fromUtf8("etr_width_edit"))
        self.horizontalLayout_6.addWidget(self.etr_width_edit)
        self.etr_high_label = QtGui.QLabel(self.et_range_box)
        self.etr_high_label.setObjectName(_fromUtf8("etr_high_label"))
        self.horizontalLayout_6.addWidget(self.etr_high_label)
        self.etr_high_edit = QtGui.QLineEdit(self.et_range_box)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.etr_high_edit.sizePolicy().hasHeightForWidth())
        self.etr_high_edit.setSizePolicy(sizePolicy)
        self.etr_high_edit.setObjectName(_fromUtf8("etr_high_edit"))
        self.horizontalLayout_6.addWidget(self.etr_high_edit)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_6.addItem(spacerItem3)
        self.verticalLayout.addLayout(self.horizontalLayout_6)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem4)
        self.et_is_distribution_cb = QtGui.QCheckBox(self.et_range_box)
        self.et_is_distribution_cb.setChecked(True)
        self.et_is_distribution_cb.setObjectName(_fromUtf8("et_is_distribution_cb"))
        self.horizontalLayout_3.addWidget(self.et_is_distribution_cb)
        spacerItem5 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_3.addItem(spacerItem5)
        self.verticalLayout.addLayout(self.horizontalLayout_3)
        self.verticalLayout_2.addWidget(self.et_range_box)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.hardmask_label = QtGui.QLabel(Frame)
        self.hardmask_label.setMinimumSize(QtCore.QSize(110, 0))
        self.hardmask_label.setObjectName(_fromUtf8("hardmask_label"))
        self.horizontalLayout_4.addWidget(self.hardmask_label)
        self.hardmask_edit = QtGui.QLineEdit(Frame)
        self.hardmask_edit.setObjectName(_fromUtf8("hardmask_edit"))
        self.horizontalLayout_4.addWidget(self.hardmask_edit)
        self.hardmask_browse = QtGui.QPushButton(Frame)
        self.hardmask_browse.setObjectName(_fromUtf8("hardmask_browse"))
        self.horizontalLayout_4.addWidget(self.hardmask_browse)
        spacerItem6 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem6)
        self.verticalLayout_2.addLayout(self.horizontalLayout_4)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.grouping_label = QtGui.QLabel(Frame)
        self.grouping_label.setMinimumSize(QtCore.QSize(110, 0))
        self.grouping_label.setObjectName(_fromUtf8("grouping_label"))
        self.horizontalLayout_5.addWidget(self.grouping_label)
        self.grouping_edit = QtGui.QLineEdit(Frame)
        self.grouping_edit.setObjectName(_fromUtf8("grouping_edit"))
        self.horizontalLayout_5.addWidget(self.grouping_edit)
        self.grouping_browse = QtGui.QPushButton(Frame)
        self.grouping_browse.setObjectName(_fromUtf8("grouping_browse"))
        self.horizontalLayout_5.addWidget(self.grouping_browse)
        spacerItem7 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_5.addItem(spacerItem7)
        self.verticalLayout_2.addLayout(self.horizontalLayout_5)
        spacerItem8 = QtGui.QSpacerItem(20, 242, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_2.addItem(spacerItem8)

        self.retranslateUi(Frame)
        QtCore.QMetaObject.connectSlotsByName(Frame)

    def retranslateUi(self, Frame):
        Frame.setWindowTitle(QtGui.QApplication.translate("Frame", "Frame", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_label.setText(QtGui.QApplication.translate("Frame", "Sample Data", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_browse.setText(QtGui.QApplication.translate("Frame", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.ei_guess_label.setText(QtGui.QApplication.translate("Frame", "Incident Energy Guess", None, QtGui.QApplication.UnicodeUTF8))
        self.ei_units_label.setText(QtGui.QApplication.translate("Frame", "meV", None, QtGui.QApplication.UnicodeUTF8))
        self.use_ei_guess_chkbox.setText(QtGui.QApplication.translate("Frame", "Use Guess (No Calculation)", None, QtGui.QApplication.UnicodeUTF8))
        self.et_range_box.setTitle(QtGui.QApplication.translate("Frame", "Energy Transfer Range (meV)", None, QtGui.QApplication.UnicodeUTF8))
        self.etr_low_label.setText(QtGui.QApplication.translate("Frame", "Low", None, QtGui.QApplication.UnicodeUTF8))
        self.etr_width_label.setText(QtGui.QApplication.translate("Frame", "Width", None, QtGui.QApplication.UnicodeUTF8))
        self.etr_high_label.setText(QtGui.QApplication.translate("Frame", "High", None, QtGui.QApplication.UnicodeUTF8))
        self.et_is_distribution_cb.setText(QtGui.QApplication.translate("Frame", "S(Phi, E) is distribution", None, QtGui.QApplication.UnicodeUTF8))
        self.hardmask_label.setText(QtGui.QApplication.translate("Frame", "Hard Mask", None, QtGui.QApplication.UnicodeUTF8))
        self.hardmask_browse.setText(QtGui.QApplication.translate("Frame", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.grouping_label.setText(QtGui.QApplication.translate("Frame", "Grouping", None, QtGui.QApplication.UnicodeUTF8))
        self.grouping_browse.setText(QtGui.QApplication.translate("Frame", "Browse", None, QtGui.QApplication.UnicodeUTF8))

