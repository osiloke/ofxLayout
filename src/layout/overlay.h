//
//  overlay.h
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/11/14.
//
//

#ifndef __OVERLAY_LAYOUT__
#define __OVERLAY_LAYOUT__

#include "layout.h"
#include <iostream>  
//#include "ofxAnimatableFloat.h"
#include "sectionFactory.h" 

namespace ofxLayout{
class OverlayLayout:public FluidLayout{
REGISTER_SECTION(OverlayLayout);
    std::string visible;
    std::string _next_visible;
    ofFbo visibleFbo;
    ofFbo notVisibleFbo; 
    std::vector<std::string> hidden;
//    ofxAnimatableFloat displayableAnimation;
    void finishedAnimating();
public:
    typedef shared_ptr<OverlayLayout> Ptr;
    OverlayLayout(std::string key, Json::Value data): FluidLayout(key, data), visible(""), _next_visible(""){};
    OverlayLayout(std::string key): FluidLayout(key), visible(""), _next_visible(""){};
    OverlayLayout(): FluidLayout("overlay"), visible(""), _next_visible(""){};
    void draw();
    void update();
    void setup();  
    void addChild(Section * section);
//    void show(Section &section);
    void hideChild(Section * section);
    void focusChild(Section * section);
    void deFocusChild(Section * section);
    void organize();
    std::string getType(){
        return "Overlay Layout";
    }
};
}

#endif /* defined(__OVERLAY_LAYOUT__) */
