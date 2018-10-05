#! /usr/bin/python
# Mantid Repository : https://github.com/mantidproject/mantid
#
# Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
#     NScD Oak Ridge National Laboratory, European Spallation Source
#     & Institut Laue - Langevin
# SPDX - License - Identifier: GPL - 3.0 +
#----------------------------------------------------------------
# Creates a new sub-class to be used as
# a custom interface
# Usage: python CreateInterfaceTemplate algorithm-name [yes|no]
#
# yes - Create the class so that it can be used with Qt designer
# no  - Create the class so that the layout is created by hand
#
# Author: Martyn Gigg, Tessella plc
#---------------------------------------------------------------
import sys
import string

# Slice off the script name
args = sys.argv[1:]

if len(args) != 2 :
    print "Usage: CreateInterfaceTemplate interface_name yes|no\n" \
    "\tyes - Create the class so that it can be used with Qt designer\n"\
    "\tno  - Create the class so that the layout must be created by hand"
    exit(1);

ifacename = args[0]
if args[1] == "yes":
    designer = True
else:
    designer = False


headerfile = open("inc/" + ifacename + ".h", 'w')

# header file
headerfile.write("#ifndef MANTIDQTCUSTOMINTERFACES_" + string.upper(ifacename) + "_H_\n"
                 "#define MANTIDQTCUSTOMINTERFACES_" + string.upper(ifacename) + "_H_\n\n"
                 "//----------------------\n"
                 "// Includes\n"
                 "//----------------------\n")
if designer == True:
    headerfile.write("#include \"MantidQtCustomInterfaces/ui_" + ifacename + ".h\"\n")

headerfile.write("#include \"MantidQtAPI/UserSubWindow.h\"\n\n"
                 "namespace MantidQt\n"
                 "{\n"
                 "namespace CustomInterfaces\n"
                 "{\n"
                 "class " + ifacename + " : public MantidQt::API::UserSubWindow\n"
                 "{\n"
                 "  Q_OBJECT\n\n"
                 "public:\n"
                 "  /// The name of the interface as registered into the factory\n"
                 "  static std::string name() { return \"" + ifacename + "\"; }\n\n"
                 "public:\n"
                 "  /// Default Constructor\n"
                 + "  " + ifacename + "(QWidget *parent = 0);\n\n"
                 "private:\n"
                 "  /// Initialize the layout\n"
                 "  virtual void initLayout();\n\n")
if designer == True:
    headerfile.write("private:\n"
                     "  //The form generated by Qt Designer\n"
                     "  Ui::" + ifacename + " m_uiForm;\n\n")
headerfile.write("};\n\n"
                 "}\n"
                 "}\n\n"
                 "#endif //MANTIDQTCUSTOMINTERFACES_" + string.upper(ifacename) + "_H_\n")

headerfile.close()

#source code
sourcefile = open("src/" + ifacename+".cpp", 'w')

sourcefile.write("//----------------------\n"
                 "// Includes\n"
                 "//----------------------\n"
                 "#include \"MantidQtCustomInterfaces/" + ifacename + ".h\"\n\n"
                 "//Add this class to the list of specialised dialogs in this namespace\n"
                 "namespace MantidQt\n"
                 "{\n"
                 "namespace CustomInterfaces\n"
                 "{\n"
                 "  DECLARE_SUBWINDOW(" + ifacename + ");\n"
                 "}\n"
                 "}\n\n"
                 "using namespace MantidQt::CustomInterfaces;\n\n"
                 "//----------------------\n"
                 "// Public member functions\n"
                 "//----------------------\n"
                 "///Constructor\n"
                 + ifacename + "::" + ifacename + "(QWidget *parent) :\n"
                 "  UserSubWindow(parent)\n"
                 "{\n"
                 "}\n\n"
                 "/// Set up the dialog layout\n"
                 "void " + ifacename + "::initLayout()\n"
                 "{\n")
if designer == True:
    sourcefile.write("  m_uiForm.setupUi(this);\n");

sourcefile.write("}\n\n")
