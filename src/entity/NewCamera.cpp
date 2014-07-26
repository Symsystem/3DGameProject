//
//  NewCamera.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 23/07/14.
//
//

#include "NewCamera.h"

NewCamera::NewCamera(const Ogre::String &name, Ogre::SceneManager *sceneMgr, Ogre::SceneNode *nodeAttached, Object3D *obj) :
mNodeAttached(nodeAttached), mObject3DAttached(obj), mNodeCamera(0), mNodeGoal(0), mNodePivot(0), mMouseBoth(false), mMouseLeft(false), mMouseRight(false)
{
    mSceneMgr = sceneMgr;
    mCamera = mSceneMgr->createCamera(name);
    
    setupCamera();
}

NewCamera::~NewCamera()
{
}

void NewCamera::setupCamera()
{
    // Noeud "pivot" pour la camera
    mNodePivot = mNodeAttached->createChildSceneNode("nodePivotCam");
    // Place le noeud pivot au même endroit que le personnage
    mNodePivot->setPosition(mNodeAttached->getPosition() + Ogre::Vector3::UNIT_Y * HEIGHT_CAMERA);
    
    // L'endroit où la caméra devrait être bientôt. Il tourne autour du pivot
    mNodeGoal = mNodePivot->createChildSceneNode("nodeGoalCam", Ogre::Vector3(0, 0, 15));
    // Endroit où se trouve actuellement la camera
    mNodeCamera = mNodeAttached->createChildSceneNode("nodeCamera");
    mNodeCamera->setPosition(mNodePivot->getPosition() + mNodeGoal->getPosition());
    
    mNodePivot->setFixedYawAxis(true);
    mNodeGoal->setFixedYawAxis(true);
    mNodeCamera->setFixedYawAxis(true);
    
    // Distance la plus proche à afficher
	mCamera->setNearClipDistance(1);
	// Distance la plus loin à afficher
	mCamera->setFarClipDistance(20000);
	// On vérifie si l'ordinateur en est capable, si c'est le cas, on fixe la distance de vue à l'infini.
	/*if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
	    mCamera->setFarClipDistance(0);*/
    
    mNodeCamera->attachObject(mCamera);
    
    mPivotPitch = 0;
}

void NewCamera::updateCamera(const Ogre::Real deltaTime)
{
    // Déplace la caméra lentement jusqu'au Goal
    Ogre::Vector3 goalOffset = mNodeGoal->_getDerivedPosition() - mNodeCamera->_getDerivedPosition();
    mNodeCamera->translate(goalOffset * deltaTime * SPEED_CAMERA);
    // La caméra regarde toujours vers le pivot
    mNodeCamera->lookAt(mNodePivot->_getDerivedPosition(), Ogre::Node::TS_WORLD);
    
    // Lorsqu'on clique sur les deux boutons de la souris
    if (mMouseLeft && mMouseRight && mObject3DAttached->isMovable())
    {
        if (!mObject3DAttached->isMoving()) mObject3DAttached->setIsMoving(true);
        mObject3DAttached->setOrientation(mNodeCamera->getOrientation());
        mMouseBoth = true;
    }
}

void NewCamera::updateCameraGoal(const Ogre::Real deltaYaw, const Ogre::Real deltaPitch, const Ogre::Real deltaZoom)
{
    mNodePivot->yaw(Ogre::Degree(deltaYaw), Ogre::Node::TS_WORLD);
    
    // bound the pitch
    if (!(mPivotPitch + deltaPitch > MAX_ANGLE && deltaPitch > 0) &&
        !(mPivotPitch + deltaPitch < MIN_ANGLE && deltaPitch < 0))
    {
        mNodePivot->pitch(Ogre::Degree(deltaPitch), Ogre::Node::TS_LOCAL);
        mPivotPitch += deltaPitch;
    }
    
    Ogre::Real dist = mNodeGoal->_getDerivedPosition().distance(mNodePivot->_getDerivedPosition());
    Ogre::Real distChange = deltaZoom * dist;
    
    // bound the zoom
    if (!(dist + distChange < MIN_ZOOM && distChange < 0) &&
        !(dist + distChange > MAX_ZOOM && distChange > 0))
    {
        mNodeGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
    }
}

void NewCamera::keyDown(const OIS::KeyEvent &e)
{
    
}

void NewCamera::keyUp(const OIS::KeyEvent &e)
{
    
}

void NewCamera::mouseMoved(const OIS::MouseEvent &e)
{
    if (mMouseLeft)
        updateCameraGoal(-0.05f * e.state.X.rel, -0.05f * e.state.Y.rel, 0);
    updateCameraGoal(0, 0, -0.0005f * e.state.Z.rel);
}

void NewCamera::mouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        mMouseLeft = true;
    }
    else if (id == OIS::MB_Right)
    {
        mMouseRight = true;
    }
}

void NewCamera::mouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        mMouseLeft = false;
    }
    else if (id == OIS::MB_Right)
    {
        mMouseRight = false;
    }
    if (mMouseBoth)
    {
        mMouseBoth = false;
        if (mObject3DAttached->isMoving()) mObject3DAttached->setIsMoving(false);
    }
}

Ogre::Camera* NewCamera::getOgreCamera(){
    return mCamera;
}

Ogre::SceneNode* NewCamera::getNodeCamera()
{
    return mNodeCamera;
}