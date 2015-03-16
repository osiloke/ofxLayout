#pragma once

#include "propertyEvent.h"
#include "intProperty.h"
#include "boolProperty.h"
#include <json/json.h> 
#include "ofxJSONElement.h" 


namespace Kabbou {
    class Section{
    protected:
        ofImage texture;
        void onPropertyChanged(PropertyEvent & args){};
    public:
        typedef shared_ptr<Section> Ptr;
        virtual Section* create(){};
        IntProperty width;
        IntProperty height;
        IntProperty x_pos;
        IntProperty y_pos;
        BoolProperty b_displayed;
        Section * parent;
        std::string key;
        Json::Value data;
        float h_p, w_p, r_h_p, r_w_p, p_p, r_p_p;
        
        Section(){};
        Section(std::string key)
        :key(key), x_pos(key + ".x"), y_pos(key + ".y"),
        width(key + ".width"), height(key + ".height"),
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
        data(data){
            
        }
        
        int X(){
            return this->x_pos.getValue().asInt();
        }
        
        int Y(){
            return this->y_pos.getValue().asInt();
        }
        
        int Width(){
            return this->width.getValue().asInt();
        }
        
        int Height(){
            return this->height.getValue().asInt();
        }
        
        void updateItem(int x, int y, int w, int h){
            this->x_pos.setValue(x);
            this->y_pos.setValue(y);
            this->width.setValue(w);
            this->height.setValue(h);
        }
        void setCalculatedValues(int w, int h, int x, int y, float w_p, float h_p, float p_p){
            this->w_p = w_p;
            this->h_p = h_p;
            this->p_p = p_p;
            this->r_w_p = w_p;
            this->r_h_p = h_p;
            this->r_p_p = p_p;
            updateItem(x, y, w, h);
        }
        
        virtual void setup(){} 
        virtual void setup(int x, int y, int w, int h){} 
        virtual void update(){} 
        virtual void draw(int x, int y){}
        virtual void draw(int x, int y, int w, int h){}
        virtual void draw(){
            draw(X(), Y(), Width(), Height());
        }        
        virtual ofPixelsRef getPixelsRef(){}
        virtual unsigned char* getPixels(){}
        virtual ofTexture &getTexture() {}
        struct Pixels{
            unsigned char* pixels;
            int width;
            int height;
            int channels;
            
        };
        
        virtual bool isFrameNew(){}
        
        virtual void onAttachedToParent(){
            setup();
        }
        
        virtual void addChild(Section* section){}
        
        virtual void removeChild(Section* section){}
        
        virtual void hideChild(Section* section){}
        
        virtual void showChild(Section* section){}
        
        virtual void focusChild(Section* section){}
        
        virtual void deFocusChild(Section* section){}
        
        void reset(){
            w_p = r_w_p;
            h_p = r_h_p;
            p_p = r_p_p;
        }
        
        void conceal(){
            if(this->parent)
                this->parent->hideChild(this);
        }
        void reveal(){
            if(this->parent)
                this->parent->showChild(this);
        }
        
        void focus(){
            if(this->parent)
                this->parent->focusChild(this);
        }
        
        void deFocus(){
            if(this->parent)
                this->parent->deFocusChild(this);
        }
        
        void setDataAttribute(std::string key, ofxJSONElement val){
            data[key] = val;
        }
        
        void setData(Json::Value data){
            this->data = data;
        }
        
        Json::Value getData(){ return data;}
        
        void printData(){
            ofLogNotice()<< this->data.toStyledString();
        }
        
        virtual void organize(){};
        
        virtual std::string getType(){
            return "Unknown Section";
        }
                
        virtual void handleMessage(ofxJSONElement message){};
        
        virtual void onShow(){};
        virtual void onHide(){};
        virtual void onFocus(){};
        virtual void onDeFocus(){};
    
    };
}
