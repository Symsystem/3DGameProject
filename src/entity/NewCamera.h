//
//  NewCamera.h
//  3DGameProject
//
//  Created by Syméon del Marmol on 23/07/14.
//
//

#ifndef ___DGameProject__NewCamera__
#define ___DGameProject__NewCamera__

#define HEIGHT_CAMERA 500
#define SPEED_CAMERA 9
#define MAX_ZOOM 25
#define MIN_ZOOM 8
#define MAX_ANGLE 25
#define MIN_ANGLE -60

#include <iostream>
#include <Ogre.h>
#include <OIS.h>
#include "Object3D.h"

class NewCamera : public Ogre::FrameListener {
    
public:
    
    NewCamera(const Ogre::String &name, Ogre::SceneManager *sceneMgr, Ogre::SceneNode *nodeAttached, Object3D *obj);
    ~NewCamera();
    
    void setupCamera();
    void updateCamera(const Ogre::Real deltaTime);
    void updateCameraGoal(const Ogre::Real deltaYaw, const Ogre::Real deltaPitch, const Ogre::Real deltaZoom);
    
    void keyDown(const OIS::KeyEvent &e);
    void keyUp(const OIS::KeyEvent &e);
    void mouseMoved(const OIS::MouseEvent &e);
    void mouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void mouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    
    Ogre::Camera* getOgreCamera();
    Ogre::SceneNode* getNodeCamera();
    
private:
    
    Ogre::SceneManager *mSceneMgr;
    Ogre::Camera *mCamera;
    
    Object3D *mObject3DAttached;
    Ogre::SceneNode *mNodeAttached;
    Ogre::SceneNode *mNodePivot;
    Ogre::SceneNode *mNodeGoal;
    Ogre::SceneNode *mNodeCamera;
    
    Ogre::Real mPivotPitch;
    
    bool mMouseBoth, mMouseLeft, mMouseRight;
    
};

#endif /* defined(___DGameProject__NewCamera__) */
