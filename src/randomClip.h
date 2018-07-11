//
//  randomClip.h
//  vjGokko
//
//  Created by shugohirao on 2018/04/15.
//
#pragma once
#include "ofMain.h"

class randomClip {

public:
    string clipFolder;
    bool bRandom = true;
    int speed = 5;
    float scale = 1;
    ofPoint pos;
    int index=0;

    void drawSelected(ofFbo &fbo, ofPoint pos=ofPoint(0,0)){
        ofPushMatrix();
        ofTranslate(pos);
        ofScale(scale,scale);
        fbo.draw(0,0);
        ofPopMatrix();
    }

    void drawRandom(vector<ofFbo> &fbo, ofPoint pos=ofPoint(0,0)){
        if(bRandom && ofGetFrameNum() % (int)speed ==0){
            index = (int)ofRandom(100) % fbo.size();
        }
        drawSelected(fbo[index], pos);
    }

};
