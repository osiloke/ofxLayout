//
//  effects.cpp
//  playlistTest
//
//  Created by Osiloke Emoekpere on 8/14/14.
//
//

#include "effects.h"

PopupBlur::PopupBlur(const ofVec2f& aspect,
                                     bool arb,
                                     float radialScale,
                                     float brightness,
                                     const ofVec3f & startColor,
                                     const ofVec3f & endColor) :
radialScale(radialScale), brightness(brightness), startColor(startColor), endColor(endColor), RenderPass(aspect, arb, "popupblur")
{
    
    string fragShaderSrc = STRINGIFY(
     uniform sampler2D myTexture;
     uniform float fAspect;
     
     uniform vec3 startColor;
     uniform vec3 endColor;
     
     uniform float radialScale;//0. - 1.0 - 2.0
     uniform float brightness;//0.-1.0, deff:2.5
     void main() {
         vec2 pos = gl_TexCoord[0].st;
         
         vec2 vUv = vec2(pos.x, 1.0-pos.y);
         vec2 vSunPositionScreenSpace = vec2(0.5);
         
         vec2 diff = vUv - vSunPositionScreenSpace;
         
         // Correct for aspect ratio
         
         diff.x *= fAspect;
         
         float prop = length( diff ) / radialScale;
         prop = clamp( 2.5 * pow( 1.0 - prop, 3.0 ), 0.0, 1.0 );
         
         vec3 color = mix( startColor, endColor, 1.0 - prop );
         
         vec4 base = texture2D(myTexture, vUv);
         gl_FragColor = vec4(base.xyz * color, 1.0);
         
     }
     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, "#version 110\n" + fragShaderSrc);
    shader.linkProgram();
    
}

void PopupBlur::render(ofFbo& readFbo, ofFbo& writeFbo)
{
    writeFbo.begin();
    
    shader.begin();
    shader.setUniformTexture("myTexture", readFbo.getTextureReference(), 0);
    shader.setUniform1f("fAspect", 1);
    shader.setUniform3f("startColor", 1, 1, 1);
    shader.setUniform3f("endColor", 0, 0, 0);
    shader.setUniform1f("radialScale", radialScale);
    shader.setUniform1f("brightness", brightness);
    
    texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
    
    shader.end();
    writeFbo.end();
}

FadeBlack::FadeBlack(const ofVec2f& aspect,
                     bool arb,
                     float prog,
                     const ofVec3f & endColor) :
endColor(endColor), progress(prog), RenderPass(aspect, arb, "fadeblack")
{
    
    string fragShaderSrc = STRINGIFY(
     uniform sampler2D tex0; 
                                     uniform vec3 endColor;
                                     uniform vec2 resolution;
     uniform float colorPhase; // if 0.0, there is no black phase, if 0.9, the black phase is very important
                                     
     uniform float progress;
                                     
     //RADIUS of our vignette, where 0.5 results in a circle fitting the screen
     const float RADIUS = 0.75;
     
     //softness of our vignette, between 0.0 and 1.0
     const float SOFTNESS = 0.45;
                                     
     void main() {
         vec2 pos = gl_TexCoord[0].st;
         vec2 vUv = vec2(pos.x, 1.0-pos.y); 
         
         float len = length(vUv - vec2(0.8)) * progress;
         
         gl_FragColor = vec4( texture2D(tex0, vUv).rgb * (1.0 - len), 1.0 );
//         gl_FragColor = mix(vec4(endColor, 1.0), texture2D(tex0, vUv), progress);//smoothstep(1.0-colorPhase, 0.0, progress));
     }
     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, "#version 110\n" + fragShaderSrc);
    shader.linkProgram();
    
}

void FadeBlack::render(ofFbo& readFbo, ofFbo& writeFbo)
{
    writeFbo.begin();
    
    shader.begin();
    shader.setUniformTexture("tex0", readFbo.getTextureReference(), 0);  
    shader.setUniform3f("endColor", 0, 0, 0); 
    shader.setUniform1f("colorPhase", 0.4f);
    shader.setUniform1f("progress", progress);
    shader.setUniform2f("resolution", readFbo.getWidth(), readFbo.getHeight());
    
    texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
    
    shader.end();
    writeFbo.end();
}