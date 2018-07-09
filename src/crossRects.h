//
//  crossRects.h
//  vjGokko
//
//  Created by shugohirao on 2018/04/08.
//

class rect {
private:
    ofFbo fbo;
    ofColor color;
    ofPoint pos;
    ofVec2f dir;
    float speed;
    float scale;

public:
    rect(){};
    ~rect(){};
    rect(ofFbo f, float minSpeed, float maxSpeed, float minScale, float maxScale)
    :fbo(f)
    ,pos(ofPoint(ofRandom(1920),ofRandom(1080)))
    ,speed(ofRandom(minSpeed, maxSpeed))
    ,scale(ofRandom(minScale, maxScale)){
        color.setHsb(ofRandom(255), 100, 255);
        switch((int)ofRandom(4)){
            case 0: dir = ofVec2f(1,0);   break;
            case 1: dir = ofVec2f(-1,0);  break;
            case 2: dir = ofVec2f(0,1);   break;
            case 3: dir = ofVec2f(0,-1);  break;
        }
        fbo.setAnchorPercent(0.5, 0.5);
    };

    void setRandomSpeed(float min,float max){
        speed = ofRandom(min,max);
    }
    void setRandomScale(float min,float max){
        scale = ofRandom(min,max);
    }

    void update(int maxX, int maxY){
        pos += dir*speed;
        if(pos.x<-100){
            pos=ofPoint(maxX+50,ofRandom(maxY));
        }else if(pos.x>maxX+100){
            pos=ofPoint(-50,ofRandom(maxY));
        }else if(pos.y<-100){
            pos=ofPoint(ofRandom(maxX),maxY+50);
        }else if(pos.y>maxY+100){
            pos=ofPoint(ofRandom(maxX),-50);
        }
    }
    void draw(){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(pos);
        ofScale(scale,scale);
        ofSetColor(color);
        fbo.draw(0,0);
        ofPopMatrix();
        ofPopStyle();
    }
};

class crossRects {
private:
    vector<rect> rects;
    vector<ofFbo> fbos;

public:
    crossRects(){};

    void setup(vector<ofFbo> &_fbos, int num, float s=ofRandom(0.3,0.6)){
        rects.resize(num);
        fbos = _fbos;
        addRects(num);
    }

    void update(int maxX, int maxY){

        for(auto &r:rects){
            r.update(maxX,maxY);
        }
    }

    void draw(){
        for(auto &r:rects){
            r.draw();
        }
    }

    void setRandomSpeed(float min,float max){
        static float tempMin;
        static float tempMax;
        if(tempMin==min && tempMax==max) return;
        for(auto &r:rects){
            r.setRandomSpeed(min, max);
        }
        tempMin = min;
        tempMax = max;
    }
    void setRandomScale(float min,float max){
        static float tempMin;
        static float tempMax;
        if(tempMin==min && tempMax==max) return;
        for(auto &r:rects){
            r.setRandomScale(min, max);
        }
        tempMin = min;
        tempMax = max;
    }

    void setRects(int n){
        int temp = rects.size();
        if(n > temp){
            addRects(n-temp);
        }else if(n < temp){
            deleteRects(temp-n);
        }
    }

    void addRects(int n=1){
        for(int i=0;i<n;i++){
            rect r(fbos[ofRandom(fbos.size())],minSpeed, maxSpeed, minScale, maxScale);
            rects.push_back(r);
        }
    }
    void deleteRects(int n=1){
        for(int i=0;i<n;i++){
            if(fbos.size()<=0)return;
            rects.pop_back();
        }
    }

    float minSpeed, maxSpeed;
    float minScale, maxScale;
    int num = 100;
};
