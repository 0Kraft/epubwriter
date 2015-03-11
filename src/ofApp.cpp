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
}

void ofApp::update()
{

    if(zipped)  ziptimer++;

    if(ziptimer>50) {
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

    ofDrawBitmapString("----------------------------------------", 5,50);

    /// DIR

    ofDrawBitmapString(epub_path_rootfile, 350,20);
    ofDrawBitmapString(epub_path_image, 350,40);
    ofDrawBitmapString(epub_path_text, 350,60);
    ofDrawBitmapString(epub_path_style, 350,80);


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

          ePubAddChapter("newchapter");

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

   /// save file

   files[currentFile].setWriteable(true);



   ofBuffer tempbuffer;
   tempbuffer.append("<body>");
   tempbuffer.append("\n");
   tempbuffer.append(args.params.asString());
   tempbuffer.append("\n");
   tempbuffer.append("</body>");


  // ofBuffer tmp_buf_file;
  // tmp_buf_file =  files[currentFile].readToBuffer();

   string tmppath;
   tmppath = files[currentFile].getAbsolutePath();

   files[currentFile].close();

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




   files[currentFile].open(tmppath,ofFile::Reference);

   ofLogVerbose("Write ") << " to File: " << currentFilename << " Nr.: " << currentFile;

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
           //ePubUnzipFlat(up_filename);
           ePubUnzip(up_filename);

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
Poco::Zip::Decompress dec(inp, "data/DocumentRoot/tempzip",false,true);

// if an error happens invoke the ZipTest::onDecompressError method

dec.decompressAllFiles();



    ofDirectory zipdir;

        zipdir.listDir("DocumentRoot/tempzip");
        zipdir.copyTo("DocumentRoot");
        zipdir.renameTo("DocumentRoot/temp");

    zipdir.close();
    zipped=true;



}



void ofApp::ePubZip(){


ePubFinalizeContent();
ePubFinalizeToc();

///MIMETYPE löschen

string correctpath;
correctpath="data/DocumentRoot/temp/mimetype";

    int ti = std::remove( correctpath.c_str() );
     if( ti != 0 ){
                          ofLogVerbose("File not Ebene: content.opf " );
                        }else {
                           ofLogVerbose("File successfully deleted: content.opf" );
                        }

///MIMETYPE löschen


string zn =  "data/DocumentRoot/"+ currentEpubname + ".epub";
std::ofstream out( zn.c_str() , std::ios::binary);
Poco::Zip::Compress c(out, true);

Poco::Path theFile("data/epubessentials/mimetype");
c.addFile(theFile, "mimetype",Poco::Zip::ZipCommon::CM_STORE ,Poco::Zip::ZipCommon::CL_NORMAL);

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




 Poco::Path data("data\\DocumentRoot\\temp");
 data.makeDirectory();
 c.addRecursive(data);



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

c.close(); // MUST be done to finalize the Zip file

Json::Value json = toJSONMethod("Server", "zipready", 0);
sendJSONMessage(json);

}


void ofApp::ePubList(){

    dir.listDir("DocumentRoot/temp"); //works

    ePubFindMetaInf();
    ePubFindPaths();




    ePubReadInContent();
    ePubReadInToc();



    ofDirectory i_dir;
    i_dir.listDir("DocumentRoot/temp/" + epub_path_root + epub_path_image);
    i_dir.copyTo("DocumentRoot/images");
    i_dir.close();

    ofLogVerbose(epub_path_rootfile);
    //ofLogVerbose(epub_path_text);


   dir.listDir("DocumentRoot/temp/" + epub_path_text); //works Hier muss der Ordner hin, der die html enthält°


    if( dir.size() > 0 ){
            files.assign(dir.size(), ofFile());

            for(int i = 0; i < (int)dir.size(); i++){
                files[i].open(dir.getPath(i),ofFile::ReadWrite);
            }
    }



    currentFile = 0;


    if((files[0].getExtension()=="html")||(files[0].getExtension()=="opf")||(files[0].getExtension()=="xhtml")){


        currentFileBuffer = ofBufferFromFile(files[0].getAbsolutePath());

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


    Json::Value json = toJSONMethod("Server", "test-dropdown-removeAll", 0);
    sendJSONMessage(json);


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

    if(currentFileBuffer.size()){

    Json::Value params2;
    params2["value"] = currentFileBuffer.getText();

    json = toJSONMethod("Server", "textarea", params2);
    sendJSONMessage(json);
    }

    Json::Value params3;
    params3["value"] = currentEpubname;
    Json::Value json5 = toJSONMethod("Server", "reset", params3);
    sendJSONMessage(json5);

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


                        string fn;
                        fn = tmpfiles[i].getAbsolutePath();

                        ofDirectory tmpDir2;
                        tmpDir2.listDir(fn);

                        ofLogVerbose("Files listed: check!" );

                        vector<ofFile> tmpfiles2 = tmpDir2.getFiles();

                        int lauf2 = (int)tmpfiles2.size();

                        for(int a = 0; a < lauf2; a++){

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

            size_t bpos2=0;

            size_t nFPos = ts4.find("item");
            size_t nFPos2 = ts5.find("media-type");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: ITEM");


                  /// JPEG

                  size_t s_search = ts4.find("image/jpeg",bpos2);
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: JPEG");

                         size_t s_path = ts4.find("href=\"images",bpos2);

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=",bpos2);

                               itscontent[i].insert(s_pathbegin+6,"images/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// JPEG

                  /// PNG

                  s_search = ts4.find("image/png",bpos2);
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: PNG");

                         size_t s_path = ts4.find("href=\"images",bpos2);

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=",bpos2);

                               itscontent[i].insert(s_pathbegin+6,"images/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// PNG

                   /// XHTML

                  s_search = ts4.find("application/xhtml+xml",bpos2);
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: XHTML");

                         size_t s_path = ts4.find("href=\"text",bpos2);

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=",bpos2);

                               itscontent[i].insert(s_pathbegin+6,"text/");

                               ofLogVerbose("Treffer: Pfad korrigiert");

                         }

                  }

                  /// XHTML

                  /// CSS

                  s_search = ts4.find("text/css",bpos2);
                  if(s_search!=std::string::npos){

                         ofLogVerbose("Treffer: CSS");

                         size_t s_path = ts4.find("href=\"styles\/",bpos2);

                         if(s_path==std::string::npos){

                               ofLogVerbose("Treffer: Pfad nicht korrekt");

                               size_t s_pathbegin = ts4.find("href=",bpos2);

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

            size_t bpos2=0;

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

            size_t bpos2=0;

            size_t nFPos = ts4.find("content");
            size_t nFPos2 = ts5.find("src=");

            if((nFPos!=std::string::npos)&&(nFPos2!=std::string::npos))
            {

                  ofLogVerbose("Treffer: Content");


                  /// Content

                  size_t s_search = ts4.find("src=\"text\/",bpos2);
                  if(s_search==std::string::npos){

                         ofLogVerbose("Treffer: Pfad nicht korrekt");


                               size_t s_pathbegin = ts4.find("src=",bpos2);

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


            size_t bpos2=0;

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



    ofstream myoutfile ("tocparsed.txt");
        if (myoutfile.is_open())
        {

             for(size_t i = 0; i < epub_toc_navpoint.size(); i++)
        {
            myoutfile << epub_toc_navpoint[i].label << std::endl;
             myoutfile << epub_toc_navpoint[i].id << std::endl;
              myoutfile << epub_toc_navpoint[i].contentpath << std::endl;
               myoutfile << epub_toc_navpoint[i].playOrder << std::endl;
                myoutfile << "-------------" << std::endl;
        }

           myoutfile.close();
        }
     else cout << "Unable to open file";



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

                epub_path_root = itscontent[i].substr(nFPos+11,nFPos_end-nFPos-22);


                 ofLogVerbose("Rootfile: ") <<  epub_path_rootfile;
                 ofLogVerbose("Path to Root: ") <<  epub_path_root;

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

            size_t bpos2=0;

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

                               epub_path_text = cutpath.substr(0,cutpath.length()-11);



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

         for(size_t i = 0; i < epub_opf_item.size(); i++)
        {

            for(int j = 0; j < 50; j++) {
            if(epub_opf_item[i].spine_pos == j){
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

   tempbuffer.append("</body>");
   tempbuffer.append("</html>");

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

 files.clear();

    if( dir.size() > 0 ){
            files.assign(dir.size(), ofFile());

            for(int i = 0; i < (int)dir.size(); i++){
                files[i].open(dir.getPath(i),ofFile::ReadWrite);
            }
    }



    currentFile = 0;


    if((files[0].getExtension()=="html")||(files[0].getExtension()=="opf")||(files[0].getExtension()=="xhtml")){


        currentFileBuffer = ofBufferFromFile(files[0].getAbsolutePath());

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

    if(currentFileBuffer.size()){

    Json::Value params2;
    params2["value"] = currentFileBuffer.getText();

    json = toJSONMethod("Server", "textarea", params2);
    sendJSONMessage(json);
    }




}




