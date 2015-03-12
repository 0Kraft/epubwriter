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


#pragma once


#include "ofMain.h"
#include "ofxJSONRPC.h"
#include "WebSocketLoggerChannel.h"

#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Zip/ZipCommon.h"
#include "Poco/Delegate.h"
#include "Poco/Checksum.h"
#include "Poco/Util/Application.h"

#include <stdio.h>

struct nav_point {
    string id;
    string playOrder;
    string label;
    string contentpath;



};

struct item {
    string id;
    string mediatype;
    string contentpath;
    string fallbackpath;
    int spine_pos;
    string line;
    string spineline;

};





class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);

    void initServerJSONRPC(int port);

    void updateGUI();

    void getTextArea2(ofx::JSONRPC::MethodArgs& args);
    void getDropdown(ofx::JSONRPC::MethodArgs& args);
    void getEpubName(ofx::JSONRPC::MethodArgs& args);

    /// \brief The server that handles the JSONRPC requests.
    ofx::HTTP::BasicJSONRPCServer::SharedPtr serverJSON;

     /// \brief Get the user text in a thread-safe way.
    /// \returns The user text.
    std::string getUserText() const;

    std::string getTextArea() const;

    /// \brief Set the user text in a thread-safe way.
    /// \param text the user text to set.
    void setUserText(const std::string& text);
    void setTextArea(const std::string& text);

     void setTextareaWeb(string fn);

    void onHTTPPostEvent(ofx::HTTP::PostEventArgs& evt);
    void onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& evt);
    void onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& evt);

    // Helpers
    Json::Value toJSONMethod(const std::string& module, const std::string& method, const Json::Value& params);
    void sendJSONMessage(Json::Value json);


   //
    string up_filename;

    void ePubList();



    // ZIP

    void ePubUnzip(string i_file);
    void ePubUnzipFlat(string i_file);
    void ePubZip();

    void ePubParseContent();
    void ePubParseToc();
    void ePubReadInToc();
    void ePubReadInContent();

    void ePubFindPaths();
    void ePubFindMetaInf();

    void ePubFinalizeContent();
    void ePubFinalizeToc();

    void ePubAddChapter(ofx::JSONRPC::MethodArgs& args);
    void ePubNewEpub(ofx::JSONRPC::MethodArgs& args);

	bool zipped;
	int ziptimer;
	// Structuring Epub

	void reset_all();
	void cleanup_structure();
	void dir_del(string fdir);


    ofDirectory dir;
    vector<ofFile> files; // Current Files of Directory
    int currentFile;   // Chosen Filenumber
    string currentFilename;
    string currentEpubname; // richtiger Name
    ofBuffer currentFileBuffer; //Content of ChosenFile
    string textarea;

    ///toc headers

    string epub_uid;
    int epub_toc_depth;

    std::vector<std::string>    epub_toc_head;
    std::vector<nav_point>    epub_toc_navpoint;


    string epub_path_root;
    string epub_path_rootfile;

    string epub_path_image;
    string epub_path_text;
    string epub_path_style;


    ///

    ///content.opf headers

    string epub_creator;
    string epub_title;
    string epub_language;


    std::vector<item>    epub_opf_item;
    std::vector<std::string>    epub_opf_head;
    string epub_opf_reference;



    string epub_opf_guide;


    std::vector<std::string>    target_opf;

    ///

    /// Display Buffer in OF

    float           nextLetterTime;
    int             lineCount;
    int             letterCount;
    vector <string> seussLines;

    /// Display Buffer in OF

    private:
    // A custom logging channel to mirror all log messages to the web clients.
    WebSocketLoggerChannel::SharedPtr loggerChannel;

    // We use a mutex to protect any variables that can be
    // modified by multiple clients.  In our case, userText must be protected.
    // We mark the mutex as mutable so that it can be used in const functions.

    mutable ofMutex mutex;

    //UploadRouter uploadRouter;
};
