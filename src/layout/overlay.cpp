//
//  overlay.cpp
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/11/14.
//
//

#include "overlay.h"


using namespace Kabbou;

FadeBlack::Ptr pass;
float target_p = 0.65f;

void OverlayLayout::setup(){
    FluidLayout::setup();
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(),h = height.getValue().asInt();
    
    post.init(w, h);
    pass = post.createPass<FadeBlack>();
    pass->setProgress(target_p);
    pass->setEnabled(true);
    
}

void OverlayLayout::draw(){
    int x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt(),
    w = width.getValue().asInt(), h = height.getValue().asInt();
    std::vector<std::string>::reverse_iterator it = displayable.rbegin();
    
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    post.begin();
    for ( it = displayable.rbegin(); it != displayable.rend(); it++){
        if(*it != visible)
        {
            FluidLayoutMember section = members.at((*it));
            section.draw(section.x, section.y, section.w, section.h);
        }
    }
    post.end();
    
    //Draw only visible item
    if (visible != ""){
        FluidLayoutMember section = members.at((visible));
        section.draw();
    }
    
}

void OverlayLayout::update(){
    FluidLayout::update();
    float dt = 1.0f / ofGetFrameRate();
    displayableAnimation.update(dt);
    
    if (displayableAnimation.hasFinishedAnimating() ){
        if (_next_visible != ""){
            visible = _next_visible;
            _next_visible = "";
            FluidLayoutMember section = members.at(visible);
            FluidLayout::show(section);
        }
    }else{
        pass->setProgress(displayableAnimation.val()); 
    }
}

void OverlayLayout::add(Section &section, float w_percent, float h_percent, float padding){
    FluidLayout::add(section, w_percent, h_percent, padding); 
//    visible = section.key;
}

void OverlayLayout::show(Section &section){
    _next_visible = section.key;
    //Do fade animation
    displayableAnimation.animateFromTo(0.0f, target_p);
    
}


void OverlayLayout::hide(Section &section){
    FluidLayout::hide(section);
    visible = "";
    _next_visible = "";
    displayableAnimation.animateFromTo(target_p, 0.0f);
}

