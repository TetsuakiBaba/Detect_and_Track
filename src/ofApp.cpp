#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  
    ofSetFrameRate(60);
    // setup camera

    camera.setDeviceID( 1 );
    camera.setDesiredFrameRate( 30 );
    
    // lower resolution faster tracking.
    camera.initGrabber( 1280, 720 );
    
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    ofSetWindowShape(camera.getWidth(), camera.getHeight());
    tracker.clear();
    
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov4-tiny.cfg"),
                   ofToDataPath("dnn/yolov4-tiny.weights"),
                   ofToDataPath("dnn/coco.names"));
    detector.setConfidenceThreshold(0.6);
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    if( camera.isFrameNew() ){
        uint64_t t0 = ofGetElapsedTimeMillis();
        for( int i = 0; i < tracker.size(); i++){
            tracker[i].update(camera.getPixels());
        }
        tracking_time = ofGetElapsedTimeMillis()-t0;
        if( tracker.size() == 0 ){
            
            detector.update(camera.getPixels());
            for(int i = 0; i < detector.object.size(); i++ ){
                if(detector.object[i].name == "person" &&
                   detector.object[i].p > 0.6 ){
                    ofxOpenCvTracker t;
                    t.setup("KCF");
                    t.set(camera.getPixels(),
                          detector.object[i].getScaledBB(camera.getWidth(), camera.getHeight()));
                    t.setName(detector.object[i].name);
                    tracker.push_back(t);
                    //i = detector.object.size();
                }
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    camera.draw(0,0);
    
    ofDrawRectangle(r);

    ofRectangle r;
    ofNoFill();

    for( int i = 0; i < tracker.size(); i++ ){
        if( tracker[i].getFound() ){
            ofRectangle r(tracker[i].getTrackerRectangle());
            ofSetColor(ofColor::navy);
            ofDrawRectangle(r);
            ofDrawBitmapStringHighlight("["+ofToString(i)+"]:"+tracker[i].name,
                                        r.x, r.y-6);
            ofDrawBitmapStringHighlight("Tracking Time: "+ofToString(tracking_time),
                                        20,20);
            

        }
        else{
            tracker.erase(tracker.begin()+i);
        }
    }
    if( tracker.size() == 0 ){
        detector.draw(0,0,ofGetWidth(), ofGetHeight());
    }
   

}

void ofApp::keyPressed(int key)
{
    if( key == 'c' ){ // clear all tracker
        tracker.clear();
    }
    
}

void ofApp::mousePressed(int x, int y, int button)
{
    r.x = x;
    r.y = y;
}
void ofApp::mouseReleased(int x, int y, int button)
{
    r.width = x-r.x;
    r.height = y-r.y;
    
    // ignore small selection
    if( r.getArea() > 10.0 ){
        ofxOpenCvTracker t;
        t.setup("KCF");
        t.set(camera.getPixels(), r);
        tracker.push_back(t);
    }
    
    r.set(0,0,0,0);
}
void ofApp::mouseDragged(int x, int y, int button){
    r.width = x-r.x;
    r.height = y-r.y;
}



