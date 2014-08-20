//
//  section.h
//  signage
//
//  Created by Osiloke Emoekpere on 3/16/14.
//
//

#ifndef signage_section_h
#define signage_section_h 

#include "actionListener.h"
#include "actionEvent.h"
#include "propertyEvent.h"
#include "intProperty.h"
#include "boolProperty.h"

namespace Kabbou {
    class Section{
    protected:
        void onPropertyChanged(PropertyEvent & args){};
    public:
        
        IntProperty width;
        IntProperty height;
        IntProperty x_pos;
        IntProperty y_pos;
        BoolProperty b_displayed;
        Section * parent;
        std::string key;

        void processAction(ActionEvent &e){
            
        }
        virtual void hideChild(Section &section){
            
        }
        
        virtual void showChild(Section &section){
            
        }
        
        virtual void onShow(){};
        virtual void onHide(){};
        
        void conceal(){
            if(this->parent)
                this->parent->hideChild(*this);
        }
        void reveal(){
            if(this->parent)
                this->parent->showChild(*this);
        }
        
        Section(){};
        Section(std::string key)
        :key(key), x_pos(key + ".x"),
        y_pos(key + ".y"),
        width(key + ".width"),
        height(key + ".height"),
        b_displayed(key + ".displayed", false)
        {
            
        }
        
        virtual void draw() = 0;
        virtual void draw(int x, int y){
            draw();
        };
        virtual void draw(int x, int y, int w, int h){
            draw();
        };
        virtual void update(){
        }
        
        virtual std::string getType(){
            return "Unknown Section";
        }
        
        void setup(){};
    
    };
}


#endif
