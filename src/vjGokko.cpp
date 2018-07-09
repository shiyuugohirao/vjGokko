#include "vjGokko.h"

//#define NO_LOAD_XML
//--------------------------------------------------------------
void vjGokko::setupOutput(){
    ofBackground(0);
    mainW = ofGetWidth();
    mainH = ofGetHeight();

    //===== initFbo =====//
    //--- renderFbo ---//
    preFbo.resize(PRE_SIZE);
    for(auto &f:preFbo){
        initFbo(f);
    }
    initFbo(finalFbo);
    initFbo(blendFbo);
    psBlend.setup(mainW,mainH);
    blendMode = 0;
    initFbo(glitchFbo);
    postGlitch.setup(&glitchFbo);

    //--- layerFbo ---//
//    initFbo(tempFbo);
//    initFbo(centerClipFbo);
//        initFbo(lrClipFbo);
    initFbo(cRFbo);
    initFbo(flowerFbo);


    //--- layerSetup ---//
    centerClip.clipFolder = "anatomy";
    temp.setup(ofGetCenter());
    centerClip.setup(true);
    lrClip.setup(false);
    flower.setupFft(1024);
}
//--------------------------------------------------------------
void vjGokko::setup(){
    ofSetFrameRate(0);
    ofSetVerticalSync(false);
    ofSetDataPathRoot("../Resources/data/");

    //--- init Images ---//
    ofDirectory dir;
    for(auto &fName:aIFolderNames){
        if(dir.listDir("images/antique/"+ fName)){
            ofFbo fbo;
            ofImage img;
            for(int i=0; i<dir.size(); i++) {
                if(img.load(dir.getPath(i))){
                    setImageFbo(fbo, img);
                    clips[fName].push_back(fbo);
//                    if(f=="anatomy"){
//                        anatomyFbos.push_back(fbo);
//                    }else if(f=="lady"){
//                        ladyFbos.push_back(fbo);
//                    }else if(f=="flower"){
//                        flowerFbos.push_back(fbo);
//                    }else if(f=="butterfly"){
//                        butterflyFbos.push_back(fbo);
//                    }else if(f=="fig"){
//                        figFbos.push_back(fbo);
//                    }else if(f=="lFace"){
//                        lrClip.lClipFbos.push_back(fbo);
//                    }else if(f=="rFace"){
//                        lrClip.rClipFbos.push_back(fbo);
//                    }
                }
            }
        }
    }
    centerClip.setClip(clips[centerClip.clipFolder]);

    if(dir.listDir("images/rects")){
        ofImage img;
        vector<ofFbo> fbos;
        for(int i=0; i<dir.size(); i++) {
            string filePath = dir.getPath(i);
            img.load(dir.getPath(i));
            ofFbo fbo;
            fbo.allocate(img.getWidth(), img.getHeight());
            fbo.begin();
            ofClear(255);
            img.draw(0,0,img.getWidth(), img.getHeight());
            fbo.end();
            fbo.setAnchorPercent(0.5, 0.5);
            fbos.push_back(fbo);
        }
        cRects.setup(fbos, 100, 0.2);
    }

    //--- init UI ---//
    setupUI();

#ifndef NO_LOAD_XML
    MONITOR->loadSettings("settings/UI.xml");
    //tempUI->loadSettings("settings/templateUI.xml");
    temp.getUI()->loadSettings("settings/templateUI.xml");
//    centerClipUI->loadSettings("settings/centerClipUI.xml");
//    lrClipUI->loadSettings("settings/lrClipUI.xml");
    centerClip.getUI()->loadSettings("settings/centerClipUI.xml");
    lrClip.getUI()->loadSettings("settings/lrClipUI.xml");

    crossRectsUI->loadSettings("settings/crossRectsUI.xml");
    flowerUI->loadSettings("settings/flowerUI.xml");

#endif
    blendMode=1;
    cout<<" - loadedUIXmls - "<<endl;
}

