#include "ofxOpenCvDnnObjectDetection.h"
std::vector<String> getOutputsNames(const Net& net);

std::string Replace( std::string String1, std::string String2, std::string String3 )
{
    std::string::size_type  Pos( String1.find( String2 ) );
    
    while( Pos != std::string::npos )
    {
        String1.replace( Pos, String2.length(), String3 );
        Pos = String1.find( String2, Pos + String3.length() );
    }
    
    return String1;
}

bool doesIncludeExtensions(string _filename, vector<string>_extensions)
{
    string extname;
    
    int ext_i = _filename.find_last_of(".");
    if( ext_i > 0 ){
        extname = _filename.substr(ext_i,_filename.size()-ext_i);
    }
    else{
        return false;
    }
    
    for( int i = 0; i < _extensions.size(); i++ ){
        if( _extensions[i] == extname ){
            return true;
        }
    }
    return false;
}

std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
    
    std::vector<std::string> result;
    
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
        
        result.push_back(subStr);
        
        first = last + 1;
        last = str.find_first_of(del, first);
        
        if (last == std::string::npos) {
            last = str.size();
        }
    }
    
    return result;
}


Object::Object()
{
    
}
Object::Object(int _class_id, string _name, float _p, float _x, float _y, float _w, float _h)
{
    class_id = _class_id;
    name = _name;
    p = _p;
    r.set(_x, _y, _w, _h);
}

Object::~Object()
{
    
}

// Constructor
ofxOpenCvDnnObjectDetection::ofxOpenCvDnnObjectDetection()
{

}
ofxOpenCvDnnObjectDetection::~ofxOpenCvDnnObjectDetection()
{
    
}

void ofxOpenCvDnnObjectDetection::enableAnnotationControl()
{
    ofAddListener(ofEvents().keyPressed ,this, &ofxOpenCvDnnObjectDetection::keyPressed);
    ofAddListener(ofEvents().keyReleased ,this, &ofxOpenCvDnnObjectDetection::keyReleased);
    ofAddListener(ofEvents().mousePressed, this, &ofxOpenCvDnnObjectDetection::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxOpenCvDnnObjectDetection::mouseReleased);
    ofAddListener(ofEvents().mouseEntered, this, &ofxOpenCvDnnObjectDetection::mouseEntered);
    ofAddListener(ofEvents().mouseExited, this, &ofxOpenCvDnnObjectDetection::mouseExit);
    ofAddListener(ofEvents().mouseMoved, this, &ofxOpenCvDnnObjectDetection::mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxOpenCvDnnObjectDetection::mouseDragged);
    ofAddListener(ofEvents().fileDragEvent, this, &ofxOpenCvDnnObjectDetection::dragEvent);
}
void ofxOpenCvDnnObjectDetection::disableAnnotationControl()
{
    ofRemoveListener(ofEvents().keyPressed, this,&ofxOpenCvDnnObjectDetection::keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this,&ofxOpenCvDnnObjectDetection::keyReleased);
    ofRemoveListener(ofEvents().mousePressed, this, &ofxOpenCvDnnObjectDetection::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxOpenCvDnnObjectDetection::mouseReleased);
    ofRemoveListener(ofEvents().mouseEntered, this, &ofxOpenCvDnnObjectDetection::mouseEntered);
    ofRemoveListener(ofEvents().mouseExited, this, &ofxOpenCvDnnObjectDetection::mouseExit);
    ofRemoveListener(ofEvents().mouseMoved, this, &ofxOpenCvDnnObjectDetection::mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxOpenCvDnnObjectDetection::mouseDragged);
    ofRemoveListener(ofEvents().fileDragEvent, this, &ofxOpenCvDnnObjectDetection::dragEvent);
}

void ofxOpenCvDnnObjectDetection::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    if( key == OF_KEY_RIGHT || key == 'f' ){
        switch( mode_annotation )
        {
            case MODE_DIR_IMAGE_ANNOTATION:
                if( seekbar < dir_annotation.size()-1 )seekbar++;
                break;
            case MODE_VIDEO_ANNOTATION:
                if(video.isLoaded())video.nextFrame();
                train.clear();
                setAnnotationFilename(path+ofGetTimestampString());
                break;
        }
    }
    else if( key == OF_KEY_LEFT || key == 'b'){
        switch( mode_annotation )
        {
            case MODE_DIR_IMAGE_ANNOTATION:
                if( seekbar > 0)seekbar--;
                break;
            case MODE_VIDEO_ANNOTATION:
                if( video.isLoaded())video.previousFrame();
                train.clear();
                setAnnotationFilename(path+ofGetTimestampString());
                break;
        }
    }
    else if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = true;
        if( mode_annotation == MODE_VIDEO_ANNOTATION && flg_pause_video == true ){
            video.update();
            update(video.getPixels());
        }
    }
    else if( key == 'X' ){
        train.clear();
        saveAnnotation();
        if(mode_annotation == MODE_CAMERA_ANNOTATION ||
           mode_annotation == MODE_VIDEO_ANNOTATION){
            removeAnnotationFiles();
        }
    }
    else if( key == 'c' ){
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a Saving Directory");
        if( file.bSuccess ){
            path = file.getPath();
            setAnnotationFilename(path+ofGetTimestampString());
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
        string camera_id = ofSystemTextBoxDialog("Choose Camera ID to Open\n"+devices, ofToString(camera.listDevices().size()-1));
        camera.setDeviceID(ofToInt(camera_id));
        camera.initGrabber(640,480);
        image_annotation.setFromPixels(camera.getPixels());
        adjustGuiComponents();
        flg_pause_camera = false;
        train.clear();
    }
    else if( key == ' ' ){
        if( mode_annotation == MODE_VIDEO_ANNOTATION){
            if( flg_pause_video )flg_pause_video=false;
            else flg_pause_video = true;
            if( flg_pause_video == true ){
                video.setPaused(true);
            }
            else{
                video.setPaused(false);
            }
            train.clear();
            setAnnotationFilename(path+ofGetTimestampString());
        }
        else if( mode_annotation == MODE_CAMERA_ANNOTATION ){
            flg_pause_camera = !flg_pause_camera;
            train.clear();
            setAnnotationFilename(path+ofGetTimestampString());
        }
    }
    else if( key == 'd' ){
        darkmode = !darkmode;
    }
    else if( key == 'o' ){
        ofSystem("open "+ofToDataPath("dnn/"));
    }
    else if( key == OF_KEY_SHIFT ){
        is_shift_pressed = true;
        b_magnify = true;
    }
    else if( key == OF_KEY_SUPER ){
        b_magnify_pointing = true;
    }
    else if( key == 'r' ){
        b_ai_checker = true;
    }
}

