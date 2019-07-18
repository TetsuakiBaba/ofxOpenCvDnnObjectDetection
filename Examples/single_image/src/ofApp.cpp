#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    img.load(ofToDataPath("dog.jpg"));
    ofSetWindowShape(img.getWidth(), img.getHeight());
    // yolo
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov2-tiny.cfg"),
               ofToDataPath("dnn/yolov2-tiny.weights"),
               ofToDataPath("dnn/coco.txt"));
    
    // SSD
    /*
    detector.setNetworkImageSize(300, 300);
    detector.setup(ofToDataPath("MobileNetSSD_DG_deploy.prototxt"),
                   ofToDataPath("MobileNetSSD_DG_deploy.caffemodel"),
                   ofToDataPath("voc.list"));
    detector.setConfidenceThreshold(0.1);
   */
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
     Here is a sample code to access each detected objects.
     for( int i = 0; i < detector.object.size(); i++ ){
     detector.object[i].r; -> rectangle bounding box, ofRectangle
     detector.object[i].name; -> class name, string
     detector.object[i].p; -> probability, float
     }
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
