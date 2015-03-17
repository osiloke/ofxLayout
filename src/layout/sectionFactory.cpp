#include "sectionFactory.h"
#include "sectionCreator.h"

SectionFactory::~SectionFactory(){
    ofLogNotice("ofxLayout::SectionFactory")<<"Destroy section factory";
}

void SectionFactory::registerit(const std::string& name, SectionCreator* creator)
{
	get_table()[name] = creator;
}

Section* SectionFactory::getSection(std::string name){
    std::map<std::string, Section*>::iterator found;
    found = get_sections().find(name);
    if (found != get_sections().end()){
        return found->second;
    }else{
        return NULL;
    }
    
};

Section* SectionFactory::create(const std::string& name, const std::string& key, const Json::Value& data){
    std::map<std::string, SectionCreator*>::iterator i;
    i = get_table().find(name);
    if (i != get_table().end()){
        ofLogNotice("ofxLayout::SectionFactory")<<"Found registered section, "<<name<<"["<<key<<"]";
        
        Section* section = i->second->create(key, data);
        ofLogNotice("ofxLayout::SectionFactory")<<"Created section "<<name<<"["<<key<<"]";
        
        get_sections().insert(std::pair<std::string, Section*>(key, section));
        return section;
    }else{
        ofLogNotice("ofxLayout::SectionFactory")<<"Could not find section of type "<<name<<"["<<key<<"]";
        return NULL;
    }
};

bool SectionFactory::isRegistered(std::string name){
    if( get_table().find(name) == get_table().end()){
        return false;
    }
    return true;
}
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

void SectionFactory::clear(){
    static std::map<std::string, Section *>::iterator it_current;
    for (it_current = get_sections().begin(); it_current != get_sections().end();) {
        ofLogNotice("ofxLayout::Parser") << "Removing " << it_current->first ;
        
        if ((*it_current).second)
            delete ((*it_current).second);
        get_sections().erase(it_current++);
    }
    get_sections().clear(); 
}