void ofxOpenCvDnnObjectDetection::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    if( key == OF_KEY_CONTROL){
        flg_show_yolo_detection = false;
    }
    else if( key == OF_KEY_SHIFT ){
        is_shift_pressed = false;
        b_magnify = false;
    }
    if( key == OF_KEY_SUPER ){
        b_magnify_pointing = false;
    }
}

ofRectangle Object::getScaledBB(float _x, float _y, float _w, float _h)
{
    ofRectangle r_return;
    r_return.set((r.x)*_w+_x,
                 (r.y)*_h+_y,
                 r.width * _w,
                 r.height * _h);
    return r_return;
}
ofRectangle Object::getScaledBB(float _w, float _h)
{
    ofRectangle r_return;
    r_return.set(r.x*_w,
                 r.y*_h,
                 r.width * _w,
                 r.height * _h);
    return r_return;
}

ofRectangle TrainObject::getScaledBB(float _w, float _h)
{
    ofRectangle r_return;
    r_return.set(r.x*_w,
                 r.y*_h,
                 r.width * _w,
                 r.height * _h);
    return r_return;
}




void ofxOpenCvDnnObjectDetection::draw(float _x, float _y, float _w, float _h)
{
    for( int i = 0; i < object.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(object[i].class_id));
        ofRectangle r_scaled = object.at(i).getScaledBB(_x, _y, _w, _h);
        ofDrawRectangle(r_scaled);
        
        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("NO["+ ofToString(i) +"] ID:["+ofToString(object.at(i).class_id)+"]: "+object.at(i).name + ": " + ofToString(object.at(i).p),
                                    r_scaled.x,r_scaled.y);
    }
}

void ofxOpenCvDnnObjectDetection::drawAnnotation(float _x, float _y, float _w, float _h)
{
    ofSetColor(255);
    if( image_annotation.isAllocated() ){
        image_annotation.draw(0,0, _w, _h);
    }

    for( int i = 0; i < train.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(train[i].id),150);
        ofRectangle r_scaled = train.at(i).getScaledBB(_w, _h);

        ofDrawRectangle(r_scaled);
        
        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("["+ofToString(train.at(i).id)+"]: "+train.at(i).name,                          r_scaled.x,r_scaled.y);

    }
    

    for( int i = 0; i < train.size(); i++ ){
        ofSetColor(detection_color[train[i].id],150);
        if( train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()). inside(ofGetMouseX(), ofGetMouseY()) ){
            ofFill();
            ofDrawRectangle(train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()));
        }
    }

}




cv::Mat ofxOpenCvDnnObjectDetection::toCV(ofPixels &pix)
{

    return cv::Mat(pix.getHeight(), pix.getWidth(), CV_MAKETYPE(CV_8U, pix.getNumChannels()), pix.getData(), 0);
}

void ofxOpenCvDnnObjectDetection::postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net)
{

    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;
    
    std::vector<int> classIds;
    std::vector<float> confidences;
    if (net.getLayer(0)->outputNameToIndex("im_info") != -1)  // Faster-RCNN or R-FCN
    {
        // Network produces output blob with a shape 1x1xNx7 where N is a number of
        // detections and an every detection is a vector of values
        // [batchId, classId, confidence, left, top, right, bottom]
        CV_Assert(outs.size() == 1);
        float* data = (float*)outs[0].data;
        for (size_t i = 0; i < outs[0].total(); i += 7)
        {
            float confidence = data[i + 2];
            if (confidence > confidenceThreshold)
            {
                int left = (int)data[i + 3];
                int top = (int)data[i + 4];
                int right = (int)data[i + 5];
                int bottom = (int)data[i + 6];
                int width = right - left + 1;
                int height = bottom - top + 1;
                classIds.push_back((int)(data[i + 1]) - 1);  // Skip 0th background class id.
//                boxes.push_back(Rect(left, top, width, height));
                confidences.push_back(confidence);
            }
        }
    }
    else if (outLayerType == "DetectionOutput") //SSD
    {
        cout <<"test"<<endl;
        // Network produces output blob with a shape 1x1xNx7 where N is a number of
        // detections and an every detection is a vector of values
        // [batchId, classId, confidence, left, top, right, bottom]
        CV_Assert(outs.size() == 1);
        float* data = (float*)outs[0].data;
        for (size_t i = 0; i < outs[0].total(); i += 7)
        {
            float confidence = data[i + 2];
            if (confidence > confidenceThreshold)
            {
                float left = (data[i + 3] * frame.cols)/input_width;
                float top = (data[i + 4] * frame.rows)/input_height;
                float  right = (data[i + 5] * frame.cols)/input_width;
                float bottom = (data[i + 6] * frame.rows)/input_height;
                float width = right - left ;
                float height = bottom - top ;
                
                String label = String(classNamesVec[data[i+1]-1]);
                ofRectangle r(left,top,width,height);
                object.push_back(Object(data[i+1]-1, label, confidence, r.x,r.y, r.width, r.height));
                
//                classIds.push_back((int)(data[i + 1]) - 1);  // Skip 0th background class id.
//                boxes.push_back(Rect(left, top, width, height));
  //              confidences.push_back(confidence);
            }
        }
    }
    else if (outLayerType == "Region") // Yolo
    {

        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Network produces output blob with a shape NxC where N is a number of
            // detected objects and C is a number of classes + 4 where the first 4
            // numbers are [center_x, center_y, width, height]
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                cv::Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confidenceThreshold)
                {
                    float centerX = (data[0] * frame.cols)/input_width;
                    float centerY = (data[1] * frame.rows)/input_height;
                    float width =  (data[2] * frame.cols)/input_width;
                    float height =  (data[3] * frame.rows)/input_height;
                    float left = centerX - width / 2;
                    float top = centerY - height / 2;
                    
                    String label = String(classNamesVec[classIdPoint.x]);
                    ofRectangle r(left,top,width,height);
                    object.push_back(Object(classIdPoint.x, label, confidence, r.x,r.y, r.width, r.height));
                }
            }
        }
    }
    else
    CV_Error(Error::StsNotImplemented, "Unknown output layer type: " + outLayerType);
    
    std::vector<int> indices;
  //  NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
    //    Rect box = boxes[idx];
