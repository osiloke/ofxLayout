//
//  sectionFactory.cpp
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#include "sectionFactory.h"
#include "sectionCreator.h"


void SectionFactory::registerit(const std::string& name, SectionCreator* creator)
{
	get_table()[name] = creator;
}

Section* SectionFactory::getSection(std::string name){
    std::map<std::string, Section*>::iterator found;
    found = get_sections().find(name);
    if (found != get_sections().end()){
        return found->second;
    }
    
};

Section* SectionFactory::create(const std::string& name, const std::string& key, const Json::Value& data){
    std::map<std::string, SectionCreator*>::iterator i;
    i = get_table().find(name);
    if (i != get_table().end()){
        Section* section = i->second->create(key, data);
        get_sections().insert(std::pair<std::string, Section*>(key, section));
        return section;
    }
};

std::map<std::string, Section*>& SectionFactory::get_sections()
{
	static std::map<std::string, Section*> sections;
	return sections;
}

std::map<std::string, SectionCreator*>& SectionFactory::get_table()
{
	static std::map<std::string, SectionCreator*> table;
	return table;
}