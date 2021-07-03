//
//  ofxDetectAndTrack.cpp
//  sample_detect_then_track
//
//  Created by Tetsuaki Baba on 2021/06/15.
//

#include "ofxDetectAndTrack.h"

ofxDetectAndTrack::ofxDetectAndTrack()
{
    
}

ofxDetectAndTrack::~ofxDetectAndTrack()
{
    
}

void ofxDetectAndTrack::setup(string _tracking_label,
                              float _tracking_image_scale,
                              float _adjustment_freq,
                              unsigned long _threshold_cancel_tracking,
                              bool _single_tracking)
{
    
    tracker.clear();
    
    detector.setNetworkImageSize(416, 416);
    detector.setup(ofToDataPath("dnn/yolov4-tiny.cfg"),
                   ofToDataPath("dnn/yolov4-tiny.weights"),
                   ofToDataPath("dnn/coco.names"));
    detector.setConfidenceThreshold(0.6);
    
    
    tracking_label = _tracking_label;
    tracking_image_scale = _tracking_image_scale;
    adjustment_freq = _adjustment_freq;
    single_tracking = _single_tracking;
    threshold_cancel_tracking = _threshold_cancel_tracking;
    timestamp_last_detection = ofGetElapsedTimeMillis();
}

void ofxDetectAndTrack::update(ofPixels &_pixels)
{
    img_original.setFromPixels(_pixels.getData(),
                               _pixels.getWidth(),
                               _pixels.getHeight(),
                               OF_IMAGE_COLOR);
    
    img_small.setFromPixels(_pixels.getData(),
                            _pixels.getWidth(),
                            _pixels.getHeight(),
                            OF_IMAGE_COLOR);
    
    img_small.resize(img_original.getWidth()*tracking_image_scale,
                     img_original.getHeight()*tracking_image_scale);
    
    
    
    uint64_t t0 = ofGetElapsedTimeMillis();

    cout << "size of timestamp: " << timestamp_last_found.size() << "," << tracker.size() << endl;
    for( int i = 0; i < timestamp_last_found.size(); i++){
        if( (ofGetElapsedTimeMillis()-timestamp_last_found[i]) > threshold_cancel_tracking ){
            tracker.erase(tracker.begin()+i);
            timestamp_last_found.erase(timestamp_last_found.begin()+i);
            break;
            cout << "clear" << endl;
        }
    }
    

    
    for( int i = 0; i < tracker.size(); i++)
    {
        tracker[i].update(img_small.getPixels());
    }
    
    
    // トラッキング対象物が一つも無いときは，物体検出を行う
    if( tracker.size() == 0 ){
        detector.update(img_original.getPixels());
        timestamp_last_detection = ofGetElapsedTimeMillis();
        
        for(int i = 0; i < detector.object.size(); i++ ){
            if(detector.object[i].name == tracking_label &&
               detector.object[i].p > 0.6 ){

                ofxOpenCvTracker t;
                t.setup("KCF");
                t.set(img_small.getPixels(),
                      detector.object[i].getScaledBB(img_small.getWidth(),
                                                     img_small.getHeight()));
                t.setName(detector.object[i].name);
                tracker.push_back(t);
                timestamp_last_found.push_back(ofGetElapsedTimeMillis());
                if( single_tracking) i = detector.object.size();
            }
        }
    }
    else{
        if(adjustment_freq == 0 || (ofGetElapsedTimeMillis()-timestamp_last_detection) > 1000/adjustment_freq){
            detector.update(img_original.getPixels());
            timestamp_last_detection = ofGetElapsedTimeMillis();

            for( int i = 0; i < detector.object.size(); i++ ){
                if(detector.object[i].name == tracking_label &&
                   detector.object[i].p > 0.6 ){

                    // 検出した person オブジェクトと，tracking中のオブジェクトに対して，
                    // 領域が一定以上重なっている場合は，検出したpersonオブジェクトに変更する
                    ofRectangle r_detected = detector.object[i].getScaledBB(img_original.getWidth(), img_original.getHeight());
                    
                    // r_detectedのサイズをtracker用の画像サイズに合わせる
                    r_detected.x *= tracking_image_scale;
                    r_detected.y *= tracking_image_scale;
                    r_detected.width *= tracking_image_scale;
                    r_detected.height *= tracking_image_scale;
                    

                    for( int j = 0; j < tracker.size(); j++ ){
                        ofRectangle r_tracked = tracker[j].getTrackerRectangle();
                        ofRectangle r_intersected = r_tracked.getIntersection(r_detected);

                        
                        if( r_intersected.getArea()/r_tracked.getArea() >= 0.5 ){
                            r_debug = r_intersected;
                            r_debug.x /= tracking_image_scale;
                            r_debug.y /= tracking_image_scale;
                            r_debug.width /= tracking_image_scale;
                            r_debug.height /= tracking_image_scale;
                            r_debug2 = r_detected;
                            r_debug2.x /= tracking_image_scale;
                            r_debug2.y /= tracking_image_scale;
                            r_debug2.width /= tracking_image_scale;
                            r_debug2.height /= tracking_image_scale;;
                            
                            tracker[j].setRectangle(r_detected, img_small.getPixels());
                            timestamp_last_found[j] = ofGetElapsedTimeMillis();
                        }
                    }
                }
            }
        }
    }
    tracking_time = ofGetElapsedTimeMillis()-t0;

}

void ofxDetectAndTrack::draw(int _x, int _y, float _w, float _h)
{
    ofNoFill();
    ofSetLineWidth(5.0);
    for( int i = 0; i < tracker.size(); i++ ){
        if( tracker[i].getFound() ){
            ofRectangle r(tracker[i].getTrackerRectangle());
            ofSetColor(ofColor::navy);
            float s_x = 1/tracking_image_scale;
            float s_y = 1/tracking_image_scale;
            r.x = r.x*s_x;
            r.y = r.y*s_y;
            r.width = r.width*s_x;
            r.height = r.height*s_y;
            
            ofDrawRectangle(r);
            ofDrawBitmapStringHighlight("["+ofToString(i)+"]:"+tracker[i].name,
                                        r.x, r.y-6);
            ofDrawBitmapStringHighlight("Tracking Time: "+ofToString(tracking_time),
                                        20,20);
        }
        else{
            tracker.erase(tracker.begin()+i);
            timestamp_last_found.erase(timestamp_last_found.begin()+i);
        }
    }
    if( tracker.size() == 0 ){
        detector.draw(_x,_y,_w, _h);
    }
}
