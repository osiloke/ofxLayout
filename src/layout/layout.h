#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "ofMain.h"
#include "section.h"
#include "propertyEvent.h"
#include "sectionFactory.h"

namespace ofxLayout{ 
    class FluidLayout: public Section{
    private:
        static const SectionCreatorImpl<FluidLayout> creator;
    public: 
        typedef shared_ptr<FluidLayout> Ptr;
        FluidLayout(std::string key):Section(key){
        }
        FluidLayout(std::string key, Json::Value data):Section(key, data){
        }
        
        FluidLayout():Section("layout"){
        }
        
        
        
        void conceal(){
            if(this->parent)
                this->parent->hideChild(this);
        }
        void reveal(){
            if(this->parent)
                this->parent->showChild(this);
        }
        
//        void focus(){
//            if(this->parent)
//                this->parent->focusChild(this);
//        }
//        
        void deFocus(){
            if(this->parent)
                this->parent->deFocusChild(this);
        } 
        
        virtual std::string getType(){
            return "Fluid Layout";
        }
    };
    
} 
