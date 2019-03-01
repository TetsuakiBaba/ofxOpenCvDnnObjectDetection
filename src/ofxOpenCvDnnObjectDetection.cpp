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


ofxOpenCvDnnObjectDetection::ofxOpenCvDnnObjectDetection()
{
    
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


ofxOpenCvDnnObjectDetection::~ofxOpenCvDnnObjectDetection()
{
    
}

void ofxOpenCvDnnObjectDetection::draw(float _x, float _y, float _w, float _h)
{
    for( int i = 0; i < object.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(object[i].class_id));
        ofRectangle r_scaled = object.at(i).getScaledBB(_w, _h);
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

void ofxOpenCvDnnObjectDetection::update(ofPixels &op)
{
    object.clear();
    
    cv::Mat frame = toCV(op);
    input_width = (int)op.getWidth();
    input_height = (int)op.getHeight();
    
    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2RGB);
    
    //! [Resizing without keeping aspect ratio]
    Mat resized;
    resize(frame, resized, cvSize(network_width, network_height));
    
    //! [Prepare blob]
    Mat inputBlob = blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images
    

    //! [Set input blob]
    //net.setInput(inputBlob, "data");                   //set the network input
    net.setInput(inputBlob);
    
    std::vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));
    
    postprocess(frame, outs, net);
}

void ofxOpenCvDnnObjectDetection::setup(string _path_to_cfg, string _path_to_weights, string _path_to_names)
{
//    putenv("OPENCV_OPENCL_RUNTIME=");
//    putenv("OPENCV_OPENCL_DEVICE=:DGPU:0");
    //cv::ocl::setUseOpenCL( true );
    
    String modelConfiguration = _path_to_cfg;
    String modelBinary = _path_to_weights;
    
    //! [Initialize network]
    net = readNet(modelConfiguration, modelBinary);
    std::vector<String> lname = net.getLayerNames();
    for (int i = 0; i < lname.size();i++) {
        std::cout << i+1 << " " << lname[i] << std::endl;
    }
    net.setPreferableBackend(DNN_BACKEND_DEFAULT);
    //    net.setPreferableBackend(DNN_BACKEND_HALIDE);
    //      net.setPreferableBackend(DNN_BACKEND_INFERENCE_ENGINE);
    net.setPreferableTarget(DNN_TARGET_CPU);
    //        net.setPreferableTarget(DNN_TARGET_OPENCL);
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
    for( int i = 0; i < classNamesVec.size(); i++ ){
        detection_color.push_back(ofColor(100*((i+1)%2)+100,
                                          50*((i+2)%3)+100,
                                          25*((i+3)%4)+100,
                                          255));
    }
    
    // for gocen detection color
    /*
    {
        ofColor c[] =
        {
            ofColor(255, 255, 255),
            ofColor(111, 12, 188),
            ofColor(25, 71, 175),
            ofColor(188, 12, 180),
            ofColor(200, 255, 255),
            ofColor(255, 230, 51),
            ofColor(34, 181, 115),
            ofColor(98, 160, 0),
            ofColor(64, 109, 5),
            ofColor(255, 137, 57),
            ofColor(255, 85, 85),
            ofColor(255, 102, 186),
            ofColor(140, 98, 57),
            ofColor(238, 118, 255),
            ofColor(117, 158, 255)
        };
        
        for( int i = 0; i < classNamesVec.size(); i++){
            detection_color.push_back(c[i]);
        }
    }
     */
    
    font_info.load(ofToDataPath("DIN Alternate Bold.ttf"),12);
    confidenceThreshold = 0.4;
    class_id_selected = 0; // default 
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

void ofxOpenCvDnnObjectDetection::setNextAnnotation()
{
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
    pos_annotation_file--;
    if( pos_annotation_file < 0 ){
        pos_annotation_file=0;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxOpenCvDnnObjectDetection::drawClassSelector(float _x, float _y, int _row)
{
    // Show Class Selector
    int row = _row;
    float x,y;
    
    float w_max = 0;
    for( int i = 0; i < classNamesVec.size(); i++ ){
        float w_tmp = font_info.getStringBoundingBox(classNamesVec.at(i), 0, 0).getWidth();
        if( w_tmp > w_max ){
            w_max = w_tmp;
        }
    }
    ofRectangle r_area(_x,
                       _y,
                       w_max*row,
                       14*classNamesVec.size()/row+14);

    ofNoFill();
    ofDrawRectangle(r_area);
    font_info.drawString("Select a class name",r_area.x,r_area.y);
    for( int i = 0; i < classNamesVec.size(); i++ ){
        x = _x+w_max*(i%row);
        y = _y+14+14*(i/row);
        ofNoFill();
        ofSetColor(detection_color.at(i));
        ofRectangle r_name;
        r_name.set(font_info.getStringBoundingBox(classNamesVec.at(i),
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
            font_info.drawString(classNamesVec.at(i),x,y);
                                 
        }
        else{
            font_info.drawString(classNamesVec.at(i),x,y);
        }
    }
    
}

void ofxOpenCvDnnObjectDetection::addTrainObject(ofRectangle _r)
{
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

