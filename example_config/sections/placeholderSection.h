//
//  placeholderSection.h
//  
//
//  Created by Osiloke Emoekpere on 10/05/14.
//
//

#ifndef __example_config__customSection__
#define __example_config__customSection__

#include <iostream>
#include "sectionFactory.h"

class PlaceholderSection: public Section{
    //Let the section factory know that you can be created dynamically
    REGISTER_SECTION(PlaceholderSection);
public:
    //Minimum constructor needed to use a section, This is assuming you are setting up sections from a section file
    PlaceholderSection(const std::string key, const Json::Value data): Section(key, data){}
    int getColor(){
        if(data.isMember("color")){
            Json::Value color = data["color"];
            if (color.isMember("hex")){
                return (int)strtol(color["hex"].asString().c_str(), NULL, 16);
            }
        }
        return 0;
    }
    void draw(int x, int y, int w, int h){
        //Draw whatever here, the x, y, w and h are handled by the parent section
        ofSetHexColor(getColor());
        ofNoFill();
        ofRect(x, y, w, h);
        ofFill();
    };
};

#endif /* defined(__example_config__placeholderSection__) */
