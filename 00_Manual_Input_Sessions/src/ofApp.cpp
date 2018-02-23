#include "ofApp.h"


void ofApp::setup()
{
    // The `hands.mp4` video used in these examples can be downloaded
    // [here](http://christopherbaker.net/dropbox/hands.mp4).
    // See the README.md in the bin/data directory.
    video.load("hands.mp4");
    video.play();

    // Load the sounds.
    // See the README.md in the bin/data directory.
    ofDirectory directory("");
    directory.allowExt("wav");
    for (auto file: directory)
    {
        sounds.push_back(ofSoundPlayer());
        sounds.back().load(file);
        sounds.back().setMultiPlay(true);
        sounds.back().setLoop(false);
    }

    // Set up the gui.
    gui.setup();
    gui.add(binaryProcessor.parameters());
    gui.add(contourProcessor.parameters());

    binaryProcessor.blurLevel = 17;
    binaryProcessor.threshold = 200;
    binaryProcessor.invertThreshold = true;

    // Set up the physics.
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.enableEvents();
    box2d.createGround();
    box2d.setFPS(30.0);

    // Set up the contour processor listeners.
    contourUpdatedListener = contourProcessor.onContourUpdated.newListener(this, &ofApp::onContourUpdated);

    // Set up the contact point listeners.
    contactStartEventsListener = box2d.contactStartEvents.newListener(this, &ofApp::onContactStart);
}


void ofApp::update()
{
    // Update the video.
    video.update();

    // If we have a new frame, process it.
    if (video.isFrameNew())
    {
        binaryProcessor.update(video.getPixels());
        contourProcessor.update(binaryProcessor.getPixels());
    }

    // Update the Box2d world.
    box2d.update();

    // Remove the negative space if it is dead.
    ofRemove(negativeSpaces, [&](std::shared_ptr<NegativeSpace> a) { return a->isDead(); });

    // Clear the current collision polygons.
    collisionPolygons.clear();
}


void ofApp::draw()
{
    ofFill();
    ofSetColor(255);
    video.draw(640, 0);

    binaryProcessor.draw(0, 0);
    contourProcessor.draw();

    ofFill();
    ofSetColor(ofColor::red);

    // Draw the negative spaces.
    for (auto negativeSpace: negativeSpaces)
    {
        negativeSpace->draw();
    }

    // Draw the gui.
    gui.draw();
}


void ofApp::onContourUpdated(const Contour& contour)
{
    // This function is called when an existing tracker is updated by the
    // tracker. `lastSeen` is the number of frames that have passes since the
    // contour was last tracked. Thus if `lastSeen == 1`, then we know that
    // the track was just lost. The `trackStrength` is also a useful member
    // where a trackStrength of 1 means it is currently being tracked and a
    // trackStrength of 0 means it is being removed. Values in between occur
    // as the tracker is "forgetting" about the contour.

    if (contour.isHole)
    {
        if (contour.lastSeen == 1)
        {
            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - Adjust the physics properties to be more similar to the NEGDROP phase of the Manual Input Sessions.
            //     - Choose a sound that changes with the area of the contour.
            //          - Bigger areas should be attached to "bigger" sounds.
            //          - Find the area in `contour.area`.
            ////////////////////////////////////////////////////////////////////////

            auto negSpace = std::make_shared<NegativeSpace>();
            negSpace->shape.addVertices(contour.polyline);
            negSpace->shape.setPhysics(1.0, 0.3, 0.3);
            negSpace->shape.triangulatePoly();
            negSpace->shape.create(box2d.getWorld());
            negSpace->shape.setData(negSpace.get());
            negSpace->soundPlayerIndex = ofRandom(sounds.size());
            negativeSpaces.push_back(negSpace);
        }
    }
    else
    {
        ////////////////////////////////////////////////////////////////////////
        /// YOUR WORK HERE ! ///////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //
        //     - Create a collision polygon for a non-hole contour with an age greater than 15.
        //     - Add it to the collisionPolygons vector.
        //
        ////////////////////////////////////////////////////////////////////////
    }
}


void ofApp::onContactStart(ofxBox2dContactArgs& contact)
{
    // We receive contact events here.
    // Each contact has two pointers, a and b.

    // First we make sure that both pointers are valid.
    if (contact.a && contact.b)
    {
        // Next we attempt to recover the user data we saved previously.
        // When creating our NegativeSpace we did it like this:
        //     negSpace->shape.setData(negSpace.get());
        // This set a pointer back to the NegativeShape object so we can access
        // it here.
        NegativeSpace* a = reinterpret_cast<NegativeSpace*>(contact.a->GetBody()->GetUserData());
        NegativeSpace* b = reinterpret_cast<NegativeSpace*>(contact.b->GetBody()->GetUserData());

        // If either of the bodies is _not_ a negative space, the pointer from
        // reinterpret_cast will be nullptr, indicating that it isn't one of our
        // Negative spaces. It's probably the ground or one of our collision
        // polygons.

        if (a && b)
        {
            // Both are Negative spaces, so skip.

            ////////////////////////////////////////////////////////////////////////
            /// YOUR WORK HERE ! ///////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////
            //
            //     - Play a different sound if two negative spaces collide.
            //
            ////////////////////////////////////////////////////////////////////////

        }
        else if (a)
        {
            // a is a negative space.
            sounds[a->soundPlayerIndex].play();
        }
        else if (b)
        {
            // b is a negative space.
            sounds[b->soundPlayerIndex].play();
        }
    }
}

