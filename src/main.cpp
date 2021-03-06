#include "ofMain.h"
#include "vjGokko.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settings;
    //========== GUI WINDOW ==========//
    settings.width = 1280;
    settings.height = 720+180;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;

    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);

    //========== OUTPUT WINDOW ==========//
    settings.width = 1920;
    settings.height = 1080;
    settings.setPosition(ofVec2f(0,-1080));
    settings.resizable = true;
    settings.shareContextWith = guiWindow;
    shared_ptr<ofAppBaseWindow> outputWindow = ofCreateWindow(settings);


    shared_ptr<vjGokko> mainApp(new vjGokko);
    ofAddListener(outputWindow->events().draw,mainApp.get(),&vjGokko::drawOutput);
    ofAddListener(guiWindow->events().exit,mainApp.get(),&vjGokko::exitOutput);
    mainApp->setupOutput();
    ofRunApp(guiWindow, mainApp);
    ofRunMainLoop();

}

