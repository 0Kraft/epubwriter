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
    reset_all();
    dir_del("uploads");
    ofDirectory createDir;
    createDir.createDirectory("uploads");



}

//--------------------------------------------------------------
void ofApp::setup()
{

    /// ZIP

    zipped = false;
    ziptimer=0;

    ///ZIP

    reset_all();

    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);


    currentFile=0;   // Chosen Filenumber
    currentFilename="null";
    currentEpubname="null";


    // JSON-RPC server
    initServerJSONRPC(8197);

    // Launch a browser with the address of the server.
    ofLaunchBrowser(serverJSON->getURL());

    epub_opf_reference="null";

    covermode=false;


}

void ofApp::update()
{

    if(zipped)  ziptimer++;

    if(ziptimer>100) {
        ePubList();
        ziptimer=0;
        zipped=false;
     }

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    ofSetColor(255,255,255);

    if(zipped){
    ofDrawBitmapString(currentFilename + " has been unzipped!", 5,5);
    }else{
     ofDrawBitmapString("Waiting for epub to unzip ...", 5,20);
    }



    /// DIR

    ofDrawBitmapString("Path Rootfile", 350,20);
    ofDrawBitmapString("Path Images", 500,20);
    ofDrawBitmapString("Path Text", 650,20);
    ofDrawBitmapString("Path Styles", 800,20);

      ofSetColor(0,255,0);

    ofDrawBitmapString(epub_path_rootfile, 350,40);
    ofDrawBitmapString(epub_path_image, 500,40);
    ofDrawBitmapString(epub_path_text, 650,40);
    ofDrawBitmapString(epub_path_style, 800,40);

    /*
    if (dir.size() > 0){


        ofSetColor(ofColor::gray);


        for(int i = 0; i < (int)dir.size(); i++){
            if(i == currentFile) {
                ofSetColor(ofColor::red);
            }	else {
                ofSetColor(ofColor::gray);
            }
            string fileInfo = "file " + ofToString(i + 1) + " = " + dir.getName(i);
            ofDrawBitmapString(fileInfo, 5,i * 20 + 70);
        }
    }
    */

       ofSetColor(ofColor::white);

    ofDrawBitmapString("-------------NavPoints-------------------", 5,60);

     ofSetColor(0,255,0);

    for(size_t i = 0; i < epub_toc_navpoint.size(); i++)
        {
            ofDrawBitmapString(ofToString(epub_toc_navpoint[i].playOrder) , 5 ,i * 20 + 80);
            ofDrawBitmapString(ofToString(epub_toc_navpoint[i].label), 30 ,i * 20 + 80);
        }

        ofSetColor(ofColor::white);

    ofDrawBitmapString("-------------MetaData-------------------", 350,60);

            ofDrawBitmapString("Filename ePub:", 350 , 80);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentEpubname, 500 ,80);

            ofSetColor(ofColor::white);
            ofDrawBitmapString("Title ePub:", 350 , 100);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentEpubTitle, 500 ,100);

             ofSetColor(ofColor::white);
            ofDrawBitmapString("OFFilename:", 350 , 200);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentFileOF.getAbsolutePath(), 500 ,200);


            ofSetColor(ofColor::white);
            ofDrawBitmapString("Filename Chapter:", 350 , 300);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentFilename, 500 ,300);

            ofSetColor(ofColor::white);
            ofDrawBitmapString("PlayOrder Chapter:", 350 , 320);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentChapterUid, 500 ,320);

            ofSetColor(ofColor::white);
            ofDrawBitmapString("Label Chapter:", 350 , 340);
            ofSetColor(0,255,0);
            ofDrawBitmapString(currentChapterLabel, 500 ,340);

            ofSetColor(ofColor::grey);
            ofDrawBitmapString(currentFileBuffer.getText(), 50 , 50);




}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{


    if(key=='+'){

    currentFile++;

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

    }

    if(key=='f'){

            ePubFinalizeContent();
             ePubFinalizeToc();

    }

     if(key=='g'){

           ePubFindMetaInf();
           ePubReadInContent();
           ePubReadInToc();

    }

    if(key=='d'){

      sendDatatoWeb();



    }


}

//--------------------------------------------------------------
void ofApp::getDropdown(ofx::JSONRPC::MethodArgs& args)
{

    ofLogVerbose("ofApp::getDropdown") << args.params.asString();

    setTextareaWeb(args.params.asString());
    currentFilename = args.params.asString();

    for(int i = 0; i < int(files.size()); i++) {
        if(args.params.asString() == files[i].getFileName()) {

          currentFile = i;


        }
    }

}

void ofApp::getSelection(ofx::JSONRPC::MethodArgs& args)
{

    ofLogVerbose("ofApp::getSelection") << args.params.asString();

    for(int i = 0; i < int(epub_toc_navpoint.size()); i++) {
            if(args.params.asString() == epub_toc_navpoint[i].label) {




              currentFilename = epub_toc_navpoint[i].contentpath;



              currentChapterUid = epub_toc_navpoint[i].playOrder;
              currentChapterLabel = epub_toc_navpoint[i].label;
              currentFile = atoi(  currentChapterUid.c_str() );

              currentFileOF.open("DocumentRoot/temp/"+ epub_path_root + epub_toc_navpoint[i].contentpath);



            }
        }


    size_t nFPos = currentFilename.find(epub_path_text);

          if((nFPos!=std::string::npos)){
           currentFilename= currentFilename.substr(nFPos+epub_path_text.length(),currentFilename.length());
          }

     nFPos = currentFilename.find("#");

          if((nFPos!=std::string::npos)){
           currentFilename= currentFilename.substr(0,nFPos);
          }else{

          }

    setTextareaWeb(currentFilename);



    for(int i = 0; i < int(files.size()); i++) {
        if(args.params.asString() == files[i].getFileName()) {

          currentFile = i;

        }
    }

}


