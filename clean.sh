#!/bin/bash
rm  ./Examples/camera/bin/data/yolov2-tiny.weights 
rm  ./Examples/single_image/bin/data/yolov2-tiny.weights 
rm  ./Examples/annotation/bin/data/yolov2-tiny.weights 
rm  ./Examples/single_image/bin/data/yolov2-tiny.cfg
rm  ./Examples/camera/bin/data/yolov2-tiny.cfg
rm  ./Examples/annotation/bin/data/yolov2-tiny.cfg
rm -fR ./Examples/camera/bin/camera*.app
rm -fR ./Examples/single_image/bin/single_image*.app
rm -fR ./Examples/annotation/bin/annotation*.app
echo "done"