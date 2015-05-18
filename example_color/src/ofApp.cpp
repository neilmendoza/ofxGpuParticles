#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
    {
    ofBackground(0);
    ofSetFrameRate(60);

    unsigned w = 1000;
    unsigned h = 1000;

    float* particlePosns = new float[w * h * 4];
    ofColor* colors = new ofColor[w * h];

    ofImage image;
    image.loadImage("colors.png");

    for (unsigned y = 0; y < image.getHeight(); ++y)
    {
        for (unsigned x = 0; x < image.getWidth(); ++x)
        {
            ofColor cur = image.getColor(x, y);
            if (cur.a > 0){
                cur.a = 255;
                unsigned idx = y * w + x;

                particlePosns[idx * 4] = 400.f * x / (float)w - 200.f;
                particlePosns[idx * 4 + 1] = 400.f * y / (float)h - 200.f;
                particlePosns[idx * 4 + 2] = 0.f;
                particlePosns[idx * 4 + 3] = 0.f;

                colors[idx] =  cur;
            }
        }
    }
    particles.init(w, h, colors);
    particles.loadDataTexture(ofxGpuParticles::POSITION, particlePosns);
    delete[] particlePosns;
    delete[] colors;

    // initial velocities
    particles.zeroDataTexture(ofxGpuParticles::VELOCITY);

    // listen for update event to set additonal update uniforms
    ofAddListener(particles.updateEvent, this, &ofApp::onParticlesUpdate);
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
    particles.update();
}

// set any update uniforms in this function
void ofApp::onParticlesUpdate(ofShader& shader)
{
    ofVec3f mouse(ofGetMouseX() - .5f * ofGetWidth(), .5f * ofGetHeight() - ofGetMouseY() , 0.f);
    shader.setUniform3fv("mouse", mouse.getPtr());
    shader.setUniform1f("elapsed", ofGetLastFrameTime());
    shader.setUniform1f("radiusSquared", 200.f * 200.f);
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();
    particles.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}