void ofApp::setTextareaWeb(string fn)
{

    ///send text to browser

    ofFile t_file(dir.getAbsolutePath() + "/" + fn);

    if((t_file.getExtension()=="html")||(t_file.getExtension()=="opf")||(t_file.getExtension()=="xhtml")){

        string selectedFilePath;

        selectedFilePath = dir.getAbsolutePath() +"/"+ fn;

        ofLogVerbose("ofApp::SentFilename") << selectedFilePath;

        //currentFileBuffer = ofBufferFromFile(selectedFilePath);



        /// Parse

         /// Search for insert

    std::vector<std::string>    itscontent;
    string line;
    bool html_body=false;

    ifstream myfile(selectedFilePath.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {


          getline (myfile,line);




          string html_search = line;
          size_t nFPos2 = html_search.find("</body>");

          if((nFPos2!=std::string::npos)){
                html_body=false;
          }

           if(html_body)  itscontent.push_back(line);

          size_t nFPos = html_search.find("<body>");

          if((nFPos!=std::string::npos)){
                html_body=true;
          }


        }
        myfile.close();
    }

   currentFileBuffer.clear();


   for(size_t i = 0;i<itscontent.size();i++)
        {
            currentFileBuffer.append(itscontent[i]);
            currentFileBuffer.append("\n");

        }



   /// Search for insert



        /// Parse




        Json::Value params2;
        params2["value"] = currentFileBuffer.getText();
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

        t_file.close();



}

void ofApp::getEpubName(ofx::JSONRPC::MethodArgs& args)
{
   ofScopedLock lock(mutex);
   ofLogVerbose("ofApp::getEpubName") << args.params.asString();



   currentEpubname = args.params.asString();
   // currentEpubname = currentEpubname.substr(12,currentEpubname.length());

   /*

    string head_search = currentEpubname;

            size_t nFPos = head_search.find("\\");
            if(nFPos!=std::string::npos){

                     currentEpubname = currentEpubname.substr(0,nFPos+2);


            }

            head_search = currentEpubname;

            nFPos = head_search.find("\\");
            if(nFPos!=std::string::npos){

                     currentEpubname = currentEpubname.substr(0,nFPos+2);


            }
            */


}


void ofApp::getTextArea2(ofx::JSONRPC::MethodArgs& args)
{
    ofScopedLock lock(mutex);

   /// save file

   currentFileOF.setWriteable(true);



   ofBuffer tempbuffer;
   tempbuffer.append("<body>");
   tempbuffer.append("\n");
   tempbuffer.append(args.params.asString());
   tempbuffer.append("\n");
   tempbuffer.append("</body>");


  // ofBuffer tmp_buf_file;
  // tmp_buf_file =  files[currentFile].readToBuffer();

   string tmppath;
   tmppath = currentFileOF.getAbsolutePath();

   currentFileOF.close();

   /// Search for insert

    std::vector<std::string>    itscontent;
    string line;

    ifstream myfile(tmppath.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen

   bool body_delete = false;
   size_t body_begin,body_end;


   for(size_t i = 0;i<itscontent.size();i++)
        {


            string html_search = itscontent[i];





            size_t nFPos = html_search.find("<body>");
            if(nFPos!=std::string::npos){

                body_delete = true;
                body_begin = i;


            }

            nFPos = html_search.find("</body>");
                        if(nFPos!=std::string::npos){

                            body_delete = false;
                            body_end = i;
                        }


        }

      itscontent.erase(itscontent.begin()+ body_begin,itscontent.begin()+body_end+1);


      itscontent.insert(itscontent.begin()+body_begin,tempbuffer.getText());



   /// Search for insert


    /// SaveFile


    ofstream myoutfile (tmppath.c_str());
        if (myoutfile.is_open())
        {



        for(size_t i = 0; i < itscontent.size(); i++)
        {
            myoutfile << itscontent[i] << std::endl;
        }



         myoutfile.close();
        }
     else cout << "Unable to open file";



    /// SaveFile







   //ofBufferToFile(tmppath,tempbuffer);




   currentFileOF.open(tmppath,ofFile::Reference);

   ofLogVerbose("Write ") << " to File: " << currentFileOF.getFileName();



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

            if(args.getFileType().toString()=="application/octet-stream"){
            ePubUnzip(up_filename);
            }

            if((args.getFileType().toString()=="image/gif")||
               (args.getFileType().toString()=="image/png")||
               (args.getFileType().toString()=="image/jpeg")||
               (args.getFileType().toString()=="image/svg+xml")){

                   if(!covermode){

            ePubAddImage(up_filename,args.getOriginalFilename(),args.getFileType().toString());
                   }else{
            ePubAddCover(up_filename,args.getOriginalFilename(),args.getFileType().toString());
                   }
            }



            break;
    }

    ofLogNotice("ofApp::onHTTPUploadEvent") << "";
    ofLogNotice("ofApp::onHTTPUploadEvent") << "         state: " << stateString;
    ofLogNotice("ofApp::onHTTPUploadEvent") << " formFieldName: " << args.getFormFieldName();
    ofLogNotice("ofApp::onHTTPUploadEvent") << "orig. filename: " << args.getOriginalFilename();
    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      filename: " << args.getFilename();
    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      fileType: " << args.getFileType().toString();
    ofLogNotice("ofApp::onHTTPUploadEvent") << "# bytes xfer'd: " << args.getNumBytesTransferred();





}


//--------------------------------------------------------------
void ofApp::initServerJSONRPC(int port)
{
    ofx::HTTP::BasicJSONRPCServerSettings settings;
    settings.setPort(port);
    //settings.setUploadRedirect("");
    settings.setMaximumFileUploadSize(2000000 * 1024); // 2 GB

    settings.setBufferSize(2000000);


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

    serverJSON->registerMethod("test-dropdown",
                               "Get the select item from the drobdown",
                               this,
                               &ofApp::getDropdown);

    serverJSON->registerMethod("textarea",
                               "Get the text!",
                               this,
                               &ofApp::getTextArea2);

    serverJSON->registerMethod("set-epubname",
                               "Set the epubname",
                               this,
                               &ofApp::getEpubName);

     serverJSON->registerMethod("zip-epub",
                               "zip epub",
                               this,
                               &ofApp::ePubZip);

     serverJSON->registerMethod("add-chapter",
                               "add chapter",
                               this,
                               &ofApp::ePubAddChapter);

     serverJSON->registerMethod("new-epub",
                               "new epub",
                               this,
                               &ofApp::ePubNewEpub);

      serverJSON->registerMethod("add-cover",
                               "add cover",
                               this,
                               &ofApp::ePubAddCoverMode);

       serverJSON->registerMethod("send-toc",
                                "send toc",
                               this,
                               &ofApp::getSelection);

    // Start the server.
    serverJSON->start();
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

void ofApp::ePubUnzipFlat(string i_file){

std::ifstream inp(i_file.c_str(), std::ios::binary);
poco_assert (inp);
// decompress to current working dir

//Poco::Zip::Decompress dec(inp, "data/temp/temp"); /// works
Poco::Zip::Decompress dec(inp, "data/DocumentRoot/tempzipflat",true,true);

// if an error happens invoke the ZipTest::onDecompressError method

dec.decompressAllFiles();


ofLogVerbose("Unzipped Flat: ") << i_file;

cleanup_structure();

zipped=true;

}


void ofApp::ePubUnzip(string i_file){

    reset_all();

   ofFile tmpfile(currentEpubname);
currentEpubname = tmpfile.getBaseName();

std::ifstream inp(i_file.c_str(), std::ios::binary);
poco_assert (inp);
// decompress to current working dir

//Poco::Zip::Decompress dec(inp, "data/temp/temp"); /// works
Poco::Zip::Decompress dec(inp, "data\\DocumentRoot\\tempzip",false,true);

// if an error happens invoke the ZipTest::onDecompressError method

dec.decompressAllFiles();



    ofDirectory zipdir;

        zipdir.listDir("DocumentRoot/tempzip");
        //zipdir.copyTo("DocumentRoot");
        zipdir.renameTo("DocumentRoot/temp");

    zipdir.close();


    zipped=true;



}

void ofApp::ePubAddCoverMode(){
covermode=true;
}


void ofApp::ePubZip(){

ofLogVerbose("Start Zip");


ePubFinalizeContent();
ePubFinalizeToc();

///MIMETYPE löschen

string correctpath;
correctpath="data/DocumentRoot/temp/mimetype";

    int ti = std::remove( correctpath.c_str() );
     if( ti != 0 ){
                          ofLogVerbose("File not Ebene: mimetype " );
                        }else {
                           ofLogVerbose("File successfully deleted: mimetype" );
                        }

///MIMETYPE löschen


string zn =  "data/DocumentRoot/"+ currentEpubname + ".epub";
//string zn =  "data/DocumentRoot/"+ currentEpubname + ".epub";
std::ofstream out( zn.c_str() , std::ios::binary);
Poco::Zip::Compress c(out, true);

ofLogVerbose("Zip") << "Objekt erstellt";

Poco::Path theFile("data\\epubessentials\\mimetype");
c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);

//Poco::Path theFile("data/epubessentials/mimetype");
//c.addFile(theFile, "mimetype");
//c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);

ofLogVerbose("Zip") << "Mimetype hinzugefügt";

/*
 string fn;
 fn = "data//DocumentRoot//temp";

  std::stringstream ss;
        for (int j = 0; j < fn.length(); j++) {
             if (fn[j] == '\\') {
                            ss << "\\\\";
                            }
                            else
                            {
                                ss << fn[j];
                            }
        }
                        /// Escape String

*/

//Poco::Path data("data/DocumentRoot/temp");
// data.makeDirectory();
// c.addRecursive(data);


/*
Poco::Path data("data\\DocumentRoot\\temp");     // WINDOWS

data.makeDirectory();

c.addRecursive(data);
*/

Poco::Path data("data/DocumentRoot/temp");
Poco::Path name("");
data.makeDirectory();
name.makeDirectory();
c.addRecursive(data, Poco::Zip::ZipCommon::CL_NORMAL, true, name);



ofLogVerbose("Zip") << "Ordner hinzugefügt";

/*
Poco::Path theFile("data/epubessentials/mimetype");

//c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);

c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);


Poco::Path meta("data/epubessentials/META-INF");
Poco::Path metaname("META-INF");
meta.makeDirectory();
metaname.makeDirectory();
c.addRecursive(meta, Poco::Zip::ZipCommon::CL_NORMAL, false, metaname);

ofDirectory tmp_zip_dir;
string cutpath;
cutpath = epub_path_rootfile;
cutpath = "DocumentRoot/temp" + cutpath.substr(0,epub_path_rootfile.length()-11);

tmp_zip_dir.listDir(cutpath);

vector<ofFile> tmpfiles = tmp_zip_dir.getFiles();

for(int i = 0; i < tmpfiles.size(); i++){

    if((tmpfiles[i].isDirectory())!=true){

        ofLogVerbose(tmpfiles[i].getFileName());
         ofLogVerbose(tmpfiles[i].getAbsolutePath());

        Poco::Path theFileD(tmpfiles[i].getAbsolutePath().c_str());
        c.addFile(theFileD, tmpfiles[i].getFileName().c_str() ,Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);

    }else{

         ofLogVerbose(tmpfiles[i].getFileName());
         ofLogVerbose(tmpfiles[i].getAbsolutePath());

        Poco::Path data(tmpfiles[i].getAbsolutePath().c_str());
        Poco::Path name(tmpfiles[i].getFileName().c_str());
        data.makeDirectory();
        name.makeDirectory();
        c.addRecursive(data, Poco::Zip::ZipCommon::CL_NORMAL, false, name);



    }

}

*/

Poco::Zip::ZipArchive archive = c.close(); // MUST be done to finalize the Zip file

ofLogVerbose("Zip Ready");

Json::Value json = toJSONMethod("Server", "zipready", 0);
sendJSONMessage(json);

}


void ofApp::ePubList(){

    dir.listDir("DocumentRoot/temp"); //works

    ofLogVerbose("List") << "Start Parsing";

    ePubFindMetaInf();

    ofLogVerbose("List") << "Meta-Inf";

    ePubFindPaths();

    ofLogVerbose("List") << "Paths";


    ePubReadInContent();

    ofLogVerbose("List") << "Readin Content";

    ePubReadInToc();
    ofLogVerbose("List") << "Readin Toc";


    ofDirectory i_dir;
    i_dir.listDir("DocumentRoot/temp/" + epub_path_root + epub_path_image);
    i_dir.copyTo("DocumentRoot/images");
    i_dir.close();

    ofLogVerbose("List") << "Copy images";


    //ofLogVerbose(epub_path_rootfile);
    //ofLogVerbose(epub_path_text);


   dir.listDir("DocumentRoot/temp/" + epub_path_text); //works Hier muss der Ordner hin, der die html enthält°

   ofLogVerbose("List") << "List Text Directory";


    if( dir.size() > 0 ){
            files.assign(dir.size(), ofFile());

            for(int i = 0; i < (int)dir.size(); i++){
                files[i].open(dir.getPath(i),ofFile::ReadOnly);
            }
    }

    ofLogVerbose("List") << "Open Text Directory Files";



    currentFile = 0;

    currentFilename = epub_toc_navpoint[0].contentpath;

    size_t nFPos = currentFilename.find(epub_path_text);

          if((nFPos!=std::string::npos)){
           currentFilename= currentFilename.substr(nFPos+epub_path_text.length(),currentFilename.length());
          }

     nFPos = currentFilename.find("#");

          if((nFPos!=std::string::npos)){
           currentFilename= currentFilename.substr(0,nFPos);
          }else{

          }

    setTextareaWeb(currentFilename);

    ofLogVerbose("List") << "Send Text to Editor";


    Json::Value json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
    sendJSONMessage(json);

    ofLogVerbose("List") << "Remove Dropdown Chapters";


    vector<string> vec;
    for(int i = 0; i < int(dir.size()); i++) {
          if((files[i].getExtension()=="html")||(files[i].getExtension()=="xhtml"))
            {
                vec.push_back(files[i].getFileName());
            }
    }


    // Create vector
    Json::Value params;
    for(int i = 0; i < vec.size(); i++) {
        params.append(vec[i]);
    }

        // Send vector
    Json::Value json2 = toJSONMethod("Server", "test-dropdown", params);
    sendJSONMessage(json2);

     ofLogVerbose("List") << "Send Dropdowns";


     ///

      vector<string> vec2;
    for(int i = 0; i < int(epub_toc_navpoint.size()); i++) {

                vec2.push_back(epub_toc_navpoint[i].label);

    }


    // Create vector
    Json::Value params2;
    for(int i = 0; i < vec2.size(); i++) {
        params2.append(vec2[i]);
    }

        // Send vector
    Json::Value jsontoc = toJSONMethod("Server", "set-toc", params2);
    sendJSONMessage(jsontoc);

     ofLogVerbose("List") << "Send Dropdowns";

     ///


    Json::Value params3;
    params3["value"] = currentEpubname;
    Json::Value json5 = toJSONMethod("Server", "reset", params3);
    sendJSONMessage(json5);

    ofLogVerbose("List") << "Reset Website";

}

void ofApp::dir_del(string fdir){

        ofDirectory tmpDir;

        if(tmpDir.doesDirectoryExist(fdir)){


        ofLogVerbose("Directory exists " ) << fdir;


        tmpDir.listDir(fdir);

        vector<ofFile> tmpfiles = tmpDir.getFiles();

        int lauf = (int)tmpfiles.size();

        for(int i = 0; i < lauf; i++){

             if((tmpfiles[i].isDirectory())!=true){

                        string fn;
                        fn = tmpfiles[i].getAbsolutePath();

                        /// Escape String
                        std::stringstream ss;
                        for (int j = 0; j < fn.length(); j++) {
                            if (fn[j] == '\\') {
                                ss << "\\\\";
                            }
                            else
                            {
                                ss << fn[j];
                            }
                        }
                        /// Escape String

                        tmpfiles[i].close();

                        int ti = std::remove( ss.str().c_str() );

                        if( ti != 0 ){}
                          //  perror("\tError deleting file");
                        else {
                           ofLogVerbose("File successfully deleted: 1. Ebene " ) << ofToString(i);
                        }

                }else{

                    ///


                        string fn;
                        fn = tmpfiles[i].getAbsolutePath();

                        ofDirectory tmpDir2;
                        tmpDir2.listDir(fn);

                        ofLogVerbose("Files listed: check!" );

                        vector<ofFile> tmpfiles2 = tmpDir2.getFiles();

                        int lauf2 = (int)tmpfiles2.size();

                        for(int a = 0; a < lauf2; a++){

                /// 3. Ebene

                    if((tmpfiles2[a].isDirectory())!=true){

                        string fn3;
                        fn3 = tmpfiles2[a].getAbsolutePath();

                        /// Escape String
                        std::stringstream ss3;
                        for (int n = 0; n < fn3.length(); n++) {
                            if (fn3[n] == '\\') {
                                ss3 << "\\\\";
                            }
                            else
                            {
                                ss3 << fn3[n];
                            }
                        }
                        /// Escape String

                        tmpfiles2[a].close();

                        int ti3 = std::remove( ss3.str().c_str() );

                        if( ti3 != 0 ){}
                          //  perror("\tError deleting file");
                        else {
                           ofLogVerbose("File successfully deleted: 2. Ebene " ) << ofToString(i);
                        }

                }else{

                    ///


                        string fn3;
                        fn3 = tmpfiles2[a].getAbsolutePath();

                        ofDirectory tmpDir3;
                        tmpDir3.listDir(fn3);

                        ofLogVerbose("Files listed: 3. Ebene check!" );

                        vector<ofFile> tmpfiles3 = tmpDir3.getFiles();

                        int lauf3 = (int)tmpfiles3.size();

                        for(int m = 0; m < lauf3; m++){

                            string fn6;
                            fn6 = tmpfiles3[m].getAbsolutePath();

                            /// Escape String
                            std::stringstream ss6;
                            for (int l = 0; l < fn6.length(); l++) {
                                if (fn6[l] == '\\') {
                                    ss6 << "\\\\";
                                }
                                else
                                {
                                    ss6 << fn6[l];
                                }
                            }
                            /// Escape String

                            tmpfiles3[m].close();

                            int ti6 = std::remove( ss6.str().c_str() );

                            if( ti6 != 0 ) {   perror("\tError deleting file");}
                            else {
                                ofLogVerbose("File successfully deleted: 3. Ebene " ) << ofToString(a);
                            }



                        }


                        string tp3;
                        tp3 = tmpDir3.getAbsolutePath();

                        if(tmpDir3.remove(false)){
                           ofLogVerbose("Unterordner 2. Ebene geloescht");
                        }else{
                            ofLogVerbose("Unterordner 2. Ebene nicht geloescht");
                        }

                        tmpDir3.close();


                    ///

                }


                            ///  3.Ebene Ende


                            string fn5;
                            fn5 = tmpfiles2[a].getAbsolutePath();

                            /// Escape String
                            std::stringstream ss2;
                            for (int b = 0; b < fn5.length(); b++) {
                                if (fn5[b] == '\\') {
                                    ss2 << "\\\\";
                                }
                                else
                                {
                                    ss2 << fn5[b];
                                }
                            }
                            /// Escape String

                            tmpfiles2[a].close();

                            int ti2 = std::remove( ss2.str().c_str() );

                            if( ti2 != 0 ) {   perror("\tError deleting file");}
                            else {
                                ofLogVerbose("File successfully deleted: 2. Ebene " ) << ofToString(a);
                            }



                        }


                        string tp2;
                        tp2 = tmpDir2.getAbsolutePath();

                        if(tmpDir2.remove(false)){
                           ofLogVerbose("Unterordner geloescht");
                        }else{
                            ofLogVerbose("Unterordner nicht geloescht");
                        }

                        tmpDir2.close();


                    ///

                }

        }

                /// wenn Ordner leer ist, Ordner löschen!

                string tp8;
                tp8 = tmpDir.getAbsolutePath();

                if(tmpDir.remove(false)){
                     ofLogVerbose("Ordner geloescht");
                }else{
                     ofLogVerbose("Ordner nicht geloescht");
                }

                tmpDir.close();




}else{
  tmpDir.close();

}
  tmpDir.close();

} //Function and exits question

void ofApp::reset_all(){

        for(int i = 0; i < (int)files.size(); i++){

                files[i].close();

        }
    currentFileOF.close();

      dir_del("DocumentRoot/images");

      dir_del("DocumentRoot/temp");

      dir_del("DocumentRoot/tempzip");

      dir_del("DocumentRoot/tempzipflat");

      dir_del("DocumentRoot/OEBPS");




}

void ofApp::cleanup_structure(){

  ofDirectory zipdir;

  ofDirectory checkdir;

  zipdir.listDir("DocumentRoot/tempzipflat");

  if(!checkdir.doesDirectoryExist("DocumentRoot/tempzipflat/images"))
  {
      checkdir.createDirectory("DocumentRoot/tempzipflat/images");
  }
  if(!checkdir.doesDirectoryExist("DocumentRoot/tempzipflat/text"))
  {
      checkdir.createDirectory("DocumentRoot/tempzipflat/text");
  }
  if(!checkdir.doesDirectoryExist("DocumentRoot/tempzipflat/styles"))
  {
      checkdir.createDirectory("DocumentRoot/tempzipflat/styles");
  }

  vector<ofFile> flatfiles = zipdir.getFiles();

    for(int i = 0; i < (int)flatfiles.size(); i++){

            if((flatfiles[i].getExtension()=="jpg")||
               (flatfiles[i].getExtension()=="jpeg")||
               (flatfiles[i].getExtension()=="png")||
               (flatfiles[i].getExtension()=="gif")||
               (flatfiles[i].getExtension()=="svg"))
            {
                   flatfiles[i].copyTo("DocumentRoot/tempzipflat/images");
                   flatfiles[i].remove();
            }

            if((flatfiles[i].getExtension()=="html")||
               (flatfiles[i].getExtension()=="xhtml"))
            {
                   flatfiles[i].copyTo("DocumentRoot/tempzipflat/text");
                   flatfiles[i].remove();
            }

            if((flatfiles[i].getExtension()=="css"))
            {
                   flatfiles[i].copyTo("DocumentRoot/tempzipflat/styles");
                   flatfiles[i].remove();
            }

            if((flatfiles[i].getBaseName()=="mimetype"))
            {
                    flatfiles[i].remove();
            }

            if((flatfiles[i].getFileName()=="container.xml"))
            {
                    flatfiles[i].remove();
            }


    flatfiles[i].close();

    }

    ofLogVerbose("cleaned UP!");
    zipdir.renameTo("DocumentRoot/temp");



    zipdir.close();


}


 void ofApp::ePubParseContent(){


    /*
    string itsmainstring;
    ofBuffer itsbuffer;
    itsbuffer = ofBufferFromFile("content.opf");
    itsmainstring = itsbuffer.getText();
    */

    /// File in Vektor einlesen

    std::vector<std::string>    itscontent;


    string line;
    ifstream myfile(epub_path_rootfile.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen


    /// content.opf Linie für Linie ändern

    for(int i = 0;i<itscontent.size();i++)
        {

            string ts4 = itscontent[i];
            string ts5 = itscontent[i];



            size_t nFPos = ts4.find("item");
            size_t nFPos2 = ts5.find("media-type");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: ITEM");


                  /// JPEG

                  size_t s_search = ts4.find("image/jpeg");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: JPEG");

                         size_t s_path = ts4.find("href=\"images");

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=");

                               itscontent[i].insert(s_pathbegin+6,"images/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// JPEG

                  /// PNG

                  s_search = ts4.find("image/png");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: PNG");

                         size_t s_path = ts4.find("href=\"images");

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=");

                               itscontent[i].insert(s_pathbegin+6,"images/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// PNG

                   /// XHTML

                  s_search = ts4.find("application/xhtml+xml");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: XHTML");

                         size_t s_path = ts4.find("href=\"text");

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=");

                               itscontent[i].insert(s_pathbegin+6,"text/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// XHTML

                  /// CSS

                  s_search = ts4.find("text/css");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: CSS");

                         size_t s_path = ts4.find("href=\"styles\/");

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=");

                               itscontent[i].insert(s_pathbegin+6,"styles/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// CSS


            }else{

                ofLogVerbose("Kein Treffer");

            }



        }



    ofstream myoutfile ("contennparsed.opf");
        if (myoutfile.is_open())
        {

             for(size_t i = 0; i < itscontent.size(); i++)
        {
            myoutfile << itscontent[i] << std::endl;
        }

           myoutfile.close();
        }
     else cout << "Unable to open file";



 }

void ofApp::ePubReadInContent(){

    /// alte Daten löschen

    epub_opf_item.clear();
    epub_opf_head.clear();


    /// File in Vektor einlesen


    std::vector<std::string>    itscontent;
    int spinecount = 0;

    string line;

    string correctpath;
    correctpath="data/DocumentRoot/temp/" + epub_path_rootfile;

    ifstream myfile(correctpath.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen


    /// content.opf Linie für Linie ändern
    bool head_in = false;

    for(int i = 0;i<itscontent.size();i++)
        {


            ///HEAD
            string head_search = itscontent[i];

            size_t nFPos = head_search.find("<?xml");
            if(nFPos!=std::string::npos){

                    head_in=true;

            }

            nFPos = head_search.find("<manifest>");
            if(nFPos!=std::string::npos){

                    head_in=false;

            }



            if(head_in) {

                epub_opf_head.push_back(itscontent[i]);
                ofLogVerbose("HeadAdd: ") << itscontent[i];

            }




            ///HEAD


            /// ITEMS


            string ts4 = itscontent[i];
            string ts5 = itscontent[i];



            nFPos = ts4.find("item");
            size_t nFPos2 = ts5.find("media-type");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: ITEM");


                         item tmp_item;

                         size_t s_path = ts4.find("href=\"");
                         size_t s_path_end = ts4.find("\"",s_path+6);
                         tmp_item.contentpath = itscontent[i].substr(s_path+6,s_path_end-s_path-6);
                            ofLogVerbose(tmp_item.contentpath);

                         size_t s_id = ts4.find("id=\"");
                         size_t s_id_end = ts4.find("\"",s_id+4);
                         tmp_item.id = itscontent[i].substr(s_id+4,s_id_end-s_id-4);
                            ofLogVerbose(tmp_item.id);

                         size_t s_media = ts4.find("media-type=\"");
                         size_t s_media_end = ts4.find("\"",s_media+12);
                         tmp_item.mediatype = itscontent[i].substr(s_media+12,s_media_end-s_media-12);
                            ofLogVerbose(tmp_item.mediatype);

                         tmp_item.line = itscontent[i];
                         tmp_item.spine_pos = 0;


                          epub_opf_item.push_back(tmp_item);






            }


            /// ITEMS


            nFPos = ts4.find("itemref");
            nFPos2 = ts5.find("idref=\"");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: Spine");


                  /// JPEG
                        string tmp_id;
                        spinecount++;

                         size_t s_path_end = ts4.find("\"",nFPos2+7);
                         tmp_id = itscontent[i].substr(nFPos2+7,s_path_end-nFPos2-7);


                         //ofLogVerbose(tmp_id);

                          for(int j = 0;j<epub_opf_item.size();j++)
                            {

                                   if(epub_opf_item[j].id == tmp_id){

                                        epub_opf_item[j].spine_pos=spinecount;
                                        epub_opf_item[j].spineline=itscontent[i];


                                        ofLogVerbose( epub_opf_item[j].id ) << " : " << ofToString( epub_opf_item[j].spine_pos);

                                   }

                            }






            }

            ///GUIDE

            nFPos2 = ts5.find("reference");
            nFPos = ts5.find("type");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos)){

                epub_opf_guide = itscontent[i];

            }




        }

         ///Find title

     for(int i = 0;i< epub_opf_head.size();i++)
        {


            ///HEAD
            string title_search = epub_opf_head[i];

            size_t nFPos = title_search.find("<dc:title");
            if(nFPos!=std::string::npos){



              size_t title_start = title_search.find(">",nFPos+9);

              size_t title_end = title_search.find("</dc:title>",title_start+1);

              currentEpubTitle = epub_opf_head[i].substr(title_start+1,title_end-title_start-1);

            }

            }



    ///






}


void ofApp::ePubParseToc(){

    epub_toc_head.clear();
    epub_toc_navpoint.clear();

    std::vector<std::string>    itscontent;

    string line;
    ifstream myfile("toc.ncx");
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen


    /// content.opf Linie für Linie ändern

    for(int i = 0;i<itscontent.size();i++)
        {

            string ts4 = itscontent[i];
            string ts5 = itscontent[i];



            size_t nFPos = ts4.find("content");
            size_t nFPos2 = ts5.find("src=");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: Content");


                  /// Content

                  size_t s_search = ts4.find("src=\"text\/");
                  if(s_search==std::string::npos){

                         ofLogVerbose("Treffer: Pfad nicht korrekt");


                               size_t s_pathbegin = ts4.find("src=");

                               itscontent[i].insert(s_pathbegin+5,"text/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }
///Content


            }else{

                ofLogVerbose("Kein Treffer");

            }



    }



    ofstream myoutfile ("tocparsed.ncx");
        if (myoutfile.is_open())
        {

             for(size_t i = 0; i < itscontent.size(); i++)
        {
            myoutfile << itscontent[i] << std::endl;
        }

           myoutfile.close();
        }
     else cout << "Unable to open file";



 }

 void ofApp::ePubReadInToc(){


    std::vector<std::string>    itscontent;

    epub_toc_head.clear();
    epub_toc_navpoint.clear();

    string line;

    string correctpath;
    string toci = "toc.ncx";
    correctpath = "data/DocumentRoot/temp/" + epub_path_root + toci;

    ifstream myfile(correctpath.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen

    ///HEAD

      bool head_in = false;

    for(int i = 0;i<itscontent.size();i++)
        {


            ///HEAD
            string head_search = itscontent[i];

            size_t nFPos = head_search.find("<?xml");
            if(nFPos!=std::string::npos){

                    head_in=true;

            }

            nFPos = head_search.find("<navMap>");
            if(nFPos!=std::string::npos){

                    head_in=false;

            }



            if(head_in) {

                epub_toc_head.push_back(itscontent[i]);
                ofLogVerbose("HeadAdd: ") << itscontent[i];

            }




            ///HEAD


    /// NavPoints



            string ts4 = itscontent[i];


             nFPos = ts4.find("navPoint");
            size_t nFPos_id = ts4.find("id");
            size_t nFPos_playOrder = ts4.find("playOrder");

            if((nFPos!=std::string::npos)&&(nFPos_playOrder!=std::string::npos))
            {

                  ofLogVerbose("Treffer: NavPoint");
                   ofLogVerbose("---------------");

                  nav_point tmp_navpoint;

                  size_t nFPos_id_end = ts4.find("\"",nFPos_id+4);
                  tmp_navpoint.id = itscontent[i].substr(nFPos_id+4,nFPos_id_end-nFPos_id-4);
                  ofLogVerbose(tmp_navpoint.id );

                  size_t nFPos_playOrder_end = ts4.find("\"",nFPos_playOrder+11);
                  tmp_navpoint.playOrder = itscontent[i].substr(nFPos_playOrder+11,nFPos_playOrder_end-nFPos_playOrder-11);
                  ofLogVerbose(tmp_navpoint.playOrder);

                  size_t nFPos_label = ts4.find("<text>");
                  if(nFPos_label==std::string::npos){


                        for(int j = 1; j<10;j++){

                            string searchstring;
                            searchstring=itscontent[i+j];
                            nFPos_label = searchstring.find("<text>");
                            if(nFPos_label!=std::string::npos){

                                    size_t nFPos_label_end = searchstring.find("</text>", nFPos_label);
                                    tmp_navpoint.label = itscontent[i+j].substr(nFPos_label+6,nFPos_label_end - nFPos_label-6);
                                    ofLogVerbose(tmp_navpoint.label);
                                    break;
                            }

                        }


                  }else{

                  size_t nFPos_label_end = ts4.find("</text>", nFPos_label+6);

                  tmp_navpoint.label = itscontent[i].substr(nFPos_label+6,nFPos_label_end - nFPos_label-6);
                  ofLogVerbose(tmp_navpoint.label);



                  }

                  /// Contentpath

                  size_t nFPos_contentpath = ts4.find("src=\"");
                  if(nFPos_contentpath==std::string::npos){

                        ofLogVerbose("Not in Line");

                        for(int j = 1; j<10;j++){
                            ofLogVerbose("Check Line ") << ofToString(j);
                            string searchstring;
                            searchstring=itscontent[i+j];
                            nFPos_contentpath = searchstring.find("src=\"");
                            if(nFPos_contentpath!=std::string::npos){

                                    ofLogVerbose("found source: ") << itscontent[i+j];
                                    size_t nFPos_contentpath_end = searchstring.find("\"",nFPos_contentpath+5);
                                     if(nFPos_contentpath_end!=std::string::npos){
                                    ofLogVerbose("found sourceend: ") << ofToString(nFPos_contentpath_end);
                                     ofLogVerbose("found sourcestart") << ofToString(nFPos_contentpath);
                                    tmp_navpoint.contentpath = itscontent[i+j].substr(nFPos_contentpath+5,nFPos_contentpath_end - nFPos_contentpath -5);
                                    ofLogVerbose(tmp_navpoint.contentpath);
                                     break;
                                     }

                            }

                        }


                  }else{

                  size_t nFPos_contentpath_end = ts4.find("\"",nFPos_contentpath+5);
                  tmp_navpoint.contentpath = itscontent[i].substr(nFPos_contentpath+5,nFPos_contentpath_end - nFPos_contentpath-5);
                  ofLogVerbose(tmp_navpoint.contentpath);

                  }

                  epub_toc_navpoint.push_back(tmp_navpoint);

            }

            else{

                ofLogVerbose("Kein Treffer");

            }



    }




 }

 void ofApp::ePubFindMetaInf(){

    std::vector<std::string>    itscontent;

    string line;
    ifstream myfile("data/DocumentRoot/temp/META-INF/container.xml");
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
          ofLogVerbose(line);
        }
        myfile.close();
    }else{

      ofLogVerbose("Pfad stimmt nicht");


    }

     for(int i = 0;i<itscontent.size();i++)
        {

            string searchstring = itscontent[i];

            size_t nFPos = searchstring.find("full-path=\"");

            if(nFPos!=std::string::npos)
            {

                ofLogVerbose("Treffer: Rootfile");

                size_t nFPos_end = searchstring.find("\"",nFPos+11);
                epub_path_rootfile = itscontent[i].substr(nFPos+11,nFPos_end-nFPos-11);

                ofFile cutfile(epub_path_rootfile);

                string cut = cutfile.getFileName();


                epub_path_root = itscontent[i].substr(nFPos+11,nFPos_end-nFPos-11-cut.size());


                 ofLogVerbose("Rootfile: ") <<  epub_path_rootfile;
                 ofLogVerbose("Path to Root: ") <<  epub_path_root;

            }else{

            }
        }


 }



 void ofApp::ePubFindPaths(){

   std::vector<std::string>    itscontent;

    string line;

    string tmpp;
    tmpp = "data/DocumentRoot/temp/" + epub_path_rootfile;

    ifstream myfile(tmpp.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
          getline (myfile,line);
          itscontent.push_back(line);
        }
        myfile.close();
    }

    /// File in Vektor einlesen


    /// content.opf Linie für Linie ändern

    for(int i = 0;i<itscontent.size();i++)
        {

            string ts4 = itscontent[i];
            string ts5 = itscontent[i];


            size_t nFPos = ts4.find("media-type=\"image");

            if(nFPos!=std::string::npos)
            {

                  ofLogVerbose("Treffer: ITEM IMAGE");

                  size_t s_search = ts4.find("href=\"");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: Path");

                         size_t s_search_end = ts4.find("\/",s_search+6);
                         size_t s_search_control = ts4.find("\"",s_search+6);



                         if((s_search_end!=std::string::npos)&&(s_search_end<s_search_control)){


                               ofLogVerbose("Treffer: Unterpfad gefunden");

                               epub_path_image = itscontent[i].substr(s_search+6,s_search_end-s_search-5);

                               ofLogVerbose("ImagePath: ") << epub_path_image;

                         }else{


                               epub_path_image = epub_path_root;

                               ofLogVerbose("ImagePath: ") << epub_path_image;

                         }

                  }
            }


            nFPos = ts4.find("media-type=\"application\/xhtml+xml");

            if(nFPos!=std::string::npos)
            {

                  ofLogVerbose("Treffer: ITEM Text");

                  size_t s_search = ts4.find("href=\"");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: Path");

                         size_t s_search_end = ts4.find("\/",s_search+6);
                         size_t s_search_control = ts4.find("\"",s_search+6);



                         if((s_search_end!=std::string::npos)&&(s_search_end<s_search_control)){

                               ofLogVerbose("Treffer: Unterpfad gefunden");

                               epub_path_text = itscontent[i].substr(s_search+6,s_search_end-s_search-5);

                               ofLogVerbose("TextPath: ") << epub_path_text;

                         }else{

                               string cutpath;
                               cutpath = epub_path_root;

                               ofLogVerbose("kein Unterordner ") << epub_path_text;

                               epub_path_text = epub_path_root;



                               ofLogVerbose("Textpath: ") << epub_path_text;

                         }

                  }
            }




            nFPos = ts4.find("text\/css");

            if(nFPos!=std::string::npos)
            {

                  ofLogVerbose("Treffer: ITEM CSS");

                  size_t s_search = ts4.find("href=\"");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: Path");

                         size_t s_search_end = ts4.find("\/",s_search+6);
                        size_t s_search_control = ts4.find("\"",s_search+6);



                         if((s_search_end!=std::string::npos)&&(s_search_end<s_search_control)){

                               ofLogVerbose("Treffer: Unterpfad gefunden");

                               epub_path_style = itscontent[i].substr(s_search+6,s_search_end-s_search-5);

                               ofLogVerbose("CSS-Path: ") << epub_path_style;

                         }else{


                               epub_path_style = epub_path_root;

                               ofLogVerbose("Css-Path: ") << epub_path_style;

                         }

                  }
            }

        nFPos = ts4.find("reference href");

            if(nFPos!=std::string::npos)
            {

                  ofLogVerbose("Treffer: Guide Cover");

                  size_t s_search = ts4.find("href=\"");
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: Path");

                         size_t s_search_end = ts4.find("\"",s_search+6);


                               epub_path_guide = itscontent[i].substr(s_search+6,s_search_end-s_search-5);
                               epub_path_guide_line = itscontent[i];

                               ofLogVerbose("CSS-Path: ") << epub_path_guide;



                  }
            }








        }

 }

 void ofApp::ePubFinalizeContent() {

    string correctpath;
    correctpath="data/DocumentRoot/temp/" + epub_path_rootfile;

    int ti = std::remove( correctpath.c_str() );
     if( ti != 0 ){
                          ofLogVerbose("File not Ebene: content.opf " );
                        }else {
                           ofLogVerbose("File successfully deleted: content.opf" );
                        }

    /*

     int ti = std::remove( ss.str().c_str() );

                        if( ti != 0 ){}
                          //  perror("\tError deleting file");
                        else {
                           ofLogVerbose("File successfully deleted: 1. Ebene " ) << ofToString(i);
                        }
    */

   ofstream myoutfile (correctpath.c_str());
        if (myoutfile.is_open())
        {



             for(size_t i = 0; i < epub_opf_head.size(); i++)
        {
            myoutfile << epub_opf_head[i] << std::endl;
        }

        myoutfile << "<manifest>" << std::endl;

         for(size_t i = 0; i < epub_opf_item.size(); i++)
        {
            myoutfile << epub_opf_item[i].line << std::endl;
        }

         myoutfile << "</manifest>" << std::endl;
         myoutfile << "<spine toc=\"ncx\">" << std::endl;



        for(int j = 0; j < 50; j++) {
            for(size_t i = 0; i < epub_opf_item.size(); i++)
            {

            if((epub_opf_item[i].spine_pos == j)&&(epub_opf_item[i].mediatype == "application/xhtml+xml"))
            {
            myoutfile << epub_opf_item[i].spineline << std::endl;
            }
             }
        }

         myoutfile << "</spine>" << std::endl;
         myoutfile << "<guide>" << std::endl;
         myoutfile << epub_opf_guide << std::endl;
         myoutfile << "</guide>" << std::endl;
         myoutfile << "</package>" << std::endl;




           myoutfile.close();
        }
     else cout << "Unable to open file";

 }


 void ofApp::ePubFinalizeToc() {

    string correctpath;

    string toci = "toc.ncx";
    correctpath = "data/DocumentRoot/temp/" + epub_path_root + toci;

    int ti = std::remove( correctpath.c_str() );
     if( ti != 0 ){
                          ofLogVerbose("File not Ebene: toc.ncx" );
                        }else {
                           ofLogVerbose("File successfully deleted: toc.ncx" );
                        }

   ofstream myoutfile (correctpath.c_str());
        if (myoutfile.is_open())
        {



        for(size_t i = 0; i < epub_toc_head.size(); i++)
        {
            myoutfile << epub_toc_head[i] << std::endl;
        }

        myoutfile << "<navMap>" << std::endl;

         for(size_t i = 0; i < epub_toc_navpoint.size(); i++)
        {
            myoutfile << "<navPoint id=\"" << epub_toc_navpoint[i].id << "\" playOrder=\"" << epub_toc_navpoint[i].playOrder << "\">" << std::endl;
             myoutfile << "<navLabel>" << std::endl;
              myoutfile << "<text>" << epub_toc_navpoint[i].label << "</text>" << std::endl;
             myoutfile << "</navLabel>" << std::endl;
                myoutfile << "<content src=\"" << epub_toc_navpoint[i].contentpath << "\"/>" << std::endl;
                 myoutfile << "</navPoint>" << std::endl;

        }

         myoutfile << "</navMap>" << std::endl;
         myoutfile << "</ncx>" << std::endl;

         myoutfile.close();
        }
     else cout << "Unable to open file";

 }


