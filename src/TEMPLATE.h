
//
//  TEMPLATE.h
//  vjGokko
//
//  Created by shugohirao on 2018/04/14.
//
#pragma once
//#include "ofMain.h"
#include "vjgHeader.h"

class TEMPLATE {
private:
//    ofPtr<ofxUISuperCanvas> UI;
    ofxUISuperCanvas *UI;
    ofFbo fbo;
    layerSettings ls;

    ofPoint pos;
    float scale;
    float speed;
    float offset;
    float cycle;
    float r;
    float deg;
    bool bRandomRes = true;

protected:


public:
    TEMPLATE(){}
    ~TEMPLATE(){}

    void setup(ofPoint _pos,float _r=100){
        pos = _pos;
        r = _r;
        offset = 1.0;
        deg=0;

        vjgTool::initFbo(fbo);
    }
    void setParams(float _scale, float _speed, float _offset){
        scale = _scale;
        speed = _speed;
        cycle = 1/speed;
        offset = _offset;
    }

    void update(){
        cycle = 1/(speed/10);
        r = (offset+sin(ofGetElapsedTimef()*TWO_PI/cycle))*scale;

        if(ls.bUpdate){
            fbo.begin();
            ofClear(255);
            ofSetColor(255,0,0);
            draw();
            fbo.end();
        }
    }

    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(pos);
        if(bRandomRes){
            switch((int)ofRandom(30)){
                case 0: ofNoFill(); break;
                case 1: ofFill(); break;
                case 2: ofSetCircleResolution(30); break;
                case 3: ofSetCircleResolution(3); break;
                case 4: ofSetCircleResolution(4); break;
                case 5: ofSetCircleResolution(5); break;
                case 6: ofSetCircleResolution(6); break;
                case 10: deg+=10;                 break;
                default: ofSetCircleResolution(3);   break;
            }
        }
        deg++;
        ofRotate(deg);
        ofDrawCircle(0, 0, r);
        ofPopStyle();
        ofPopMatrix();
    }

    void render(){
        fbo.draw(0,0);
    }

    
    void setUI(){
        UI = new ofxUISuperCanvas("template");
        vjgTool::setLayerUI(UI, fbo, ls);

        UI->addSlider("Scale", 0, 30, &scale);
        UI->addSlider("Speed", 0, 15, &speed);
        UI->addSlider("Offset", 0, 100, &offset);

        UI->autoSizeToFitWidgets();
//        ofAddListener(UI->newGUIEvent, this, &vjGokko::guiEvent);
    }

    ofxUISuperCanvas* getUI(){ return UI; }
    layerSettings getLS(){ return ls; }

};
