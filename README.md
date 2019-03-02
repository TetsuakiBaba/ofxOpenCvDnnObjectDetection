# ofxOpenCvDnnObjectDetection

## Description
ofxOpenCvDnnObjectDetection is an addon for openframeworks.

OpenCV v.3.3.1 or upper includes dnn module ( https://docs.opencv.org/3.4.1/da/d9d/tutorial_dnn_yolo.html ) in their own package. Therefore I designed ofxOpenCvDnnObjectDetection with OpenCV v.3.3.1 or upper version.

I created this addon for beginers or learner who are going to try object detection with Yolo, SSD and R-CNN on OF.

## Usage
See Examples

### single_image
![single_image screenshot](/screenshot.png)
Detection example for single image.

### camera
Realtime Detection example for webcam video.

### annotation
![annotation screenshot](/screenshot_annotation.png)
Simple annotation example to export a yolo format.

## Install
### 1. Install OpenCV version.3 by brew.

    $ brew install opencv@3

### 2. confirm your opencv version
Based on OpenCV@3(version.3) version, Please re-write ofxOpenCvDnnObjectDetection/addon_config.mk file. End of the file, there are some description about opencv path for your system. Please re-write the version according to your OpenCV@3 version. Current (2019/3/1) is 3.4.5, If you find other version on your directory tree, /usr/local/Cellar/opencv@3 , please do not forget fill in the correct version on the addon_config.mk file. You can get your current opencv version by typing below command.

    $ ls /usr/local/Cellar/opencv@3

### 3. Download cfg and weights file
I put a download shell script on the ofxOpenCvDnnObjectDetection directory. it will download Yolov2 file(cfg and weights) to each examples directories. You can find download ofxYolov2 weights and cfg files on each example directories.

    $ cd ofxOpenCvDnnObjectDetection
    $ sh getWeights.sh

### 4. Update each examples by project generator, then run.
 Example/camera, single_image or annotation by project generator and add only ofxOpenDvDnnObjectDetection as an addon. Then update it. _Attention Please do not include OpenCV addon in OF.  

## Compatibility
- only macOS ( tested on Mojave)
- of version: 0.10.1

## Licence
[MIT](https://opensource.org/licenses/MIT)

## Author
[TetsuakiBaba](https://github.com/TetsuakiBaba)

## Reference
1. Model Zoo on OpenCV DNN Module README page: https://github.com/opencv/opencv/blob/master/samples/dnn/README.md
2. How to implement OpenCV Yolo on Openframeworks: https://qiita.com/buchayaty/items/4020100f531c07418f38

