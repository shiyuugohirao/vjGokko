
//
//  videoPlayer.h
//  vjGokko
//
//  Created by shugohirao on 2018/07/11.
//
#pragma once

class videoPlayer {
public:

    string videoName;
    ofVideoPlayer video;
    ofPoint pos;
    float scale;
    float speed;
    bool bRandomRes = true;
    float adjustScale;

    videoPlayer(){
        ofAddListener(ofEvents().update, this, &videoPlayer::update);
    }
    ~videoPlayer(){
        ofRemoveListener(ofEvents().update, this, &videoPlayer::update);
    }


    void init(ofVideoPlayer &v){
        video = v;
        cout<<"init video : "<<video.getMoviePath()<<endl;
        video.setVolume(0);
        video.setLoopState(OF_LOOP_NORMAL);
        float ratioX = WIDTH / video.getWidth();
        float ratioY = HEIGHT / video.getHeight();
        adjustScale = max(ratioX, ratioY);
        if(!video.isPlaying()){ video.play(); }
    }

    void update(ofEventArgs &a){
        if(video.isPlaying()){ video.update(); }
    }

    void start(){ video.play(); }
    void stop(){ video.stop();}

    void render(){
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(WIDTH/2, HEIGHT/2);
        ofScale(adjustScale,adjustScale);
        video.draw(-video.getWidth()/2, -video.getHeight()/2);
        //cout<<"draw video : "<<video.getMoviePath()<<endl;
        ofPopStyle();
        ofPopMatrix();
    }
};
