#include "ofApp.h"


void ofApp::changeSeekbar(int &_frame)
{
    video.setFrame(_frame);
    yolo.train.clear();
    yolo.filename = path+ofGetTimestampString();
    yolo.filename_jpg = yolo.filename + ".jpg";
    yolo.filename_txt = yolo.filename + ".txt";
    
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot("../Resources/data/");
    // yolo setting up
    yolo.setup(ofToDataPath("yolov2-tiny.cfg"),
               ofToDataPath("yolov2-tiny.weights"),
               ofToDataPath("isee.txt"));
        
    yolo.setConfidenceThreshold(0.4);
    mode_annotation = MODE_IMAGE_ANNOTATION;
    flg_pause_camera = true;
    
    yolo.filepath = ofToDataPath("");
    yolo.filename = "test";
    yolo.filename_jpg = "test.jpg";
    yolo.filename_txt = "test.txt";
    
    is_first_camera_annotation = true;
    
    gui_video.setup();
    gui_video.setName("Seek Bar");
    gui_video.add(seekbar.set("frame", 0));
    seekbar.addListener(this, &ofApp::changeSeekbar);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    switch( mode_annotation )
    {
        case MODE_IMAGE_ANNOTATION:
            break;
        case MODE_CAMERA_ANNOTATION:
            if( yolo.train.size() >  0 )flg_pause_camera = true;
            else flg_pause_camera = false;
            
            if( flg_pause_camera == false ){
                camera.update();
                if( camera.isFrameNew() ){
                    yolo.image_annotation.setFromPixels(camera.getPixels());
                    if( flg_show_yolo_detection)yolo.update(camera.getPixels());
                }
            }
            break;
            
        case MODE_VIDEO_ANNOTATION:

            video.update();
            
            if( video.isFrameNew() ){
                yolo.image_annotation.setFromPixels(video.getPixels());
                if( flg_show_yolo_detection){
                    yolo.update(video.getPixels());
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
    yolo.drawClassSelector(ofGetWidth()-250,20, 2);

    // Show YOLO annotation
    yolo.drawAnnotation(0,0,
                        yolo.image_annotation.getWidth(),
                        yolo.image_annotation.getHeight());


    // Show YOLO Realtime detection result
    if( flg_show_yolo_detection ){
        yolo.draw(0,0,
                  yolo.image_annotation.getWidth(),
                  yolo.image_annotation.getHeight());
    }
    
    // Show bounding box selecting operation
    ofNoFill();
    ofSetLineWidth(2.0);
    ofDrawRectangle(r);
    

    switch( mode_annotation )
    {
        case MODE_IMAGE_ANNOTATION:
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

    
    
    // show cross 
    ofSetLineWidth(1.0);
    ofSetColor(255,255,255,100);
    ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
    ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == OF_KEY_RIGHT || key == OF_KEY_SHIFT){
        switch( mode_annotation )
        {
            case MODE_IMAGE_ANNOTATION:
                yolo.setNextAnnotation();
                break;
            case MODE_VIDEO_ANNOTATION:
                if(video.isLoaded())video.nextFrame();
                yolo.train.clear();
                
                yolo.filename = path+ofGetTimestampString();
                yolo.filename_jpg = yolo.filename + ".jpg";
                yolo.filename_txt = yolo.filename + ".txt";
                break;
        }
    }
    else if( key == OF_KEY_LEFT || key == OF_KEY_ALT ){
        switch( mode_annotation )
        {
            case MODE_IMAGE_ANNOTATION:
                yolo.setPreviousAnnotation();
                break;
            case MODE_VIDEO_ANNOTATION:
                if( video.isLoaded())video.previousFrame();
                yolo.train.clear();
                
                yolo.filename = path+ofGetTimestampString();
                yolo.filename_jpg = yolo.filename + ".jpg";
                yolo.filename_txt = yolo.filename + ".txt";
                break;
        }
    }
    else if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = true;
        if( mode_annotation == MODE_VIDEO_ANNOTATION && flg_pause_video == true ){
            video.update();
            yolo.update(video.getPixels());
        }
    }
    else if( key == OF_KEY_RETURN ){
        yolo.train.clear();
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
        
        yolo.train.clear();
        
        yolo.filename = path+ofGetTimestampString();
        yolo.filename_jpg = yolo.filename + ".jpg";
        yolo.filename_txt = yolo.filename + ".txt";
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
    r_img.set(0,0, yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight());
    if( r_img.inside(x,y) ){
        r.width = x-r.x;
        r.height = y-r.y;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    r.set(x,y,0,0);

    if( flg_show_yolo_detection == false ){
        for( int i = 0; i < yolo.train.size(); i++ ){
            if( yolo.train.at(i).getScaledBB(yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight()).inside(x,y)){
                yolo.train.erase(yolo.train.begin()+i);
                yolo.saveAnnotation();
                if( yolo.train.size() == 0 ){
                    ofSystem("rm "+yolo.filename_jpg);
                    ofSystem("rm "+yolo.filename_txt);
                }
            }
        }
    }
    else{
        for( int i = 0; i < yolo.object.size(); i++ ){
            if( yolo.object.at(i).getScaledBB(yolo.image_annotation.getWidth(),                                              yolo.image_annotation.getHeight()).inside(x,y) ){
                yolo.train.push_back(TrainObject(yolo.object.at(i).class_id,
                                                 yolo.object.at(i).name,
                                                 yolo.object.at(i).r));
                yolo.saveAnnotation();
                if( yolo.train.size() == 1 ){
                    ofSaveImage(yolo.image_annotation, yolo.filename_jpg, OF_IMAGE_QUALITY_BEST);

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
        if( yolo.train.size() == 0 && flg_pause_camera == false && r.getArea() > 10){
            if( is_first_camera_annotation == true ){
                ofFileDialogResult file = ofSystemSaveDialog("myAnnotaion_", "Choose a directory for annotation files");
                path = file.getPath();
                is_first_camera_annotation = false;
            }
            
            yolo.filename = path+ofGetTimestampString();
            yolo.filename_jpg = yolo.filename + ".jpg";
            yolo.filename_txt = yolo.filename + ".txt";
            
            ofSaveImage(yolo.image_annotation, yolo.filename_jpg, OF_IMAGE_QUALITY_BEST);

        }
    }
    
    if( mode_annotation == MODE_VIDEO_ANNOTATION ){
        if( yolo.train.size() == 0 &&  r.getArea() > 10){
            
            yolo.filename = path+ofGetTimestampString();
            yolo.filename_jpg = yolo.filename + ".jpg";
            yolo.filename_txt = yolo.filename + ".txt";
            
            ofSaveImage(yolo.image_annotation, yolo.filename_jpg,OF_IMAGE_QUALITY_BEST);

        }
    }
    
    ofRectangle r_img;
    r_img.set(0,0, yolo.image_annotation.getWidth(), yolo.image_annotation.getHeight());
    if( r.getArea() > 10 &&  r_img.inside(x,y) ){
        r.x = r.x/yolo.image_annotation.getWidth();
        r.y = r.y/yolo.image_annotation.getHeight();
        r.width = r.width/yolo.image_annotation.getWidth();
        r.height = r.height/yolo.image_annotation.getHeight();
        
        yolo.addTrainObject(r);
        yolo.saveAnnotation();
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
        yolo.loadAnnotationDir(dragInfo.files[0]);
        mode_annotation = MODE_IMAGE_ANNOTATION;
    }
    else if( doesIncludeExtensions(dragInfo.files[0], {".mov", ".mp4"}) ){
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a saving directory");
        if( !file.bSuccess )return;
        path = file.getPath();
        yolo.filename = file.getPath()+ofGetTimestampString();
        yolo.filename_jpg = yolo.filename + ".jpg";
        yolo.filename_txt = yolo.filename + ".txt";
        
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
    else if( doesIncludeExtensions(dragInfo.files[0], {".txt", ".list"}) ){
        yolo.classNamesVec.clear();
        yolo.setup("","",dragInfo.files[0]);
    }
    else{
        ofSystemAlertDialog("Error: Filetype is not allowed.");
    }
    dir.close();
}
