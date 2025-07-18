# @author Patrick Bos, NL eScience Center, 2019-2022

add_library(RooFit_multiprocess_testing_utils INTERFACE)
target_link_libraries(RooFit_multiprocess_testing_utils INTERFACE RooFitCore RooBatchCompute)
target_include_directories(RooFit_multiprocess_testing_utils INTERFACE ${RooFitMultiProcess_INCLUDE_DIR})

ROOT_ADD_GTEST(test_RooFit_MultiProcess_Job test_Job.cxx LIBRARIES RooFitMultiProcess Core)
# link to the INTERFACE library separately, ROOT_EXECUTABLE cannot handle INTERFACE library properties:
target_link_libraries(test_RooFit_MultiProcess_Job PUBLIC RooFit_multiprocess_testing_utils)

ROOT_ADD_GTEST(test_RooFit_MultiProcess_ProcessManager test_ProcessManager.cxx LIBRARIES RooFitMultiProcess)
ROOT_ADD_GTEST(test_RooFit_MultiProcess_Messenger test_Messenger.cxx LIBRARIES RooFitMultiProcess)

ROOT_ADD_GTEST(test_RooFit_MultiProcess_Queue test_Queue.cxx LIBRARIES RooFitMultiProcess)
#nlohmann is only a private dependency of RF_MP, but this test includes a header in res/
ROOT_ADD_GTEST(test_RooFit_MultiProcess_ProcessTimer test_ProcessTimer.cxx LIBRARIES RooFitMultiProcess nlohmann_json::nlohmann_json)
ROOT_ADD_GTEST(test_RooFit_MultiProcess_HeatmapAnalyzer test_HeatmapAnalyzer.cxx LIBRARIES RooFitMultiProcess
  COPY_TO_BUILDDIR ${CMAKE_CURRENT_SOURCE_DIR}/test_logs/p_0.json
  COPY_TO_BUILDDIR ${CMAKE_CURRENT_SOURCE_DIR}/test_logs/p_1.json
  COPY_TO_BUILDDIR ${CMAKE_CURRENT_SOURCE_DIR}/test_logs/p_999.json)
