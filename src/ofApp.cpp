/* 
 * This function detects hue value for any point on the picture.
 * It is used before runnning Virtual Audio Player program to detect
 * the hue value for the enviroment. After detecting, we can
 * set the hue value in the Virtual Audio Player program.
 *
 * Move the mouse to the place where you want to detect the hue
 * value, and click the mouse. The hue value will displayed on the
 * console. Also, a red dot will show up and tracking the item that
 * has that hue value on the picture.
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    
    // 1280 * 960 for web camera
    // 1280 * 720 for Macbook Pro camera
    w = 1280;
    h = 960;
    movie.setup(w, h);
    
    // sign to start color tracking, controled by pressing keyboard
    // 't' to start tracking, and 's' to stop tracking
    colorTrackingSign = false;
    
    movie.initGrabber(w, h, true);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::update(){
    movie.update();
    
    //copy webcam pixels to rgb image
    rgb.setFromPixels(movie.getPixels());
    
    //mirror horizontal
    rgb.mirror(true, true);
    
    if (movie.isFrameNew() && colorTrackingSign) {
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-2,findHue+2) ? 255 : 0;
        }
        std::cout << findHue << std::endl;
        filtered.flagImageChanged();
        contours.findContours(filtered, 50, w*h/2, 1, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    
    rgb.draw(0,0);
    
    if (colorTrackingSign) {
        // draw red dot
        ofSetColor(255, 0, 0);
        ofFill();
        //draw red circles for found blobs
        for (int i=0; i<contours.nBlobs; i++) {
            ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
        }
        
    }
}

void ofApp::keyPressed(int key){
    if (key == 't') {
        colorTrackingSign = true;
    }
    if (key == 's') {
        colorTrackingSign = false;
    }
}

/*
 * Click mouse to the place that you want to detect the hue value
 */
void ofApp::mousePressed(int x, int y, int button){
    //calculate local mouse x,y in image
    int mx = x % w;
    int my = y % h;
    
    //get hue value on mouse position
    findHue = hue.getPixels()[my*w+mx];
}