void ofApp::ePubAddChapter(ofx::JSONRPC::MethodArgs& args)
{


   string chaptername = args.params.asString();

   ///verify
   bool verify = true;

   for(int i = 0; i<epub_opf_item.size();i++) {

   if(epub_opf_item[i].id=="id_" + chaptername) verify = false;

   }

   ///verify

   if(verify) {

   ofBuffer tempbuffer;

   tempbuffer = ofBufferFromFile("epubessentials/header.html");
   tempbuffer.append("\n");

    tempbuffer.append("New Chapter\n");

   tempbuffer.append("</body>\n");
   tempbuffer.append("</html>");


   /// Titel korrigieren



  string title_search = tempbuffer.getText();
  string newbuffer = title_search;


   size_t title_begin = title_search.find("<title>");
            if(title_begin!=std::string::npos){

               newbuffer.insert(title_begin+7,chaptername);

            }

 tempbuffer.clear();
 tempbuffer = newbuffer;




   /// Title korrigieren





   string correctpath;
   correctpath = epub_path_text + chaptername + ".xhtml";

   ofBufferToFile("DocumentRoot/temp/" + correctpath,tempbuffer);

   item tmp_item;
   tmp_item.mediatype = "application/xhtml+xml";
   tmp_item.contentpath = correctpath;
   tmp_item.id = "id_" + chaptername;
   tmp_item.line = "<item href=\""+correctpath+"\" id=\""+tmp_item.id+"\" media-type=\"application/xhtml+xml\"/>";

   int getlastspinepos = 1;
   int checkspinepos;

   for(int i = 0; i<epub_opf_item.size();i++) {

   if(epub_opf_item[i].spine_pos>0)
        getlastspinepos++;

   }

   tmp_item.spine_pos = getlastspinepos;
   tmp_item.spineline = "<itemref idref=\""+tmp_item.id+"\" linear=\"yes\"/>";

   epub_opf_item.push_back(tmp_item);


   nav_point tmp_np;

   tmp_np.id = "id_" + chaptername;
   tmp_np.contentpath = correctpath;
   tmp_np.label = chaptername;
   tmp_np.playOrder = ofToString(getlastspinepos-2);

   epub_toc_navpoint.push_back(tmp_np);

   }

 updateGUI();



}

