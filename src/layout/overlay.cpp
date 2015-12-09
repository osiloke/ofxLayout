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
float target_p = 10.0f;

void OverlayLayout::setup(){
    FluidLayout::setup();
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(),h = height.getValue().asInt();
     
    
}

void OverlayLayout::draw(){
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(), h = height.getValue().asInt();
    std::vector<std::string>::reverse_iterator it = displayable.rbegin();
    
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    if(visible != ""){ 
        for ( it = displayable.rbegin(); it != displayable.rend(); it++){
            if(*it != visible)
            {
                Section * section = members.at((*it));
                section->draw(section->X(), section->Y(), section->Width(), section->Height());
            } 
        } 
        
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
    /*
     t_h = derived height from y_percent
     cum_height = next y pos
     cum_width = next x pos
     cum_height = 0
     cum_height = h - (h - cum_height - (t_h))
     */ 
    int t_x = cum_width, t_y = cum_height, t_w = 0, t_h = 0;
    
    calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
    updateMaxPos(t_w, t_h);
    
    calculatePadding(t_x, t_y, t_w, t_h, padding);
    
    FluidLayout::add(section, t_x, t_y, t_w, t_h);
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
    
}

