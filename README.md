ofxLayout
=========

A layout management framework for managing multiple drawable classes in openframeworks. It allows you to easily place any drawable class into a layout, like Row, Column, Overlay etc. This is based on my short experience with python Kivy. It also has a WIP layout language similar to kivy's kv language.

Requires openframeworks >= v0.8.3

TODO:
	☐	Respond to window size changes
	☐	Create layout/section enter and exit animations
	☐	Fix layout positioning and padding
	☐	Respond to live changes to layout/section properties
	☐	Add layout/section mouse events

Depends:
ofxObservableProperty - https://github.com/osiloke/ofxObservableProperty - An addon for creating variables which can be tracked globally via events. You can listen to changes in an int, bool, string etc property of a class.

ofxJSON - https://github.com/jefftimesten/ofxJSON - Used for storing the layout json config file

ofxPostProcessing - https://github.com/neilmendoza/ofxPostProcessing - Used for Eye candy

ofxAnimatable - https://github.com/armadillu/ofxAnimatable - Used for Eye candy

