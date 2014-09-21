//
//  effects.h
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/14/14.
//
//

#ifndef __effects__
#define __effects__

#include "ofMain.h"
#include <iostream>

#include "RenderPass.h"

class PopupBlur : public itg::RenderPass{
public:
    typedef shared_ptr<PopupBlur> Ptr;
    
    PopupBlur(const ofVec2f& aspect,
                      bool arb,
                      float radialScale = 1.2,
                      float brightness = 2.5,
                      const ofVec3f & startColor = ofVec3f(1.0,1.0,1.0),
                      const ofVec3f & endColor = ofVec3f(1.0,1.0,1.0));
    
    void render(ofFbo& readFbo, ofFbo& writeFbo);
    
    void setRadialScale(float val) { radialScale = val; };
    float getRadialScale() { return radialScale; }
    
    void setBrightness(float val) { brightness = val; }
    float getBrightness() { return brightness; }
    
    void setStartColor(const ofVec3f & val) { startColor = val; }
    const ofVec3f getStartColor() { return startColor; }
    
    void setEndColor(const ofVec3f & val) { endColor = val; }
    const ofVec3f getEndColor() { return endColor; }
private:
    ofShader shader;
    ofVec3f startColor;
    ofVec3f endColor;
    float radialScale;
    float brightness;
};

class FadeBlack : public itg::RenderPass{
public:
    typedef shared_ptr<FadeBlack> Ptr;
    
    FadeBlack(const ofVec2f& aspect,
              bool arb,
              float progress = 0.65f,
              const ofVec3f & endColor = ofVec3f(1.0,1.0,1.0), const ofVec2f& resolution = ofVec2f(100.f, 100.f));
    
    void render(ofFbo& readFbo, ofFbo& writeFbo);   
    
    void setEndColor(const ofVec3f & val) { endColor = val; }
    const ofVec3f getEndColor() { return endColor; }
    
    void setResolution(const ofVec2f & val) { resolution = val; }
    const ofVec2f getResolution() { return resolution; }
    
    void setColorPhase(float val) { colorPhase = val; }
    float getColorPhase() { return colorPhase; }
    
    void setProgress(float val) { progress = val; }
    float getProgress() { return progress; }
    
private:
    ofShader shader;
    ofVec3f startColor;
    ofVec3f endColor;
    ofVec2f resolution;
    float colorPhase;
    float progress;
};
#endif /* defined(__effects__) */
