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

	void organize() {
		this->resetMaxPosition();
		int h = this->height.getValue().asInt(), w = this->width.getValue().asInt();

		// Calculate total height
		int total_h = 0;
		for (auto & id : displayable) {
			Section * s = members.at(id);
			total_h += (s->h_p + s->p_p) * h;
		}

		std::string valign = data.get("valign", "top").asString();
		if (valign == "center") {
			cum_height += (h - total_h) / 2;
		} else if (valign == "bottom") {
			cum_height += (h - total_h);
		}

		std::string halign = data.get("halign", "left").asString();

		ofLogNotice(this->getType()) << this->displayable.size() << " sections will be organized";
		std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();

		for (it = this->displayable.rbegin(); it != this->displayable.rend(); it++) {
			int t_x = this->cum_width, t_y = this->cum_height, t_w, t_h;

			Section * section = this->members.at((*it));
			this->calcTargets(section->w_p, section->h_p, section->p_p, t_x, t_y, t_w, t_h);
			this->updateMaxPos(t_w, t_h);

			if (halign == "center") {
				t_x += (w - t_w) / 2;
			} else if (halign == "right") {
				t_x += (w - t_w);
			}

			this->calculatePadding(t_x, t_y, t_w, t_h, section->p_p);
			section->updateItem(t_x, t_y, t_w, t_h);
			section->organize();
			ofLogNotice(" ==> " + section->key) << " has been organized";
		}
	}

	virtual void clip(int & t_x, int & t_y, int & t_w, int & t_h) {
		int h = height.getValue().asInt(), w = width.getValue().asInt();
		t_x = x_pos.getValue().asInt();

		if (t_h > (h - cum_height))
			t_y = 0;
	}

	virtual std::string getType() {
		return "Fluid Layout";
	}
};

}
