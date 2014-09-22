//
//  sectionCreator.h
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#ifndef __example_config__sectionCreator__
#define __example_config__sectionCreator__

#include <iostream>
#include "section.h"

class SectionCreator
{
public: 
	SectionCreator(std::string name); 
    virtual ~SectionCreator(){};
	virtual Kabbou::Section::Ptr create(const std::string key, const Json::Value data) = 0;
    typedef shared_ptr<SectionCreator> Ptr;
};
#endif /* defined(__example_config__sectionCreator__) */
