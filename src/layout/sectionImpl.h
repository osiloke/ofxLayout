//
//  sectionImpl.h
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#ifndef __example_config__sectionImpl__
#define __example_config__sectionImpl__

#include <iostream> 

#include <string>

#include "section.h"
#include "sectionCreator.h"

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
#endif /* defined(__example_config__sectionImpl__) */