void ofApp::ePubAddChapter(string chaptername)
{




   ///verify
   bool verify = true;

   for(int i = 0; i<epub_opf_item.size();i++) {

   if(epub_opf_item[i].id=="id_" + chaptername) verify = false;

   }

   ///verify

   if(verify) {

   ofBuffer tempbuffer;

   tempbuffer = ofBufferFromFile("epubessentials/header.html");
   tempbuffer.append("\n");

    tempbuffer.append("New Chapter\n");

   tempbuffer.append("</body>");
   tempbuffer.append("</html>");


   /// Titel korrigieren



  string title_search = tempbuffer.getText();
  string newbuffer = title_search;


   size_t title_begin = title_search.find("<title>");
            if(title_begin!=std::string::npos){

               newbuffer.insert(title_begin+7,chaptername);

            }

 tempbuffer.clear();
 tempbuffer = newbuffer;




   /// Title korrigieren





   string correctpath;
   correctpath = epub_path_text + chaptername + ".xhtml";

   ofBufferToFile("DocumentRoot/temp/" + correctpath,tempbuffer);


   item tmp_item;
   tmp_item.mediatype = "application/xhtml+xml";
   tmp_item.contentpath = correctpath;
   tmp_item.id = "id_" + chaptername;
   tmp_item.line = "<item href=\""+correctpath+"\" id=\""+tmp_item.id+"\" media-type=\"application/xhtml+xml\"/>";

   int getlastspinepos = 1;
   int checkspinepos;

   for(int i = 0; i<epub_opf_item.size();i++) {

   if(epub_opf_item[i].spine_pos>0)
        getlastspinepos++;

   }

   tmp_item.spine_pos = getlastspinepos;
   tmp_item.spineline = "<itemref idref=\""+tmp_item.id+"\" linear=\"yes\"/>";

   epub_opf_item.push_back(tmp_item);


   nav_point tmp_np;

   tmp_np.id = "id_" + chaptername;
   tmp_np.contentpath = correctpath;
   tmp_np.label = chaptername;
   tmp_np.playOrder = ofToString(getlastspinepos-2);

   epub_toc_navpoint.push_back(tmp_np);

   }

 updateGUI();



}

