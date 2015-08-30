
#include "section.h" 
using namespace ofxLayout;

void Section::enableAllEvents() {
    enableMouseEvents();
    enableKeyEvents();
    enableAppEvents();
}

//--------------------------------------------------------------
void Section::disableAllEvents() {
    disableMouseEvents();
    disableKeyEvents();
    disableAppEvents();
}


//--------------------------------------------------------------
void Section::enableMouseEvents() {
    ofAddListener(ofEvents().mousePressed, this, &Section::_mousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &Section::_mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &Section::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &Section::_mouseReleased);
}

//--------------------------------------------------------------
void Section::disableMouseEvents() {
    ofRemoveListener(ofEvents().mousePressed, this, &Section::_mousePressed);
    ofRemoveListener(ofEvents().mouseMoved, this, &Section::_mouseMoved);
    ofRemoveListener(ofEvents().mouseDragged, this, &Section::_mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &Section::_mouseReleased);
}

//--------------------------------------------------------------
void Section::enableKeyEvents() {
    ofAddListener(ofEvents().keyPressed, this, &Section::_keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &Section::_keyReleased);
}

//--------------------------------------------------------------
void Section::disableKeyEvents() {
    ofRemoveListener(ofEvents().keyPressed, this, &Section::_keyPressed);
    ofRemoveListener(ofEvents().keyReleased, this, &Section::_keyReleased);
}

//--------------------------------------------------------------
void Section::enableAppEvents() {
    ofAddListener(ofEvents().setup, this, &Section::_setup);
    ofAddListener(ofEvents().update, this, &Section::_update);
    ofAddListener(ofEvents().draw, this, &Section::_draw);
    ofAddListener(ofEvents().exit, this, &Section::_exit);
}

//--------------------------------------------------------------
void Section::disableAppEvents() {
    ofRemoveListener(ofEvents().setup, this, &Section::_setup);
    ofRemoveListener(ofEvents().update, this, &Section::_update);
    ofRemoveListener(ofEvents().draw, this, &Section::_draw);
    ofRemoveListener(ofEvents().exit, this, &Section::_exit);
}


//--------------------------------------------------------------
//void Section::setPosition(float _x, float _y) {
//	x = _x;
//	y = _y;
//}

//--------------------------------------------------------------
void Section::setSize(float _w, float _h) {
}

//--------------------------------------------------------------
//void Section::setPosAndSize(float _x, float _y, float _w, float _h) {
//	setPosition(_x, _y);
//	setSize(_w, _h);
//}

//--------------------------------------------------------------
bool Section::isMouseOver() const {
    return _isMouseOver;
}

//--------------------------------------------------------------
bool Section::isMousePressed(int mouseButton) const {
    if(_isMousePressed.find(mouseButton) == _isMousePressed.end()) return false;
    return _isMousePressed.at(mouseButton);
}


//--------------------------------------------------------------
int Section::getMouseX() const {
    return ofGetMouseX();
}

//--------------------------------------------------------------
int Section::getMouseY() const {
    return ofGetMouseY();
}

//--------------------------------------------------------------
unsigned long Section::getStateChangeMillis() const {
    return ofGetElapsedTimeMillis() - _stateChangeTimestampMillis;
}

//--------------------------------------------------------------
bool Section::hitTest(int tx, int ty) {
    int x = X(), y = Y(), height = Height(), width = Width();
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}



//--------------------------------------------------------------
void Section::_setup(ofEventArgs &e) {
    if(!enabled) return;
    setup();
}

//--------------------------------------------------------------
void Section::_update(ofEventArgs &e) {
    if(!enabled) return;

    // check to see if object has moved, and if so update mouse events
//	if(oldRect.x != this->x || oldRect.y != this->y || oldRect.width != this->width ||oldRect.height != this->height) {
//		ofMouseEventArgs e;
//		e.button = _mouseButton;
//		e.x = _mouseX;
//		e.y = _mouseY;
//		if(_isMousePressed) _mouseDragged(e);
//		else _mouseMoved(e);
//
//		oldRect =  (ofRectangle) (*this);
//	}
//    update();
}

