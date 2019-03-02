#!/usr/bin/env bash
set -euxo pipefail
path_to_cfg="https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov2-tiny.cfg"
path_to_weights="https://pjreddie.com/media/files/yolov2-tiny.weights"
if wget ${path_to_weights}; then
	echo "downloading weights"
else
	echo "wget is not installed. Falling back to curl"
	curl -O "${path_to_weights}"
fi
cp ./yolov2-tiny.weights ./Examples/camera/bin/data/yolov2-tiny.weights
cp ./yolov2-tiny.weights ./Examples/single_image/bin/data
cp ./yolov2-tiny.weights ./Examples/annotation/bin/data
if wget ${path_to_cfg}; then
	echo "downloading cfg"
else
	echo "wget is not installed. Falling back to curl"
	curl -O "${path_to_cfg}"
fi
cp ./yolov2-tiny.cfg ./Examples/single_image/bin/data/
cp ./yolov2-tiny.cfg ./Examples/camera/bin/data/
cp ./yolov2-tiny.cfg ./Examples/annotation/bin/data/
rm -f ./yolov2-tiny.cfg
rm -f ./yolov2-tiny.weights
echo "done"
