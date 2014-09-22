//
//  sectionCreator.cpp
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#include "sectionCreator.h" 
#include "sectionFactory.h"

SectionCreator::SectionCreator(std::string name)
{
    SectionCreator::Ptr p(this);
	SectionFactory::registerit(name, p);
}