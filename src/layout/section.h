#pragma once

#include "propertyEvent.h"
#include "intProperty.h"
#include "boolProperty.h"
#include <json/json.h> 
#include "ofxJSONElement.h" 


namespace ofxLayout {
    class Section{
    protected:
        ofImage texture;
        void onPropertyChanged(PropertyEvent & args){};
        virtual void updateMaxPos(int t_w, int t_h){
            int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
            
            cum_height = cum_height + t_h;
            if (cum_width != w)
                cum_width = cum_width + t_w;
        };
        
        int  cum_width, cum_height;
        std::map<std::string, Section *> members;
        std::vector<std::string> displayable; //stores displayable sections, it is ordered by the position of sections in the members map;
        std::vector<std::string>::iterator it_displayable;
        std::map<std::string, std::vector<std::string>::iterator> member_index;
        void _setup_interaction(){
            _isMouseOver	= false;
            enabled		= true;
            verbose		= true;
            _stateChangeTimestampMillis = 0;

//            enableAppEvents();
            disableMouseEvents();
            disableKeyEvents();
        }

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
        

        Section(){
            _setup_interaction();
        };
        Section(std::string key)
        :key(key), x_pos(key + ".x"), y_pos(key + ".y"),
        width(key + ".width"), height(key + ".height"),
        b_displayed(key + ".displayed", false)
        {
            data["name"] = key;
            _setup_interaction();
        };
        
