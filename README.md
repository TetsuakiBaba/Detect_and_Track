# Detect and Track

<img src="sample_images/May-28-2021 15-38-41.gif" width="100%">

## Description
This is an automatic tracking sample. Once it detects the object, such as person, car, anything in coco 80 class labels 
ofxOpenCvTracker is a realtime object tracking addon for openframeworks.

Tracker sample is included opencv_contrib. I built opencv with contrib library and made it easier to use on Openframeworks. Here is a link to learn opencv tracking module. https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/


## Usage
Check the code out.

## Install
### 1. Clone ofxOpenCvDnnObjectDetection to your of/addon directory
    $ git clone https://github.com/TetsuakiBaba/ofxOpenCvDnnObjectDetection.git
### 2. Download models. please check ofxOpenCvDnnObjectDetection README>
### 3. Clone ofxOpenCvTracker to your of/addon directory
    $ git clone https://github.com/TetsuakiBaba/ofxOpenCvTracker.git
### 4. Download OpenCV.framework to ofxOpenCvTracker/libs directory. 
    $ curl -O https://tetsuakibaba.jp/tmp/opencv2.framework.arm64.zip
    $ unzip opencv2.framework.arm64.zip
   
Or you may build your own opencv2.framework from opencv source. ( https://tetsuakibaba.jp/ws/doku.php?id=opencv_dnn:opencv2.framework )

## Getting Started
### 1. Update sample_detect_then_track with the projectGenerator.
### 2. Run. Please make sure that you have to copy config and weights file for object detection.

## Tracking is not perfect
Please note that tracking will not work properly under these conditions.
<img src="sample_images/May-28-2021 15-20-39.gif" width=100%>
## Compatibility
- macOS(arm64)
- of version: 0.11.2

## Licence
- Source Code: [MIT](https://opensource.org/licenses/MIT)
- Video by cottonbro from Pexels (https://www.pexels.com/@cottonbro)

## Author
[TetsuakiBaba](https://github.com/TetsuakiBaba)

## Reference
1. opencv_contrib: https://github.com/opencv/opencv_contrib


