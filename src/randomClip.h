//
//  randomClip.h
//  vjGokko
//
//  Created by shugohirao on 2018/04/15.
//
#pragma once
#include "ofMain.h"

class randomClip {
private:
    //    ofPtr<ofxUISuperCanvas> UI;
    ofxUISuperCanvas *UI;
    ofFbo fbo;
    layerSettings ls;

public:
    string clipFolder;
    bool bRandom = true;
    int speed = 5;
    float scale = 1;
    ofPoint pos;
    int index=0;
    vector<ofFbo> rClipFbos;
    vector<ofFbo> lClipFbos;

    void setup(bool centerClip){
        vjgTool::initFbo(fbo);
        if(centerClip){
//            rClipFbos.clear();
//            lClipFbos.clear();
//            rClipFbos.shrink_to_fit();
//            lClipFbos.shrink_to_fit();
        }
    }

    void setClip(vector<ofFbo> &clips){
        lClipFbos = clips;
    }

    void updateCenterClip(){
        if(ls.bUpdate){
            cout<<"--- uodateCenterCLip"<<endl;
            fbo.begin();
            ofPushMatrix();
            ofPushStyle();
            ofClear(255);
            ofTranslate(vjgTool::ofGetCenter());
            ofSetColor(255);
            if("anatomy"==clipFolder) drawRandom(lClipFbos);
            if("lady"==clipFolder) drawRandom(lClipFbos);
            if("flower"==clipFolder) drawRandom(lClipFbos);
            if("butterfly"==clipFolder) drawRandom(lClipFbos);
            if("fig"==clipFolder) drawRandom(lClipFbos);
            ofPopStyle();
            ofPopMatrix();
            fbo.end();

        }
    }
    void updateLRClip(){
        if(ls.bUpdate){
            cout<<"--- uodateLRCLip"<<endl;
            fbo.begin();
            ofPushMatrix();
            ofPushStyle();
            ofClear(255);
            ofTranslate(vjgTool::ofGetCenter());
            ofSetColor(255);
            drawRandom(lClipFbos,ofPoint(400,0));
            drawRandom(rClipFbos,ofPoint(-400,0));
            ofPopStyle();
            ofPopMatrix();
            fbo.end();
        }
    }

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

    void render(){
        fbo.draw(0,0);
    }

    //--------------------------------------------------------------
    void setCenterClipUI(vector<string> aIFolderNames){
        UI = new ofxUISuperCanvas("AntiqueImages");
        vjgTool::setLayerUI(UI, fbo, ls);

        UI->addSlider("Sclae", 1, 3, &scale);
        UI->addIntSlider("Speed", 1, 30, &speed);
        ofxUIDropDownList *ddl = (ofxUIDropDownList *)UI->addDropDownList("antiqueFolder", aIFolderNames);
        ddl->setSingleSelected(1);
        ddl->setAutoClose(true);
        ddl->setShowCurrentSelected(true);
        UI->autoSizeToFitWidgets();
        //ofAddListener(UI->newGUIEvent, this, &vjGokko::guiEvent);
    }
    void setLRClipUI(){
        UI = new ofxUISuperCanvas("LR clip");
        vjgTool::setLayerUI(UI, fbo, ls);

        UI->addSlider("LR_Scale", 1, 3, &scale);
        UI->addIntSlider("LR_Speed", 1, 30, &speed);
        UI->autoSizeToFitWidgets();
        //ofAddListener(UI->newGUIEvent, this, &vjGokko::guiEvent);
    }

    ofxUISuperCanvas* getUI(){ return UI; }
    layerSettings getLS(){ return ls; }

};
