//
//  sectionFactory.h
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#ifndef __example_config__sectionFactory__
#define __example_config__sectionFactory__

#include <iostream>
#include "sectionImpl.h"

#include "section.h"
using namespace ofxLayout;

class SectionFactory{
private:  
    static std::map<std::string, SectionCreator*>& get_table();
    static std::map<std::string, Section*>& get_sections();
public:
    SectionFactory(){};
    ~SectionFactory();
    static void registerit(const std::string& name, SectionCreator* creator);
    static Section* create(const std::string& name, const std::string& key, const Json::Value& data);
    static Section* getSection(std::string name);
    static bool isRegistered(std::string name);
    static void clear();
    static void handleWindowResize(int w, int h);
    typedef shared_ptr<SectionFactory> Ptr;
};

#define REGISTER_SECTION(classname) \
private: \
static const SectionCreatorImpl<classname> creator;

#define REGISTERSECTIONIMPL(classname) \
const SectionCreatorImpl<classname> classname::creator(#classname);

#endif /* defined(__example_config__sectionFactory__) */
