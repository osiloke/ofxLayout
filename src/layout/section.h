#pragma once

#include "boolProperty.h"
#include "intProperty.h"
#include "ofMain.h"
#include "ofxJSONElement.h"
#include "propertyEvent.h"
#include <json/json.h>

namespace ofxLayout {
class Section {
protected:
	ofImage texture;
	void onPropertyChanged(PropertyEvent & args) { };
	/**
	 * @brief Updates the cumulative position tracking for layout calculation.
	 *
	 * @param t_w Target width of the current element being processed
	 * @param t_h Target height of the current element being processed
	 */
	virtual void updateMaxPos(int t_w, int t_h) {
		int h = height.getValue().asInt(), w = width.getValue().asInt(), x = x_pos.getValue().asInt(), y = y_pos.getValue().asInt();

		cum_height = cum_height + t_h;
		if (cum_width != w)
			cum_width = cum_width + t_w;
	};

	int cum_width, cum_height;
	std::map<std::string, Section *> members;
	std::vector<std::string> displayable; //stores displayable sections, it is ordered by the position of sections in the members map;
	std::vector<std::string>::iterator it_displayable;
	std::map<std::string, std::vector<std::string>::iterator> member_index;

public:
	typedef shared_ptr<Section> Ptr;
	virtual Section * create() { return new Section(); };
	IntProperty width;
	IntProperty height;
	IntProperty x_pos;
	IntProperty y_pos;
	BoolProperty b_displayed;
	Section * parent;
	std::string key;
	Json::Value data;
	float h_p, w_p, r_h_p, r_w_p, p_p, r_p_p;
    bool matchesWindowWidth, matchesWindowHeight;
    bool isListeningToResize;

	Section()
		: h_p(0.0f)
		, w_p(0.0f)
		, r_h_p(0.0f)
		, r_w_p(0.0f)
		, p_p(0.0f)
		, r_p_p(0.0f)
        , matchesWindowWidth(false)
        , matchesWindowHeight(false)
        , isListeningToResize(false){ };
	Section(std::string key)
		: key(key)
		, x_pos(key + ".x")
		, y_pos(key + ".y")
		, width(key + ".width")
		, height(key + ".height")
		, b_displayed(key + ".displayed", false)
		, h_p(0.0f)
		, w_p(0.0f)
		, r_h_p(0.0f)
		, r_w_p(0.0f)
		, p_p(0.0f)
		, r_p_p(0.0f)
        , matchesWindowWidth(false)
        , matchesWindowHeight(false)
        , isListeningToResize(false){
		data["name"] = key;
	};

	Section(std::string key, Json::Value data)
		: key(key)
		, x_pos(key + ".x")
		, y_pos(key + ".y")
		, width(key + ".width")
		, height(key + ".height")
		, b_displayed(key + ".displayed", false)
		, data(data)
		, h_p(0.0f)
		, w_p(0.0f)
		, r_h_p(0.0f)
		, r_w_p(0.0f)
		, p_p(0.0f)
		, r_p_p(0.0f)
        , matchesWindowWidth(false)
        , matchesWindowHeight(false)
        , isListeningToResize(false){
	}

	int X() {
		return this->x_pos.getValue().asInt();
	}

	int Y() {
		return this->y_pos.getValue().asInt();
	}

	int Width() {
		return this->width.getValue().asInt();
	}

	int Height() {
		return this->height.getValue().asInt();
	}

	/**
	 * @brief Updates the position and dimensions of the section.
	 *
	 * @param x New x-coordinate
	 * @param y New y-coordinate
	 * @param w New width
	 * @param h New height
	 */
	void updateItem(int x, int y, int w, int h) {
		this->x_pos.setValue(x);
		this->y_pos.setValue(y);
		this->width.setValue(w);
		this->height.setValue(h);
	}
	//        void setCalculatedValues(int w, int h, int x, int y, float w_p, float h_p, float p_p){
	//            this->w_p = w_p;
	//            this->h_p = h_p;
	//            this->p_p = p_p;
	//            this->r_w_p = w_p;
	//            this->r_h_p = h_p;
	//            this->r_p_p = p_p;
	//            updateItem(x, y, w, h);
	//        }
	virtual ofPixelsRef getPixelsRef() { return texture.getPixels(); }
	virtual unsigned char * getPixels() { return texture.getPixels().getData(); }
	virtual ofTexture & getTexture() { return texture.getTexture(); }
	struct Pixels {
		unsigned char * pixels;
		int width;
		int height;
		int channels;
	};

	virtual bool isFrameNew() { return false; }

	void reset() {
		w_p = r_w_p;
		h_p = r_h_p;
		p_p = r_p_p;
	}

	void setDataAttribute(std::string key, ofxJSONElement val) {
		data[key] = val;
	}

	void setData(Json::Value data) {
		this->data = data;
	}

	Json::Value getData() { return data; }

	void printData() {
		ofLogNotice() << this->data.toStyledString();
	}

