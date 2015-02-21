// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::exit()
{
    // Set the logger back to the default to make sure any
    // remaining messages are logged correctly.
    ofLogToConsole();

    /*
    string tp;

    tp = files[3].getAbsolutePath();

    files[3].close();

    int ti = remove( files[3].getAbsolutePath().c_str() );

    if( ti != 0 )
         ofLogVerbose("Error deleting file: " ) << ti;
    else
         ofLogVerbose("File successfully deleted" );
    */


    for(int i = 0; i < (int)files.size(); i++){

        if((files[i].isDirectory())!=true){
                string fn;
                fn = files[i].getAbsolutePath();

                  std::stringstream ss;
                    for (int i = 0; i < fn.length(); ++i) {
                if (fn[i] == '\\') {
                ss << "\\\\";
                }
                 else {
                   ss << fn[i];
                 }
                }
                files[i].close();

                ofLogVerbose("Path" ) << ss.str();

                int ti = std::remove( ss.str().c_str() );

                if( ti != 0 )
                    perror("\tError deleting file");
                else
                    ofLogVerbose("File successfully deleted: " ) << ofToString(i);
        }
        else
        {

                ofDirectory tmpDir;
                string fn;
                fn = files[i].getAbsolutePath();
                tmpDir.listDir(fn);
                string tp;
                tp = tmpDir.getAbsolutePath();

                vector<ofFile> tmpfiles = tmpDir.getFiles();

                for(int i = 0; i < (int)tmpfiles.size(); i++){

                                string fn;
                        fn = tmpfiles[i].getAbsolutePath();

                          std::stringstream ss;
                            for (int i = 0; i < fn.length(); ++i) {
                        if (fn[i] == '\\') {
                        ss << "\\\\";
                        }
                         else {
                           ss << fn[i];
                         }
                        }
                        tmpfiles[i].close();

                        ofLogVerbose("Path" ) << ss.str();

                        int ti = std::remove( ss.str().c_str() );

                        if( ti != 0 )
                            perror("\tError deleting file");
                        else
                            ofLogVerbose("File successfully deleted: " ) << ofToString(i);

                }

                tmpDir.remove(false);



        }





    }




}

//--------------------------------------------------------------
void ofApp::setup()
{

    /// ZIP

    zipped = false;
    ziptimer=0;


    ///ZIP





    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    textarea="initiated";

	/*
    // Set up our camera
    cam.listDevices();
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(60);
    cam.initGrabber(640, 480);


    // Init FBO
    fbo.allocate(cam.getWidth(), cam.getHeight());
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();

    // Init image
    img.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);
    */


    // JSON-RPC server
    initServerJSONRPC(8197);

    // Video server
    initServerVideo(7890);

    // Launch a browser with the address of the server.
    ofLaunchBrowser(serverJSON->getURL());
}

