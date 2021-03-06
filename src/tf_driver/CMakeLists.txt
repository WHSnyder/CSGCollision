#Tests for basic TF models w/C++ & Python APIs

cmake_minimum_required(VERSION 3.2)
project(tf_tests)

if (BUILDTEST)

	add_library(test_TFCPP STATIC)

	target_sources(test_TFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/StyleTransfer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/cpp_tf_test.cpp)

	target_include_directories(test_TFCPP PUBLIC ${OpenCV_INCLUDE_DIRS})

	target_link_directories(test_TFCPP PUBLIC ${OpenCV_LIBRARY_DIRS})
	target_link_libraries(test_TFCPP PUBLIC ${OpenCV_LIBRARIES})
	target_link_libraries(test_TFCPP PUBLIC -lpthread)

	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/tensorflow/lite/tools/make/downloads/flatbuffers/include)
	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl)
	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/tensorflow/lite/tools/make/downloads/absl)
	target_include_directories(test_TFCPP PUBLIC ${CD}/src/tf_driver)

	#target_link_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite/c)

	#target_link_directories(test_TFCPP PUBLIC /Users/will/projects/unity_tfl/Assets/TensorFlowLite/Plugins/macOS)
	#target_link_libraries(test_TFCPP PUBLIC tensorflowlite_c)

	target_link_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite)
	target_link_libraries(test_TFCPP PUBLIC tensorflowlite)

	if (APPLE)
		target_link_libraries(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/tensorflow_lite_gpu_dylib.dylib)

		#target_link_libraries(test_TFCPP PUBLIC /Users/will/projects/unity_tfl/Assets/TensorFlowLite/Plugins/macOS/libtensorflowlite_metal_delegate.dylib)

		target_link_options(test_TFCPP PUBLIC -undefined dynamic_lookup)
	else()
		target_link_libraries(test_TFCPP PUBLIC -ldl)
		target_link_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu)
		target_link_libraries(test_TFCPP PUBLIC tensorflowlite_gpu_delegate)
		target_link_libraries(test_TFCPP PUBLIC "-Wl,--no-as-needed")
	endif()
endif()