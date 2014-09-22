//
//  column.h
//  signage
//
//  A column layout places objects horizontally i.e Members always have max height
//  Created by Osiloke Emoekpere on 4/17/14.
//
//

#ifndef __signage__column__
#define __signage__column__
#include "layout.h"
#include <iostream>
#include "sectionFactory.h"

namespace Kabbou{
    class ColumnLayout:public FluidLayout{
        REGISTER_SECTION(ColumnLayout);
    public:
        typedef shared_ptr<ColumnLayout> Ptr;
        ColumnLayout(std::string key, Json::Value data): FluidLayout(key, data){};
        ColumnLayout(std::string key): FluidLayout(key){};
        ColumnLayout(): FluidLayout("column"){};
        
        void add(Kabbou::Section &section, float w_percent=1.0f, float padding=0.0f);
        
        void changeRatio(Section &section, float w_percent=0.0f);
        void changeRatio(std::string section, float w_percent=0.0f);
        void updateMaxPos(int t_w, int t_h);
        /**
         Calculates the actual position and dimensions for the object
         **/
        virtual void calcTargets(float w_percent, float h_percent, float padding, int &t_x, int &t_y, int &t_w, int &t_h);
        virtual void clip(int &t_x, int &t_y, int &t_w, int &t_h);
        virtual void calculatePadding(int &t_x, int &t_y, int &t_w, int &t_h, float p);
        void setup();
        void hide(Section &section);
        void show(Section &section);
        void hideChild(Section &section);
        void showChild(Section &section);
        std::string getType(){
            return "Column Layout";
        }
    };
};
#endif /* defined(__signage__column__) */
