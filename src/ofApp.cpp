#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);

    // setup camera
    camera.setDeviceID( 1 );
    camera.setDesiredFrameRate( 30 );
    
    for( int i = 0; i < camera.listDevices().size(); i++ ){
        cout << i << "," << camera.listDevices()[i].deviceName << endl;
    }
    
    // lower resolution faster tracking.
    camera.initGrabber( 1280/2, 720/2 );
    
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    dat.setup("person", 0.2, 1, 5000, true);
  
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();

    if( camera.isFrameNew() ){
        dat.update(camera.getPixels());
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    camera.draw(0,0);
    dat.draw(0, 0, ofGetWidth(), ofGetHeight());
}



