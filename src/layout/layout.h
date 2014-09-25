//
//  layout.h
//  signage
//
//  Created by Osiloke Emoekpere on 3/20/14.
//
//

#ifndef __signage__layout__
#define __signage__layout__

#include <iostream>
#include <map>
#include <vector>
#include "ofMain.h"
#include "section.h"
#include "propertyEvent.h"
#include "sectionFactory.h"

namespace Kabbou{
    
    class FluidLayoutMember{
    public:
        Section * section;
        int w, h, x, y;
        float h_p, w_p, r_h_p, r_w_p, p_p, r_p_p;
        
        FluidLayoutMember(Section * section, int w, int h, int x, int y, float w_p, float h_p, float p_p)
        :section(section), w_p(w_p), h_p(h_p), p_p(p_p), r_w_p(w_p), r_h_p(h_p), r_p_p(p_p){
            updateItem(x, y, w, h);
        }
        
        virtual void draw(){
            section->draw();
        }
        
        
        virtual void draw(int x, int y){
            section->draw(x, y);
        }
        
        virtual void draw(int x, int y, int w, int h){
            section->draw(x, y, w, h);
        }
        
        void update(){
            section->update();
        };
        
        void reset(){
            w_p = r_w_p;
            h_p = r_h_p;
            p_p = r_p_p;
        };
        
        void updateItem(int x, int y, int w, int h){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            section->width.setValue(w);
            section->height.setValue(h);
            section->x_pos.setValue(x);
            section->y_pos.setValue(y);
        }
        
        void show(){
            section->onShow();
        }
    };
    class FluidLayout: public Section{
    private:
        static const SectionCreatorImpl<FluidLayout> creator;
    public:
        
        int w, h, x, y;
        typedef shared_ptr<FluidLayout> Ptr;
        FluidLayout(std::string key):Section(key){
        }
        FluidLayout(std::string key, Json::Value data):Section(key, data){
        }
        
        FluidLayout():Section("layout"){
        }
        
        void setKey(std::string key){
            this->key = key;
        }
        
        virtual void setup(){
            resetMaxPosition();
            it_displayable = displayable.begin();
        }
        
        virtual void update(){
            std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();
            for ( it = displayable.rbegin(); it != this->displayable.rend(); it++){
                FluidLayoutMember section = this->members.at((*it));
                section.update();
            };
        };
        
        virtual void draw(){
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                FluidLayoutMember section = members.at((*it));
                section.draw();
//                std::map<std::string, FluidLayoutMember>::iterator found;
//                found = members.find((*it));
//                if (found != members.end()){
//                    FluidLayoutMember section = found->second;
//                    
//                    section->draw();
//                }
            }

        }
        