void ofApp::update()
{

    if(zipped)        ziptimer++;

    if(ziptimer>30) {
     ePubList();
     ziptimer=0; zipped=false;}

/*
 // Update the video grabber.
    //cam.update();

    // If the frame is new, then send it to the server to be broadcast.
 //   if(cam.isFrameNew())
 //   {
        fbo.begin();
        ofClear(255,255,255, 0);

        // Draw the video on the screen.
  //      cam.draw(0,0);


  std::stringstream ss;

        ss << "Text: " << userText << endl;
        ss << "Slider: " << sliderValue << endl;
        ss << "Checkbox: " << checkboxValue << endl;

        ofDrawBitmapStringHighlight(ss.str(), 20, 20);

        fbo.end();

        // Put the FBO into an ofImage
        fbo.readToPixels(img.getPixelsRef());
        img.update();
//    }
*/
//


}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);

    std::stringstream ss;

        ss << "Text: " << userText << endl;
        ss << "Slider: " << sliderValue << endl;
        ss << "Checkbox: " << checkboxValue << endl;
     //   ss << "Textarea: " << filecontent << endl;

    ofSetColor(30,30,30);
    ofDrawBitmapString(ss.str(), 20, 20);

    //  img.draw(0, 0);
    /*
    if(serverVideo->isRunning()) {
        // This can be any kind of pixels.
        serverVideo->send(img.getPixelsRef());
    }
    */

    ///ZIP
    ofSetColor(255,255,255);
    ofDrawBitmapString("Z for unzip!", 300,20);

    if(zipped){
    ofDrawBitmapString("ePub has been unzipped!", 300,35);
    }


    ofSetColor(255,0,0);
    ofDrawBitmapString(currentFilename, 50,25);
     ofDrawBitmapString(ofToString(currentFile), 30,25);

    /// DIR

    if (dir.size() > 0){
            ofSetColor(ofColor::white);
            ofSetColor(ofColor::gray);


            /*
            for(int i = 0;i<seussLines.size();i++)
            {
                 filecontent=filecontent + seussLines[i] + "\n";
            }
            */


            /*

            string pathInfo = dir.getName(currentFile) + " " + dir.getPath(currentFile) + "\n\n" +
                filecontent;
            ofSetColor(10,10,70);
            ofDrawBitmapString(pathInfo, 300, 80);
            */


        ofSetColor(ofColor::gray);
        for(int i = 0; i < (int)dir.size(); i++){
            if(i == currentFile) {
                ofSetColor(ofColor::red);
            }	else {
                ofSetColor(ofColor::gray);
            }
            string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
            ofDrawBitmapString(fileInfo, 50,i * 20 + 80);
        }
    }



    if (files.size() > 0){

           /*
           ofSetColor(40,40,100);
           ofBuffer tempbuffer;
           tempbuffer = ofBufferFromFile(files[currentFile].getAbsolutePath());
           ofDrawBitmapString(tempbuffer.getText(), 20,20);
           */


        for(int i = 0; i < (int)files.size(); i++){
            if(i == currentFile) {
                ofSetColor(ofColor::red);
            }	else {
                ofSetColor(ofColor::gray);
            }
            string fileInfo = "file " + ofToString(i + 1) + " = " + files[i].getFileName();
            ofDrawBitmapString(fileInfo, 300,i * 20 + 80);
        }

    }



    ///ZIP


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // Populate drop-down menu from vector
    if(key == 'd') {
        vector<string> vec;
        vec.push_back("OF item 1");
        vec.push_back("OF item number 2");
        vec.push_back("OF item is the tree");

        // Create vector
        Json::Value params;
        for(int i = 0; i < vec.size(); i++) {
            params.append(vec[i]);
        }

        // Send vector
        Json::Value json = toJSONMethod("Server", "test-dropdown", params);
        sendJSONMessage(json);
    }
    // Empty drop-down menu
    if(key == 'D') {
        Json::Value json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
        sendJSONMessage(json);
    }
    // Checkbox
    else if(key == 'c') {
        Json::Value params;
        params["bool"] = false;

        Json::Value json = toJSONMethod("Server", "test-checkbox", params);
        sendJSONMessage(json);
    }
    // Checkbox
    else if(key == 'C') {
        Json::Value params;
        params["bool"] = true;

        Json::Value json = toJSONMethod("Server", "test-checkbox", params);
        sendJSONMessage(json);
    }

    if(key == 't') {



    }

    if(key == 'T') {

      textarea="woooot";
    }

      if(key=='z'){





    }

    if(key=='+'){

    currentFile++;

    /*
    Json::Value params;
    params["value"] = buffer.getText();

    Json::Value json = toJSONMethod("Server", "textarea", params);
    sendJSONMessage(json);
    */

    Json::Value params;
    params["value"] = files[currentFile].getFileName();
    currentFilename = files[currentFile].getFileName();

    Json::Value json = toJSONMethod("Server", "set-dropdown", params);
    sendJSONMessage(json);

    setTextareaWeb(files[currentFile].getFileName());

    }

     if(key=='-'){

    currentFile--;

    Json::Value params;
    params["value"] = files[currentFile].getFileName();
    currentFilename = files[currentFile].getFileName();

    Json::Value json = toJSONMethod("Server", "set-dropdown", params);
    sendJSONMessage(json);

    setTextareaWeb(files[currentFile].getFileName());

    /*

    Json::Value params;
    params["value"] = buffer.getText();

    Json::Value json = toJSONMethod("Server", "textarea", params);
    sendJSONMessage(json);

    */

    }

    if(key=='f'){

        ePubZip();

    }


}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // Map the horizontal position of the mouse to the slider
    float slider = ofMap(x, 0, ofGetWindowWidth(), 0.0, 1.0);

    Json::Value params;
    params["value"] = slider;

    Json::Value json = toJSONMethod("Server", "test-slider", params);
    sendJSONMessage(json);
}

//--------------------------------------------------------------
void ofApp::getCheckBox(ofx::JSONRPC::MethodArgs& args)
{
    // Set the user text.
    setUserText(args.params.asString());
    setCheckboxValue(args.params.asBool());
    ofLogVerbose("ofApp::getCheckBox") << args.params.asString();
}