//        drawPred(classIds[idx], confidences[idx], box.x, box.y,
//                 box.x + box.width, box.y + box.height, frame);
    }
}

void ofxOpenCvDnnObjectDetection::drawAnnotationControls()
{
    mouseX = ofGetMouseX();
    mouseY = ofGetMouseY();
    if( darkmode ){
        ofBackground(150);
    }
    else{
        ofBackground(60);
    }
    ofSetColor(255);
    
    // Show YOLO Class Selector
    drawClassSelector(image_annotation.getWidth()+20,20, 3);
    
    
    
    // Show YOLO Realtime detection result
    if( flg_show_yolo_detection ){
        ofSetColor(255);
        image_annotation.draw(0,0);
        draw(0,0,
             image_annotation.getWidth(),
             image_annotation.getHeight());
    }
    else{
        // Show only annotation and image
        drawAnnotation(0,0,
                       image_annotation.getWidth(),
                       image_annotation.getHeight());
        
    }
    
    // Show bounding box selecting operation
    ofNoFill();
    ofSetLineWidth(2.0);
    ofDrawRectangle(r);
    
    drawReconfirmAnnotation();
    ofSetColor(ofColor::ghostWhite);
    font_debug.drawString(str_debug,
                          20+image_annotation.getWidth(),
                          60+gui_basic.getHeight()+r_class_selector.getHeight());
    
    switch( mode_annotation )
    {
        case MODE_DIR_IMAGE_ANNOTATION:
            gui_frame.setPosition(0, image_annotation.getHeight());
            gui_frame.setWidthElements(image_annotation.getWidth());
            gui_frame.setDefaultWidth(image_annotation.getWidth());
            gui_frame.setSize(image_annotation.getWidth(), 20);
            gui_frame.draw();
            ofSetColor(ofColor::black);
            ofDrawRectangle(0, image_annotation.getHeight(),
                            image_annotation.getWidth(), 18);
            ofSetColor(ofColor::white);
            font_message.drawString(filename_jpg,
                                    0,
                                    image_annotation.getHeight()+14);
            break;
        case MODE_IMAGE_ANNOTATION:
            font_message.drawString(filename_jpg,
                                    0,
                                    image_annotation.getHeight()+14);
            break;
        case MODE_CAMERA_ANNOTATION:
            font_message.drawString(filename_jpg,
                                    0,
                                    image_annotation.getHeight()+14);
            break;
        case MODE_VIDEO_ANNOTATION:
            gui_frame.draw();
            font_message.drawString(filename_jpg,
                                    0,
                                    image_annotation.getHeight()+14);
            
            break;
    }
    
    
    
    gui_basic.draw();
    
    // show cross
    ofSetLineWidth(1.0);
    if( darkmode ){
        ofSetColor(255,0,0,100);
    }
    else{
        ofSetColor(255,255,255,100);
    }
    ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
    ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
    
    // Magnify a mouse hoverd annotation
    if( b_magnify == true ){
        bool is_showing_magnify = false;
        for( int i = 0; i < train.size(); i++ ){
            ofRectangle r_hover = train.at(i).getScaledBB(image_annotation.getWidth(),
                                                          image_annotation.getHeight());
            if( r_hover.inside(ofGetMouseX(), ofGetMouseY())){
                ofImage img = image_annotation;
                img.crop(r_hover.x,r_hover.y, r_hover.width, r_hover.height);
                ofSetColor(255);
                ofFill();
                if( ofGetMouseY() > image_annotation.getHeight()/2 ){
                    ofDrawRectangle(ofGetMouseX()-5+10, ofGetMouseY()-5-r_hover.height*magnify,
                                    r_hover.width*magnify+10, r_hover.height*magnify+10);
                    img.draw(ofGetMouseX()+10,ofGetMouseY()-r_hover.height*magnify,
                             r_hover.width*magnify, r_hover.height*magnify);
                }
                else{
                    ofDrawRectangle(ofGetMouseX()-5+10, ofGetMouseY()-5,
                                    r_hover.width*magnify+10, r_hover.height*magnify+10);
                    img.draw(ofGetMouseX()+10, ofGetMouseY(),
                             r_hover.width*magnify, r_hover.height*magnify);
                }
                ofNoFill();
                is_showing_magnify = true;
            }
            
        }
    }
    if( b_magnify_pointing ){
        ofRectangle r;
        r.set(0,0,
              image_annotation.getWidth(),
              image_annotation.getHeight());
        if( r.inside(ofGetMouseX(), ofGetMouseY()) ){
            ofImage img = image_annotation;
            float size = 200.0;
            float scale = 3.5*magnify;
            float crop_size = size/scale;
            ofSetColor(255);
            ofFill();
            img.crop(mouseX-crop_size/2, mouseY-crop_size/2, crop_size, crop_size);
            ofDrawRectangle(mouseX-size/2-5,mouseY-size/2-5, size+10, size+10);
            img.draw(mouseX-size/2,mouseY-size/2, size, size);
            ofSetColor(color_crossbar);
            ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
            ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
            ofHideCursor();
        }
    }
    else{
        ofShowCursor();
    }
    
    
    if( mode_annotation == MODE_SELECT_ANNOTATION ){
        static unsigned long timestamp = 0;
        ofFill();
        ofSetColor(0,0,0,200);
        ofDrawRectangle(0,0,ofGetWidth(), ofGetHeight());
        ofNoFill();
        ofSetColor(255);
        String str = "Drag and drop your file to start ";
        if( ofGetSeconds() %2 == 0)str += ">";
        
        str+="\n\n";
        str += "[README]\n\nSingle image annotation: Drag and Drop an image.\n - .jpg is only allowd.\nImage directory annotation: Drag and Drop a directory.\n - needs to include .jpg and .txt(yolo) file.\nCamera annotation: press 'c' key.\nVideo file annotation: Drag and Drop a video file.\n - .mov and .mp4 are allowd.\n\nIf you wanna replace yolov2 files, press 'o' to open a data directory, \nthen restart this app.\n\nAuthor:Tetsuaki Baba, https://tetsuakibaba.jp, 2019\nLisence:MIT License";
        ofRectangle r = font_message.getStringBoundingBox(str,0,0);
        font_message.drawString(str, ofGetWidth()/2-r.getWidth()/2, ofGetHeight()/2-r.getHeight()/2);
    }
}
void ofxOpenCvDnnObjectDetection::updateControls()
{
    fps = ofGetFrameRate();
  
    setNetworkImageSize(network_image_size, network_image_size);
    gui_basic.setPosition(getClassSelectorBoundingBox().getX(),
                          getClassSelectorBoundingBox().getHeight()+
                          getClassSelectorBoundingBox().getY()+20);
    gui_basic.setSize(r_class_selector.getWidth(),18);
    gui_basic.setWidthElements(r_class_selector.getWidth());
    gui_basic.setDefaultWidth(r_class_selector.getWidth());
    
    
    switch( mode_annotation )
    {
        case MODE_CAMERA_ANNOTATION:
            if( train.size() >  0 )flg_pause_camera = true;
            
            if( flg_pause_camera == false ){
                camera.update();
                if( camera.isFrameNew() ){
                    image_annotation.setFromPixels(camera.getPixels());
                    if( flg_show_yolo_detection)update(camera.getPixels());
                }
            }
            break;
            
        case MODE_VIDEO_ANNOTATION:
            
            video.update();
            if( video.isFrameNew() ){
                image_annotation.setFromPixels(video.getPixels());
                if( flg_show_yolo_detection){
                    update(video.getPixels());
                }
                
                seekbar.removeListener(this, &ofxOpenCvDnnObjectDetection::changeSeekbar);
                seekbar = video.getCurrentFrame();
                seekbar.addListener(this, &ofxOpenCvDnnObjectDetection::changeSeekbar);
                
            }
            break;
    }
    
    // dnn detection checker, it runs one by one step for image check.
    if( b_ai_checker && mode_annotation == MODE_DIR_IMAGE_ANNOTATION){
        if( !checkAnnotationImage(threshold_precision) ){
            b_ai_checker = false;
        }
        else{
            if( seekbar < seekbar.getMax() )seekbar++;
            if( seekbar == seekbar.getMax())b_ai_checker = false;
        }
    }
    else if( b_ai_checker ){
        ofSystemAlertDialog("Auto annotation checker runs on only IMAGE DIRECTORY ANNOTATION MODE. Please drag and drop your annotation directory on this window to run it.");
        b_ai_checker = false;
    }
}
void ofxOpenCvDnnObjectDetection::update(ofPixels &op)
{
//    setConfidenceThreshold(threshold);
    object.clear();
    
    cv::Mat frame = toCV(op);
    input_width = (int)op.getWidth();
    input_height = (int)op.getHeight();
    
    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2RGB);
    
    //! [Resizing without keeping aspect ratio]
    Mat resized;
    resize(frame, resized, cv::Size(network_width, network_height));
    
    //! [Prepare blob]
    Mat inputBlob = blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images
    

    //! [Set input blob]
    //net.setInput(inputBlob, "data");                   //set the network input
    net.setInput(inputBlob);
    
    std::vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));
    
    postprocess(frame, outs, net);
}

