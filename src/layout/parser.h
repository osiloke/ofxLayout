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
                
                std::string s_width = root.get("width", "").asString();
                std::string s_height = root.get("height", "").asString();
                
                int width = 0, height =0;
                if (s_width.find("Window")!=std::string::npos)
                {
                    width = ofGetWidth();
                }else{
                    width = atoi(s_width.c_str());
                }
                
                if (s_height.find("Window")!=std::string::npos)
                {
                    height = ofGetHeight();
                }else{
                    height = atoi(s_height.c_str());
                }
                
                rootLayout->setup(0, 0, width, height);
                
                ofLogVerbose()<<"Created "<<key<<" ["<<rootLayout->key<<"]";
                
                if (!children.isNull() && children.size() > 0){
                    computeChildren(children, rootLayout);
                }
                
            }
    }
    void  computeChildren(ofxJSONElement children, Section::Ptr parentLayout){
        for(int i = 0; i < children.size(); i++)
        {
            Json::Value child = children[i];
            for( Json::ValueIterator itr = child.begin() ; itr != child.end() ; itr++ ){
                std::string key = itr.key().asString();
                Json::Value root = child[key];
                ostringstream ss;
                ss << key << "_"<<i;
                
                Json::Value nextChildren = root.removeMember("children");
                Section::Ptr rootLayout = SectionFactory::create(key, root["key"].asString(), root);
                
                parentLayout->addChild(rootLayout);
                
                ofLogVerbose()<<"Created "<<key<<" ["<<rootLayout->key<<"]";
                
                if (!nextChildren.isNull() && nextChildren.size() > 0){
                    computeChildren(nextChildren, rootLayout);
                } 
                
            }
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