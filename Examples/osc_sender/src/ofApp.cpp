#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);
    // setup camera
    int device_id = 0;
    cout << camera.listDevices().size() << endl;
    if( camera.listDevices().size() > 0 ){
        string str_devices;
        for( int i = 0; i < camera.listDevices().size(); i++ ){
            str_devices += ofToString(i) + ": " + camera.listDevices()[i].deviceName + "\n";
        }
        device_id = ofToInt(ofSystemTextBoxDialog("You have 2 more camera devices, choose ID: "+str_devices));
    }

    camera.setDeviceID( device_id );
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
    
    // open an outgoing connection to HOST:PORT
    sender.setup("localhost", 12345);
    
}

//--------------------------------------------------------------
void ofApp::update(){
        
    camera.update();
    if( camera.isFrameNew() ){
        // camera image automatically will be croped to detector.setNetworkImageSige
        uint64_t t0 = ofGetElapsedTimeMillis();
        detector.update(camera.getPixels());
        detection_time = ofGetElapsedTimeMillis()-t0;
        for( int i = 0; i < detector.object.size(); i++ ){
            // class id: object[i].class_id;
            // possibility: detector.object[i].p;
            // bounding box: detector.object[i].r;
            
            // send id, possibility and r via osc
            ofxOscMessage m;
            m.setAddress("/object");
            m.addIntArg(detector.object[i].class_id);
            m.addStringArg(detector.object[i].name);
            m.addFloatArg(detector.object[i].r.x);
            m.addFloatArg(detector.object[i].r.y);
            m.addFloatArg(detector.object[i].r.width);
            m.addFloatArg(detector.object[i].r.height);
            sender.sendMessage(m);
            
        }
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