void ofxOpenCvDnnObjectDetection::setupAnnotationGui()
{
  
    gui_frame.setup();
    gui_frame.setName("Seek Bar");
    gui_frame.add(seekbar.set("frame", 0));
    threshold.addListener(this, &ofxOpenCvDnnObjectDetection::changeThreshold);
    seekbar.addListener(this, &ofxOpenCvDnnObjectDetection::changeSeekbar);
    darkmode.addListener(this, &ofxOpenCvDnnObjectDetection::changeDarkMode);
    button_forward.addListener(this, &ofxOpenCvDnnObjectDetection::forwardAnnotationButton);
    button_back.addListener(this, &ofxOpenCvDnnObjectDetection::backAnnotationButton);
    
    gui_basic.setup();
    gui_basic.setName("Settings");
    gui_basic.loadFont(ofToDataPath("font/DIN Alternate Bold.ttf"), 10);
    gui_basic.setPosition(20,20);
    
    gui_basic.setSize(r_class_selector.getWidth(),18);
    gui_basic.setWidthElements(r_class_selector.getWidth());
    gui_basic.setDefaultWidth(r_class_selector.getWidth());
    gui_basic.add(fps.set("FPS", 30, 0, 60));
    gui_basic.add(threshold.set("Threshold", 0.4, 0.01, 1.0));
    gui_basic.add(network_image_size.set("Network Image", 416, 64,416));
    gui_basic.add(darkmode.setup("Dark Mode[d]", false));
    gui_basic.add(b_magnify.setup("Magnify Annotation [Shift]", false));
    gui_basic.add(b_magnify_pointing.setup("Magnify Pointing [Cmd]",false));
    gui_basic.add(magnify.set("Magnify Scale",1.0, 1.0, 4.0));
    //    gui_basic.add(text_search_id.set("Search Class ID", "all"));
    gui_basic.add(button_forward.setup("Forward Annotation Image [f]"));
    gui_basic.add(button_back.setup("Back Annotation Image [b]"));
    gui_basic.add(b_ai_checker.setup("Run Auto Annotation Checker", false));
    gui_basic.add(threshold_precision.set("Precision Threshold", 0.8, 0.0, 1.0));
    enableAnnotationControl();
}
void ofxOpenCvDnnObjectDetection::setup(string _path_to_cfg, string _path_to_weights, string _path_to_names)
{
    mode_annotation = MODE_SELECT_ANNOTATION;
    flg_pause_camera = true;
    
    filepath = ofToDataPath("");
    filename = "test";
    filename_jpg = "test.jpg";
    filename_txt = "test.txt";
    
    font_message.load(ofToDataPath("font/DIN Alternate Bold.ttf"), 12);
    font_info.load(ofToDataPath("font/DIN Alternate Bold.ttf"), 12);
    font_debug.load(ofToDataPath("font/DIN Alternate Bold.ttf"), 8);
    
    
//    putenv("OPENCV_OPENCL_RUNTIME=");
//    putenv("OPENCV_OPENCL_DEVICE=:DGPU:0");
    str_debug+="[DEBUG]\n";
//    ocl::setUseOpenCL( true );
    
    String modelConfiguration = _path_to_cfg;
    String modelBinary = _path_to_weights;
    
    //! [Initialize

    net = readNet(modelConfiguration, modelBinary);
//    net = readNetFromModelOptimizer(modelConfiguration, modelBinary);

    str_debug+=ofSystem("system_profiler SPHardwareDataType");
    str_debug+="classlist file: "+_path_to_names+"\n";
    str_debug+="cfg file: "+_path_to_cfg+"\n";
    str_debug+="weight file: "+_path_to_weights+"\n";
    
    std::vector<String> lname = net.getLayerNames();
    for (int i = 0; i < lname.size();i++) {
        std::cout << i+1 << " " << lname[i] << std::endl;
    }


    //net.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    // net.setPreferableBackend(DNN_BACKEND_DEFAULT);
    //    net.setPreferableBackend(DNN_BACKEND_HALIDE);
    //      net.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    //net.setPreferableTarget(DNN_TARGET_CPU);
    //            net.setPreferableTarget(DNN_TARGET_OPENCL);
    //    net.setPreferableTarget(DNN_TARGET_OPENCL_FP16);
    


    if (net.empty())
    {
        cout << "Can't load network by using the following files: " << endl;
        cout << "cfg-file:     " << modelConfiguration << endl;
        cout << "weights-file: " << modelBinary << endl;
    }
    
    // objectClassName
    cout << _path_to_names << endl;
    ifstream classNamesFile(_path_to_names);
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className)){
            classNamesVec.push_back(className);
        }
        
        for( auto itr : classNamesVec )
        {
            string cName = itr;
            //cout << "classNames :" << cName << endl;
        }
    }
    
    // set default Detection Color
    setAnnotationColorMode(ANNOTATION_COLOR_MODE_DEFAULT);
    
    confidenceThreshold = 0.4;
    class_id_selected = 0; // default
    
    
}

