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
#include <json/json.h>
//#include "sectionFactory.h"


namespace Kabbou {
    class Section{
    protected:
        void onPropertyChanged(PropertyEvent & args){};
    public:
        typedef shared_ptr<Section> Ptr;
        virtual Section::Ptr create(){};
        IntProperty width;
        IntProperty height;
        IntProperty x_pos;
        IntProperty y_pos;
        BoolProperty b_displayed;
        Section * parent;
        std::string key;
        Json::Value data;

        void processAction(ActionEvent &e){
            
        }
        virtual void addChild(Section::Ptr section){
            
        }
        virtual void onAttachedToParent(){
            
        }
        virtual void removeChild(Section::Ptr section){
            
        }
        virtual void hideChild(Section &section){
            
        }
        
        virtual void showChild(Section &section){
            
        }
        
        virtual void focusChild(Section &section){
            
        }
        
        virtual void deFocusChild(Section &section){
            
        }
        
        virtual void onShow(){};
        virtual void onHide(){};
        virtual void onFocus(){};
        virtual void onDeFocus(){};
        
        void conceal(){
            if(this->parent)
                this->parent->hideChild(*this);
        }
        void reveal(){
            if(this->parent)
                this->parent->showChild(*this);
        }
        
        void focus(){
            if(this->parent)
                this->parent->focusChild(*this);
        }
        
        void deFocus(){
            if(this->parent)
                this->parent->deFocusChild(*this);
        }
        
        void setDataAttribute(std::string key, ofxJSONElement val){
            data[key] = val;
        }
        
        void setData(Json::Value data){
            this->data = data;
        }
        
        void printData(){
            ofLogNotice()<< this->data.toStyledString();
        }
        
        Section(){};
        Section(std::string key)
        :key(key), x_pos(key + ".x"),
        y_pos(key + ".y"),
        width(key + ".width"),
        height(key + ".height"),
        b_displayed(key + ".displayed", false)
        {
            data["name"] = key;
        };
        
        Section(std::string key, Json::Value data):
        key(key), x_pos(key + ".x"),
        y_pos(key + ".y"),
        width(key + ".width"),
        height(key + ".height"),
        b_displayed(key + ".displayed", false),
        data(data){}
        virtual void setKey(std::string key){
            this->key = key;
            this->x_pos.key = key + ".x";
            this->y_pos.key = key + ".y";
            this->width.key = key + ".width";
            this->height.key = key + ".height";
            this->b_displayed = key + ".displayed";
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
        
        virtual void organize(){};
        
        virtual std::string getType(){
            return "Unknown Section";
        }
        
        void setup(){};
    
    };
}


#endif
