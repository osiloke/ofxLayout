//
//  column.cpp
//  signage
//
//  Created by Osiloke Emoekpere on 4/17/14.
//
//

#include "column.h"

REGISTERSECTIONIMPL(ColumnLayout);

void ColumnLayout::add(Kabbou::Section &section, float w_percent, float padding){
    FluidLayout::add(section, w_percent, 1.0f, padding);
}

void ColumnLayout::changeRatio(Section &section, float w_percent){
    changeRatio(section.key, w_percent);
}
void ColumnLayout::changeRatio(std::string section, float w_percent){
    FluidLayout::changeRatio(section, w_percent, 1.0f);
}
void ColumnLayout::updateMaxPos(int t_w, int t_h){
    int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
    cum_height = h;
    if (cum_width != w)
        cum_width = cum_width + t_w;
};
/**
 Calculates the actual position and dimensions for the object
 **/
void ColumnLayout::calcTargets(float w_percent, float h_percent, float padding, int &t_x, int &t_y, int &t_w, int &t_h){
    /*
     t_h = derived height from y_percent
     cum_height = next y pos
     cum_width = next x pos
     cum_height = 0
     cum_height = h - (h - cum_height - (t_h))
     */
    int h = height.getValue().asInt(), w = width.getValue().asInt();
    //Pad on all sides
    float w_ = w_percent + padding;
    float h_ = h_percent;// + padding;
    t_w = w_ * w;
    t_h = h_ * h;
    
    clip(t_x, t_y, t_w, t_h);
    
};
void ColumnLayout::clip(int &t_x, int &t_y, int &t_w, int &t_h){
    int h = height.getValue().asInt(), w = width.getValue().asInt();
//    if (t_h >= (h + cum_height))
//        t_y = 0;
    t_y = y_pos.getValue().asInt(); 
}
void ColumnLayout::calculatePadding(int &t_x, int &t_y, int &t_w, int &t_h, float p){
    int w_p = p * t_w; //What to take off the left and right site
    int h_p = p * t_h; //What to take off the top and bottom sides
    
    t_w = t_w - (w_p * 2); //Shave off padding
    t_h = t_h - (h_p * 2); //Shave off padding
    
    //Reposition to center
    t_x = t_x + w_p;
    t_y = t_y + h_p;
}
void ColumnLayout::setup(){
    FluidLayout::setup();
}
void ColumnLayout::hide(Section &section){
//    section.width.setValue(0);
    FluidLayout::hide(section);
//    organize();
}
void ColumnLayout::show(Section &section){
    FluidLayoutMember _section = members.at(section.key);
    FluidLayout::show(_section);
//    organize();
}
void ColumnLayout::hideChild(Section &section){
    hide(section);
}

void ColumnLayout::showChild(Section &section){
    show(section);
}