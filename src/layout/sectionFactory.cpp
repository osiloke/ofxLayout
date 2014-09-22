//
//  sectionFactory.cpp
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#include "sectionFactory.h"
#include "sectionCreator.h"


void SectionFactory::registerit(const std::string& name, SectionCreator::Ptr creator)
{
	get_table()[name] = creator;
}


Section::Ptr SectionFactory::create(const std::string& name, const std::string& key, const Json::Value& data){
    std::map<std::string, SectionCreator::Ptr>::iterator i;
    i = get_table().find(name);
    if (i != get_table().end()){
        return i->second->create(key, data);
    }
};

std::map<std::string, SectionCreator::Ptr>& SectionFactory::get_table()
{
	static std::map<std::string, SectionCreator::Ptr> table;
	return table;
}