	virtual std::string getType() {
		return "Unknown Section";
	}

	virtual void handleMessage(ofxJSONElement message) { };

	virtual void onShow() { };
	virtual void onHide() { };
	virtual void onFocus() { };
	virtual void onDeFocus() { };

	void setup(int x, int y, int w, int h) {
		this->x_pos.setValue(x);
		this->y_pos.setValue(y);
		this->width.setValue(w);
		this->height.setValue(h);
		setup();
		ofLogNotice() << "Setup " << key << width.getValue() << " by " << height.getValue();
	}

	virtual void setup() {
		resetMaxPosition();
		it_displayable = displayable.begin();
        if(!isListeningToResize && (matchesWindowWidth || matchesWindowHeight)){
            ofAddListener(ofEvents().windowResized, this, &Section::_windowResized);
            isListeningToResize = true;
        }
	}
    
    void _windowResized(ofResizeEventArgs & args){
         bool changed = false;
         if(matchesWindowWidth){
             this->width.setValue(args.width);
             changed = true;
         }
         if(matchesWindowHeight){
             this->height.setValue(args.height);
             changed = true;
         }
         if(changed){
             this->organize();
         }
    }

	virtual void update() {
		std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();
		for (it = displayable.rbegin(); it != this->displayable.rend(); it++) {
			Section * section = this->members.at((*it));
			section->update();
		};
	};
	/**
         Overloaded virtual function which draws all children sections attached to this layout.
         @return void
         */
	virtual void draw() {
		draw(X(), Y(), Width(), Height());
		this->draw(0, 0);
	}

	/**
         Overloaded virtual function which draws all children sections attached to this layout.
         This draws based on lilo, last in last out.
         TODO: Does lilo make sense
         @return void
         */
	virtual void draw(int x, int y) {
		std::vector<std::string>::reverse_iterator it = displayable.rbegin();
		for (it = displayable.rbegin(); it != displayable.rend(); it++) {
			std::map<std::string, Section *>::iterator found;
			found = members.find((*it));
			if (found != members.end()) {
				Section * section = found->second;
				section->draw();
			}
		}
	}
	virtual void draw(int x, int y, int w, int h) { }

	/**
	 * @brief Adds a child section to this layout.
	 *
	 * @param section Pointer to the section to add
	 * @param t_x Target x-coordinate
	 * @param t_y Target y-coordinate
	 * @param t_w Target width
	 * @param t_h Target height
	 */
	virtual void add(Section * section, int t_x, int t_y, int t_w, int t_h) {
		ofLogNotice() << "Placed " << section->key << " at " << t_x << "," << t_y << "," << t_w << "," << t_h;
		section->updateItem(t_x, t_y, t_w, t_h);

		this->members.insert(std::pair<std::string, Section *>(section->key, section));

		it_displayable = this->displayable.insert(it_displayable++, section->key);
		this->member_index.insert(std::pair<std::string, std::vector<std::string>::iterator>(section->key, it_displayable));
		section->parent = this;

		ofLogNotice(this->getType()) << "[Section] " << section->key << " added";
		ofLogNotice(this->getType()) << this->displayable.size() << " displayable section(s)";

		section->onAttachedToParent();
		showChild(section);
	};

	/**
	 * @brief Adds a child section using its internal properties for dimensions.
	 *
	 * @param section Pointer to the section to add
	 */
	virtual void addChild(Section * section) {
		Json::Value props = section->getData();
		int w = props.get("width", Width()).asInt();
		int h = props.get("height", Height()).asInt();

		float w_percent = props.get("w_percent", 1.0f).asFloat();
		float h_percent = props.get("h_percent", 1.0f).asFloat();
		float padding = props.get("padding", 0.0f).asFloat();

		section->w_p = w_percent;
		section->h_p = h_percent;
		section->p_p = padding;
		section->r_w_p = w_percent;
		section->r_h_p = h_percent;
		section->r_p_p = padding;

		int x = X(), y = Y();
		add(section, x, y, w, h);
	}

	virtual void removeChild(Section * section) { }

	virtual void showChild(Section * child) {
		//Insert this section in its rightful place in displayable array
		//This all seems unneccessarily complicated

		std::map<std::string, std::vector<std::string>::iterator>::iterator it = this->member_index.find(child->key); //find its index
		if (it != member_index.end()) {
			std::vector<std::string>::iterator position = std::find(this->displayable.begin(), this->displayable.end(), child->key);
			if (position == this->displayable.end()) // == vector.end() means the element was not found
			{
				//Show the section
				ofLogNotice(this->getType()) << "Showing " << it->first;
				this->displayable.insert(it->second, it->first);

				child->onShow();
			}
		}
	};

	virtual void hideChild(Section * section) {
		std::vector<std::string>::iterator position = std::find(displayable.begin(), displayable.end(), section->key);
		if (position != displayable.end()) // == vector.end() means the element was not found
			displayable.erase(std::remove(displayable.begin(), displayable.end(), section->key));
	}