//--------------------------------------------------------------
void ofApp::getSlider(ofx::JSONRPC::MethodArgs& args)
{
    // Set the user text.
    setUserText(args.params.asString());
    setSliderValue(ofToFloat(args.params.asString()));
    ofLogVerbose("ofApp::getSlider") << args.params.asString();

}

//--------------------------------------------------------------
void ofApp::getButton()
{
    ofLogVerbose("ofApp::getButton") << "Pushed!";
}

//--------------------------------------------------------------
void ofApp::getDropdown(ofx::JSONRPC::MethodArgs& args)
{
    // Set the user text.
    setUserText(args.params.asString());
    ofLogVerbose("ofApp::getDropdown") << args.params.asString();

    setTextareaWeb(args.params.asString());
    currentFilename = args.params.asString();

    for(int i = 0; i < int(files.size()); i++) {
        if(args.params.asString() == files[i].getFileName()) {

          currentFile = i;


        }
    }

}

void ofApp::setTextareaWeb(string fn)
{
    // Set the user text.

    ofFile t_file(dir.getAbsolutePath() + "/" + fn);



    if((t_file.getExtension()=="html")||(t_file.getExtension()=="opf")||(t_file.getExtension()=="xhtml")){

    filename3 = dir.getAbsolutePath() +"/"+ fn;

    ofLogVerbose("ofApp::SentFilename") << filename3;

    buffer = ofBufferFromFile(filename3);

    Json::Value params2;
    params2["value"] = buffer.getText();
    Json::Value json;
    json = toJSONMethod("Server", "textarea", params2);
    sendJSONMessage(json);
    }
    else{

    Json::Value params2;
    params2["value"] = "Es kann kein Text angezeigt werden";
    Json::Value json;
    json = toJSONMethod("Server", "textarea", params2);
    sendJSONMessage(json);

    }



}

void ofApp::getEpubName(ofx::JSONRPC::MethodArgs& args)
{
   ofScopedLock lock(mutex);

   ofLogVerbose("ofApp::getEpubName") << args.params.asString();

   currentEpubname = args.params.asString();




}


void ofApp::getTextArea2(ofx::JSONRPC::MethodArgs& args)
{
     ofScopedLock lock(mutex);
    // Set the user text.
   // setTextArea(args.params.asString());
   ofLogVerbose("ofApp::getTextArea") << args.params.asString();


   files[currentFile].setWriteable(true);
   ofBuffer tempbuffer;
   tempbuffer.set(args.params.asString());

   string tmppath;

   tmppath = files[currentFile].getAbsolutePath();

   files[currentFile].close();

   ofBufferToFile(tmppath,tempbuffer);


   files[currentFile].open(tmppath,ofFile::Reference);



   ofLogVerbose("Write ") << " to File: " << currentFilename << " Nr.: " << currentFile;

}

//--------------------------------------------------------------
void ofApp::startVideoServer()
{
    // Set the user text.
    ofLogVerbose("ofApp::startVideoServer");

    // Start the video server
    if(!serverVideo->isRunning()) {
        serverVideo->start();
    }
}

//--------------------------------------------------------------
void ofApp::stopVideoServer()
{
    // Set the user text.
    ofLogVerbose("ofApp::stopVideoServer");

    // Stop the video server
    if(serverVideo->isRunning()) {
        serverVideo->stop();
    }
}

//--------------------------------------------------------------
std::string ofApp::getUserText() const
{
    ofScopedLock lock(mutex);
    return userText;
}


//--------------------------------------------------------------
void ofApp::setUserText(const std::string& text)
{
    ofScopedLock lock(mutex);
    userText = text;
}

//--------------------------------------------------------------
float ofApp::getSliderValue() const
{
    ofScopedLock lock(mutex);
    return sliderValue;
}


//--------------------------------------------------------------
void ofApp::setSliderValue(const float& f)
{
    ofScopedLock lock(mutex);
    sliderValue = f;
}

void ofApp::setTextArea(const std::string& text)
{
   ofScopedLock lock(mutex);






}

std::string ofApp::getTextArea() const
{
    ofScopedLock lock(mutex);
    return textarea;
}

//--------------------------------------------------------------
bool ofApp::getCheckboxValue() const
{
    ofScopedLock lock(mutex);
    return checkboxValue;
}


//--------------------------------------------------------------
void ofApp::setCheckboxValue(const bool& b)
{
    ofScopedLock lock(mutex);
    checkboxValue = b;
}




