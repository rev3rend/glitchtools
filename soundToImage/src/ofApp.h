#pragma once

#define LIBAIFF_NOCOMPAT 1 // do not use LibAiff 2 API compatibility
#include "libaiff.h"

#include "ofMain.h"
#include "ofxAudioDecoder.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	void dragEvent(ofDragInfo dragInfo);
	
	void loadFile(string filename);
	
	ofxAudioDecoder audio;
	ofMesh left, right;
    //ofImage img;
    ofShortImage img;
    string themess;
    int cuttype;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void gotMessage(ofMessage msg);
		
};
