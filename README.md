ofxLayout
=========

A layout management framework for managing multiple drawable classes in openframeworks. It allows you to easily place any drawable class into a layout, like Row, Column, Overlay etc. 

A layout consists of multiple sections arranged in either rows, columns or overlayed on each other. Each section can have child sections.

ofxLayout uses json documents for defining layouts. You can see an example json doc in the example project. The json doc allows you to specify extra parameters which you can use in configuring a section.

Any existing class can be converted into a section, as long as they implement the setup, update and draw function common to most openframeworks classes. You will also need to register your class as a section using the REGISTER_SECTION macro. This macro simply tells ofxLayout that it can create a section based on your class. For e.g 

```
class WeatherSection: public Section{
//Let the section factory know that you can be created dynamically.
REGISTER_SECTION(WeatherSection);
	int privateNumber;
public:
	void draw(int x, int y, int w, int h){
		//Draw something
	}
```

After doing this, you can use your class in your layout.

```
{
  "FluidLayout": {
    "key": "root",
    "width": "Window",
    "height": "Window",
    "x_pos": 0,
    "y_pos": 0,
    "children": [
      {
        "ColumnLayout": {
          "key": "container",
          "children": [
            {
              "WeatherSection": {
                "key": "weather",
                "w_percent": 0.5,
                "h_percent": 0.5,
                "halign": "left",
                "valign": "bottom",
                "visible": "yes" 
              }
            } 
          ]
        }
      }
    ]
  }
}
```

Requires openframeworks >= v0.8.3

# TODO
- [ ] Respond to window size changes
- [ ] Create layout/section enter and exit animations
- [ ] Fix layout positioning and padding
- [ ] Respond to live changes to layout/section properties
- [ ] Add layout/section mouse events
- [ ] Implement some css like attributes (or just use css styles instead of json)
- [ ] Make sections spit out their default attributes via some command
- [ ] Maybe create a layout editor using html5

Make a request for a feature.

Depends:
ofxObservableProperty - https://github.com/osiloke/ofxObservableProperty - An addon for creating variables which can be tracked globally via events. You can listen to changes in an int, bool, string etc property of a class.

ofxJSON - https://github.com/jefftimesten/ofxJSON - Used for storing the layout json config file

ofxPostProcessing - https://github.com/neilmendoza/ofxPostProcessing - Used for Eye candy

ofxAnimatable - https://github.com/armadillu/ofxAnimatable - Used for Eye candy