void ofApp::updateGUI(){

 dir.listDir("DocumentRoot/temp/" + epub_path_text); //works Hier muss der Ordner hin, der die html enthält°

 ofLogVerbose("UpdateGui") << "List new Text Dir.";

 files.clear();

 ofLogVerbose("UpdateGui") << "Files cleared";

    if( dir.size() > 0 ){
            files.assign(dir.size(), ofFile());

            for(int i = 0; i < (int)dir.size(); i++){
                files[i].open(dir.getPath(i),ofFile::ReadWrite);
            }
    }

 ofLogVerbose("UpdateGui") << "Files loaded";

 currentFile = 0;

  if( files.size() > 0 ){


    if((files[0].getExtension()=="html")||(files[0].getExtension()=="opf")||(files[0].getExtension()=="xhtml")){


            setTextareaWeb(files[0].getFileName());

        }
        else{

            Json::Value params2;
            params2["value"] = "Es kann kein Text angezeigt werden";
            Json::Value json;
            json = toJSONMethod("Server", "textarea", params2);
            sendJSONMessage(json);

        }

                     Json::Value json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
                sendJSONMessage(json);


                vector<string> vec;
                for(int i = 0; i < int(dir.size()); i++) {
                       if((files[0].getExtension()=="html")||(files[0].getExtension()=="opf")||(files[0].getExtension()=="xhtml"))
                        {
                            vec.push_back(files[i].getFileName());
                        }
                }


                // Create vector
                Json::Value params;
                for(int i = 0; i < vec.size(); i++) {
                    params.append(vec[i]);
                }

                    // Send vector
                Json::Value json2 = toJSONMethod("Server", "test-dropdown", params);
                sendJSONMessage(json2);



    }else{

    Json::Value params2;
        params2["value"] = "Es ist noch kein Kapitel vorhanden. Bitte erstellen Sie mit 'add chapter' ein neues Kapitel.";
        Json::Value json;
        json = toJSONMethod("Server", "textarea", params2);
        sendJSONMessage(json);

    json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
                sendJSONMessage(json);

    }


}