//--------------------------------------------------------------
void vjGokko::update(){
    if(bRandomPS){
        if(ofGetFrameNum()%randomSpan == 0) blendMode>=24 ? blendMode= 0 : blendMode++;
    }
    blendLabel->setLabel(psBlend.getBlendMode(blendMode));

    //---------- UPDATE LAYERS ----------//
//    if(lsTemp.bUpdate){
//        tempFbo.begin();
//        ofClear(255);
//        temp.update();
//        ofSetColor(255,0,0);
//        temp.draw();
//        tempFbo.end();
//    }
    temp.update();

    centerClip.updateCenterClip();
    lrClip.updateLRClip();
//    if(lsCenterClip.bUpdate){
//        centerClipFbo.begin();
//        ofPushMatrix();
//        ofPushStyle();
//        ofClear(255);
//        ofTranslate(ofGetCenter());
//        ofSetColor(255);
//        if("anatomy"==centerClip.clipFolder) centerClip.drawRandom(anatomyFbos);
//        if("lady"==centerClip.clipFolder) centerClip.drawRandom(ladyFbos);
//        if("flower"==centerClip.clipFolder) centerClip.drawRandom(flowerFbos);
//        if("butterfly"==centerClip.clipFolder) centerClip.drawRandom(butterflyFbos);
//        if("fig"==centerClip.clipFolder) centerClip.drawRandom(figFbos);
//        ofPopStyle();
//        ofPopMatrix();
//        centerClipFbo.end();
//    }
//    if(lsLRClip.bUpdate){
//        lrClipFbo.begin();
//        ofPushMatrix();
//        ofPushStyle();
//        ofClear(255);
//        ofTranslate(ofGetCenter());
//        ofSetColor(255);
//        lClip.drawRandom(lClipFbos,ofPoint(400,0));
//        rClip.drawRandom(rClipFbos,ofPoint(-400,0));
//        ofPopStyle();
//        ofPopMatrix();
//        lrClipFbo.end();
//    }
    if(lsCrossRects.bUpdate){
        cRFbo.begin();
        ofPushStyle();
        ofClear(255);
        cRects.update(mainW,mainH);
        cRects.draw();
        ofPopStyle();
        cRFbo.end();
    }
    if(lsFlower.bUpdate){
        flowerFbo.begin();
        ofPushMatrix();
        ofPushStyle();
        ofClear(255);
        ofTranslate(ofGetCenter());
        if(flower.bRotate){
            ofRotateZ(flower.deg); flower.deg+=0.5;
        }
        flower.draw();
        ofPopStyle();
        ofPopMatrix();
        flowerFbo.end();
    }

    //==============//
    //=== preFbo ===//
    //==============//
    for(int i=0; i<preFbo.size(); i++){
        preFbo[i].begin();
        {
            ofClear(255); // blackBG
            //--- UNDER_MASK ---//
            if (temp.getLS().bDraw[i]) {
                ofSetColor(255,temp.getLS().alpha);
                temp.render();
            }
            if(centerClip.getLS().bDraw[i]){
                ofSetColor(255,centerClip.getLS().alpha);
                centerClip.render();
            }
            if(lsCrossRects.bDraw[i]){
                ofSetColor(255, lsCrossRects.alpha);
                cRFbo.draw(0,0);
            }
            if(lsFlower.bDraw[i]){
                ofSetColor(255, lsFlower.alpha);
                flowerFbo.draw(0,0);
            }
            if(lrClip.getLS().bDraw[i]){
                ofSetColor(255, lrClip.getLS().alpha);
                lrClip.render();
            }
        }
        preFbo[i].end();
    }

    //--- PSBlend FBO ---//
    psBlend.begin();
//    ofClear(0,255);
    for(int i=0; i<PRE_SIZE;i++){
        ofSetColor(255,preAlpha[i]);
        if(bBlend[i]) preFbo[i].draw(0,0);
    }
    psBlend.end();

    blendFbo.begin();
    ofClear(255);
    ofSetColor(255);
    for(int i=0; i<PRE_SIZE; i++){
        if(!bBlend[i]) psBlend.draw(preFbo[i].getTexture(), blendMode);
    }
    blendFbo.end();

    //--- PostGlich Fbo ---//
    glitchFbo = blendFbo;
    postGlitch.generateFx();

    //--- Final FBO ---//
    finalFbo.begin();
    {
        ofClear(0,255);
        //--- BG draw ---//
        if(lsFlower.bBG) flowerFbo.draw(0,0);
        if(centerClip.getLS().bBG) centerClip.render();
        if(lsCrossRects.bBG) cRFbo.draw(0,0);
        if(temp.getLS().bBG) temp.render();
        if(lrClip.getLS().bBG) lrClip.render();

        glitchFbo.draw(0,0);

        //--- ON draw ---//
        if(lsFlower.bON) flowerFbo.draw(0,0);
        if(centerClip.getLS().bON) centerClip.render();
        if(lsCrossRects.bON) cRFbo.draw(0,0);
        if(temp.getLS().bON) temp.render();
        if(lrClip.getLS().bON) lrClip.render();

    }
    finalFbo.end();

}
//--------------------------------------------------------------
void vjGokko::draw(){
    ofColor color;
    color.setHsb(ofMap(ofGetFrameRate(), 0, 60, 0, 20,true)*5, 255, 255, 255);
    fpsMonitor->setColorFill(color);
}
//--------------------------------------------------------------
void vjGokko::drawOutput(ofEventArgs &args){
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    finalFbo.draw(0,0);
    ofPopStyle();
}

