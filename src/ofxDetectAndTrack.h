//
//  ofxDetectAndTrack.hpp
//  sample_detect_then_track
//
//  Created by Tetsuaki Baba on 2021/06/15.
//

#ifndef ofxDetectAndTrack_h
#define ofxDetectAndTrack_h

#include <stdio.h>
#include "ofxOpenCvTracker.h"
#include "ofxOpenCvDnnObjectDetection.h"

#endif /* ofxDetectAndTrack_ */

class ofxDetectAndTrack{
public:
    ofxDetectAndTrack();
    ~ofxDetectAndTrack();
    
    /*
     tracking_image_scale: tracker用の画像スケール（小さい方が処理が高速になる）
     adjustment_freq: 一秒間に何回物体検出を使ってtrackingの位置を修正するか．0の場合は一度検出したら位置修正は行わない
     single_tracking: 物体検出は一つだけにするかどうか
     */
    void setup(string _tracking_label,
               float _tracking_image_scale,
               float _adjustment_freq,
               unsigned long _threshold_cancel_tracking,
               bool _single_tracking);
    void draw(int _x, int _y, float _w, float _h);
    void update(ofPixels &_pixels);

    ofImage img_original;
    ofImage img_small;
    vector<ofxOpenCvTracker> tracker;
    vector<unsigned long> timestamp_last_found; // 任意の物体を検出したときのtimestamp
    ofRectangle r;
    uint64_t tracking_time;
    ofImage img_camera_small;
    ofxOpenCvDnnObjectDetection detector;
    float tracking_image_scale;
    float adjustment_freq;
    unsigned long threshold_cancel_tracking;
    bool single_tracking;
    string tracking_label;
    unsigned long timestamp_last_detection; // ディテクションを実行したtimestamp

    ofRectangle r_debug;
    ofRectangle r_debug2;

};
