#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetDataPathRoot("../Resources/data/");
    
    // yolo setting up
    ofSetFrameRate(60);
    detector.setup(ofToDataPath("dnn/yolov2-tiny.cfg"),
               ofToDataPath("dnn/yolov2-tiny.weights"),
               ofToDataPath("dnn/classlist.txt")
               );
    detector.setupAnnotationGui();

}

//--------------------------------------------------------------
void ofApp::update(){
    detector.updateControls();
}

//--------------------------------------------------------------
void ofApp::draw(){
    detector.drawAnnotationControls();
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
