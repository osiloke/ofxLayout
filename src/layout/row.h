//
//  row.h
//  signage
//
//  Created by Osiloke Emoekpere on 4/25/14.
//
//

#ifndef __signage__row__
#define __signage__row__
#include "layout.h"
#include <iostream>

namespace Kabbou{
    class RowLayout:public FluidLayout{
    public:
        RowLayout(): FluidLayout("row"){};
        
        void add(Kabbou::Section &section, float h_percent=1.0f){
            FluidLayout::add(section, 1.0f, h_percent);
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
        void hide(Section &section){
            section.width.setValue(0);
            FluidLayout::hide(section);
            organize();
        }
        void show(Section &section){
            FluidLayout::show(section);
            organize();
        }
        void hideChild(Section &section){
            hide(section);
        }
        
        void showChild(Section &section){
            show(section);
        }
    };
};
#endif /* defined(__signage__row__) */
