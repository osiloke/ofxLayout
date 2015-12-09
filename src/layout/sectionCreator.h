#pragma once

#include <iostream>
#include "section.h"

using namespace ofxLayout;
class SectionCreator
{
private:
    string name;
public: 
	SectionCreator(std::string name);
    virtual ~SectionCreator(){ofLogNotice("ofxLayout::SectionCreator")<<name<<" Destruction";};
	virtual Section* create(const std::string key, const Json::Value data) = 0;
//    typedef shared_ptr<SectionCreator> Ptr;
}; 
