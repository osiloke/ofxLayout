#pragma once

#include "layout.h"
#include <iostream>
#include "sectionFactory.h"

namespace ofxLayout{
    class RowLayout:public FluidLayout{
        REGISTER_SECTION(RowLayout);
    public:
        typedef shared_ptr<RowLayout> Ptr;
        RowLayout(std::string key, Json::Value data): FluidLayout(key, data){};
        RowLayout(std::string key): FluidLayout(key){}; 
        RowLayout(): FluidLayout("row"){};
        
        void addChild(Section * section){
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
            float w_percent = 1.0f;
            float h_percent = props.get("h_percent", 1.0f).asFloat();
            float padding = props.get("padding", 0.0f).asFloat();
            int t_x = cum_width, t_y = cum_height, t_w = 0, t_h = 0;
            
            calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
            updateMaxPos(t_w, t_h);
            
            calculatePadding(t_x, t_y, t_w, t_h, padding);
            
            FluidLayout::add(section, t_x, t_y, t_w, t_h);
        } 
        
        void changeRatio(Section &section, float h_percent=0.0f){
            changeRatio(section.key, h_percent);
        }
        
        void changeRatio(std::string section, float h_percent=0.0f){
            FluidLayout::changeRatio(section, 1.0f, h_percent);
        }
        
        void updateMaxPos(int t_w, int t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            if (cum_height != h)
                cum_height = cum_height + t_h;
            cum_width = w;
        };
        
        virtual void clip(int &t_x, int &t_y, int &t_w, int &t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt();
            if (t_w >= (w - cum_width))
                t_x = 0;
            
        }
        void setup(){
            FluidLayout::setup();
        }
        
        void draw(){
            FluidLayout::draw();
        }
        
        void hideChild(Section * section){
            section->width.setValue(0);
            FluidLayout::hideChild(section);
            organize();
        }
        
        void showChild(Section * section){
            FluidLayout::showChild(section);
            organize();
        }
        
        std::string getType(){
            return "Row Layout";
        }
    };
}; 
