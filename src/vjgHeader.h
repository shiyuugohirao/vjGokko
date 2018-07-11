//
//  vjgHeader.h
//  vjGokko
//
//  Created by shugohirao on 2018/07/09.
//

//#define NO_LOAD_XML

#pragma once
#include "ofxUI.h"

const int uiWIDTH = 1280; //ofGetWidth();
const int uiHEIGHT = 720; //ofGetHeight();
const int WIDTH = 1920; //ofGetScreenWidth();
const int HEIGHT = 1080; //ofGetScreenHeight();

const int PRE_SIZE = 3;
const int themeNum = 43; //ofRandom(45);

static unordered_map<string, ofColor> vjgColor = {
    {"BLUE",   ofColor(34,242,255)},
    {"WATER",   ofColor(220,242,255)},
    {"SKY",   ofColor(135,185,255)},
    {"ICE",   ofColor(200,220,255)},
};
const string COLOR[] ={
    "BLUE",
    "WATER",
    "SKY",
    "ICE",
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
        fbo.allocate(WIDTH, HEIGHT);
        fbo.begin();
        ofClear(255);
        fbo.end();
        if(anc==CENTER){
            fbo.setAnchorPercent(0.5,0.5);
        }else if(anc==TOPLEFT){
            fbo.setAnchorPercent(0,0);
        }
    }

    static ofPoint ofGetCenter(){ return ofPoint(WIDTH/2,HEIGHT/2); }
    static ofPoint ofGetUICenter(){ return ofPoint(uiWIDTH/2,uiHEIGHT/2); }

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


