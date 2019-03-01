#include "ofApp.h"


void ofApp::changeSeekbar(int &_frame)
{
    video.setFrame(_frame);
    detector.train.clear();
    detector.filename = path+ofGetTimestampString();
    detector.filename_jpg = detector.filename + ".jpg";
    detector.filename_txt = detector.filename + ".txt";
    
}

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetDataPathRoot("../Resources/data/");
    // yolo setting up
    
    detector.setup(ofToDataPath("MobileNetSSD_DG_deploy.prototxt"),
               ofToDataPath("MobileNetSSD_DG_deploy.caffemodel"),
               ofToDataPath("gocen.txt"));
     /*
    
    detector.setup(ofToDataPath("yolov2-tiny-isee.cfg"),
               ofToDataPath("yolov2-tiny-isee.weights"),
               ofToDataPath("isee.list")
               );
     */
        
    detector.setConfidenceThreshold(0.4);
    mode_annotation = MODE_IMAGE_ANNOTATION;
    flg_pause_camera = true;
    
    detector.filepath = ofToDataPath("");
    detector.filename = "test";
    detector.filename_jpg = "test.jpg";
    detector.filename_txt = "test.txt";
    
    is_first_camera_annotation = true;
    
    gui_video.setup();
    gui_video.setName("Seek Bar");
    gui_video.add(seekbar.set("frame", 0));
    seekbar.addListener(this, &ofApp::changeSeekbar);
    
    gui_basic.setup();
    gui_basic.setName("Settings");
    gui_basic.add(fps.set("FPS", 30, 0, 60));
    gui_basic.add(threshold.set("Threshold", 0.4, 0.01, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    fps = ofGetFrameRate();
    detector.setConfidenceThreshold(threshold);
    switch( mode_annotation )
    {
        case MODE_IMAGE_ANNOTATION:
            break;
        case MODE_CAMERA_ANNOTATION:
            if( detector.train.size() >  0 )flg_pause_camera = true;
            else flg_pause_camera = false;
            
            if( flg_pause_camera == false ){
                camera.update();
                if( camera.isFrameNew() ){
                    detector.image_annotation.setFromPixels(camera.getPixels());
                    if( flg_show_yolo_detection)detector.update(camera.getPixels());
                }
            }
            break;
            
        case MODE_VIDEO_ANNOTATION:

            video.update();
            
            if( video.isFrameNew() ){
                detector.image_annotation.setFromPixels(video.getPixels());
                if( flg_show_yolo_detection){
                    detector.update(video.getPixels());
                    cout << "annotation" << endl;
                }
                
                seekbar.removeListener(this, &ofApp::changeSeekbar);
                seekbar = video.getCurrentFrame();
                seekbar.addListener(this, &ofApp::changeSeekbar);
                
            }
            
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(60);
    ofSetColor(255);

    // Show YOLO Class Selector
    detector.drawClassSelector(ofGetWidth()-250,20, 2);

    // Show YOLO annotation
    detector.drawAnnotation(0,0,
                        detector.image_annotation.getWidth(),
                        detector.image_annotation.getHeight());


    // Show YOLO Realtime detection result
    if( flg_show_yolo_detection ){
        detector.draw(0,0,
                  detector.image_annotation.getWidth(),
                  detector.image_annotation.getHeight());
    }
    
    // Show bounding box selecting operation
    ofNoFill();
    ofSetLineWidth(2.0);
    ofDrawRectangle(r);
    

    switch( mode_annotation )
    {
        case MODE_IMAGE_ANNOTATION:
            gui_video.draw();
            ofDrawBitmapStringHighlight("IMAGE ANNOTATION MODE", 0,ofGetHeight()-48);
            break;
        case MODE_CAMERA_ANNOTATION:
            ofDrawBitmapStringHighlight("CAMERA ANNOTATION MODE", 0,ofGetHeight()-48);
            break;
        case MODE_VIDEO_ANNOTATION:
            gui_video.draw();
            ofDrawBitmapStringHighlight("VIDEO ANNOTATION MODE", 0,ofGetHeight()-48);
            break;
    }
    ofDrawBitmapStringHighlight("image annotation mode: Drag and Drop an image directory.\ncamera annotation mode: press 'c' key.\nvideo annotation mode: Drag and Drop a video file.", 0,ofGetHeight()-30);

    
    gui_basic.draw();
    
    // show cross 
    ofSetLineWidth(1.0);
    ofSetColor(255,255,255,100);
    ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
    ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == OF_KEY_RIGHT ){
        switch( mode_annotation )
        {
            case MODE_IMAGE_ANNOTATION:
                detector.setNextAnnotation();
                break;
            case MODE_VIDEO_ANNOTATION:
                if(video.isLoaded())video.nextFrame();
                detector.train.clear();
                
                detector.filename = path+ofGetTimestampString();
                detector.filename_jpg = detector.filename + ".jpg";
                detector.filename_txt = detector.filename + ".txt";
                break;
        }
    }
    else if( key == OF_KEY_LEFT ){
        switch( mode_annotation )
        {
            case MODE_IMAGE_ANNOTATION:
                detector.setPreviousAnnotation();
                break;
            case MODE_VIDEO_ANNOTATION:
                if( video.isLoaded())video.previousFrame();
                detector.train.clear();
                
                detector.filename = path+ofGetTimestampString();
                detector.filename_jpg = detector.filename + ".jpg";
                detector.filename_txt = detector.filename + ".txt";
                break;
        }
    }
    else if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = true;
        if( mode_annotation == MODE_VIDEO_ANNOTATION && flg_pause_video == true ){
            video.update();
            detector.update(video.getPixels());
        }
    }
    else if( key == OF_KEY_RETURN ){
        detector.train.clear();
    }
    else if( key == 'c' ){
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a Saving Directory");
        if( file.bSuccess ){
            path = file.getPath();
        }
        else{
            return;
        }
        if( video.isLoaded() )video.close();
        mode_annotation = MODE_CAMERA_ANNOTATION;
        string devices;
        for( int i = 0; i < camera.listDevices().size(); i++ ){
            devices=devices+ofToString(camera.listDevices()[i].id) +
            ": " +
            camera.listDevices()[i].deviceName + 
            "\n";
        }
        string camera_id = ofSystemTextBoxDialog("Choose Camera ID to Open\n"+devices);
        camera.setDeviceID(ofToInt(camera_id));
        camera.initGrabber(640,480);
        flg_pause_camera = false;
    }
    else if( key == ' ' ){
        if( mode_annotation == MODE_VIDEO_ANNOTATION){
            if( flg_pause_video )flg_pause_video=false;
            else flg_pause_video = true;
            
        }

        if( flg_pause_video == true ){
            video.setPaused(true);
        }
        else{
            video.setPaused(false);
        }
        
        detector.train.clear();
        
        detector.filename = path+ofGetTimestampString();
        detector.filename_jpg = detector.filename + ".jpg";
        detector.filename_txt = detector.filename + ".txt";
    }
    else if( key == 'D' ){
        flg_show_yolo_detection = !flg_show_yolo_detection;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofRectangle r_img;
    float w,h;
    switch( mode_annotation )
    {
        case MODE_IMAGE_ANNOTATION:
            break;
        case MODE_CAMERA_ANNOTATION:
            break;
    }
    r_img.set(0,0, detector.image_annotation.getWidth(), detector.image_annotation.getHeight());
    if( r_img.inside(x,y) ){
        r.width = x-r.x;
        r.height = y-r.y;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    r.set(x,y,0,0);
    
    if( flg_show_yolo_detection == false ){
        for( int i = 0; i < detector.train.size(); i++ ){
            if( detector.train.at(i).getScaledBB(detector.image_annotation.getWidth(), detector.image_annotation.getHeight()).inside(x,y)){
                detector.train.erase(detector.train.begin()+i);
                detector.saveAnnotation();
                if( detector.train.size() == 0 ){
                    ofSystem("rm "+detector.filename_jpg);
                    ofSystem("rm "+detector.filename_txt);
                }
            }
        }
    }
    else{
        for( int i = 0; i < detector.object.size(); i++ ){
            if( detector.object.at(i).getScaledBB(detector.image_annotation.getWidth(),                                              detector.image_annotation.getHeight()).inside(x,y) ){
                detector.train.push_back(TrainObject(detector.object.at(i).class_id,
                                                 detector.object.at(i).name,
                                                 detector.object.at(i).r));
                detector.saveAnnotation();
                if( detector.train.size() == 1 ){
                    ofSaveImage(detector.image_annotation, detector.filename_jpg, OF_IMAGE_QUALITY_BEST);

                }

            }
        }
    }
        
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    r.width = x-r.x;
    r.height = y-r.y;
    r.standardize();

    if( mode_annotation == MODE_CAMERA_ANNOTATION ){
        if( detector.train.size() == 0 && flg_pause_camera == false && r.getArea() > 10){
            if( is_first_camera_annotation == true ){
                ofFileDialogResult file = ofSystemSaveDialog("myAnnotaion_", "Choose a directory for annotation files");
                path = file.getPath();
                is_first_camera_annotation = false;
            }
            
            detector.filename = path+ofGetTimestampString();
            detector.filename_jpg = detector.filename + ".jpg";
            detector.filename_txt = detector.filename + ".txt";
            
            ofSaveImage(detector.image_annotation, detector.filename_jpg, OF_IMAGE_QUALITY_BEST);

        }
    }
    
    if( mode_annotation == MODE_VIDEO_ANNOTATION ){
        if( detector.train.size() == 0 &&  r.getArea() > 10){
            
            detector.filename = path+ofGetTimestampString();
            detector.filename_jpg = detector.filename + ".jpg";
            detector.filename_txt = detector.filename + ".txt";
            
            ofSaveImage(detector.image_annotation, detector.filename_jpg,OF_IMAGE_QUALITY_BEST);

        }
    }
    
    ofRectangle r_img;
    r_img.set(0,0, detector.image_annotation.getWidth(), detector.image_annotation.getHeight());
    if( r.getArea() > 10 &&  r_img.inside(x,y) ){
        r.x = r.x/detector.image_annotation.getWidth();
        r.y = r.y/detector.image_annotation.getHeight();
        r.width = r.width/detector.image_annotation.getWidth();
        r.height = r.height/detector.image_annotation.getHeight();
        
        detector.addTrainObject(r);
        detector.saveAnnotation();
    }
    r.set(0,0,0,0);
    
    
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
    ofDirectory dir;
    dir.open(dragInfo.files[0]);

    if( dir.isDirectory() ){
        dir.close();
        detector.loadAnnotationDir(dragInfo.files[0]);
        mode_annotation = MODE_IMAGE_ANNOTATION;
        //seekbar.set("frame",0,0 detector.dir_annotation.size());
        detector.dir_annotation.size();
        //pos_annotation_file > dir_annotation.size()-1
    }
    else if( doesIncludeExtensions(dragInfo.files[0], {".mov", ".mp4"}) ){
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a saving directory");
        if( !file.bSuccess )return;
        path = file.getPath();
        detector.filename = file.getPath()+ofGetTimestampString();
        detector.filename_jpg = detector.filename + ".jpg";
        detector.filename_txt = detector.filename + ".txt";
        
        if( video.isLoaded() )video.close();
        video.load(dragInfo.files[0]);
        mode_annotation = MODE_VIDEO_ANNOTATION;
        seekbar.set("frame", 0, 0, video.getTotalNumFrames());
        gui_video.setPosition(0,video.getHeight());
        gui_video.setSize(video.getWidth(), 20);
        gui_video.setDefaultWidth(video.getWidth());
        gui_video.setWidthElements(video.getWidth());
        flg_pause_video = false;
        video.play();
    }
    else{
        ofSystemAlertDialog("Error: Filetype is not allowed.");
    }
    dir.close();
}
