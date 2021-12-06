/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _Cam_id)
: Cam_id(_Cam_id)
{
	font.load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
}

/******************************
******************************/
ofApp::~ofApp(){
	printf("Good-bye\n");
	fflush(stdout);
}


/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("ofxOpenCv");
	
	ofSetWindowShape( WINDOW_WIDTH, WINDOW_HEIGHT );
	ofSetVerticalSync(true);
	ofSetFrameRate(FrameRate);
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	setup_Camera();
	
	/********************
	********************/
	colorImg.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayImage.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayBg.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayDiff.allocate(CAM_WIDTH, CAM_HEIGHT);
	
	bLearnBakground = true;
	threshold = 80;
}

/******************************
******************************/
void ofApp::setup_Camera()
{
	/********************
	********************/
	printf("> setup camera\n");
	fflush(stdout);
	
	/********************
	細かく情報出してもらう
	********************/
	ofSetLogLevel(OF_LOG_VERBOSE);
	cam.setVerbose(true);
	
	vector< ofVideoDevice > Devices = cam.listDevices();// 上 2行がないと、List表示されない.
	
	/********************
	********************/
	if(Cam_id == -1){
		ofExit();
		return;
	}else{
		if(Devices.size() <= Cam_id) { ERROR_MSG(); ofExit(); return; }
		
		cam.setDeviceID(Cam_id);
		printf("\n> cam set device id = %d\n", Cam_id);
		printf("> DeviceID	 = %d\n", Devices[Cam_id].id);
		printf("> CamName	  = %s\n", Devices[Cam_id].deviceName.c_str());
		printf("> HardwareName = %s\n", Devices[Cam_id].hardwareName.c_str());
		printf("> serialID	 = %s\n", Devices[Cam_id].serialID.c_str());
		
		printf( "\n> Cam size asked  = (%d, %d)\n", int(CAM_WIDTH), int(CAM_HEIGHT) );
		/*
			https://openframeworks.cc/documentation/video/ofVideoGrabber/#show_setDesiredFrameRate
				Set's the desired frame rate of the grabber. This should be called before initGrabber(), which will try to initialize at the desired frame rate. 
				Not all frame rates will be supported, but this at least gives you some abilitity to try grab at different rates.
		*/
		cam.setDesiredFrameRate(30);
		cam.initGrabber(CAM_WIDTH, CAM_HEIGHT);
		printf( "> Cam size actual = (%d, %d)\n\n", int(cam.getWidth()), int(cam.getHeight()) );
		fflush(stdout);
	}
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	cam.update();
	if(cam.isFrameNew()){
		colorImg.setFromPixels(cam.getPixels());
		
		grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20 /* minArea */, (320*240)/3 /* maxArea */, nConsidered, b_FindHoles);
	}
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(100);
	
	ofSetColor(255, 255, 255);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
	
	// then draw the contours:
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(360,540,320,240);
	ofSetHexColor(0xffffff);
	
	// we could draw the whole contour finder
	//contourFinder.draw(360,540);
	
	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
		
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole", contourFinder.blobs[i].boundingRect.getCenter().x + 360, contourFinder.blobs[i].boundingRect.getCenter().y + 540);
		}
    }

	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << threshold << " (press: +/-)" << endl
			  << "nConsidered " << nConsidered << " (press: ↑/↓)" << endl
			  << "b_FindHoles = " << b_FindHoles << "(press h to toggle)" << endl
			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
			
		case 'h':
			b_FindHoles = !b_FindHoles;
			break;
			
		case '+':
			threshold ++;
			if (255 < threshold) threshold = 255;
			break;
			
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
			
		case OF_KEY_UP:
			nConsidered++;
			if(30 < nConsidered) nConsidered = 30;
			break;
			
		case OF_KEY_DOWN:
			nConsidered--;
			if(nConsidered < 1) nConsidered = 1;
			break;
	}

}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
