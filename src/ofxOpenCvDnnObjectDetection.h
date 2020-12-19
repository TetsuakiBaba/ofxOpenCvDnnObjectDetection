#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/ocl.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#define ANNOTATION_COLOR_MODE_DEFAULT 0
#define ANNOTATION_COLOR_MODE_DARK 1

#define MODE_SELECT_ANNOTATION 0
#define MODE_IMAGE_ANNOTATION 1
#define MODE_DIR_IMAGE_ANNOTATION 5
#define MODE_CAMERA_ANNOTATION 2
#define MODE_VIDEO_ANNOTATION 3
#define MODE_AUDIO_ANNOTATION 4

using namespace std;
using namespace cv;
using namespace dnn;

std::string Replace( std::string String1, std::string String2, std::string String3 );
bool doesIncludeExtensions(string _filename, vector<string>_extensions);

class TrainObject{
public:
    TrainObject(int _id, string _name, ofRectangle _r){
        id = _id;
        name = _name;
        r.set(_r);
    }
    ~TrainObject(){
        
    }
    int id;
    string name;
    ofRectangle r;
    ofRectangle getScaledBB(float _w, float _h);

    
};

class Object{
public:
    Object();
    Object(int _class_id, string _name, float _p, float _x, float _y, float _w, float _h);
    ~Object();
    ofRectangle r;
    ofRectangle getScaledBB(float _w, float _h);
    ofRectangle getScaledBB(float _x, float _y, float _w, float _h);
    string name;
    float p;
    int class_id;
};

class ofxOpenCvDnnObjectDetection{
public:
    ofxOpenCvDnnObjectDetection();
    ~ofxOpenCvDnnObjectDetection();
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    void mousePressed(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void mouseEntered(ofMouseEventArgs &e);
    void mouseExit(ofMouseEventArgs &e);
    void mouseMoved(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void dragEvent(ofDragInfo &dragInfo);

    void enableAnnotationControl();
    void disableAnnotationControl();
    
    bool is_dragging;
    
    void setup(string _path_to_cfg, string _path_to_weights, string _path_to_names);
    void setupAnnotationGui();
    void draw(float _x, float _y, float _w, float _h);
    void drawAnnotation(float _x, float _y, float _w, float _h);
    void drawAnnotationControls();
    uint64_t getInferenceTime();
    void update(ofPixels &op);
    void updateControls();
    std::vector<String> getOutputsNames(const Net& net);
    void postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net);
    void setConfidenceThreshold(float _threshold);
    void setNetworkImageSize(int _w, int _h);

    void adjustGuiComponents();
    void setNextAnnotation();
    void setPreviousAnnotation();
    void setPositionAnnotation(int _pos, string _text_search_id);
    
    void loadAnnotationDir(string _path_to_file);
    void loadAnnotationImage(string _path_to_file);
    void loadBoundingBoxFile(string _path_to_file);
    void saveAnnotation();
    void saveAnnotationImage();
    void saveBoundingBoxToFile(string _path_to_file);
    void removeAnnotationFiles();

    bool checkAnnotationImage(float _threshold_area);
    void drawClassSelector(float _x, float _y, int _row);
    ofRectangle getClassSelectorBoundingBox();
    void setAnnotationFilename(string _filename);
    void setAnnotationColorMode(int _mode);
    void drawReconfirmAnnotation();
    ofRectangle r_class_selector;
    void addTrainObject(int _class_id_selected, ofRectangle _r);

    
    vector<Object> object;
    vector<TrainObject>train;
    uint64_t inference_time;
    
    cv::Mat toCV(ofPixels &pix);
    dnn::Net net;
    int network_width = 416;
    int network_height = 416;
    int input_width;
    int input_height;
    vector<string> classNamesVec;
    vector<ofColor> detection_color;
    float confidenceThreshold;
    ofTrueTypeFont font_info;

    string filename;
    string filename_jpg;
    string filename_txt;
    string filepath;

    ofDirectory dir_annotation;
    int pos_annotation_file;
    ofImage image_annotation;
    int class_id_selected;
    
    string str_debug;
    ofRectangle r_reconfirm;
    int r_reconfirm_alpha;
    
    int mode_annotation;
    ofVideoPlayer video;
    ofVideoGrabber camera;
    
    ofxPanel gui_frame;
    ofParameter<int>seekbar;
    
    ofxPanel gui_basic;
    ofParameter<int>fps;
    ofParameter<float>threshold;
    ofParameter<int>network_image_size;
    ofxToggle darkmode;
    ofxToggle b_magnify;
    ofxToggle b_magnify_pointing;
    ofParameter<float>magnify;
    ofParameter<string>text_search_id;
    ofxButton button_forward;
    ofxButton button_back;
    ofxToggle b_ai_checker;
    ofParameter<float>threshold_precision;
    void changeTextSearchID(string _str);
    void changeDarkMode(bool &_mode);
    void changeSeekbar(int &_frame);
    void changeThreshold(float &_th);
    void forwardAnnotationButton();
    void backAnnotationButton();
    string path;
    bool flg_show_yolo_detection;
    bool flg_pause_camera;
    bool flg_pause_video;
    bool is_shift_pressed;
    ofColor color_crossbar;
    
    ofRectangle r;
    ofTrueTypeFont font_debug;
    ofTrueTypeFont font_message;
    float mouseX;
    float mouseY;
};
