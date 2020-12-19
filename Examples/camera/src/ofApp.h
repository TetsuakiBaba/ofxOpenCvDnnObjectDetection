#pragma once

#include "ofMain.h"
#include "ofxOpenCvDnnObjectDetection.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    ofVideoGrabber camera;
    ofVideoPlayer video; // for video detection
    ofxOpenCvDnnObjectDetection detector;
    
    void keyPressed(int key);
    ofImage img;
    uint64_t detection_time;


};
