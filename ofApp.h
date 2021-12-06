/************************************************************
■Media Art II 2013 第6回：openFrameworks Addonを使う 2 - ofxOpenCV と ofxCv
	https://www.slideshare.net/tado/media-art-ii-2013-6openframeworks-addon-2-ofxopencv-ofxcv
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxOpenCv.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		WINDOW_WIDTH = 700,
		WINDOW_HEIGHT = 800,
	};
	enum{
		CAM_WIDTH	= 320,
		CAM_HEIGHT	= 240,
	};
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	int FrameRate = 30;
	int now = 0;
	ofTrueTypeFont font;

	/********************
	********************/
	ofVideoGrabber cam;
	int Cam_id;
	
	/********************
	********************/
	ofxCvColorImage			colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	int 				threshold;
	bool				bLearnBakground;
	bool				b_FindHoles = false;
	int					nConsidered = 10;
	
	
	/****************************************
	****************************************/
	void setup_Camera();
	
public:
	ofApp(int _Cam_id);
	~ofApp();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
