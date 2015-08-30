
#include "column.h"

REGISTERSECTIONIMPL(ColumnLayout);

void ColumnLayout::addChild(Section *section){ 
    /**
     Add a child section/layout
     **/
    Json::Value props = section->getData();
    /*
     t_h = derived height from y_percent
     cum_height = next y pos
     cum_width = next x pos
     cum_height = 0
     cum_height = h - (h - cum_height - (t_h))
     */
    float w_percent = props.get("w_percent", 1.0f).asFloat();
    float h_percent = 1.0f;
    float padding = props.get("padding", 0.0f).asFloat();
    int width = props.get("width", 1).asInt(), height = props.get("height", 1).asInt();
    int t_x = cum_width, t_y = cum_height, t_w = 0, t_h = 0;
    
    calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
    int t_h_scaled = t_w * height/width;
    if (t_h_scaled != t_h){
        t_h = t_h_scaled;
    }
    updateMaxPos(t_w, t_h);
    
    calculatePadding(t_x, t_y, t_w, t_h, padding);
    
    FluidLayout::add(section, t_x, t_y, t_w, t_h); 
}

void ColumnLayout::changeRatio(Section *section, float w_percent){
    changeRatio(section->key, w_percent);
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