//--------------------------------------------------------------
void ofApp::onHTTPPostEvent(ofx::HTTP::PostEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPPostEvent") << "Data: " << args.getBuffer().getText();
}


//--------------------------------------------------------------
void ofApp::onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& args)
{
    ofLogNotice("ofApp::onHTTPFormEvent") << "";
    ofx::HTTP::HTTPUtils::dumpNameValueCollection(args.getForm(), ofGetLogLevel());
}


//--------------------------------------------------------------
void ofApp::onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& args)
{
    std::string stateString = "";

    switch (args.getState())
    {
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_STARTING:
            stateString = "STARTING";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_PROGRESS:
            stateString = "PROGRESS";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_FINISHED:
            stateString = "FINISHED";
            up_filename=args.getFilename();
            ePubUnzip(up_filename);
            break;
    }

 //   ofLogNotice("ofApp::onHTTPUploadEvent") << "";
 //   ofLogNotice("ofApp::onHTTPUploadEvent") << "         state: " << stateString;
 //   ofLogNotice("ofApp::onHTTPUploadEvent") << " formFieldName: " << args.getFormFieldName();
  //  ofLogNotice("ofApp::onHTTPUploadEvent") << "orig. filename: " << args.getOriginalFilename();
  //  ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      filename: " << args.getFilename();
 //   ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      fileType: " << args.getFileType().toString();
//    ofLogNotice("ofApp::onHTTPUploadEvent") << "# bytes xfer'd: " << args.getNumBytesTransferred();





}


//--------------------------------------------------------------
void ofApp::initServerJSONRPC(int port)
{
    ofx::HTTP::BasicJSONRPCServerSettings settings;
    settings.setPort(port);
    //settings.setUploadRedirect("");
    settings.setMaximumFileUploadSize(2000000 * 1024); // 2 GB

    // Initialize the server.
    serverJSON = ofx::HTTP::BasicJSONRPCServer::makeShared(settings);

    // Must register for all events before initializing server.
//    ofSSLManager::registerAllEvents(this);
    //
    serverJSON->getPostRoute()->registerPostEvents(this);
//    serverJSON->getWebSocketRoute()->registerWebSocketEvents(this);


    // Create a new logger channel.
    loggerChannel = WebSocketLoggerChannel::makeShared();

    // Attach the websocket route.
    loggerChannel->setWebSocketRoute(serverJSON->getWebSocketRoute());

    // Set the logger channel as active.
    ofSetLoggerChannel(loggerChannel);

    // Register RPC methods.
    serverJSON->registerMethod("test-checkbox",
                               "Get the value of a checkbox",
                               this,
                               &ofApp::getCheckBox);

    serverJSON->registerMethod("test-slider",
                               "Get the value of a slider",
                               this,
                               &ofApp::getSlider);

    serverJSON->registerMethod("test-button",
                               "Get the value of a button",
                               this,
                               &ofApp::getButton);

    serverJSON->registerMethod("test-dropdown",
                               "Get the select item from the drobdown",
                               this,
                               &ofApp::getDropdown);

    serverJSON->registerMethod("start-videoServer",
                               "Start the BasicIPVideoServer.",
                               this,
                               &ofApp::startVideoServer);

    serverJSON->registerMethod("stop-videoServer",
                               "Stop the BasicIPVideoServer.",
                               this,
                               &ofApp::stopVideoServer);

    serverJSON->registerMethod("textarea",
                               "Get the text!",
                               this,
                               &ofApp::getTextArea2);

    serverJSON->registerMethod("set-epubname",
                               "Set the epubname",
                               this,
                               &ofApp::getEpubName);

   //  serverJSON->registerMethod("setDropbox",
   //                            "Set selected File!",
   //                            this,
   //                            &ofApp::setDropbox);



    // Start the server.
    serverJSON->start();
}


//--------------------------------------------------------------
void ofApp::initServerVideo(int port)
{
    ofx::HTTP::BasicIPVideoServerSettings settings;

    // Many other settings are available.
    settings.setPort(port);
    settings.setDefaultIndex("liveView.html");

    // Apply the settings.
    serverVideo = ofx::HTTP::BasicIPVideoServer::makeShared(settings);
}

//--------------------------------------------------------------
Json::Value ofApp::toJSONMethod(const std::string& module, const std::string& method, const Json::Value& params)
{
    Json::Value json;
    json["main"] = "1.0";
    json["module"] = module;
    json["method"] = method;
    json["params"] = params;
    return json;
}

