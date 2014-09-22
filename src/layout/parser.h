#include "ofxJSONElement.h"
#include "factory.h"
#include "sectionFactory.h"
#include <algorithm>
#include <string>
#include "layout/utils.h"

class LayoutParser{
private:
	ofxJSONElement config; 
	void computeLayout(){
		//First element in config is root 
            for( Json::ValueIterator itr = config.begin() ; itr != config.end() ; itr++ ){
                
                std::string key = itr.key().asString();
                ofxJSONElement root = config[key];
                Json::Value children = root.removeMember("children");
                
                Section::Ptr rootLayout = SectionFactory::create(key, root["key"].asString(), root);
                
                ofLogVerbose()<<"Created "<<key<<" ["<<rootLayout->key<<"]";
                
                if (!children.isNull()){
                    computeChildren(children, rootLayout);
                }
                
            }
    }
    void  computeChildren(ofxJSONElement children, Section::Ptr parentLayout){
        int i = 0;
        for( Json::ValueIterator itr = children.begin() ; itr != children.end() ; itr++ ){
            std::string key = itr.key().asString();
            Json::Value root = children[key];
            ostringstream ss;
            ss << key << "_"<<i;
            
            Json::Value children = root.removeMember("children");
            Section::Ptr rootLayout = SectionFactory::create(key, root["key"].asString(), root);
            
            parentLayout->addChild(rootLayout);
            
            ofLogVerbose()<<"Created "<<key<<" ["<<rootLayout->key<<"]";
            rootLayout->printData();
            if (!children.isNull()){
                computeChildren(children, rootLayout);
            } 
            i++;
            
        }
    }
public:
    LayoutParser(){};
	LayoutParser(ofxJSONElement config):config(config){};
    void fromString(std::string config_data){
        config.parse(config_data);
        computeLayout();
    }
    void fromFile(std::string filename){
        bool done = config.open(filename);
        computeLayout();
    }
};