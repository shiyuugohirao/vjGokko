
//
//  TEMPLATE.h
//  vjGokko
//
//  Created by shugohirao on 2018/04/14.
//
#pragma once

class TEMPLATE {
public:
    ofPoint pos;
    float scale;
    float speed;
    float r;
    float deg;
    float offset;
    float cycle;
    bool bRandomRes = true;


    TEMPLATE(){}
    ~TEMPLATE(){}

    void setup(ofPoint _pos,float _r=100){
        pos = _pos;
        r = _r;
        offset = 1.0;
        deg=0;
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


};
