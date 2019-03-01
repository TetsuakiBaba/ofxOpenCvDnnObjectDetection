#pragma once

#include "ofMain.h"
#include "ofxOpenCvDnnObjectDetection.h"
#include "ofxGui.h"

#define MODE_IMAGE_ANNOTATION 1
#define MODE_CAMERA_ANNOTATION 2
#define MODE_VIDEO_ANNOTATION 3


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOpenCvDnnObjectDetection detector;
    ofRectangle r;
    bool flg_show_yolo_detection;
    bool flg_pause_camera;
    bool flg_pause_video;
    bool is_first_camera_annotation;
    string path;
    int mode_annotation;
    ofVideoGrabber camera;
    ofVideoPlayer video;
    
    ofxPanel gui_video;
    ofParameter<int>seekbar;
    
    ofxPanel gui_basic;
    ofParameter<int>fps;
    ofParameter<float>threshold;
    
    void changeSeekbar(int &_frame);
};
