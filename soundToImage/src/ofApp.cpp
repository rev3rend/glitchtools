// testing commit!!!!
// testing another commit!!!!!!

#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
    themess = "hi there";
	loadFile("test.aiff");
    cuttype = 0;
}

void ofApp::update() {
	
}

void ofApp::draw() {
    
    ofBackground(0);
    ofSetColor(ofColor::white);
    ofDrawBitmapString(themess, 50, 50);
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
    
    if(key=='c') {
        cuttype = 1-cuttype;
        if(cuttype==0) themess = "cuttype is square";
        if(cuttype==1) themess = "cuttype is beats";
    }
    if(key=='s') {
        // draw picture
        themess = "sound to picture... saving";
        cout << "sound to picture... saving" << endl;
        const vector<float>& rawSamples = audio.getRawSamples();
        int channels = audio.getChannels();
        int n = rawSamples.size();
        
        if(cuttype==0) { // square picture
            int s = int(sqrt(n/channels));
            cout << "img size: " << s << " by " << s << endl;
            img.allocate(s, s, OF_IMAGE_COLOR);
            int iptr = 0; // which audio sample are we on?
            for(int i = 0;i<s;i++)
            {
                for(int j = 0;j<s;j++)
                {
                    float r = ofMap(rawSamples[iptr], -1., 1., 0., 65535.); // left side
                    float g = ofMap(rawSamples[iptr+1], -1., 1., 0., 65535.); // right side
                    cout << r << " " << g << endl;
                    img.setColor(j, i, ofShortColor(r, g, 0));
                    
                    iptr+=channels;
                }
            }
        }

        else if(cuttype==1) { // not square picture
            int nrows = 64;
            int subdivs = 4;
            int s = int((n/channels)/nrows);
            cout << "img size: " << s << " by " << nrows << endl;
            img.allocate(s, nrows, OF_IMAGE_COLOR);
            int iptr = 0; // which audio sample are we on?
            for(int i = 0;i<nrows;i++)
            {
                for(int j = 0;j<s;j++)
                {
                    float r = ofMap(rawSamples[iptr], -1., 1., 0., 65535.); // left side
                    float g = ofMap(rawSamples[iptr+1], -1., 1., 0., 65535.); // right side
                    float b = (j%(s/subdivs)==0)*65535.;
                    cout << r << " " << g << endl;
                    img.setColor(j, i, ofShortColor(r, g, b));
                    
                    iptr+=channels;
                }
            }
        }

        
        
        img.reloadTexture();
        img.saveImage("test.png");

    }
    if(key=='l') {
        themess = "picture to sound";
        // generate sound from picture
        cout << "picture to sound" << endl;
        ofShortImage foo;
        foo.loadImage("test.png");
        vector<int32_t> theSamps;
        cout << "size: " << foo.getWidth() << " by " << foo.getHeight() << endl;
        float s = foo.getWidth();
        float t = foo.getHeight();

        for(int i = 0;i<t;i++)
        {
            for(int j = 0;j<s;j++)
            {
                ofShortColor c = foo.getColor(j, i);
                float rs = ofMap(c.r, 0., 65535., -1., 1.);
                float gs = ofMap(c.g, 0., 65535., -1., 1.);
                int32_t r =int(rs*536870912.); // 2^29 (wtf?)
                int32_t g =int(gs*536870912.); // 2^29 (wtf?)
                cout << "sample: " << c.r << " " << c.g << endl;
                theSamps.push_back(r);
                theSamps.push_back(g);

            }
        }
 
        
        // try dumping the original sound
/*        const vector<float>& rawSamples = audio.getRawSamples();
        int channels = audio.getChannels();
        int n = rawSamples.size();
        for(int i = 0;i<n;i++)
        {
            int32_t s =int(rawSamples[i]*500000000.);
            cout << rawSamples[i] << " " <<s << endl;
            theSamps.push_back(s);
        }*/

        
        // insert file writing code to dump theSamps to disk here:
        
        AIFF_Ref ref ;
        
        string p = ofToDataPath("foo.aiff", true);
        cout << p << endl;
        
        ref = AIFF_OpenFile(p.c_str(), F_WRONLY) ;
        if( ref )
        {
            puts("File opened successfully.");
            AIFF_SetAudioFormat(ref, 2, 44100., 16);
            AIFF_StartWritingSamples(ref);
            int32_t* v = theSamps.data();
            AIFF_WriteSamples32Bit(ref, v, theSamps.size());
            AIFF_EndWritingSamples(ref);
            AIFF_CloseFile(ref);
            puts("Closed file.");
        }
    
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


