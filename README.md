# ofxOpenCvDnnObjectDetection

## Description
ofxYolov2 is a YOLO(You Look Only Once) addon for openframeworks.
Please refer YOLO official website for more details of YOLO. https://pjreddie.com/darknet/yolo/
If you have a Nvidia GPU(s) on your PC, I just recommend to use ofxDarknet instead of this addon, https://github.com/mrzl/ofxDarknet.

OpenCV v.3.3.1 or upper includes Yolov2 library ( https://docs.opencv.org/3.4.1/da/d9d/tutorial_dnn_yolo.html ) in
their own package. Therefore I designed ofxYolov2 with OpenCV v.3.3.1 or upper version.

I created this addon for beginers or learner who are going to try object detection with Yolo on OF.
Attention:  it runs on OpenCL.


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
### 1. Install OpenCV by brew.

    $ brew install opencv

### 2. confirm your opencv version
Based on OpenCV version, Please re-write ofxYolov2/addon_config.mk file. End of the file, there are some description about opencv path for your system. Please re-write the version according to your OpenCV version. Current (2018/6/15) is 3.4.1_5, If you find other version on your directory tree, /usr/local/Cellar/opencv , please do not forget fill in the correct version on the addon_config.mk file. You can get your current opencv version by typing below command.

    $ ls /usr/local/Cellar/opencv

### 3. Download cfg and weights file
Download cfg and weights file to each examples directories. You can find download shell script on the top of ofxYolov2 directory.

    $ cd ofxYolov2
    $ sh getYolov2Weights.sh

### 4. Edit yolov2-tiny.cfg
Edit "thresh .6" to "thresh .01" on each yolov2-tiny.cfg.
### 5. Update each examples by project generator, then run.
 Example/camera, single_image or annotation by project generator and add only ofxYolov2 as an addon. Then update it. _Attention Please do not include OpenCV addon in OF.  Be sure to add only ofxYolov2 addon by projectgenerator._

## Compatibility
- only macOS ( tested on Hight Sierra and Sierra)
- of version: 0.9.8

## Licence
[MIT](https://opensource.org/licenses/MIT)

## Author
[TetsuakiBaba](https://github.com/TetsuakiBaba)

## Reference
1. How to implement OpenCV Yolo on Openframeworks: https://qiita.com/buchayaty/items/4020100f531c07418f38
2. YOLO: https://pjreddie.com/darknet/yolo/
