// testing commit!!!!
// testing another commit!!!!!!

#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	loadFile("test.aiff");
}

void ofApp::update() {
	
}

void ofApp::draw() {
	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofScale((float) ofGetWidth() / audio.getNumFrames(), ofGetHeight() / 2);
	ofTranslate(0, 1);
	ofSetColor(ofColor::red);
	left.drawWireframe();
	ofSetColor(ofColor::green);
	right.drawWireframe();
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
	loadFile(dragInfo.files[0]);
}

void ofApp::loadFile(string filename) {
	audio.load(filename);
	
	left.clear();
	right.clear();
	
	left.setMode(OF_PRIMITIVE_LINE_STRIP);
	right.setMode(OF_PRIMITIVE_LINE_STRIP);
	
	const vector<float>& rawSamples = audio.getRawSamples();
	int channels = audio.getChannels();
	int n = rawSamples.size();
    cout << "samplecount: " << n << endl;
    
    // draw waveform
    for(int i = 0; i < n; i+= channels) {
                left.addVertex(ofVec2f(i/channels, rawSamples[i]));
                right.addVertex(ofVec2f(i/channels, rawSamples[i+1]));
                
	}
    
    
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='s') {
        // draw picture
        cout << "sound to picture... saving" << endl;
        const vector<float>& rawSamples = audio.getRawSamples();
        int channels = audio.getChannels();
        int n = rawSamples.size();
        int s = int(sqrt(n/channels));
        cout << "img size: " << s << " by " << s << endl;
        img.allocate(s, s, OF_IMAGE_COLOR);
        int iptr = 0; // which audio sample are we on?
        for(int i = 0;i<s;i++)
        {
            for(int j = 0;j<s;j++)
            {
                float r = ofMap(rawSamples[iptr], -1., 1., 0., 255.); // left side
                float g = ofMap(rawSamples[iptr+1], -1., 1., 0., 255.); // right side
                img.setColor(i, j, ofColor(r, g, 0));
                
                iptr+=channels;
            }
        }
        img.reloadTexture();
        img.saveImage("test.png");

    }
    if(key=='l') {
        // generate sound from picture
        cout << "picture to sound" << endl;
        ofImage foo;
        foo.loadImage("test.png");
        cout << "size: " << foo.getWidth() << " by " << foo.getHeight() << endl;
        float s = foo.getWidth();
        float t = foo.getHeight();
        vector<float> theSamps;

        for(int i = 0;i<s;i++)
        {
            for(int j = 0;j<t;j++)
            {
                ofColor c = foo.getColor(i, j);
                float r =ofMap(c.r, 0., 255., -1., 1.);
                float g =ofMap(c.g, 0., 255., -1., 1.);
                cout << "sample: " << r << " " << g << endl;
                theSamps.push_back(r);
                theSamps.push_back(g);

            }
        }
        // insert file writing code to dump theSamps to disk here:
        
        

    
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}


