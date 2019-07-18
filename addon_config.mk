# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file
# system
#
# The PG can be used to detect errors in this file, just create a new project with this addon
# and the PG will write to the console the kind of error and in which line it is

meta:
ADDON_NAME = ofxOpenCvDnnObjectDetection
ADDON_DESCRIPTION = OpenCV DNN Module for Object Detection addon. YOLO, SSD, R-CNN implementation for Openframeworks.
ADDON_AUTHOR = Tetsuaki Baba
ADDON_TAGS = "computer vision" "opencv" "image processing"  "darknet" "yolov2" "object detection" "classification"
ADDON_URL = https://github.com/TetsuakiBaba

common:
ADDON_INCLUDES  += libs/opencv2.framework
ADDON_FRAMEWORKS += OpenCL

#ADDON_INCLUDES += /opt/intel/openvino/opencv/include
#ADDON_LDFLAGS += /opt/intel/openvino/opencv/lib/libopencv_core.4.1.0.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/opencv/lib/libopencv_imgproc.4.1.0.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/opencv/lib/libopencv_dnn.4.1.0.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/opencv/lib/libopencv_highgui.4.1.0.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/opencv/lib/libopencv_imgcodecs.4.1.0.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libinference_engine.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libcpu_extension.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libMKLDNNPlugin.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libMKLDNNPlugind.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libcpu_extensiond.dylib
#ADDON_LDFLAGS += /opt/intel/openvino/inference_engine/lib/intel64/libinference_engined.dylib
#ADDON_FRAMEWORKS += OpenCL
