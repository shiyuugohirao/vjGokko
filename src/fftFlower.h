//
//  fftFlower.h
//  vjTool
//
//  Created by shugohirao on 2018/04/09.
//
#pragma once
#include "ofxFft.h"
#include "ofxProcessFFT.h"

#define MIN_FREQ 0
#define MAX_FREQ 16000

class FFT: public ofBaseApp {
protected:
    ofxFft *fft;

    ofSoundStream stream;
    ofMutex soundMutex;
    vector<float> rawBuffer, drawBins, audioBins;

public:
    vector<float> bufNow;

    void setupFft(int bufferSize = 1024) {
        int audioBufferSize = 256;
        if(bufferSize < audioBufferSize) {
            ofLogWarning("ofxEasyFft") << "bufferSize (" << bufferSize << ") less than audioBufferSize (" << audioBufferSize << "), using " << audioBufferSize;
            bufferSize = audioBufferSize;
        }
        fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_BASIC);

        bufNow.assign(MAX_FREQ, 0.0);
        rawBuffer.resize(bufferSize);

        drawBins.resize(fft->getBinSize());// 512/2+1

        stream.printDeviceList();
        stream.setDeviceID(0);
        stream.setup(this,0, 1, 44100, audioBufferSize, 4);

    }

    float getBinFromFrequency(float freq){

    }

    //-------------------------------------------
    void audioIn(float* input, int bufferSize, int nChannels) {
        memcpy(&rawBuffer[0], input, sizeof(float) * bufferSize);
        //###cout<<"bufferSize"<<bufferSize<<endl;
        fft->setSignal(&rawBuffer[0]);

        vector<float> buffer;
        buffer.resize(bufferSize);
        int count = 0;
        for(int i=0; i<10000; i++){  // ## toDo  fix !!!
            buffer.push_back(fft->getAmplitudeAtFrequency(i,44100));
//            buffer.push_back(fft->getAmplitudeAtBin(i));
        }

        soundMutex.lock();
        bufNow = buffer;
        soundMutex.unlock();
    }
    
};


class fftFlower :public FFT{
public:
    void draw(ofPoint pos = ofPoint(0,0)){
        if(bFlowers){
            drawFftFlowers(pos);
            return;
        }
        drawFftFlower(pos);
    }

    void drawFftFlower(ofPoint pos){
        drawFftFlower(pos,bufNow);
    }
    void drawFftFlower(ofPoint pos, vector<float>& buffer) {
        int num = buffer.size();
        float radian = 0;
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(pos);
        ofSetLineWidth(lineWidth);
        bFill ? ofFill() : ofNoFill();
        ofBeginShape();
        for(int i=0; i<num; i+=interval){
            int index=i;
            if(kaleidoNum>1){
                int n = num/kaleidoNum;
                index = i%n;
            }
            float radius = buffer[index];
            calcV(radius, radian);
            radian+= TWO_PI/(num/interval);
        }
        if(!bRadiation) calcV(buffer[0], 0);
        ofEndShape();
        ofPopStyle();
        ofPopMatrix();
    }

    void drawFftFlowers(ofPoint pos){
        drawFftFlowers(pos,bufNow);
    }
    void drawFftFlowers(ofPoint pos,vector<float>& buffer) {
        // superLow  #ignore    // 0 -32
        vector<float> bufL;     // 33-125
        vector<float> bufLM;    // 126-250
        vector<float> bufM;     // 251-1000
        vector<float> bufHM;    // 1001-4000
        vector<float> bufH;     // 4001-8000
        vector<float> bufHH;    // 8001-16000
        // superHigh  #ignore   // 16001-

        bufL.resize(abs(32-125));
        bufLM.resize(abs(126-250));
        bufM.resize(abs(251-1000));
        bufHM.resize(abs(1001-4000));
        bufH.resize(abs(4001-8000));
        bufHH.resize(abs(8001-16000));
        copy(buffer.begin()+32, buffer.begin()+125,bufL.begin());
        copy(buffer.begin()+126, buffer.begin()+250,bufLM.begin());
        copy(buffer.begin()+251, buffer.begin()+1000,bufM.begin());
        copy(buffer.begin()+1001, buffer.begin()+4000,bufHM.begin());
        copy(buffer.begin()+4001, buffer.begin()+8000,bufH.begin());
        copy(buffer.begin()+8001, buffer.begin()+16000,bufHH.begin());

        ofPushStyle();
        int a = 200;
        if(bShowL){
            ofSetHsbColor(32*255/16000, 255, 255, a);
            drawFftFlower(pos, bufL);
        }
        if(bShowLM){
            ofSetHsbColor(125*255/16000, 255, 255, a);
            drawFftFlower(pos, bufLM);
        }
        if(bShowM){
            ofSetHsbColor(250*255/16000, 255, 255, a);
            drawFftFlower(pos, bufM);
        }
        if(bShowHM){
            ofSetHsbColor(1000*255/16000, 255, 255, a);
            drawFftFlower(pos, bufHM);
        }
        if(bShowH){
            ofSetHsbColor(4000*255/16000, 255, 255, a);
            drawFftFlower(pos, bufH);
        }
        if(bShowHH){
            ofSetHsbColor(8000*255/16000, 255, 255, a);
            drawFftFlower(pos, bufHH);
        }
        ofPopStyle();

    }

    //---- Parameters ----------------------------------------------------------
    bool bFlowers = true;
    float ratio=500;
    float offset=100;
    int interval=1;
    float lineWidth=3;
    bool bCurve = false;
    bool bFill = true;
    bool bRadiation = false;
    bool bRotate = true;
    float deg=0;
    int kaleidoNum = 1;

    bool bShowL = true;     // 33-125
    bool bShowLM = true;    // 126-250
    bool bShowM = true;     // 251-1000
    bool bShowHM = true;    // 1001-4000
    bool bShowH = true;     // 4001-8000
    bool bShowHH = true;    // 8001-16000

    //--------------------------------------------------------------
private:
    void calcV(float radius, float radian){
        float x = cos(radian) * (radius*ratio + offset);
        float y = sin(radian) * (radius*ratio + offset);
        if(bRadiation){
            ofDrawLine(0,0,x,y);
            return;
        }
        if(bCurve){
            ofCurveVertex(x, y);
            return;
        }
        ofVertex(x,y);
    }

};
