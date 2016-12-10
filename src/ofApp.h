#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    
    ofVideoGrabber movie;
    ofxCvColorImage rgb,hsb;
    ofxCvGrayscaleImage hue,sat,bri,filtered;
    ofxCvContourFinder contours;
    
    int w,h;
    int findHue;
    
    bool colorTrackingSign;
};
