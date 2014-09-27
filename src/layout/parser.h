#include "ofxJSONElement.h"
#include "factory.h"
#include "sectionFactory.h"
#include <algorithm>
#include <string> 

class LayoutParser{
private:
	ofxJSONElement config; 
	void computeLayout(){
		//First element in config is root 
            for( Json::ValueIterator itr = config.begin() ; itr != config.end() ; itr++ ){
                
                std::string key = itr.key().asString();
                ofxJSONElement root = config[key];
                Json::Value children = root.removeMember("children");
                
                Section* rootLayout = SectionFactory::create(key, root["key"].asString(), root);
                
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
                
                ofLogVerbose("ofxLayout::Parser")<<"Created "<<key<<" ["<<rootLayout->key<<"]";
                
                if (!children.isNull() && children.size() > 0){
                    computeChildren(children, rootLayout);
                }
                
            }
    }
    void computeChildren(ofxJSONElement children, Section* parentLayout){
        int c_Size = children.size();
        for(int i = 0; i < c_Size; i++)
        {
            Json::Value child = children[i];
            for( Json::ValueIterator itr = child.begin() ; itr != child.end() ; itr++ ){
                std::string type = itr.key().asString();
                Json::Value root = child[type];
                ostringstream ss;
                ss << type << "_"<<i;
                Json::Value nextChildren = root.removeMember("children");
                ofLogVerbose("ofxLayout::Parser")<<"Processing child of type "<<type;
                if(SectionFactory::isRegistered(type))
                {
                    ofLogVerbose("ofxLayout::Parser")<<"Child of type "<<type<<" exists";
                
                    Section* rootLayout = SectionFactory::create(type, root["key"].asString(), root);
                
                    parentLayout->addChild(rootLayout);
                    
                    ofLogVerbose("ofxLayout::Parser")<<"Created "<<type<<" ["<<rootLayout->key<<"]";
                    int ch_Size = nextChildren.size();
                    if (!nextChildren.isNull() && c_Size > 0){
                        computeChildren(nextChildren, rootLayout);
                    } 
                }else{
                    ofLogError("ofxLayout::Parser")<<"Could not create child section ["<<type<<"] "<<"Section is not registered.";
                }
                
            }
        }
    }
public:
    LayoutParser(){};
	LayoutParser(ofxJSONElement config):config(config){};
    void fromJson(Json::Value data){
        config = data;
        computeLayout();
    }
    void fromString(std::string config_data){
        config.parse(config_data);
        computeLayout();
        
    }
    void fromFile(std::string filename){
        if(ofFile::doesFileExist(filename.c_str())){
            bool done = config.open(filename);
            if (done){
                computeLayout();
            }
        }else{
            ofLogError("LayoutParser")<<"Config file does not exist";
        }
    }
};