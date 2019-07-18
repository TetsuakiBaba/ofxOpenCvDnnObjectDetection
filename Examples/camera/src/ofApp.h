#pragma once

#include "ofMain.h"
#include "ofxOpenCvDnnObjectDetection.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    //ofVideoGrabber video;
    ofVideoPlayer video;
    ofxOpenCvDnnObjectDetection detector;
    
    void keyPressed(int key);
    ofImage img;
};
