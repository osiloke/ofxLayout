
#include "column.h"

REGISTERSECTIONIMPL(ColumnLayout);

/**
 * @brief Adds a child section to the ColumnLayout.
 *
 * In a ColumnLayout, children are stacked vertically.
 * The width is forced to 100% (1.0f) relative to the parent,
 * and the height is determined by the 'h_percent' property of the child.
 *
 * @param section Pointer to the child section to be added.
 */
void ColumnLayout::addChild(Section * section) {
	Json::Value props = section->getData();
	/*
     t_h = derived height from h_percent
     cum_height = next y pos (vertical stacking)
     cum_width = x pos (always starts at 0 relative to parent)
     */
	float w_percent = props.get("w_percent", 1.0f).asFloat(); // Width can be variable
	float h_percent = props.get("h_percent", 1.0f).asFloat(); // Height is variable
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
 * @brief Changes the height ratio of a specific section.
 *
 * @param section Pointer to the section to modify.
 * @param h_percent New height percentage (0.0 to 1.0).
 */
void ColumnLayout::changeRatio(Section * section, float h_percent) {
	changeRatio(section->key, h_percent);
}

/**
 * @brief Changes the height ratio of a section identified by its key.
 *
 * This implementation enforces that only the height ratio is changed,
 * keeping the width ratio fixed at 1.0f for ColumnLayout.
 *
 * @param section Key string of the section.
 * @param h_percent New height percentage (0.0 to 1.0).
 */
void ColumnLayout::changeRatio(std::string section, float h_percent) {
	// Only height ratio changes are allowed in ColumnLayout
	FluidLayout::changeRatio(section, 1.0f, h_percent);
}

/**
 * @brief Updates the cumulative position tracking for the layout.
 *
 * Increments the cumulative height to position the next element vertically.
 *
 * @param t_w The width of the element just added (unused in ColumnLayout for accumulation).
 * @param t_h The height of the element just added.
 */
void ColumnLayout::updateMaxPos(int t_w, int t_h) {
	// Update cumulative height for next element placement
	int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();
	cum_width = w; // Width is always full
	cum_height = cum_height + t_h; // Increment height accumulator
};

/**
 * @brief Calculates the actual position and dimensions for the object.
 *
 * Computes the pixel values for width and height based on the percentages provided.
 *
 * @param w_percent Width percentage (0.0 - 1.0).
 * @param h_percent Height percentage (0.0 - 1.0).
 * @param padding Padding percentage to apply.
 * @param t_x Reference to output target x position.
 * @param t_y Reference to output target y position.
 * @param t_w Reference to output target width.
 * @param t_h Reference to output target height.
 */
void ColumnLayout::calcTargets(float w_percent, float h_percent, float padding, int & t_x, int & t_y, int & t_w, int & t_h) {
	int h = height.getValue().asInt(), w = width.getValue().asInt();

	float w_ = w_percent; // Width is usually 1.0 for Column items
	float h_ = h_percent + padding; // Height includes percentage + padding
	t_w = w_ * w;
	t_h = h_ * h;

	clip(t_x, t_y, t_w, t_h);
};

/**
 * @brief Adjusts the calculated coordinates to fit within the layout constraints.
 *
 * For ColumnLayout, this ensures the x-position aligns with the parent's x-position.
 *
 * @param t_x Reference to target x position (modified).
 * @param t_y Reference to target y position.
 * @param t_w Reference to target width.
 * @param t_h Reference to target height.
 */
void ColumnLayout::clip(int & t_x, int & t_y, int & t_w, int & t_h) {
	int h = height.getValue().asInt(), w = width.getValue().asInt();
	// Ensure x position aligns with parent's x position (no horizontal offset)
	t_x = x_pos.getValue().asInt();
}

void ColumnLayout::organize() {
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

void ColumnLayout::calculatePadding(int & t_x, int & t_y, int & t_w, int & t_h, float p) {
	int w_p = p * t_w; //What to take off the left and right site
	int h_p = p * t_h; //What to take off the top and bottom sides

	t_w = t_w - (w_p * 2); //Shave off padding
	t_h = t_h - (h_p * 2); //Shave off padding

	//Reposition to center
	t_x = t_x + w_p;
	t_y = t_y + h_p;
}