void ofxOpenCvDnnObjectDetection::setAnnotationColorMode(int _mode)
{
    
    detection_color.clear();
    if( _mode == ANNOTATION_COLOR_MODE_DEFAULT ){
        for( int i = 0; i < classNamesVec.size(); i++ ){
            detection_color.push_back(ofColor(100*((i+1)%2)+100,
                                              50*((i+2)%3)+100,
                                              25*((i+3)%4)+100,
                                              255));
        }
    }
    else{
        for( int i = 0; i < classNamesVec.size(); i++ ){
            detection_color.push_back(ofColor(100*((i+1)%2),
                                              50*((i+2)%3),
                                              25*((i+3)%4),
                                              255));
        }
    }
}
void ofxOpenCvDnnObjectDetection::setConfidenceThreshold(float _threshold)
{
    if( 0.0 <= _threshold && _threshold <= 1.0){
        confidenceThreshold = _threshold;
    }
}

void ofxOpenCvDnnObjectDetection::setNetworkImageSize(int _w, int _h)
{
    network_width = _w;
    network_height = _h;
}


void ofxOpenCvDnnObjectDetection::loadBoundingBoxFile(string _path_to_file)
{
    train.clear();
    vector<string>str_bb;
    
    if( !ofFile::doesFileExist(_path_to_file) ){
        cout << "no such a file" << endl;
        ofSystem("touch "+_path_to_file);
    }
    ofBuffer ofbuf = ofBufferFromFile(_path_to_file);

    ofbuf.getLines();
    for( auto line: ofbuf.getLines() ){
        str_bb.push_back(line);
    }
    
    for( int j = 0; j < str_bb.size(); j++ ){
        auto string = str_bb[j];
        auto separator = std::string(" ");
        auto separator_length = separator.length();
        auto list = std::vector<std::string>();
        
        if (separator_length == 0) {
            list.push_back(string);
        }
        else{
            auto offset = std::string::size_type(0);
            while (1) {
                auto pos = string.find(separator, offset);
                if (pos == std::string::npos) {
                    list.push_back(string.substr(offset));
                    break;
                }
                list.push_back(string.substr(offset, pos - offset));
                offset = pos + separator_length;
            }
        }
        
        
        if( list.size() == 5 ){
            float x,y,w,h;
            x = ofToFloat(list[1]);
            y = ofToFloat(list[2]);
            w = ofToFloat(list[3]);
            h = ofToFloat(list[4]);
            x = x -w/2.0;
            y = y -h/2.0;
            ofRectangle r(x,y,w,h);
            TrainObject t(ofToInt(list[0]), classNamesVec.at(ofToInt(list[0])), r);
            train.push_back(t);
        }
    }
}

