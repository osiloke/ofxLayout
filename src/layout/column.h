#pragma once
#include "layout.h"
#include "sectionFactory.h"

namespace ofxLayout{
    class ColumnLayout:public FluidLayout{
    REGISTER_SECTION(ColumnLayout);
    public:
        typedef shared_ptr<ColumnLayout> Ptr;
        ColumnLayout(std::string key, Json::Value data): FluidLayout(key, data){};
        ColumnLayout(std::string key): FluidLayout(key){};
        ColumnLayout(): FluidLayout("column"){};
         
        void addChild(Section * section);
        
        void changeRatio(Section *section, float w_percent=0.0f);
        void changeRatio(std::string section, float w_percent=0.0f);
        void updateMaxPos(int t_w, int t_h);
        /**
         Calculates the actual position and dimensions for the object
         **/
        virtual void calcTargets(float w_percent, float h_percent, float padding, int &t_x, int &t_y, int &t_w, int &t_h);
        virtual void clip(int &t_x, int &t_y, int &t_w, int &t_h);
        virtual void calculatePadding(int &t_x, int &t_y, int &t_w, int &t_h, float p); 
        std::string getType(){
            return "Column Layout";
        }
    };
};
