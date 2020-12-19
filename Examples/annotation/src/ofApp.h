#pragma once

#include "ofMain.h"
#include "ofxOpenCvDnnObjectDetection.h"
#include "ofxGui.h"




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
    bool is_dragging;
    bool is_shift_pressed;


   
};
