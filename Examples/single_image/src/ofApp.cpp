#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    img.load(ofToDataPath("dog.jpg"));
    ofSetWindowShape(img.getWidth(), img.getHeight());
    // yolo
//    detector.setNetworkImageSize(416, 416);
//    detector.setup(ofToDataPath("dnn/yolov3.cfg"),
//               ofToDataPath("dnn/yolov3.weights"),
//               ofToDataPath("dnn/coco.txt"));
    
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov3-tiny.cfg"),
               ofToDataPath("dnn/yolov3-tiny.weights"),
               ofToDataPath("dnn/coco.txt"));
    
    // MobileNet-SSD
//    detector.setNetworkImageSize(300, 300);
//    detector.setup(ofToDataPath("dnn/MobileNetSSD.prototxt"),
//               ofToDataPath("dnn/MobileNetSSD.caffemodel"),
//               ofToDataPath("dnn/voc.txt"));



    detector.setConfidenceThreshold(0.1);
    
    detector.update(img.getPixels());
    inference_count = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if( inference_count >= 0 ){

        uint64 t0 = ofGetElapsedTimeMillis();
        detector.update(img.getPixels());
        inference_count++;
        sum_inference_time += detector.getInferenceTime();
        if( inference_count > 100 ){
            average_inference = sum_inference_time/(float)inference_count;
            inference_count = -1;
        }
    }
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
    ofDrawBitmapStringHighlight("Inference Time:"+ofToString(detector.getInferenceTime()), 20,20);
    if( inference_count == -1){
    ofDrawBitmapStringHighlight("Average Inference Time:"+ofToString(average_inference)+"\nPress space key to get average of inference time", 20, 60);
    }
    else{
        ofDrawBitmapStringHighlight("Average Inference Time: Calculating...\nPress space key to get average of inference time", 20, 60);
    }
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' ' ){
        inference_count = 0;
        sum_inference_time = 0;
        average_inference = 0;
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