	virtual void focusChild(Section * section) { }

	virtual void deFocusChild(Section * section) { }
	/**
	 * @brief Reorganizes all child sections, recalculating their positions and sizes.
	 * This is typically called when the layout dimensions change or children are added/removed.
	 */
	virtual void organize() {
		//Re calculate placement for each item
		this->resetMaxPosition();
		int h = this->height.getValue().asInt(), w = this->width.getValue().asInt(), x = this->x_pos.getValue().asInt(), y = this->y_pos.getValue().asInt();

		ofLogNotice(this->getType()) << this->displayable.size() << " sections will be organized";
		std::vector<std::string>::reverse_iterator it = this->displayable.rbegin();

		for (it = this->displayable.rbegin(); it != this->displayable.rend(); it++) {
			int t_x = this->cum_width, t_y = this->cum_height, t_w, t_h;

			Section * section = this->members.at((*it));
			this->calcTargets(section->w_p, section->h_p, section->p_p, t_x, t_y, t_w, t_h);
			this->updateMaxPos(t_w, t_h);

			this->calculatePadding(t_x, t_y, t_w, t_h, section->p_p);
			section->updateItem(t_x, t_y, t_w, t_h);
			section->organize();
			ofLogNotice(" ==> " + section->key) << " has been organized";
		}
	}

	virtual void changeRatio(Section * section, float w_percent = 0.0f, float h_percent = 0.0f) {
		changeRatio(section->key, w_percent, h_percent);
	}

	virtual void changeRatio(std::string section, float w_percent = 0.0f, float h_percent = 0.0f) {
		std::map<std::string, Section *>::iterator it = members.find(section);
		if (it != members.end()) {
			(*it).second->w_p = w_percent;
			(*it).second->h_p = h_percent;
		}
	}

	virtual void resetSection(std::string section) {
		std::map<std::string, Section *>::iterator it = members.find(section);
		if (it != members.end()) {
			(*it).second->reset();
		}
	}

	virtual void resetMaxPosition() {
		cum_width = x_pos.getValue().asInt();
		cum_height = y_pos.getValue().asInt();
	}

	/**
         Calculates the actual position and dimensions for the object
         **/
	/**
	 * @brief Calculates target position and dimensions based on percentages and padding.
	 *
	 * @param w_percent Width percentage (0.0 - 1.0)
	 * @param h_percent Height percentage (0.0 - 1.0)
	 * @param padding Padding percentage
	 * @param t_x Reference to output target x
	 * @param t_y Reference to output target y
	 * @param t_w Reference to output target width
	 * @param t_h Reference to output target height
	 */
	virtual void calcTargets(float w_percent, float h_percent, float padding, int & t_x, int & t_y, int & t_w, int & t_h) {
		int h = height.getValue().asInt(), w = width.getValue().asInt();
		//Pad on all sides
		float w_ = w_percent + padding;
		float h_ = h_percent + padding;
		t_w = w_ * w;
		t_h = h_ * h;

		clip(t_x, t_y, t_w, t_h);
	};

	virtual void clip(int & t_x, int & t_y, int & t_w, int & t_h) {
		int h = height.getValue().asInt(), w = width.getValue().asInt();
		if (t_h > (h - cum_height))
			t_y = 0;

		if (t_w > (w - cum_width))
			t_x = 0;
	}

	void onAttachedToParent() {
		/**
             Gets called immediately after a layout has been attached to a parent
             **/
		resetMaxPosition();
		setup();
	}

	virtual void calculatePadding(int & t_x, int & t_y, int & t_w, int & t_h, float p) {
		int w_p = p * t_w; //What to take off the left and right site
		int h_p = p * t_h; //What to take off the top and bottom sides

		t_w = t_w - (w_p * 2); //Shave off padding
		t_h = t_h - (h_p * 2); //Shave off padding

		//Reposition to center
		t_x = t_x + w_p;
		t_y = t_y + h_p;
	}

	void conceal() {
		if (this->parent)
			this->parent->hideChild(this);
	}
	void reveal() {
		if (this->parent)
			this->parent->showChild(this);
	}

	void focus() {
		if (this->parent)
			this->parent->focusChild(this);
	}
	//
	void deFocus() {
		if (this->parent)
			this->parent->deFocusChild(this);
	}

	virtual void clearChildren() {
		std::map<std::string, Section *>::iterator it_current;
		for (it_current = members.begin(); it_current != members.end();) {
			ofLogNotice() << "Removing " << it_current->first;

			if ((*it_current).second)
				delete ((*it_current).second);
			members.erase(it_current++);
		}
		members.clear();
		member_index.clear();
		displayable.clear();
	};

	virtual ~Section() {
        if(isListeningToResize){
            ofRemoveListener(ofEvents().windowResized, this, &Section::_windowResized);
        }
		//            clearChildren();
	};
};
}
