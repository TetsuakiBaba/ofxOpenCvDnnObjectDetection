#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    img.load(ofToDataPath("dog.jpg"));
    // yolo
    detector.setup(ofToDataPath("yolov2-tiny.cfg"),
               ofToDataPath("yolov2-tiny.weights"),
               ofToDataPath("coco.list"));
    detector.setConfidenceThreshold(0.4);
    
    detector.update(img.getPixels());
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    img.draw(0,0);
    
    /*
     yolo.object[i].r; -> rectangle bounding box
     yolo.object[i].name; -> class name
     yolo.object[i].p; -> probability
     see yolo.draw() method for further object information
     */
    detector.draw(0,0,img.getWidth(), img.getHeight());

        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