        virtual void draw(int x, int y){
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){                
                std::map<std::string, FluidLayoutMember>::iterator found;
                found = members.find((*it));
                if (found != members.end()){
                    FluidLayoutMember section = found->second;
                    section.draw();
                }
            }
            
        }
        
        virtual void resetMaxPosition(){
            cum_width = x_pos.getValue().asInt();
            cum_height = y_pos.getValue().asInt();
        }
        
        void setup(int x, int y, int w, int h){
            this->x_pos.setValue(x);
            this->y_pos.setValue(y);
            this->width.setValue(w);
            this->height.setValue(h);
            setup();            
            ofLogNotice()<<"Setup "<<key<<width.getValue()<<" by "<<height.getValue(); 
        }
        
        /**
         Calculates the actual position and dimensions for the object
         **/
        virtual void calcTargets(float w_percent, float h_percent, float padding, int &t_x, int &t_y, int &t_w, int &t_h){
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
            float h_ = h_percent + padding;
            t_w = w_ * w;
            t_h = h_ * h;
            
            clip(t_x, t_y, t_w, t_h);

        };
        
        virtual void clip(int &t_x, int &t_y, int &t_w, int &t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt();
            if (t_h > (h - cum_height))
                t_y = 0;
            
            if (t_w > (w - cum_width))
                t_x = 0;
        }
        
        virtual void addChild(Section* section){
            /**
             Add a child section/layout
             **/
            Json::Value props = section->getData();
            add(*section, props.get("w_percent", 1.0f).asFloat(),
                props.get("h_percent", 1.0f).asFloat(),
                props.get("padding", 0.0f).asFloat()); 
            section->onAttachedToParent();
        }
        void onAttachedToParent(){
            /**
             Gets called immediately after a layout has been attachd to a parent
             **/
            resetMaxPosition();
            setup();
            organize();
        }
        void addLayout(FluidLayout &layout, float w_percent=1.0f, float h_percent=1.0f, float padding=0.0f){
            add(layout, w_percent, h_percent, padding);
            layout.resetMaxPosition();
            layout.setup();
            layout.organize();
        }
        
        virtual void calculatePadding(int &t_x, int &t_y, int &t_w, int &t_h, float p){
            int w_p = p * t_w; //What to take off the left and right site
            int h_p = p * t_h; //What to take off the top and bottom sides
            
            t_w = t_w - (w_p * 2); //Shave off padding
            t_h = t_h - (h_p * 2); //Shave off padding
            
            //Reposition to center
            t_x = t_x + w_p;
            t_y = t_y + h_p;
        }
        virtual void add(Section &section, ofxJSONElement props){
            
        }
        
        virtual void add(Section &section, float w_percent=1.0f, float h_percent=1.0f, float padding=0.0f){
            /*
             t_h = derived height from y_percent
             cum_height = next y pos
             cum_width = next x pos
             cum_height = 0
             cum_height = h - (h - cum_height - (t_h))
             */
            int t_x = cum_width, t_y = cum_height, t_w = 0, t_h = 0;
            
            calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
            updateMaxPos(t_w, t_h);
            
            calculatePadding(t_x, t_y, t_w, t_h, padding);
            
            FluidLayoutMember n_section = *new FluidLayoutMember(&section, t_w, t_h, t_x, t_y, w_percent, h_percent, padding);
            members.insert(std::pair<std::string, FluidLayoutMember>(section.key, n_section));
            
            it_displayable = this->displayable.insert(it_displayable++, section.key);
            this->member_index.insert(std::pair<std::string, std::vector<std::string>::iterator>(section.key, it_displayable));
            section.parent = this;
            
            ofLogVerbose(this->getType())<<"[Section] "<<section.key<<" added";
            ofLogVerbose(this->getType())<<this->displayable.size()<<" sections are displayable";
        };
        void hide(Section &section){
            std::vector<std::string>::iterator position = std::find(displayable.begin(), displayable.end(), section.key);
            if (position != displayable.end()) // == vector.end() means the element was not found
                displayable.erase(std::remove(displayable.begin(), displayable.end(), section.key));
        };
        void show(Section &section){
            //Insert this section in its rightful place in displayable array
            //This all seems unneccessarily complicated
            std::map<std::string, std::vector<std::string>::iterator>::iterator it = member_index.find( section.key ); //find its index
            if( it != member_index.end() )
            {
                std::vector<std::string>::iterator position = std::find(displayable.begin(), displayable.end(), section.key);
                if (position == displayable.end()) // == vector.end() means the element was not found
                {
                    //Show the section
                    ofLogVerbose(this->getType())<<"Showing "<<it->first;
                    displayable.insert(it->second, it->first);
                    
                    section.onShow();
                }
            }
        };
        
        virtual void focus(Section &section){
            
        }
        
        void show(FluidLayoutMember &member){
            //Insert this section in its rightful place in displayable array
            //This all seems unneccessarily complicated
            
            ofLogNotice(this->getType())<<"Before showing "<<member.section->key<<", there are "<<displayable.size()<<" sections";
            std::map<std::string, std::vector<std::string>::iterator>::iterator it = member_index.find( member.section->key ); //find its index
            if( it != member_index.end() )
            {
                std::vector<std::string>::iterator position = std::find(displayable.begin(), displayable.end(), member.section->key);
                if (position == displayable.end()) // == vector.end() means the element was not found
                {
                    //Show the section
                    ofLogVerbose(this->getType())<<"Showing "<<it->first;
                    displayable.insert(it->second, it->first);
                    
                    member.show();
                }
            }
            ofLogNotice(this->getType())<<"After showing "<<member.section->key<<", there are "<<displayable.size()<<" sections";
        };
        
        virtual void organize(){
            //Re calculate placement for each item
            resetMaxPosition();
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            ofLogNotice(this->getType())<<displayable.size()<<" sections are displayable and can be organized";
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                int t_x = cum_width, t_y = cum_height, t_w, t_h;
                
                FluidLayoutMember section = members.at((*it));
                calcTargets(section.w_p, section.h_p, section.p_p, t_x, t_y, t_w, t_h);
                updateMaxPos(t_w, t_h);
                
                calculatePadding(t_x, t_y, t_w, t_h, section.p_p);
                
//                if (it+1 == displayable.rend()){
//                    //Maximize item since its the last item. Dont leave any space
//                    section->updateItem(t_x, t_y, t_w + ((w + x) - cum_width), t_h + ((h + y) - cum_height));
//                }else{
                    section.updateItem(t_x, t_y, t_w, t_h);
//                }
                
                section.section->organize();
            }
        }
        
        void changeRatio(Section &section, float w_percent=0.0f, float h_percent=0.0f){
            changeRatio(section.key, w_percent, h_percent);
        }
        
        void changeRatio(std::string section, float w_percent=0.0f, float h_percent=0.0f){
            std::map<std::string, FluidLayoutMember>::iterator it = members.find( section );
            if( it != members.end() )
            { 
                (*it).second.w_p = w_percent;
                (*it).second.h_p = h_percent;
            }
        }
        
        void resetSection(std::string section){
            std::map<std::string, FluidLayoutMember>::iterator it = members.find( section );
            if( it != members.end() )
            {
                (*it).second.reset();
            }
        }
        
        virtual std::string getType(){
            return "Fluid Layout";
        }
        
    protected:
        virtual void updateMaxPos(int t_w, int t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            cum_height = cum_height + t_h;
            if (cum_width != w)
                cum_width = cum_width + t_w;
        };

        virtual void hideChild(Section &section){
            hide(section);
        }
        
        virtual void showChild(Section &section){
            show(section);
        }
        
        virtual void focusChild(Section &section){
            focus(section);
        }
        
        int  cum_width, cum_height;
        std::map<std::string, FluidLayoutMember> members;
        std::vector<std::string> displayable; //stores displayable sections, it is ordered by the position of sections in the members map;
        std::vector<std::string>::iterator it_displayable;
        std::map<std::string, std::vector<std::string>::iterator> member_index;
        void processAction(ActionEvent &e);
   };
}
#endif /* defined(__signage__layout__) */
