#pragma once

#include "layout.h"
#include "sectionFactory.h"
#include <iostream>

namespace ofxLayout {
class RowLayout : public FluidLayout {
	REGISTER_SECTION(RowLayout);

public:
	typedef shared_ptr<RowLayout> Ptr;
	RowLayout(std::string key, Json::Value data)
		: FluidLayout(key, data) { };
	RowLayout(std::string key)
		: FluidLayout(key) { };
	RowLayout()
		: FluidLayout("row") { };

	/**
	 * @brief Adds a child section to the RowLayout.
	 *
	 * In a RowLayout, children are arranged horizontally.
	 * The height is forced to 100% (1.0f) relative to the parent,
	 * and the width is determined by the 'w_percent' property of the child.
	 *
	 * @param section Pointer to the child section to be added.
	 */
	void addChild(Section * section) {
		Json::Value props = section->getData();

		// t_w = derived width from w_percent
		// cum_width = next x pos (horizontal stacking)
		// cum_height = y pos (always starts at 0 relative to parent)

		float w_percent = props.get("w_percent", 1.0f).asFloat(); // Width is variable
		float h_percent = props.get("h_percent", 1.0f).asFloat(); // Height can be variable
		float padding = props.get("padding", 0.0f).asFloat();
		int t_x = cum_width, t_y = cum_height, t_w = 0, t_h = 0;

		// Store original properties for resetting or organizing later
		section->w_p = w_percent;
		section->h_p = h_percent;
		section->p_p = padding;
		section->r_w_p = w_percent;
		section->r_h_p = h_percent;
		section->r_p_p = padding;

		calcTargets(w_percent, h_percent, padding, t_x, t_y, t_w, t_h);
		updateMaxPos(t_w, t_h);

		calculatePadding(t_x, t_y, t_w, t_h, padding);

		FluidLayout::add(section, t_x, t_y, t_w, t_h);
	}

	/**
	 * @brief Changes the width ratio of a specific section.
	 *
	 * @param section Pointer to the section to modify.
	 * @param w_percent New width percentage (0.0 to 1.0).
	 */
	void changeRatio(Section * section, float w_percent) {
		changeRatio(section->key, w_percent);
	}

	/**
	 * @brief Changes the width ratio of a section identified by its key.
	 *
	 * This implementation enforces that only the width ratio is changed,
	 * keeping the height ratio fixed at 1.0f for RowLayout.
	 *
	 * @param section Key string of the section.
	 * @param w_percent New width percentage (0.0 to 1.0).
	 */
	void changeRatio(std::string section, float w_percent) {
		// Only width ratio changes are allowed in RowLayout
		FluidLayout::changeRatio(section, w_percent, 1.0f);
	}

	/**
	 * @brief Updates the cumulative position tracking for the layout.
	 *
	 * Increments the cumulative width to position the next element horizontally.
	 *
	 * @param t_w The width of the element just added.
	 * @param t_h The height of the element just added (unused in RowLayout for accumulation).
	 */
	void updateMaxPos(int t_w, int t_h) {
		// Update cumulative width for next element placement
		int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();

		cum_width = cum_width + t_w; // Increment width accumulator
	};

	/**
	 * @brief Calculates the target position and dimensions for a child section.
	 *
	 * Computes the pixel values for width and height based on the percentages provided.
	 * For RowLayout, width includes padding calculation, while height is typically full.
	 *
	 * @param w_percent Width percentage (0.0 - 1.0).
	 * @param h_percent Height percentage (0.0 - 1.0).
	 * @param padding Padding percentage to apply.
	 * @param t_x Reference to output target x position.
	 * @param t_y Reference to output target y position.
	 * @param t_w Reference to output target width.
	 * @param t_h Reference to output target height.
	 */
	virtual void calcTargets(float w_percent, float h_percent, float padding, int & t_x, int & t_y, int & t_w, int & t_h) {
		int h = height.getValue().asInt(), w = width.getValue().asInt();

		float w_ = w_percent + padding; // Width includes percentage + padding
		float h_ = h_percent; // Height is usually 1.0 for Row items
		t_w = w_ * w;
		t_h = h_ * h;

		clip(t_x, t_y, t_w, t_h);
	};

	/**
	 * @brief Adjusts the calculated coordinates to fit within the layout constraints.
	 *
	 * For RowLayout, this ensures the y-position aligns with the parent's y-position.
	 *
	 * @param t_x Reference to target x position.
	 * @param t_y Reference to target y position (modified).
	 * @param t_w Reference to target width.
	 * @param t_h Reference to target height.
	 */
	virtual void clip(int & t_x, int & t_y, int & t_w, int & t_h) {
		int h = height.getValue().asInt();
		t_y = y_pos.getValue().asInt();
	}

	void organize() {
		this->resetMaxPosition();
		int h = this->height.getValue().asInt(), w = this->width.getValue().asInt();

		// Calculate total width
		int total_w = 0;
		for (auto & id : displayable) {
			Section * s = members.at(id);
			total_w += (s->w_p + s->p_p) * w;
		}

		std::string halign = data.get("halign", "left").asString();
		if (halign == "center") {
			cum_width += (w - total_w) / 2;
		} else if (halign == "right") {
			cum_width += (w - total_w);
		}

		std::string valign = data.get("valign", "top").asString();

		ofLogNotice(this->getType()) << this->displayable.size() << " sections will be organized";
		std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();

		for (it = this->displayable.rbegin(); it != this->displayable.rend(); it++) {
			int t_x = this->cum_width, t_y = this->cum_height, t_w, t_h;

			Section * section = this->members.at((*it));
			this->calcTargets(section->w_p, section->h_p, section->p_p, t_x, t_y, t_w, t_h);
			this->updateMaxPos(t_w, t_h);

			if (valign == "center") {
				t_y += (h - t_h) / 2;
			} else if (valign == "bottom") {
				t_y += (h - t_h);
			}

			this->calculatePadding(t_x, t_y, t_w, t_h, section->p_p);
			section->updateItem(t_x, t_y, t_w, t_h);
			section->organize();
			ofLogNotice(" ==> " + section->key) << " has been organized";
		}
	}

	void setup() {
		FluidLayout::setup();
	}

	void draw() {
		FluidLayout::draw();
	}

	void hideChild(Section * section) {
		section->width.setValue(0);
		FluidLayout::hideChild(section);
		organize();
	}

	void showChild(Section * section) {
		FluidLayout::showChild(section);
		organize();
	}

	std::string getType() {
		return "Row Layout";
	}
};
};