void ofxOpenCvDnnObjectDetection::saveAnnotation()
{
    saveBoundingBoxToFile(filename_txt);
    if( !ofFile::doesFileExist(filename_jpg) ){
        saveAnnotationImage();
    }
}
void ofxOpenCvDnnObjectDetection::saveAnnotationImage()
{
    ofSaveImage(image_annotation, filename_jpg, OF_IMAGE_QUALITY_BEST);
}
void ofxOpenCvDnnObjectDetection::removeAnnotationFiles()
{
    ofSystem("rm "+filename_jpg);
    ofSystem("rm "+filename_txt);
}
void ofxOpenCvDnnObjectDetection::loadAnnotationDir(string _path_to_file)
{
    train.clear();
    filepath = _path_to_file;
    pos_annotation_file = 0;
    dir_annotation.open(filepath);
    dir_annotation.allowExt("jpg");
    dir_annotation = dir_annotation.getSorted();

    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");

    // Load first jpg image
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}
void ofxOpenCvDnnObjectDetection::loadAnnotationImage(string _path_to_file)
{
    image_annotation.load(_path_to_file);
    update(image_annotation.getPixels());
}

void ofxOpenCvDnnObjectDetection::saveBoundingBoxToFile(string _path_to_file)
{
    ofFile file;
    if( file.open(_path_to_file, ofFile::WriteOnly) ){
        cout << filename_txt << endl;
        for( int i = 0; i < train.size(); i++){
            train[i].r.standardize();
            file <<
            train[i].id << " " <<
            train[i].r.x + train[i].r.getWidth()/2.0 << " " <<
            train[i].r.y + train[i].r.getHeight()/2.0 << " " <<
            train[i].r.getWidth() << " " <<
            train[i].r.getHeight() << endl;
        }
        file.close();
        train.clear();
        loadBoundingBoxFile(_path_to_file);
    }
    else{
        cout << "Failed: _path_to_file: " + _path_to_file << endl;
    }
    
}