void ofApp::ePubNewEpub(ofx::JSONRPC::MethodArgs& args){

    string titlename = args.params.asString();

    currentEpubname = titlename;

    reset_all(); // Delete Files

    ofDirectory newTemp;
    newTemp.createDirectory("DocumentRoot/temp");
    newTemp.createDirectory("DocumentRoot/temp/text");
    newTemp.createDirectory("DocumentRoot/temp/images");
    newTemp.createDirectory("DocumentRoot/temp/styles");

    ofLogVerbose("Create") << "Directory Structure";

    ofDirectory Meta;
    Meta.listDir("epubessentials/META-INF");
    Meta.copyTo("DocumentRoot/temp/META-INF");

    ofFile tmpstyle("epubessentials/stylesheet.css");
    tmpstyle.copyTo("DocumentRoot/temp/styles");




    epub_toc_head.clear(); // Delete Navpoints
    epub_toc_navpoint.clear();

    epub_toc_head.push_back("<?xml version='1.0' encoding='utf-8'?>");
    epub_toc_head.push_back("<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"eng\">");
    epub_toc_head.push_back("<head>");
    epub_toc_head.push_back("<meta content=\"0815\" name=\"dtb:uid\"/>");
    epub_toc_head.push_back("<meta content=\"1\" name=\"dtb:depth\"/>");
    epub_toc_head.push_back("<meta content=\"0\" name=\"dtb:totalPageCount\"/>");
    epub_toc_head.push_back("<meta content=\"0\" name=\"dtb:maxPageNumber\"/>");
    epub_toc_head.push_back("</head>");
    epub_toc_head.push_back("<docTitle>");
    epub_toc_head.push_back("<text>" + titlename + "</text>");
    epub_toc_head.push_back("</docTitle>");




    epub_opf_item.clear();
    epub_opf_head.clear();

    epub_opf_head.push_back("<?xml version=\"1.0\"  encoding=\"UTF-8\"?>");
    epub_opf_head.push_back("<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"uuid_id\">");
    epub_opf_head.push_back("<metadata xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:calibre=\"http://calibre.kovidgoyal.net/2009/metadata\"");
    epub_opf_head.push_back("xmlns:dc=\"http://purl.org/dc/elements/1.1/\">");
    epub_opf_head.push_back("<dc:creator opf:role=\"aut\" opf:file-as=\"VfH\">VfH</dc:creator>");
    epub_opf_head.push_back("<dc:identifier id=\"uuid_id\" opf:scheme=\"uuid\">0815</dc:identifier>");
    epub_opf_head.push_back("<meta name=\"cover\" content=\"cover\"/>");
    epub_opf_head.push_back("<dc:title>" + titlename + "</dc:title>");
     epub_opf_head.push_back("<dc:language>en</dc:language>");
    epub_opf_head.push_back("</metadata>");


   item tmp_item;
   tmp_item.mediatype = "application/x-dtbncx+xml";
   tmp_item.contentpath = "toc.ncx";
   tmp_item.id = "ncx";
   tmp_item.line = "<item href=\"toc.ncx\" id=\"ncx\" media-type=\"application/x-dtbncx+xml\"/>";

   epub_opf_item.push_back(tmp_item);

   item tmp_item2;
   tmp_item2.mediatype = "text/css";
   tmp_item2.contentpath = "styles/stylesheet.css";
   tmp_item2.id = "css";
   tmp_item2.line = " <item href=\"styles/stylesheet.css\" id=\"css\" media-type=\"text/css\"/>";

   epub_opf_item.push_back(tmp_item2);


   epub_path_rootfile = "content.opf";
   epub_path_root = "/";

   epub_path_image = "images/";
     epub_path_text = "text/";
     epub_path_style = "styles/";

    updateGUI();

    Json::Value params3;
    params3["value"] = currentEpubname;
    Json::Value json5 = toJSONMethod("Server", "reset", params3);
    sendJSONMessage(json5);



}

