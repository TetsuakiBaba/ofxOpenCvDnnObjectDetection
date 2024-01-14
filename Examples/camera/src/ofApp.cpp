#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);
    // setup camera
    camera.setDeviceID( 0 );
    camera.initGrabber( 1280, 720 );
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov2-tiny.cfg"),
                   ofToDataPath("dnn/yolov2-tiny.weights"),
                   ofToDataPath("dnn/coco.txt"));
    
    // For Video detection example
    //    video.load(ofToDataPath("sample.mp4"));
    //    ofSetWindowShape(video.getWidth(), video.getHeight());
    //    // setup dnn network for object detection. Case: Yolov2-tiny
    //    detector.setNetworkImageSize(608, 608);
    //    detector.setup(ofToDataPath("dnn/yolov3-spp.cfg"),
    //                   ofToDataPath("dnn/yolov3-spp.weights"),
    //                   ofToDataPath("dnn/coco.txt"));
    //video.play();
    
    detector.setConfidenceThreshold(0.6);
    cout << "version:" << CV_VERSION << endl;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
        
    camera.update();
    if( camera.isFrameNew() ){
        // camera image automatically will be croped to detector.setNetworkImageSige
        uint64_t t0 = ofGetElapsedTimeMillis();
        detector.update(camera.getPixels());
        detection_time = ofGetElapsedTimeMillis()-t0;
    }
    
    // For video detection
    //    video.update();
    //    video.nextFrame();
    //    if( video.isFrameNew() ){
    //        // camera image automatically will be croped to detector.setNetworkImageSige
    //        uint64_t t0 = ofGetElapsedTimeMillis();
    //        detector.update(video.getPixels());
    //        detection_time = ofGetElapsedTimeMillis()-t0;
    //
    //    }


}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    
    camera.draw(0,0);
    // For video detection
    //    video.draw(0,0);
    
    detector.draw(0,0, ofGetWidth(), ofGetHeight());
    ofDrawBitmapStringHighlight("FPS:"+ofToString(ofGetFrameRate(), 0),20, 20);
    ofDrawBitmapStringHighlight("Detection Time:"+ofToString(detection_time),20, 40);
}

void ofApp::keyPressed(int key)
{

}