void ofxOpenCvDnnObjectDetection::setAnnotationFilename(string _filename)
{
    filename = _filename;
    filename_jpg = filename+".jpg";
    filename_txt = filename+".txt";
}
void ofxOpenCvDnnObjectDetection::setNextAnnotation()
{
    if( dir_annotation.size() == 0 )return;
    pos_annotation_file++;
    if( pos_annotation_file > dir_annotation.size()-1 ){
        pos_annotation_file = dir_annotation.size()-1;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxOpenCvDnnObjectDetection::setPreviousAnnotation()
{
    if( dir_annotation.size() == 0 )return;
    pos_annotation_file--;
    if( pos_annotation_file < 0 ){
        pos_annotation_file=0;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxOpenCvDnnObjectDetection::adjustGuiComponents()
{
    float w, h;
   
    
    if( image_annotation.getHeight()+20*2 < r_class_selector.getHeight()+gui_basic.getHeight()+20*3 ){
        h = r_class_selector.getHeight()+gui_basic.getHeight()+20*3;
    }
    else{
        h = image_annotation.getHeight()+20*2;
    }
    ofSetWindowShape(image_annotation.getWidth()+getClassSelectorBoundingBox().getWidth()+20+20,h);

}
void ofxOpenCvDnnObjectDetection::setPositionAnnotation(int _pos, string _text_search_id)
{
    if( _text_search_id == "all"){
        // do nothing
    }
    else{
        vector<string>search_id = split(_text_search_id, ',');
        //here
    }
    
    pos_annotation_file = _pos;
    if( pos_annotation_file < 0 ){
        pos_annotation_file=0;
    }
    if( pos_annotation_file >= dir_annotation.size())pos_annotation_file = dir_annotation.size()-1;
    
    
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
    adjustGuiComponents();
    
}
void ofxOpenCvDnnObjectDetection::drawClassSelector(float _x, float _y, int _row)
{
    // Show Class Selector
    int row = _row;
    float x,y;
    
    float w_max = 0;
    for( int i = 0; i < classNamesVec.size(); i++ ){
        float w_tmp = font_info.getStringBoundingBox(" ["+ofToString(i)+"]"+classNamesVec.at(i)+" ", 0, 0).getWidth();
        if( w_tmp > w_max ){
            w_max = w_tmp;
        }
    }
    r_class_selector.set(_x, _y,  w_max*row, 16*classNamesVec.size()/row+16);

    ofNoFill();
    ofDrawRectangle(r_class_selector);
    font_info.drawString("Select a class name",r_class_selector.x,r_class_selector.y);
    for( int i = 0; i < classNamesVec.size(); i++ ){
        x = _x+w_max*(i%row);
        y = _y+16+16*(i/row);
        ofNoFill();
        ofSetColor(detection_color.at(i));
        ofRectangle r_name;
        r_name.set(font_info.getStringBoundingBox(" ["+ofToString(i)+"]"+classNamesVec.at(i)+"  ",
                                                  x,y));

        if( r_name.inside(ofGetMouseX(),ofGetMouseY()) ){
            if( ofGetMousePressed() ){
                class_id_selected = i;
            }
            else{
                ofDrawRectangle(r_name);
            }
        }
        
        if( class_id_selected == i ){
            ofFill();
            ofDrawRectangle(r_name);
            
            ofSetColor(detection_color.at(i).getInverted());
            font_info.drawString(" ["+ofToString(i)+"]"+classNamesVec.at(i),x,y);
                                 
        }
        else{
            font_info.drawString(" ["+ofToString(i)+"]"+classNamesVec.at(i),x,y);
        }
    }
    
}

void ofxOpenCvDnnObjectDetection::addTrainObject(int _class_id_selected, ofRectangle _r)
{
    class_id_selected = _class_id_selected;
    train.push_back(TrainObject(class_id_selected, classNamesVec.at(class_id_selected), _r));
}


std::vector<String> ofxOpenCvDnnObjectDetection::getOutputsNames(const Net& net)
{
    static std::vector<String> names;
    if (names.empty())
    {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<String> layersNames = net.getLayerNames();
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
        names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

ofRectangle ofxOpenCvDnnObjectDetection::getClassSelectorBoundingBox()
{
    return r_class_selector;
}

bool ofxOpenCvDnnObjectDetection::checkAnnotationImage(float _threshold_area)
{
    update(image_annotation.getPixels());
    for( int i = 0; i < object.size(); i++ ){
        for( int j = 0; j < train.size(); j++ ){
            // What is precision?: https://github.com/AlexeyAB/darknet
            float precision = object[i].r.getIntersection(train[j].r).getArea()/object[i].r.getArea();
            if( precision > _threshold_area){
                if( object[i].class_id != train[j].id ){
                    r_reconfirm = object[i].r.getIntersection(train[j].r);
                    r_reconfirm_alpha = 255;
                    str_debug = "Found a annotation to be reconfirmed.\n";
                    str_debug += " Train:"+train[j].name+" and Detect: "+object[i].name+"\n";
                    str_debug += " Precision: "+ofToString(precision)+"\n";
                    return false;
                }
            }
        }
    }

    
    r_reconfirm.set(0,0,0,0);
    return true;
}

void ofxOpenCvDnnObjectDetection::drawReconfirmAnnotation()
{
    // Show Bounding Box to be reconfirmed.
    
    ofSetColor(ofColor::darkRed, r_reconfirm_alpha);
    ofFill();
    float scale_x, scale_y;
    scale_x = image_annotation.getWidth();
    scale_y = image_annotation.getHeight();
    ofDrawRectangle(r_reconfirm.x*scale_x,
                    r_reconfirm.y*scale_y,
                    r_reconfirm.getWidth()*scale_x,
                    r_reconfirm.getHeight()*scale_y);

    if( r_reconfirm_alpha > 0 )r_reconfirm_alpha=r_reconfirm_alpha-2;
    
    
}

void ofxOpenCvDnnObjectDetection::changeThreshold(float &_th)
{
    if( image_annotation.isAllocated() ){
      update(image_annotation.getPixels());
    }
}
void ofxOpenCvDnnObjectDetection::changeSeekbar(int &_frame)
{
    r_reconfirm_alpha = 0;
    if( _frame > seekbar.getMax() )return;
    if( _frame < 0)return;
    if( mode_annotation == MODE_VIDEO_ANNOTATION){
        video.setFrame(_frame);
        train.clear();
        setAnnotationFilename(path+ofGetTimestampString());
    }
    else if( mode_annotation == MODE_DIR_IMAGE_ANNOTATION){
        train.clear();
        setPositionAnnotation(_frame, text_search_id);
    }
}

void ofxOpenCvDnnObjectDetection::changeDarkMode(bool & _mode)
{
    setAnnotationColorMode(_mode);

    if( _mode == true )color_crossbar.set(ofColor::darkRed);
    if( _mode == false )color_crossbar.set(ofColor::white);
}

void ofxOpenCvDnnObjectDetection::changeTextSearchID(string _str)
{
    cout << "Edited" <<endl;
}

void ofxOpenCvDnnObjectDetection::forwardAnnotationButton()
{
    if( seekbar < seekbar.getMax() )seekbar++;
}

void ofxOpenCvDnnObjectDetection::backAnnotationButton()
{
    if( seekbar > 0 )seekbar--;
}

void ofxOpenCvDnnObjectDetection::mouseMoved(ofMouseEventArgs &e)
{
    float x = e.x;
    float y = e.y;
    if( flg_show_yolo_detection ){
        vector<Object>obj =  object;
        for( int i = 0; i < obj.size(); i++ ){
            if( obj[i].getScaledBB(image_annotation.getWidth(),
                                   image_annotation.getHeight()).inside(x,y)){
                class_id_selected =  obj[i].class_id;
            }
        }
    }
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::mouseDragged(ofMouseEventArgs &e)
{
    float x = e.x;
    float y = e.y;
    is_dragging = true;
    ofRectangle r_img;
    float w,h;
    switch( mode_annotation )
    {
    }
    r_img.set(0,0, image_annotation.getWidth(), image_annotation.getHeight());
    if( r_img.inside(x,y) ){
        r.width = x-r.x;
        r.height = y-r.y;
    }
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::mousePressed(ofMouseEventArgs &e)
{
    float x = e.x;
    float y = e.y;
    r.set(x,y,0,0);
    
    if( flg_show_yolo_detection == false ){
        for( int i = 0; i < train.size(); i++ ){
            if( train.at(i).getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()).inside(x,y)){
                train.erase(train.begin()+i);
                saveAnnotation();
                if(mode_annotation == MODE_VIDEO_ANNOTATION ||
                   mode_annotation == MODE_CAMERA_ANNOTATION ){
                    if( train.size() == 0 ){
                        removeAnnotationFiles();
                    }
                }
            }
        }
    }
    else{
        for( int i = 0; i < object.size(); i++ ){
            if( object.at(i).getScaledBB(image_annotation.getWidth(),                                              image_annotation.getHeight()).inside(x,y) ){
                
                
                addTrainObject(object.at(i).class_id,
                               object.at(i).r);
                saveAnnotation();
            }
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::mouseReleased(ofMouseEventArgs &e)
{
    float x = e.x;
    float y = e.y;
    is_dragging = false;
    r.width = x-r.x;
    r.height = y-r.y;
    r.standardize();
    
    if( mode_annotation == MODE_CAMERA_ANNOTATION ){
        if( train.size() == 0 && r.getArea() > 10){
            setAnnotationFilename(path+ofGetTimestampString());
        }
    }
    
    if( mode_annotation == MODE_VIDEO_ANNOTATION ){
        if( train.size() == 0 &&  r.getArea() > 10){
            setAnnotationFilename(path+ofGetTimestampString());
        }
    }
    
    if( mode_annotation == MODE_AUDIO_ANNOTATION){
        
    }
    
    
    ofRectangle r_img;
    r_img.set(0,0, image_annotation.getWidth(), image_annotation.getHeight());
    if( r.getArea() > 10 &&  r_img.inside(x,y) ){
        r.x = r.x/image_annotation.getWidth();
        r.y = r.y/image_annotation.getHeight();
        r.width = r.width/image_annotation.getWidth();
        r.height = r.height/image_annotation.getHeight();
        
        addTrainObject(class_id_selected, r);
        saveAnnotation();
    }
    r.set(0,0,0,0);
    
    
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::mouseEntered(ofMouseEventArgs &e)
{
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::mouseExit(ofMouseEventArgs &e)
{
}

//--------------------------------------------------------------
void ofxOpenCvDnnObjectDetection::dragEvent(ofDragInfo &dragInfo){
    ofDirectory dir;
    dir.open(dragInfo.files[0]);
    str_debug = "Drag Event:\n "+dragInfo.files[0];
   
    
    
    if( dir.isDirectory() ){
        if( video.isLoaded() )  video.closeMovie();
        loadAnnotationDir(dragInfo.files[0]);
        mode_annotation = MODE_DIR_IMAGE_ANNOTATION;
        gui_frame.setPosition(0,image_annotation.getHeight());
        gui_frame.setSize(image_annotation.getWidth(), 20);
        gui_frame.setDefaultWidth(image_annotation.getWidth());
        gui_frame.setWidthElements(image_annotation.getWidth());
        seekbar.set("frame", 0, 0, dir_annotation.size()-1);
        adjustGuiComponents();
        dir.close();
    }
    else if( doesIncludeExtensions(dragInfo.files[0], {".mov", ".mp4"}) ){
        if( video.isLoaded() ){
            video.closeMovie();
        }
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a saving directory");
        if( !file.bSuccess )return;
        path = file.getPath();
        setAnnotationFilename(file.getPath()+ofGetTimestampString());
        
        if( video.isLoaded() )video.close();
        video.load(dragInfo.files[0]);
        mode_annotation = MODE_VIDEO_ANNOTATION;
        seekbar.set("frame", 0, 0, video.getTotalNumFrames());
        gui_frame.setPosition(0,video.getHeight());
        gui_frame.setSize(video.getWidth(), 20);
        gui_frame.setDefaultWidth(video.getWidth());
        gui_frame.setWidthElements(video.getWidth());
        video.play();

        image_annotation.setFromPixels(video.getPixels());
        adjustGuiComponents();
        flg_pause_video = false;

    }
    else if( doesIncludeExtensions(dragInfo.files[0], {".jpg"})){
        if( video.isLoaded() ){
            video.closeMovie();
        }
        string str = dragInfo.files[0];
        str.erase(str.begin()+str.length()-1);
        str.erase(str.begin()+str.length()-1);
        str.erase(str.begin()+str.length()-1);
        str.erase(str.begin()+str.length()-1);
        setAnnotationFilename(str);
        mode_annotation = MODE_IMAGE_ANNOTATION;
        loadAnnotationImage(filename_jpg);
        loadBoundingBoxFile(filename_txt);
        adjustGuiComponents();
        
    }
    else if( doesIncludeExtensions(dragInfo.files[0], {".wav", "WAV"})){
        if( video.isLoaded() ){
            video.closeMovie();
        }
        ofFileDialogResult file = ofSystemSaveDialog("myAnnotation_", "Choose a saving directory");
        if( !file.bSuccess )return;
        path = file.getPath();
        setAnnotationFilename(file.getPath()+ofGetTimestampString());
        mode_annotation = MODE_AUDIO_ANNOTATION;
    }
    else{
        ofSystemAlertDialog("Error: Filetype is not allowed.");
    }
    dir.close();
}
