#pragma once

#include "ofMain.h"
#include "propertyEvent.h"
#include "section.h"
#include "sectionFactory.h"
#include <iostream>
#include <map>
#include <vector>

namespace ofxLayout {
class FluidLayout : public Section {
private:
	static const SectionCreatorImpl<FluidLayout> creator;

public:
	typedef shared_ptr<FluidLayout> Ptr;
	FluidLayout(std::string key)
		: Section(key) {
	}
	FluidLayout(std::string key, Json::Value data)
		: Section(key, data) {
	}

	FluidLayout()
		: Section("layout") {
	}

	~FluidLayout() {
		ofLogNotice() << "Fluid layout destructor";
	}

	void showChild(Section * section) {
		Section::showChild(section);
		organize();
	}

	void hideChild(Section * section) {
		Section::hideChild(section);
		organize();
	}

	virtual std::string getType() {
		return "Fluid Layout";
	}
};

}