//--------------------------------------------------------------
void Section::_draw(ofEventArgs &e) {
    if(!enabled) return;
    draw();
}

//--------------------------------------------------------------
void Section::_exit(ofEventArgs &e) {
    if(!enabled) return;
    exit();
}


//--------------------------------------------------------------
void Section::_mouseMoved(ofMouseEventArgs &e) {
    int x = e.x;
    int y = e.y;
    int button = e.button;
    if(!enabled) return;

//	_mouseX = x;
//	_mouseY = y;

    if(hitTest(x, y)) {						// if mouse is over the object
        if(!_isMouseOver) {						// if wasn't over previous frame
            _isMouseOver = true;						// update flag
            onRollOver(x, y);						// call onRollOver
        }
        onMouseMove(x, y);						// and trigger onMouseMove

        if(verbose)
            ofLogNotice("Section::" + key)<<"_mouseMoved(x: "<<x<<", y:"<< y+")";
    } else if(_isMouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
        onRollOut();							// call onRollOut
        _isMouseOver = false;						// update flag
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();

    mouseMoved(x, y);
}


//--------------------------------------------------------------
void Section::_mousePressed(ofMouseEventArgs &e) {
    int x = e.x;
    int y = e.y;
    int button = e.button;

    if(verbose) printf("Section::_mousePressed(x: %i, y: %i, button: %i)\n", x, y, button);
    if(!enabled) {
        _isMousePressed[button] = false;
        return;
    }

    if(hitTest(x, y)) {						// if mouse is over
        if(!isMousePressed(button)) {						 // if wasn't down previous frame
            _isMousePressed[button] = true;						// update flag
            onPress(x, y, button);					// call onPress
        }
    } else {								// if mouse is not over
        _isMousePressed[button] = false;	// update flag
        onPressOutside(x, y, button);
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();

    mousePressed(x, y, button);
}

//--------------------------------------------------------------
void Section::_mouseDragged(ofMouseEventArgs &e) {
    int x = e.x;
    int y = e.y;
    int button = e.button;

    if(verbose) printf("Section::_mouseDragged(x: %i, y: %i, button: %i)\n", x, y, button);
    if(!enabled) {
        _isMousePressed[button] = false;
        return;
    }

    if(hitTest(x, y)) {						// if mouse is over the object
        if(!_isMouseOver) {						// if wasn't over previous frame
            //				onPress(x, y);							// call onPress - maybe not
            _isMouseOver = true;						// update flag
            onRollOver(x, y);						// call onRollOver
        }
        onDragOver(x, y, button);				// and trigger onDragOver
    } else {
        if(_isMouseOver) {					// if mouse is not over the object, but the flag is true (From previous frame)
            onRollOut();							// call onRollOut
            _isMouseOver = false;						// update flag
        }
        if(isMousePressed(button)) {
            onDragOutside(x, y, button);
        }
        _isMousePressed[button] = false;
    }

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();

    mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void Section::_mouseReleased(ofMouseEventArgs &e) {
    int x = e.x;
    int y = e.y;
    int button = e.button;

    if(verbose) printf("Section::_mouseReleased(x: %i, y: %i, button: %i)\n", x, y, button);
    if(!enabled) {
        _isMousePressed[button] = false;
        return;
    }

    if(hitTest(x, y)) {
        onRelease(x, y, button);
    } else {
        if(isMousePressed(button)) onReleaseOutside(x, y, button);
    }
    _isMousePressed[button] = false;

    _stateChangeTimestampMillis = ofGetElapsedTimeMillis();

    mouseReleased(x, y, button);
}


//--------------------------------------------------------------
void Section::_keyPressed(ofKeyEventArgs &e) {
    int key = e.key;
    if(verbose) printf("Section::_keyPressed(key: %i)\n", key);
    if(!enabled) return;
    if(isMouseOver()) onKeyPress(key);
    keyPressed(key);
}


//--------------------------------------------------------------
void Section::_keyReleased(ofKeyEventArgs &e) {
    int key = e.key;
    if(verbose) printf("Section::_keyReleased(key: %i)\n", key);
    if(!enabled) return;
    if(isMouseOver()) onKeyRelease(key);
    keyReleased(key);
}
