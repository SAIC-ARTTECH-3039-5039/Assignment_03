#pragma once


#include "ofMain.h"
#include "ofxARTTECH3039.h"
#include "ofxBox2d.h"


class NegativeSpace
{
public:
    void draw()
    {
        shape.draw();
    }

    bool isDead()
    {
        // The shape is dead if it is off the screen.
        return !ofRectangle(0, 0, ofGetWidth(), ofGetHeight()).inside(shape.getPosition());
    }

    ofxBox2dPolygon shape;

    std::size_t soundPlayerIndex = 0;

};


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void onContourAdded(const Contour& contour);
    void onContourUpdated(const Contour& contour);
    void onContourRemoved(const Contour& contour);

    void onContactStart(ofxBox2dContactArgs& contact);

    ofVideoPlayer video;

    BinaryPreprocessor binaryProcessor;
    ContourProcessor contourProcessor;

    ofxPanel gui;

    // Box2D
    ofxBox2d box2d;

    std::vector<std::shared_ptr<NegativeSpace>> negativeSpaces;
    std::vector<std::shared_ptr<ofxBox2dPolygon>> collisionPolygons;


    std::vector<ofSoundPlayer> sounds;

    ofEventListener contourUpdatedListener;
    ofEventListener contactStartEventsListener;

};



