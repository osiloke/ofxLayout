#pragma once

#include <iostream>
#include "sectionFactory.h"
#include "ofMain.h"

class VideoSection: public Section{
//Let the section factory know that you can be created dynamically
REGISTER_SECTION(VideoSection);
    ofVideoGrabber videoGrabber;
    ofPolyline polyline;
public:
    //Minimum constructor needed to use a section, This is assuming you are setting up sections from a section file
    VideoSection(const std::string key, const Json::Value data): Section(key, data){}
    ~VideoSection(){ofLogNotice("VideoSection")<<"destruct";}
    int getColor(){
        /**
         Example of using custom arguments from the layout file
         **/
        if(data.isMember("color")){
            Json::Value color = data["color"];
            if (color.isMember("hex")){
                return (int)strtol(color["hex"].asString().c_str(), NULL, 16);
            }
        }
        return 0;
    }
    void setup(){
        vector<ofVideoDevice> devices = videoGrabber.listDevices();
        
        for(int i = 0; i < devices.size(); i++){
            cout << devices[i].id << ": " << devices[i].deviceName;
            if( devices[i].bAvailable ){
                cout << endl;
            }else{
                cout << " - unavailable " << endl;
            }
        }
        videoGrabber.setDeviceID(0);
        videoGrabber.setDesiredFrameRate(30);
        bool init = videoGrabber.initGrabber(720, 480); 
    }
    void onAttachedToParent(){
        /**
         Gets called immediately after this section has been attached for the first time to a parent
         **/ 
        Section::onAttachedToParent();
        /** ... **/
    }
    void onRefresh(){
        /**
         Called when parent has been altered and children need to be refreshed
         **/
    }
    void update(){ 
        videoGrabber.update();
    }
    
    void draw(int x, int y, int w, int h){ 
        //Draw whatever here, the x, y, w and h are handled by the parent section
        //Use our color variable to set border color
        ofSetHexColor(getColor());
        ofNoFill();
        ofRect(x, y, w, h);
        ofSetColor(255, 255, 255); 
        videoGrabber.draw(x, y, w, h);
    };
}; 