//--------------------------------------------------------------
void vjGokko::exit(){
    MONITOR->saveSettings("settings/UI.xml");
    temp.getUI()->saveSettings("settings/templateUI.xml");

    centerClip.getUI()->saveSettings("settings/centerClipUI.xml");
    crossRectsUI->saveSettings("settings/crossRectsUI.xml");
    flowerUI->saveSettings("settings/flowerUI.xml");
    lrClip.getUI()->saveSettings("settings/lrClipUI.xml");
    delete MONITOR;
//    delete tempUI;
    delete temp.getUI();
    delete centerClip.getUI();
    delete crossRectsUI;
    delete flowerUI;
    delete lrClip.getUI();
}
//--------------------------------------------------------------
void vjGokko::exitOutput(ofEventArgs &args){
    std::exit(0);
}

//--------------------------------------------------------------
void vjGokko::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.widget->getKind();

    if(name == "rectSpeed"){
        cRects.setRandomSpeed(cRects.minSpeed, cRects.maxSpeed);
    }else if(name == "rectNum"){
        cRects.setRects(cRects.num);
    }else if(name == "rectScale"){
        cRects.setRandomScale(cRects.minScale, cRects.maxScale);
    }else if(name == "antiqueFolder"){
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++){
            cout << "SELECTED: ("<<i<<") " << selected[i]->getName() << endl;
            //centerClip.clipFolder = selected[i]->getName();
            centerClip.setClip(clips[selected[i]->getName()]);
        }
    }else if(name == "FillFlower"){
        flower.bRadiation =false;
    }else if(name == "Radiation"){
         flower.bFill =false;
    }
    if(name == "LR_Scale"){
        //lrClip.scale=lClip.scale;
    }else if(name == "LR_Speed"){
        //lrClip.speed=lClip.speed;
    }
    
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void vjGokko::setupUI(){
    ofBackground(0);
//    themeNum = ofRandom(45);
    themeNum=43;
    cout<<"theme : "<<themeNum<<endl;
    setTitleUI();
    setPreUI();
    setMonitorUI();

    temp.setUI();
    ofAddListener(temp.getUI()->newGUIEvent, this, &vjGokko::guiEvent);

    centerClip.setCenterClipUI(aIFolderNames);
    lrClip.setLRClipUI();
    ofAddListener(centerClip.getUI()->newGUIEvent, this, &vjGokko::guiEvent);
    ofAddListener(lrClip.getUI()->newGUIEvent, this, &vjGokko::guiEvent);


    //--- setup layer UI ---//
    //setTempUI();
//    setCenterClipUI();
//    setLRClipUI();
    setCrossRectsUI();
    setFlowerUI();

    cout<<" - setupUI - "<<endl;
}

