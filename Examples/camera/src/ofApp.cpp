#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);
    // setup camera
    //video.setDeviceID( 0 );
//    video.setDesiredFrameRate( 30 );
//    video.initGrabber( 1280, 720 );
    video.load(ofToDataPath("/Users/baba/Downloads/Pexels Videos 3653.mp4"));
    
    ofSetWindowShape(video.getWidth(), video.getHeight());

    // setup dnn network for object detection. Case: Yolov2-tiny

    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov2-tiny.cfg"),
                   ofToDataPath("dnn/yolov2-tiny.weights"),
                   ofToDataPath("dnn/coco.txt"));

    // setup dnn network for object detection. Case: SSD-MobileNet
    
/*
    detector.setNetworkImageSize(300, 300);
    detector.setup(ofToDataPath("MobileNetSSD_deploy.xml"),
               ofToDataPath("MobileNetSSD_deploy.bin"),
               ofToDataPath("names-voc.txt"));
*/
    detector.setConfidenceThreshold(0.4);
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
    video.play();
}


