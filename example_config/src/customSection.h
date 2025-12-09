#pragma once

#include "sectionFactory.h"
#include <iostream>

class CustomSection : public Section {
	//Let the section factory know that you can be created dynamically
	REGISTER_SECTION(CustomSection);

public:
	//Minimum constructor needed to use a section, This is assuming you are setting up sections from a section file
	CustomSection(const std::string key, const Json::Value data)
		: Section(key, data) { }
	int getColor() {
		if (data.isMember("color")) {
			Json::Value color = data["color"];
			if (color.isMember("hex")) {
				return (int)strtol(color["hex"].asString().c_str(), NULL, 16);
			}
		}
		return 0;
	}
	void onRefresh() {
		/**
         Called when parent has been altered and children need to be refreshed
         **/
	}
	void update() {
	}
	void draw(int x, int y, int w, int h) {
		//Draw whatever here, the x, y, w and h are handled by the parent section
		ofSetHexColor(getColor());
		ofFill();
		ofDrawRectangle(x, y, w, h);
		ofNoFill();
	};
};
