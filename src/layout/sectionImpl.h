#pragma once

#include <iostream> 

#include <string>

#include "section.h"
#include "sectionCreator.h"

using namespace ofxLayout;

template <class T>
class SectionCreatorImpl: public SectionCreator
{
public: 
	SectionCreatorImpl<T>(std::string name): SectionCreator(name) {}
    virtual ~SectionCreatorImpl<T>() {}
    virtual Section* create(const std::string key, const Json::Value data) {
//        Section* p( new T(key, data));
        return new T(key, data);
    }
    typedef shared_ptr<SectionCreatorImpl> Ptr;
    
};  
