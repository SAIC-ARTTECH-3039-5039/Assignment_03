#include "ofApp.h"


int main()
{
    ofSetupOpenGL(1280, 480, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
