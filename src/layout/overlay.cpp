//
//  overlay.cpp
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/11/14.
//
//

#include "overlay.h"


using namespace ofxLayout;
REGISTERSECTIONIMPL(OverlayLayout);
FadeBlack::Ptr pass;
float target_p = 10.0f;

void OverlayLayout::setup(){
    FluidLayout::setup();
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(),h = height.getValue().asInt();
    
    post.init(w, h);
    pass = post.createPass<FadeBlack>();
    pass->setProgress(target_p);
    pass->setResolution(ofVec2f(w, h));
    pass->setEnabled(true);
    
}

void OverlayLayout::draw(){
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(), h = height.getValue().asInt();
    std::vector<std::string>::reverse_iterator it = displayable.rbegin();
    
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    if(visible != ""){
        post.begin();
        for ( it = displayable.rbegin(); it != displayable.rend(); it++){
            if(*it != visible)
            {
                Section * section = members.at((*it));
                section->draw(section->X(), section->Y(), section->Width(), section->Height());
            } 
        }
        post.end();
        
        Section * section = members.at((visible));
        section->draw();
        
    }else{
        for ( it = displayable.rbegin(); it != displayable.rend(); it++){ 
                Section * section = members.at((*it));
                section->draw(section->X(), section->Y(), section->Width(), section->Height());
            } 
    }
    
}

void OverlayLayout::update(){
    FluidLayout::update();
    float dt = 1.0f / ofGetFrameRate();
//    displayableAnimation.update(dt);
    
//    if (displayableAnimation.hasFinishedAnimating() ){
        if (_next_visible != ""){
            visible = _next_visible;
            _next_visible = "";
            Section * section = members.at(visible);
            FluidLayout::showChild(section);
        }
//    }else{
//        pass->setProgress(displayableAnimation.val()); 
//    }
}
void OverlayLayout::addChild(Section* section){
    /**
     Add a child section/layout
     **/
    Json::Value props = section->getData();
    this->add(section, props);
    section->onAttachedToParent();
}
void OverlayLayout::add(Section * section, ofxJSONElement props){
    std::string halign = props.get("halign", "left").asString();
    std::string valign = props.get("valign", "bottom").asString();
    
    float w_percent = atof(props.get("w_percent", "1.0").asString().c_str());
    float h_percent = atof(props.get("h_percent", "1.0").asString().c_str());
    float padding = atof(props.get("padding", "0.0").asString().c_str());
    std::string visible = props.get("visible", "no").asString();
    
    if (visible == "yes"){
        visible = section->key;
    }
    if(halign == "center"){
        padding = padding + 1.0f - w_percent;
    }
    
    FluidLayout::add(section, w_percent, h_percent, padding);
}

void OverlayLayout::add(Section * section, float w_percent, float h_percent, float padding){
    FluidLayout::add(section, w_percent, h_percent, padding);
//    visible = section.key;
}

void OverlayLayout::focusChild(Section * section){
    _next_visible = section->key;
    //Do fade animation
//    displayableAnimation.animateFromTo(0.0f, target_p);
    
}

void OverlayLayout::deFocusChild(Section * section){
    _next_visible = "";
    //Do fade animation
//    displayableAnimation.animateFromTo(target_p, 0.0f);
    
}


void OverlayLayout::hideChild(Section * section){
    FluidLayout::hideChild(section);
    visible = "";
    _next_visible = "";
//    displayableAnimation.animateFromTo(target_p, 0.0f);
}

void OverlayLayout::organize(){
    FluidLayout::organize();
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(),h = height.getValue().asInt();
    
    post.init(w, h); 
    
}

