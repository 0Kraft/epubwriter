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


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);

    ofVideoGrabber cam;
    ofFbo fbo;

    ofImage img;

    void initServerJSONRPC(int port);
    void initServerVideo(int port);


    // Registered methods.
    void getCheckBox(ofx::JSONRPC::MethodArgs& args);
    void getSlider(ofx::JSONRPC::MethodArgs& args);
    void getButton(void);
    void getTextArea2(ofx::JSONRPC::MethodArgs& args);
    void getDropdown(ofx::JSONRPC::MethodArgs& args);
    void getEpubName(ofx::JSONRPC::MethodArgs& args);
    void startVideoServer(void);
    void stopVideoServer(void);

    /// \brief The server that handles the JSONRPC requests.
    ofx::HTTP::BasicJSONRPCServer::SharedPtr serverJSON;

    /// \brief The server that handles the Video stream.
    ofx::HTTP::BasicIPVideoServer::SharedPtr serverVideo;

    /// \brief Get the user text in a thread-safe way.
    /// \returns The user text.
    std::string getUserText() const;

    std::string getTextArea() const;

    /// \brief Set the user text in a thread-safe way.
    /// \param text the user text to set.
    void setUserText(const std::string& text);
    void setTextArea(const std::string& text);

    float getSliderValue() const;
    void setSliderValue(const float& f);

    bool getCheckboxValue() const;
    void setCheckboxValue(const bool& b);


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
    void ePubZip();
	void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info);
    void onOk(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info);
	bool zipped;

	// Delete

	string dir_del(string dir);



		string test;
			string test2;

			string filename3;

			int ziptimer;

        ofDirectory dir;


        vector<ofFile> files; // Current Files of Directory
        int currentFile;   // Chosen File
        string currentFilename;
        string currentEpubname;
        ofBuffer currentFileBuffer; //Content of ChosenFile




        float           nextLetterTime;
        int             lineCount;
        int             letterCount;
        vector <string> seussLines;

        ofBuffer buffer;

        string filecontent;
         string filecontent_file;


    ///ZIPSHIT







private:
    // A custom logging channel to mirror all log messages to the web clients.
    WebSocketLoggerChannel::SharedPtr loggerChannel;

    // This piece of text might be modified by multiple client threads.
    // Thus we must use a mutex to protect it during multi-threaded access.
    std::string userText;
    float sliderValue;
    bool checkboxValue;

    std::string textarea;

    // We use a mutex to protect any variables that can be
    // modified by multiple clients.  In our case, userText must be protected.
    // We mark the mutex as mutable so that it can be used in const functions.
    mutable ofMutex mutex;

    //UploadRouter uploadRouter;
};
