# testhelpers Python module

set(PY_FILES
    __init__.py
    algorithm_decorator.py
    illhelpers.py
    tempfile_wrapper.py
    testrunner.py
    mlzhelpers.py)

# Copy python files to output directory
set(OUTPUT_DIR
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/testhelpers)

# Copy the files for the testhelpers to the build directory
copy_files_to_dir("${PY_FILES}"
                  ${CMAKE_CURRENT_SOURCE_DIR}
                  ${OUTPUT_DIR}
                  PYTHON_INSTALL_FILES)

# Create the targets for this directory
set(FRAMEWORK_DIR ../../..)
include_directories(${FRAMEWORK_DIR}/CurveFitting/inc
                    ${FRAMEWORK_DIR}/DataHandling/inc
                    ${FRAMEWORK_DIR}/DataObjects/inc
                    ${FRAMEWORK_DIR}/Nexus/inc
                    ${FRAMEWORK_DIR}/TestHelpers/inc)

# WorkspaceCreationHelper
set(SRC_FILES
    WorkspaceCreationHelperModule.cpp
    ${FRAMEWORK_DIR}/TestHelpers/src/ComponentCreationHelper.cpp
    ${FRAMEWORK_DIR}/TestHelpers/src/MDEventsTestHelper.cpp
    ${FRAMEWORK_DIR}/TestHelpers/src/InstrumentCreationHelper.cpp
    ${FRAMEWORK_DIR}/TestHelpers/src/WorkspaceCreationHelper.cpp)
add_library(PythonWorkspaceCreationHelper
            ${SRC_FILES}
            ${INC_FILES}
            ${PYTHON_INSTALL_FILES})
set_python_properties(PythonWorkspaceCreationHelper WorkspaceCreationHelper)

# Override folder
set_property(TARGET PythonWorkspaceCreationHelper
             PROPERTY FOLDER "UnitTests/Python")
target_link_libraries(PythonWorkspaceCreationHelper
                      LINK_PRIVATE
                      ${TCMALLOC_LIBRARIES_LINKTIME}
                      PythonAPIModule
                      DataObjects
                      DataHandling
                      API
                      HistogramData
                      Indexing
                      Types
                      Kernel
                      Geometry
                      ${PYTHON_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${POCO_LIBRARIES})

# Overall testhelpers target
add_custom_target(testhelpers DEPENDS PythonWorkspaceCreationHelper)
add_dependencies(FrameworkTests testhelpers)
set_property(TARGET testhelpers PROPERTY FOLDER "UnitTests/Python")
