#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxPSBlend.h"
#include "ofxPostGlitch.h"

#include "vjgHeader.h"
#include "TEMPLATE.h"
#include "crossRects.h"
#include "fftFlower.h"
#include "randomClip.h"
#include "videoPlayer.h"

class vjGokko : public ofBaseApp{
public:
    //=================================//
    //========== Output window ========//
    //=================================//
    void setupOutput();
    void drawOutput(ofEventArgs &args);
    void exitOutput(ofEventArgs &args);

//    float mainW,mainH;
    bool blackBg = false;
    float preAlpha[PRE_SIZE]={255,255,255};
    bool bBlend[PRE_SIZE]={1,0,0};

    ofFbo maskFbo;
    ofxPSBlend psBlend;
    int blendMode;
    ofxPostGlitch postGlitch;

    //--- fbo ---//
    vector<ofFbo> preFbo;
    ofFbo blendFbo;
    ofFbo glitchFbo;
    ofFbo finalFbo;
    float outputAlpha;


    //=============================//
    //========== UI app ==========//
    //=============================//
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed( ofKeyEventArgs & key );
    void keyReleased( ofKeyEventArgs & key );

    void setupUI();

    void guiEvent(ofxUIEventArgs &e);

    void setTitleUI();
    void setPreUI();
    void setMonitorUI();

    bool bRandomPS = false;
    int randomSpan=10;
    int themeNum;
    ofxUISuperCanvas *TITLE;
    vector<ofxUISuperCanvas*> preUI;
    ofxUISuperCanvas *MONITOR;
    ofxUILabel * blendName;
    ofxUIFPSSlider *fpsMonitor;
    ofxUILabel *blendLabel;
    
    //--- antiqueImages ---//
    vector<string> aIFolderNames = {"anatomy","lady","flower","butterfly","fig","rFace","lFace"};
    unordered_map<string, vector<ofFbo> > antiqueFbos;

    vector<string> videoNames = {"nostalgy01.mp4","sunshine.mp4","hokori.mp4","city.mp4"};
    unordered_map<string, ofVideoPlayer > videos;

    //===============================================//
    //============== LAYERS SETTINGS ================//
    //===============================================//
    TEMPLATE temp;
    crossRects cRects;
    fftFlower flower;
    randomClip centerClip;
    randomClip lClip, rClip;

    videoPlayer vPlayer;

    //--- Fbo
    ofFbo tempFbo;
    ofFbo centerClipFbo;
    ofFbo lrClipFbo;
    ofFbo cRFbo;
    ofFbo flowerFbo;
    ofFbo videoFbo;

    //--- layerSettings
    layerSettings lsTemp;
    layerSettings lsCrossRects;
    layerSettings lsCenterClip;
    layerSettings lsLRClip;
    layerSettings lsFlower;
    layerSettings lsVideo;

    //--- UI
    ofxUISuperCanvas *allLayer;
    ofxUISuperCanvas *tempUI;
    ofxUISuperCanvas *centerClipUI;
    ofxUISuperCanvas *lrClipUI;
    ofxUISuperCanvas *crossRectsUI;
    ofxUISuperCanvas *flowerUI;
    ofxUISuperCanvas *videoUI;

    inline void setLayerUI(ofxUISuperCanvas *ui, ofFbo &fbo, layerSettings &ls);
    void setTempUI();
    void setCenterClipUI();
    void setCrossRectsUI();
    void setFlowerUI();
    void setLRClipUI();
    void setVideoUI();

//    //----------------------//
//    //--- generalMethods ---//
//    //----------------------//
//
//    void initFbo(ofFbo &fbo , ANCHOR anc=TOPLEFT){
//        fbo.allocate(WIDTH,HEIGHT);
//        fbo.begin();
//        ofClear(255);
//        fbo.end();
//        if(anc==CENTER){
//            fbo.setAnchorPercent(0.5,0.5);
//        }else if(anc==TOPLEFT){
//            fbo.setAnchorPercent(0,0);
//        }
//    }
//
//    void setImageFbo(ofFbo &fbo, ofImage img, ANCHOR anc=CENTER){
//        fbo.allocate(img.getWidth(), img.getHeight());
//        fbo.begin();
//        ofClear(255);
//        img.draw(0,0);
//        fbo.end();
//        if(anc==CENTER){
//            fbo.setAnchorPercent(0.5,0.5);
//        }else if(anc==TOPLEFT){
//            fbo.setAnchorPercent(0,0);
//        }
//    }
};
