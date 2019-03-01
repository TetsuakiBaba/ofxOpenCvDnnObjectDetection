#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    // setup camera
    video.setDeviceID( 0 );
    video.setDesiredFrameRate( 30 );
    video.initGrabber( 1280, 720 );

    // setup dnn network for object detection. Case.SSD
    detector.setNetworkImageSize(300, 300);
    detector.setup(ofToDataPath("MobileNetSSD_deploy.prototxt"),
               ofToDataPath("MobileNetSSD_deploy.caffemodel"),
               ofToDataPath("voc.list"));
    
    detector.setConfidenceThreshold(0.6);
}

//--------------------------------------------------------------
void ofApp::update(){
    video.update();
    if( video.isFrameNew() ){
        // video image automatically will be croped to detector.setNetworkImageSige
        detector.update(video.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    video.draw(0,0);
    detector.draw(0,0, video.getWidth(), video.getHeight());
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 0),20, 20);
}

void ofApp::keyPressed(int key)
{
}


