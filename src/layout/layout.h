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
#include "actionListener.h"
#include "propertyEvent.h"

namespace Kabbou{
    
    class FluidLayout: public ActionListener, public Section{
    public: 
                
        int w, h, x, y;
        FluidLayout(std::string key):ActionListener(key),Section(key){
        }
        
        FluidLayout(): ActionListener("layout"), Section("layout"){
        }
        
        void setKey(std::string key){
            this->key = key;
        }
        
        virtual void setup(){
            resetMaxPosition();
            it_displayable = displayable.begin();
            ActionListener::setup();
        }
        
        void update();
        
        virtual void draw(){            
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                
                FluidLayoutMember section = members.at((*it));
                section.draw();
            }

        }
        
        virtual void draw(int x, int y){
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                
                FluidLayoutMember section = members.at((*it));
                section.draw(x, y);
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
            ofLogNotice()<<"Setup "<<key<<" Layout - "<<width.getValue()<<" by "<<height.getValue(); 
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
        
        void addLayout(FluidLayout &layout, float w_percent=1.0f, float h_percent=1.0f, float padding=0.0f){
            layout.setup();
            add(layout, w_percent, h_percent, padding);
            layout.resetMaxPosition();
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
        
        virtual void add(Section &section, float w_percent=1.0f, float h_percent=1.0f, float padding=0.0f){
            /*
             t_h = derived height from y_percent
             cum_height = next y pos
             cum_width = next x pos
             cum_height = 0
             cum_height = h - (h - cum_height - (t_h))
             */
            int t_x = cum_width, t_y = cum_height, t_w, t_h;
            
            calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
            updateMaxPos(t_w, t_h);
            
            calculatePadding(t_x, t_y, t_w, t_h, padding);
            members.insert(std::pair<std::string, FluidLayoutMember>(section.key, *new FluidLayoutMember(&section, t_w, t_h, t_x, t_y, w_percent, h_percent, padding)));
            
            it_displayable = displayable.insert(it_displayable++, section.key);
            member_index.insert(std::pair<std::string, std::vector<std::string>::iterator>(section.key, it_displayable));
            section.parent = this;
            
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
                    ofLogNotice()<<"Showing "<<it->first;
                    displayable.insert(it->second, it->first);
                }
            }
        };
        
        void organize(){
            //Re calculate placement for each item
            resetMaxPosition();
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                int t_x = cum_width, t_y = cum_height, t_w, t_h;
                
                FluidLayoutMember section = members.at((*it));
                calcTargets(section.w_p, section.h_p, section.p_p, t_x, t_y, t_w, t_h);
                updateMaxPos(t_w, t_h);
                
                calculatePadding(t_x, t_y, t_w, t_h, section.p_p);
                
//                if (it+1 == displayable.rend()){
//                    //Maximize item since its the last item. Dont leave any space
//                    section.updateItem(t_x, t_y, t_w + ((w + x) - cum_width), t_h + ((h + y) - cum_height));
//                }else{
                    section.updateItem(t_x, t_y, t_w, t_h);
//                }
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
        
    protected:
        virtual void updateMaxPos(int t_w, int t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            cum_height = cum_height + t_h;
            if (cum_width != w)
                cum_width = cum_width + t_w;
        };

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
            
            virtual void update(){
                section->update();
            }
            
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
        virtual void hideChild(Section &section){
            hide(section);
        }
        
        virtual void showChild(Section &section){
            show(section);
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
