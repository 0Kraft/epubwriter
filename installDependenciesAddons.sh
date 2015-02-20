#!/bin/bash
echo "Cloning dependencies addons for ofxHTTP"
echo "---------------------------------------"

echo "cloning ofxJSON"
git clone https://github.com/jefftimesten/ofxJSON.git

echo "cloning ofxJSONRPS"
git clone https://github.com/bakercp/ofxJSONRPC.git

echo "cloning ofxHTTP"
git clone https://github.com/bakercp/ofxHTTP.git

echo "cloning ofxIO"
git clone https://github.com/bakercp/ofxIO.git

echo "cloning ofxMediaType"
git clone https://github.com/bakercp/ofxMediaType.git

echo "cloning ofxSSLManager"
git clone https://github.com/bakercp/ofxSSLManager.git

echo "cloning ofxTaskQueue"
git clone https://github.com/bakercp/ofxTaskQueue.git