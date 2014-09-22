#include "ofxJSONElement.h"
#include "layout/factory.h"

class LayoutParser{
private:
	ofxJSONElement config;
	LayoutFactory l_factory;
public:
	LayoutParser(ofxJSONElement config):config(config){}
	void computeLayout(){
		//First element in config is root
		ofxJSONElement root = config[0];
		for (int i = 0; i < config.size(); i++) {
            if (items[i]["impressions"].asInt() > 0) {
                addItem(items[i]);
            }
        }
	}
};