        Section(std::string key, ofxJSONElement data):
        key(key), x_pos(key + ".x"),
        y_pos(key + ".y"),
        width(key + ".width"),
        height(key + ".height"),
        b_displayed(key + ".displayed", false),
        data(data){
            _setup_interaction();
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
        
        void reset(){
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
        
        virtual std::string getType(){
            return "Unknown Section";
        }
                
        virtual void handleMessage(ofxJSONElement message){};
        
        virtual void onShow(){};
        virtual void onHide(){};
        virtual void onFocus(){};
        virtual void onDeFocus(){};
        
        
        void setup(int x, int y, int w, int h){
            this->x_pos.setValue(x);
            this->y_pos.setValue(y);
            this->width.setValue(w);
            this->height.setValue(h);
            setup();
        }
        
        virtual void setup(){
            resetMaxPosition();
            it_displayable = displayable.begin();
            ofLogNotice()<<"Setup "<<key<<width.getValue()<<" by "<<height.getValue();
        }
        
        virtual void update(){
            std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();
            for ( it = displayable.rbegin(); it != this->displayable.rend(); it++){
                Section * section = this->members.at((*it));
                section->update();
            };
        };
        /**
         Overloaded virtual function which draws all children sections attached to this layout.
         @return void
         */
        virtual void draw(){
            draw(X(), Y(), Width(), Height());
            this->draw(0, 0);
        }
        
        /**
         Overloaded virtual function which draws all children sections attached to this layout.
         This draws based on lilo, last in last out.
         TODO: Does lilo make sense
         @return void
         */
        virtual void draw(int x, int y){
            std::vector<std::string>::reverse_iterator it = displayable.rbegin();
            for ( it = displayable.rbegin(); it != displayable.rend(); it++){
                std::map<std::string, Section *>::iterator found;
                found = members.find((*it));
                if (found != members.end()){
                    Section * section = found->second;
                    section->draw();
                }
            }
        }
        virtual void draw(int x, int y, int w, int h){}

//        virtual void organize(){
//            //Re calculate placement for each item
//            this->resetMaxPosition();
//            int h = this->height.getValue().asInt(), w = this->width.getValue().asInt(), x = this->x_pos.getValue().asInt(), y = this->y_pos.getValue().asInt();

//            ofLogNotice(this->getType())<<this->displayable.size()<<" sections will be organized";
//            std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();

//            for ( it = this->displayable.rbegin(); it != this->displayable.rend(); it++){
//                int t_x = this->cum_width, t_y = this->cum_height, t_w, t_h;

//                Section * section = this->members.at((*it));
//                this->calcTargets(section->w_p, section->h_p, section->p_p, t_x, t_y, t_w, t_h);
//                this->updateMaxPos(t_w, t_h);

//                this->calculatePadding(t_x, t_y, t_w, t_h, section->p_p);
//                section->updateItem(t_x, t_y, t_w, t_h);
//                section->organize();
//                ofLogNotice(" ==> " + section->key)<<" has been organized";
//            }
//        }

        virtual void exit() {}      // called when app quites
        virtual void add(Section *section, int x, int y, int w, int h){
            ofLogNotice()<<"Placed "<<section->key<<" at "<<x<<","<<y<<","<<w<<","<<h;
            section->updateItem(x, y, w, h);
            
            this->members.insert(std::pair<std::string, Section *>(section->key, section));
            
            it_displayable = this->displayable.insert(it_displayable++, section->key);
            this->member_index.insert(std::pair<std::string, std::vector<std::string>::iterator>(section->key, it_displayable));
            section->parent = this;
            
            ofLogNotice(this->getType())<<"[Section] "<<section->key<<" added";
            ofLogNotice(this->getType())<<this->displayable.size()<<" displayable section(s)";

            showChild(section);
            section->onAttachedToParent();
        };        
        
        virtual void addChild(Section* section){
            /**
             Add a child section/layout
             **/
            Json::Value props = section->getData();
            int w = props.get("width", Width() ).asInt();
            int h = props.get("height", Height() ).asInt();
            int x = X(), y = Y();
            add(section, x, y, w, h);
        }
        
        virtual void removeChild(Section* section){}
        
        virtual void showChild(Section *child){
            //Insert this section in its rightful place in displayable array
            //This all seems unneccessarily complicated
            
            std::map<std::string, std::vector<std::string>::iterator>::iterator it = this->member_index.find( child->key ); //find its index
            if( it != member_index.end() )
            {
                std::vector<std::string>::iterator position = std::find(this->displayable.begin(), this->displayable.end(), child->key);
                if (position == this->displayable.end()) // == vector.end() means the element was not found
                {
                    //Show the section
                    ofLogNotice(this->getType())<<"Showing "<<it->first;
                    this->displayable.insert(it->second, it->first);
                    
                    child->onShow();
                }
            }
        };
        
        virtual void hideChild(Section *section){
            std::vector<std::string>::iterator position = std::find(displayable.begin(), displayable.end(), section->key);
            if (position != displayable.end()) // == vector.end() means the element was not found
                displayable.erase(std::remove(displayable.begin(), displayable.end(), section->key));
        }
        
        
        virtual void focusChild(Section* section){}
        
        virtual void deFocusChild(Section* section){}
        
        virtual void changeRatio(Section * section, float w_percent=0.0f, float h_percent=0.0f){
            changeRatio(section->key, w_percent, h_percent);
        }
        virtual void changeRatio(std::string section, float w_percent=0.0f, float h_percent=0.0f){};
        
//        virtual void changeRatio(std::string section, float w_percent=0.0f, float h_percent=0.0f){
//            std::map<std::string, Section *>::iterator it = members.find( section );
//            if( it != members.end() )
//            {
//                (*it).second->w_p = w_percent;
//                (*it).second->h_p = h_percent;
//            }
//        }
        
        virtual void resetSection(std::string section){
            std::map<std::string, Section *>::iterator it = members.find( section );
            if( it != members.end() )
            {
                (*it).second->reset();
            }
        }
        
        
        virtual void resetMaxPosition(){
            cum_width = x_pos.getValue().asInt();
            cum_height = y_pos.getValue().asInt();
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
        
        void onAttachedToParent(){
            /**
             Gets called immediately after a layout has been attached to a parent
             **/
            resetMaxPosition();
            setup(); 
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
        //
        void deFocus(){
            if(this->parent)
                this->parent->deFocusChild(this);
        }
        
        virtual void clearChildren() {
            std::map<std::string, Section *>::iterator it_current;
            for (it_current = members.begin(); it_current != members.end();) {
                ofLogNotice() << "Removing " << it_current->first ;
                
                if ((*it_current).second)
                    delete ((*it_current).second);
                members.erase(it_current++);
            }
            members.clear();
            member_index.clear();
            displayable.clear();
        };
        
        virtual ~Section() {
//            clearChildren();
        };

//Interaction, no holds barred lift off of ofxMSAInteractiveObject
        bool		enabled;				// set this to false to temporarily disable all events
        bool		verbose;

        void enableAllEvents();				// enable all event callbacks
        void disableAllEvents();			// disable all event callbacks

        void enableMouseEvents();			// call this if object should receive mouse events
        void disableMouseEvents();			// call this if object doesn't need to receive mouse events (default)

        void enableKeyEvents();				// call this if object should receive key events
        void disableKeyEvents();			// call this if object doesn't need to receive key events (default)

        void enableAppEvents();				// call this if object should update/draw automatically	(default)
        void disableAppEvents();			// call this if object doesn't need to update/draw automatically


    //	void setPosition(float _x, float _y);	// replaced with ofRectangle::setPosition
        void setSize(float _w, float _h);	// set size of object
    //	void setPosAndSize(float _x, float _y, float _w, float _h);		// replaced with ofRectangle::set


        bool isMouseOver() const;                     // returns true if mouse is over object (based on position and size)
        bool isMousePressed(int mouseButton=0) const;    // returns true if mouse button is down and was pressed over object (based on position and size)


        int	 getMouseX() const;                       // returns mouse X (in screen coordinates)
        int  getMouseY() const;                       // returns mouse Y (in screen coordinates)

        unsigned long getStateChangeMillis() const;   // returns milliseconds since last state change

        virtual bool hitTest(int tx, int ty);		// returns true if given (x, y) coordinates (in screen space) are over the object (based on position and size)


        // these behave very similar to those in flash
        virtual void onRollOver(int x, int y) {}                    // called when mouse enters object x, y, width, height
        virtual void onRollOut() {}                                 // called when mouse leaves object x, y, width, height
        virtual void onMouseMove(int x, int y) {}                   // called when mouse moves while over object x, y, width, height
        virtual void onDragOver(int x, int y, int button) {}        // called when mouse moves while over object and button is down
        virtual void onDragOutside(int x, int y, int button) {}     // called when mouse moves while outside the object after being clicked on it
        virtual void onPress(int x, int y, int button) {}           // called when mouse presses while over object
        virtual void onPressOutside(int x, int y, int button) {}    // called when mouse presses while outside object
        virtual void onRelease(int x, int y, int button) {}         // called when mouse releases while over object
        virtual void onReleaseOutside(int x, int y, int button) {}  // called when mouse releases outside of object after being pressed on object
        virtual void onKeyPress(int key) {}                         // called when keypressed while mouse over the object
        virtual void onKeyRelease(int key) {}                        // called when keyreleased while mouse over the object


        // these are called when the relevant event occurs without caring where it actually happened
        // i.e. its the raw event
        virtual void mouseMoved(int x, int y) {}                     // called when mouse moves anywhere
        virtual void mousePressed(int x, int y, int button) {}       // called when mouse pressed anywhere
        virtual void mouseDragged(int x, int y, int button) {}       // called when mouse dragged anywhere
        virtual void mouseReleased(int x, int y, int button) {}      // called when mouse released anywhere

        virtual void keyPressed(int key) {}                          // called when keypressed anywhere
        virtual void keyReleased(int key) {}                         // called when keyreleased anywhere


        // you shouldn't need access to any of these unless you know what you are doing
        // (i.e. disable auto updates and call these manually)
        void _setup(ofEventArgs &e);
        void _update(ofEventArgs &e);
        void _draw(ofEventArgs &e);
        void _exit(ofEventArgs &e);

        void _mouseMoved(ofMouseEventArgs &e);
        void _mousePressed(ofMouseEventArgs &e);
        void _mouseDragged(ofMouseEventArgs &e);
        void _mouseReleased(ofMouseEventArgs &e);

        void _keyPressed(ofKeyEventArgs &e);
        void _keyReleased(ofKeyEventArgs &e);
private:
        bool            _isMouseOver;       // is mouse over the rect
        map<int, bool>    _isMousePressed;       // is mouse down over the rect (for any given mouse button)
        unsigned long   _stateChangeTimestampMillis;
        ofRectangle	oldRect;
    }; 
}
