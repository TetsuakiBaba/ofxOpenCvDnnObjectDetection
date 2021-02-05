#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot("../Resources/data/");
    
    // yolo setting up
    ofSetFrameRate(60);
    detector.setup(
                   ofToDataPath("dnn/yolo.cfg"),
                   ofToDataPath("dnn/yolo.weights"),
                   ofToDataPath("dnn/classlist.txt")
                   );
//    detector.setup(ofToDataPath("dnn/tmp/yolov3_m2.prototxt"),  ofToDataPath("dnn/tmp/yolov3_m2.caffemodel"),ofToDataPath("dnn/classlist.txt"));
    /*
    detector.setup(ofToDataPath("dnn/ssd.pbtxt"),
                   ofToDataPath("dnn/ssd.pb"),
                   ofToDataPath("dnn/classlist.txt"));
     */
    detector.setupAnnotationGui();
    detector.setNetworkImageSize(416, 416);
    //detector.setNetworkImageSize(190, 320);
    detector.flg_show_yolo_detection = true;
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
    ofImage img;
    if( key == 'G' ){
        img.grabScreen(0, 0, detector.image_annotation.getWidth(), detector.image_annotation.getHeight());
        img.save(ofToDataPath("saved_images/")+ofGetTimestampString()+".png");
    }
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
