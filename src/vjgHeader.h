//
//  vjgHeader.h
//  vjGokko
//
//  Created by shugohirao on 2018/07/09.
//

#pragma once
#include "ofxUI.h"

const int WIDTH = 1280; //ofGetWidth();
const int HEIGHT = 720; //ofGetHeight();
const int sWIDTH = 1920; //ofGetScreenWidth();
const int sHEIGHT = 1080; //ofGetScreenHeight();

const int PRE_SIZE = 3;
const int themeNum = 43; //ofRandom(45);

static unordered_map<string, ofColor> vjgColor = {
    {"RED",    ofColor(255,90,118)},
    {"PINK",   ofColor(255,145,225)},
    {"ORANGE", ofColor(255,165,37)},
    {"YELLOW", ofColor(255,253,66)},
    {"PURPLE", ofColor(167,114,255)},
    {"BLUE",   ofColor(34,242,255)},
};
const string COLOR[] ={
    "RED",
    "PINK",
    "ORANGE",
    "YELLOW",
    "PURPLE",
    "BLUE",
};

enum ANCHOR{
    CENTER,
    TOPLEFT,
};

struct layerSettings{
    bool bUpdate = true;
    bool bON = false;
    bool bBG = false;
    bool bDraw[PRE_SIZE] = {0,0,0};
    bool bPSBlend = false;
    float alpha = 255; //0~255
};


class vjgTool{
public:
    static void setLayerUI(ofxUISuperCanvas *ui, ofFbo &fbo, layerSettings &ls){
        ui->setTheme(themeNum);
        int w = 200;
        //ui->setColorBack(ofColor(222,22));
        ui->setDrawOutline(true);
        ui->setWidth(w);
#ifdef NO_LOAD_XML
        static int i=1;
        ui->setPosition(i*w,100);
        i++;
#endif
        ui->addBaseDraws("monitor", &fbo,w,w/16*9);
        ui->addToggle("Update", &ls.bUpdate);
        ui->addToggle("DirectON", &ls.bON);
        ui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        ui->addToggle("DirectBG", &ls.bBG);
        ui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        for(int i=0;i<PRE_SIZE;i++){
            ui->addToggle("Draw"+to_string(i), &ls.bDraw[i]);
            ui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        }
        ui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        ui->addSlider("alpha", 0, 255, &ls.alpha)->setColorBack(ofColor::darkRed);
        ui->addSpacer();
    }

    static void initFbo(ofFbo &fbo , ANCHOR anc=TOPLEFT){
        fbo.allocate(sWIDTH,sHEIGHT);
        fbo.begin();
        ofClear(255);
        fbo.end();
        if(anc==CENTER){
            fbo.setAnchorPercent(0.5,0.5);
        }else if(anc==TOPLEFT){
            fbo.setAnchorPercent(0,0);
        }
    }

    static ofPoint ofGetCenter(){
        return ofPoint((float)WIDTH/2,(float)HEIGHT/2);
    }
    static ofPoint ofGetScreenCenter(){
        return ofPoint((float)sWIDTH/2,(float)sHEIGHT/2);
    }

    static void setImageFbo(ofFbo &fbo, ofImage img, ANCHOR anc=CENTER){
        fbo.allocate(img.getWidth(), img.getHeight());
        fbo.begin();
        ofClear(255);
        img.draw(0,0);
        fbo.end();
        if(anc==CENTER){
            fbo.setAnchorPercent(0.5,0.5);
        }else if(anc==TOPLEFT){
            fbo.setAnchorPercent(0,0);
        }
    }

};