void ofApp::ePubAddImage(string up_file,string original_filename, string file_Type){

       ofFile tmp_img(up_file);
       ofFile tmp_baseimg2(original_filename);


       /// Validator

       bool img_validation = true;

       for(size_t i = 0; i < epub_opf_item.size(); i++)
        {
            if(epub_opf_item[i].id=="id_" + tmp_baseimg2.getBaseName() ) {

                img_validation=false;
                ofLogVerbose("IMG") << "valid=false";

                file_del("bin/data/DocumentRoot/temp/" + epub_path_image + original_filename );
                 file_del("bin/data/DocumentRoot/images/" + original_filename );


               tmp_img.copyTo("DocumentRoot/temp/" + epub_path_image + original_filename,true,true );
               tmp_img.copyTo("DocumentRoot/images/" + original_filename,true,true );



               ofFile tmp_baseimg(ofToDataPath("DocumentRoot/temp/" + epub_path_image + original_filename));

                epub_opf_item[i].mediatype = file_Type;
                epub_opf_item[i].contentpath = epub_path_image + original_filename;
                epub_opf_item[i].id = "id_" + tmp_baseimg.getBaseName();
                epub_opf_item[i].line = "<item href=\""+epub_opf_item[i].contentpath+"\" id=\""+epub_opf_item[i].id+"\" media-type=\""+file_Type+"\"/>";



            }
        }

       /// Validator



       if(img_validation)
       {

       tmp_img.copyTo("DocumentRoot/temp/" + epub_path_image + original_filename );
       tmp_img.copyTo("DocumentRoot/images/" + original_filename );
       ofFile tmp_baseimg(ofToDataPath("DocumentRoot/temp/" + epub_path_image + original_filename));

       item tmp_item;
       tmp_item.mediatype = file_Type;
       tmp_item.contentpath = epub_path_image + original_filename;
       tmp_item.id = "id_" + tmp_baseimg.getBaseName();
       tmp_item.line = "<item href=\""+tmp_item.contentpath+"\" id=\""+tmp_item.id+"\" media-type=\""+file_Type+"\"/>";
       epub_opf_item.push_back(tmp_item);



        }

        string html_code;

       if((file_Type=="image/png")||(file_Type=="image/jpeg")||(file_Type=="image/gif"))
        {html_code = "<div style=\"width:100%;\"><img style=\"width:100%;\" alt=\"img\" src=\"../images/" + original_filename + "\"/></div>";}

        if((file_Type=="image/svg+xml")){
        html_code = "<div style=\"width:100%;\"><object data=\"../images/" + original_filename + "\" type=\"image/svg+xml\" width=\"100%\" height=\"100%\"></object></div>";
        }



        Json::Value params3;
        params3["value"] = html_code;
        Json::Value json5 = toJSONMethod("Server", "addimageready", params3);
        sendJSONMessage(json5);

}