//--------------------------------------------------------------
void ofApp::sendJSONMessage(Json::Value json)
{
    Json::FastWriter writer;

    ofx::HTTP::WebSocketFrame frame(writer.write(json));
    serverJSON->getWebSocketRoute()->broadcast(frame);

}

void ofApp::ePubUnzip(string i_file){

ofFile tmpfile(currentEpubname);
currentEpubname = tmpfile.getBaseName();

std::ifstream inp(i_file.c_str(), std::ios::binary);
poco_assert (inp);
// decompress to current working dir

//Poco::Zip::Decompress dec(inp, "data/temp/temp"); /// works
Poco::Zip::Decompress dec(inp, "data/DocumentRoot/temp");


// if an error happens invoke the ZipTest::onDecompressError method
dec.EError += Poco::Delegate<ofApp, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ofApp::onDecompressError);
dec.decompressAllFiles();

dec.EError -= Poco::Delegate<ofApp, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &ofApp::onDecompressError);
dec.EOk -= Poco::Delegate<ofApp, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &ofApp::onOk);

zipped=true;



}

void ofApp::ePubZip(){


std::ofstream out("test.epub", std::ios::binary);
Poco::Zip::Compress c(out, true);
// Poco::Path theFile("data/temp/temp/mimetype");/// works
Poco::Path theFile("data/DocumentRoot/temp/mimetype");

c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);
// Poco::Path data("data/temp/temp");/// works
Poco::Path data("data/DocumentRoot/temp");
data.makeDirectory();
c.addRecursive(data, Poco::Zip::ZipCommon::CL_NORMAL , true);
c.close(); // MUST be done to finalize the Zip file


}



void ofApp::onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
{



}

void ofApp::onOk(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
{
	test = "OK: " + info.second.toString(Poco::Path::PATH_UNIX);



}

void ofApp::ePubList(){


    if(dir.doesDirectoryExist("DocumentRoot/temp/OPS"))
    {

         dir.listDir("DocumentRoot/temp/OPS");

    }else if(dir.doesDirectoryExist("DocumentRoot/temp/OEBPS")) {

     dir.listDir("DocumentRoot/temp/OEBPS");

    }else{

    dir.listDir("DocumentRoot/temp");
    }
    // dir.listDir("temp/temp"); //works


    ofDirectory i_dir;

    if(i_dir.doesDirectoryExist("DocumentRoot/temp/images")){
    i_dir.listDir("DocumentRoot/temp/images");
    i_dir.copyTo("DocumentRoot/images");
    }
    if(i_dir.doesDirectoryExist("DocumentRoot/temp/OPS/images")){
    i_dir.listDir("DocumentRoot/temp/OPS/images");
    i_dir.copyTo("DocumentRoot/images");
    }
    if(i_dir.doesDirectoryExist("DocumentRoot/temp/OEBPS/images")){
    i_dir.listDir("DocumentRoot/temp/OEBPS/images");
    i_dir.copyTo("DocumentRoot/images");
    }


    i_dir.close();

    if( dir.size() > 0 ){
            files.assign(dir.size(), ofFile());

            for(int i = 0; i < (int)dir.size(); i++){
                files[i].open(dir.getPath(i),ofFile::ReadWrite);
            }
    }

    currentFile = 0;

    buffer = ofBufferFromFile(dir.getPath(currentFile));
    if(buffer.size()) {

            // we now keep grabbing the next line
            // until we reach the end of the file
            while(buffer.isLastLine() == false) {

            // move on to the next line
            string line = buffer.getNextLine();

            // copy the line to draw later
            // make sure its not a empty line
            if(line.empty() == false) {
                seussLines.push_back(line);
            }

            }
    }

    Json::Value json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
    sendJSONMessage(json);


    vector<string> vec;
    for(int i = 0; i < int(dir.size()); i++) {
        vec.push_back(files[i].getFileName());
    }


    // Create vector
    Json::Value params;
    for(int i = 0; i < vec.size(); i++) {
        params.append(vec[i]);
    }

        // Send vector
    Json::Value json2 = toJSONMethod("Server", "test-dropdown", params);
    sendJSONMessage(json2);

    Json::Value params2;
    params2["value"] = buffer.getText();

    json = toJSONMethod("Server", "textarea", params2);
    sendJSONMessage(json);

    Json::Value params3;
    params3["value"] = currentEpubname;
    Json::Value json5 = toJSONMethod("Server", "reset", params3);
    sendJSONMessage(json5);

}