//--------------------------------------------------------------
void vjGokko::setTitleUI(){
    TITLE = new ofxUISuperCanvas(" vjGokko - version0.1 ",OFX_UI_FONT_LARGE);
    TITLE->setTheme(themeNum);
    TITLE->setPosition(1, 1);
    TITLE->setDrawPadding(false);
    TITLE->setWidth(300);
    TITLE->setHeight(180);
    TITLE->addTextArea("info", "this tool is for VJ 201804",OFX_UI_FONT_SMALL);
    TITLE->addSpacer();
    fpsMonitor = TITLE->addFPSSlider("FRAME RATE");
    TITLE->addToggle("RandomPS -> ", &bRandomPS);
    TITLE->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    blendLabel = TITLE->addLabel("Blend");
    blendLabel->setColorFill(ofColor::lightSteelBlue);
    TITLE->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    TITLE->addIntSlider("RandomSpan", 5, 60, &randomSpan);


    fpsMonitor->setMax(60);
    TITLE->autoSizeToFitWidgets();
}
//--------------------------------------------------------------
void vjGokko::setPreUI(){
    int w = 300;
    preUI.resize(PRE_SIZE);
    preFbo.resize(PRE_SIZE);
    for(int i=0;i<preUI.size();i++){
        preUI[i] = new ofxUISuperCanvas("Pre"+to_string(i),OFX_UI_FONT_SMALL);
        preUI[i]->setTheme(themeNum);
        preUI[i]->setDrawPadding(false);
        preUI[i]->setDrawPaddingOutline(false);
        preUI[i]->setPosition((i+1)*(w+10), 0);
        preUI[i]->setWidth(w);
        preUI[i]->addBaseDraws("Pre"+to_string(i), &preFbo[i],w,w/16*9);
        //preUI[i]->addSlider("alpha", 0, 255, &preAlpha[i]);
        preUI[i]->addToggle("PSBlend", &bBlend[i]);
        preUI[i]->autoSizeToFitWidgets();

    }
}
//--------------------------------------------------------------
void vjGokko::setMonitorUI(){
    int w=500;
    int h=w/16*9;
    MONITOR = new ofxUISuperCanvas("finalMonitor");
    MONITOR->setTheme(themeNum);
    MONITOR->setPosition(0, h);
    MONITOR->setWidth(w);
    MONITOR->setHeight(h);
    MONITOR->setDrawOutlineHighLight(true);
    MONITOR->addBaseDraws("Monitor", &finalFbo,w,h);
    MONITOR->autoSizeToFitWidgets();
}
//--------------------------------------------------------------
inline void vjGokko::setLayerUI(ofxUISuperCanvas *ui, ofFbo &fbo, layerSettings &ls){
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

////--------------------------------------------------------------
//void vjGokko::setTempUI(){
//    tempUI = new ofxUISuperCanvas("template");
//    setLayerUI(tempUI, tempFbo, lsTemp);
//
////    tempUI->addSlider("Scale", 0, 30, &temp.scale);
////    tempUI->addSlider("Speed", 0, 15, &temp.speed);
////    tempUI->addSlider("Offset", 0, 100, &temp.offset);
//
//    tempUI->autoSizeToFitWidgets();
//    ofAddListener(tempUI->newGUIEvent, this, &vjGokko::guiEvent);
//}
////--------------------------------------------------------------
//void vjGokko::setCenterClipUI(){
//    centerClipUI = new ofxUISuperCanvas("AntiqueImages");
//    setLayerUI(centerClipUI, centerClipFbo, lsCenterClip);
//
//    centerClipUI->addSlider("Sclae", 1, 3, &centerClip.scale);
//    centerClipUI->addIntSlider("Speed", 1, 30, &centerClip.speed);
//    ofxUIDropDownList *ddl = (ofxUIDropDownList *)centerClipUI->addDropDownList("antiqueFolder", aIFolderNames);
//    ddl->setSingleSelected(1);
//    ddl->setAutoClose(true);
//    ddl->setShowCurrentSelected(true);
//    centerClipUI->autoSizeToFitWidgets();
//    ofAddListener(centerClipUI->newGUIEvent, this, &vjGokko::guiEvent);
//}
////--------------------------------------------------------------
//void vjGokko::setLRClipUI(){
//    lrClipUI = new ofxUISuperCanvas("LR clip");
//    setLayerUI(lrClipUI, lrClipFbo, lsLRClip);
//
//    lrClipUI->addSlider("LR_Scale", 1, 3, &lClip.scale);
//    lrClipUI->addIntSlider("LR_Speed", 1, 30, &lClip.speed);
//    lrClipUI->autoSizeToFitWidgets();
//    ofAddListener(lrClipUI->newGUIEvent, this, &vjGokko::guiEvent);
//}
//--------------------------------------------------------------
void vjGokko::setCrossRectsUI(){
    crossRectsUI = new ofxUISuperCanvas("crossRects");
    setLayerUI(crossRectsUI, cRFbo, lsCrossRects);

    crossRectsUI->addIntSlider("rectNum", 0, 1000, &cRects.num);
    crossRectsUI->addRangeSlider("rectSpeed", 1, 100, &cRects.minSpeed, &cRects.maxSpeed);
    crossRectsUI->addRangeSlider("rectScale", 0, 3, &cRects.minScale, &cRects.maxScale);
    crossRectsUI->autoSizeToFitWidgets();
    ofAddListener(crossRectsUI->newGUIEvent, this, &vjGokko::guiEvent);
}
//--------------------------------------------------------------
void vjGokko::setFlowerUI(){
    flowerUI = new ofxUISuperCanvas("flower");
    setLayerUI(flowerUI, flowerFbo, lsFlower);

    flowerUI->addToggle("fftFlowers", &flower.bFlowers);
    flowerUI->addSlider("Ratio",0,50000,&flower.ratio);
    flowerUI->addSlider("Offset", 0, 1000, &flower.offset);
    flowerUI->addIntSlider("Interval", 10, 100, &flower.interval);
    flowerUI->addSlider("LineWidth", 1, 30, &flower.lineWidth);
    flowerUI->addToggle("Rotate", &flower.bRotate);
    flowerUI->addToggle("FillFlower", &flower.bFill);
    flowerUI->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    flowerUI->addToggle("Radiation", &flower.bRadiation);
    flowerUI->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    flowerUI->addToggle("Curve", &flower.bCurve);
    flowerUI->addIntSlider("Kaleido", 1, 5, &flower.kaleidoNum);
    flowerUI->autoSizeToFitWidgets();
    ofAddListener(flowerUI->newGUIEvent, this, &vjGokko::guiEvent);

}

//--------------------------------------------------------------
void vjGokko::keyPressed(ofKeyEventArgs &key){
    switch (key.key) {
        case OF_KEY_RETURN:{
            crossRectsUI->toggleVisible();
            centerClip.getUI()->toggleVisible();
//            tempUI->toggleVisible();
            temp.getUI()->toggleVisible();
            lrClip.getUI()->toggleVisible();
            flowerUI->toggleVisible();
            preUI[0]->toggleVisible();
            preUI[1]->toggleVisible();
            preUI[2]->toggleVisible();
        }break;
            //--- PSBlend Mode --//
        case OF_KEY_LEFT: blendMode<=0  ? blendMode=24 : blendMode--; break;
        case OF_KEY_RIGHT: blendMode>=24 ? blendMode= 0 : blendMode++; break;
            //--- PostGlitch --//
        case '1':postGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , true);break;
        case '2':postGlitch.setFx(OFXPOSTGLITCH_GLOW           , true);break;
        case '3':postGlitch.setFx(OFXPOSTGLITCH_SHAKER         , true);break;
        case '4':postGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER      , true);break;
        case '5':postGlitch.setFx(OFXPOSTGLITCH_TWIST          , true);break;
        case '6':postGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , true);break;
        case '7':postGlitch.setFx(OFXPOSTGLITCH_NOISE          , true);break;
        case '8':postGlitch.setFx(OFXPOSTGLITCH_SLITSCAN       , true);break;
        case '9':postGlitch.setFx(OFXPOSTGLITCH_SWELL          , true);break;
        case '0':postGlitch.setFx(OFXPOSTGLITCH_INVERT         , true);break;
        case 'q':postGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);break;
        case 'w':postGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE   , true);break;
        case 'e':postGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , true);break;
        case 'r':postGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE  , true);break;
        case 't':postGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT  , true);break;
        case 'y':postGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT   , true);break;
        case 'u':postGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT , true);break;
    }

}
//--------------------------------------------------------------
void vjGokko::keyReleased(ofKeyEventArgs &key){
    switch (key.key) {
            //--- PostGlitch --//
        case '1':postGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE    , false);break;
        case '2':postGlitch.setFx(OFXPOSTGLITCH_GLOW           , false);break;
        case '3':postGlitch.setFx(OFXPOSTGLITCH_SHAKER         , false);break;
        case '4':postGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER      , false);break;
        case '5':postGlitch.setFx(OFXPOSTGLITCH_TWIST          , false);break;
        case '6':postGlitch.setFx(OFXPOSTGLITCH_OUTLINE        , false);break;
        case '7':postGlitch.setFx(OFXPOSTGLITCH_NOISE          , false);break;
        case '8':postGlitch.setFx(OFXPOSTGLITCH_SLITSCAN       , false);break;
        case '9':postGlitch.setFx(OFXPOSTGLITCH_SWELL          , false);break;
        case '0':postGlitch.setFx(OFXPOSTGLITCH_INVERT         , false);break;
        case 'q':postGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);break;
        case 'w':postGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE   , false);break;
        case 'e':postGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE    , false);break;
        case 'r':postGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE  , false);break;
        case 't':postGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT  , false);break;
        case 'y':postGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT   , false);break;
        case 'u':postGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT , false);break;
    }

}