void ofApp::file_del(string fdir){

   string fn;
                        fn = fdir;

                        /// Escape String
                        std::stringstream ss;
                        for (int j = 0; j < fn.length(); j++) {
                            if (fn[j] == '\\') {
                                ss << "\\\\";
                            }
                            else
                            {
                                ss << fn[j];
                            }
                        }
                        /// Escape String

                        int ti = std::remove( ss.str().c_str() );

                        if( ti != 0 ){
                          ofLogVerbose("File not deleted " ) << fdir;}
                        else {
                           ofLogVerbose("File successfully deleted" ) << fdir;
                        }

}

void ofApp::ePubAddCover(string up_file,string original_filename, string file_Type){



    /// ContentOPF

    if(epub_opf_reference=="null"){
         epub_opf_reference = "<reference href=\"text/titlepage.xhtml\" type=\"cover\" title=\"Cover\"/>";



      item tmp_item;
      tmp_item.mediatype = file_Type;
      tmp_item.contentpath = epub_path_image + original_filename;
      tmp_item.id = "id_cover";
      tmp_item.line = "<item href=\""+tmp_item.contentpath+"\" id=\""+tmp_item.id+"\" media-type=\""+file_Type+"\"/>";



       ofFile tmp_img(up_file);
       tmp_img.copyTo("DocumentRoot/temp/" + epub_path_image + original_filename );
       tmp_img.copyTo("DocumentRoot/images/" + original_filename );

       /// ContentOPF

 /// alles überprüfen

   ofBuffer tempbuffer;

   tempbuffer = ofBufferFromFile("epubessentials/titlepage2.html");
   tempbuffer.append("\n");

   tempbuffer.append("<svg xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"100%\" height=\"100%\" viewBox=\"0 0 423 600\" preserveAspectRatio=\"none\">\n<image width=\"423\" height=\"600\" xlink:href=\"../images/" + original_filename + "\"/>\n</svg>\n</div>\n</body>\n</html>");


   /// Titel korrigieren



  string title_search = tempbuffer.getText();
  string newbuffer = title_search;


   size_t title_begin = title_search.find("<title>");
            if(title_begin!=std::string::npos){

               newbuffer.insert(title_begin+7,currentEpubname);

            }

 tempbuffer.clear();
 tempbuffer = newbuffer;




   /// Title korrigieren





   string correctpath;
   correctpath = epub_path_text + "titlepage.xhtml";

   ofBufferToFile("DocumentRoot/temp/" + correctpath,tempbuffer);



   item tmp_itemtext;
   tmp_itemtext.mediatype = "application/xhtml+xml";
   tmp_itemtext.contentpath = correctpath;
   tmp_itemtext.id = "id_titlepage";
   tmp_itemtext.line = "<item href=\""+correctpath+"\" id=\""+tmp_itemtext.id+"\" media-type=\"application/xhtml+xml\"/>";

   tmp_itemtext.spine_pos = 0;
   tmp_itemtext.spineline = "<itemref idref=\""+tmp_itemtext.id+"\" linear=\"yes\"/>";

   epub_opf_item.push_back(tmp_itemtext);
   epub_opf_item.push_back(tmp_item);

   for(int i = 0; i<epub_opf_item.size();i++) {


       if(epub_opf_item[i].mediatype == "application/xhtml+xml")
        epub_opf_item[i].spine_pos++;

   }



   epub_opf_guide = "<reference href=\"text/titlepage.xhtml\" type=\"cover\" title=\"Cover\"/>";




    updateGUI();

    ofLogVerbose("Cover") << "added";

    }

covermode=false;

}

void ofApp::sendDatatoWeb(){


    /// Inhaltsverzeichnis

      vector<string> vec2;
    for(int i = 0; i < int(epub_toc_navpoint.size()); i++) {

                vec2.push_back(epub_toc_navpoint[i].label);

    }


    // Create vector
    Json::Value params2;
    for(int i = 0; i < vec2.size(); i++) {
        params2.append(vec2[i]);
    }

        // Send vector
    Json::Value jsontoc = toJSONMethod("Server", "set-toc", params2);
    sendJSONMessage(jsontoc);

     ofLogVerbose("List") << "Send Dropdowns";

    /// Inhaltsverzeichnis



}



