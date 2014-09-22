#include "ofApp.h"
#define STRINGIFY(A) #A
// TODO  - Handle missing layouts
//--------------------------------------------------------------
void ofApp::setup(){
    //Property events
    ofAddListener(PropertyEvent::events, this, &ofApp::onPropertyChanged);
    parser.fromFile("example.json");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    SectionFactory::getSection("root")->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    SectionFactory::getSection("root")->draw(); }

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
void ofApp::onPropertyChanged(PropertyEvent & args){
    ofLogNotice("Application ")<<"onPropertyChanged - "+args.property->key<< " <=> "<<args.property->getLastValue()<< " to "<<args.property->getValue();
};
