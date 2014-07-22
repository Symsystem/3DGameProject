/*
 * InputListener.cpp
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#include "InputListener.h"

InputListener::InputListener(Ogre::RenderWindow *wnd, Ogre::Camera *camera, Ogre::SceneNode *node, Ogre::AnimationState *animationState)
{
	mWindow = wnd;
	mCamera = camera;

	mContinuer = true;
	mClick = false;
    
    mEvt = 0;
    
    mMouseLeft = false;
    mMouseBoth = false;
	mNodePersonnage = node;
	mAnimationState = animationState;
	mAngleMouseX = 0.0;
    mAngleMouseY = 0.0;
	mMouvement = Ogre::Vector3::ZERO;
	mVitesse = 100;
	mVitesseRotation = 0.2;

	// démarrage d'OIS
	startOIS();
}
InputListener::~InputListener()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
}

bool InputListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

    mEvt = &evt;

	if (mKeyboard)
		mKeyboard->capture();
	if (mMouse)
		mMouse->capture();

    
    // Rotation du personnage et/ou caméra
    if (mMouse->getMouseState().buttonDown(OIS::MB_Left))
    {
    	mMouseLeft = true;
    	mNodePersonnage->getChild("perso")->rotate(Ogre::Quaternion(mAnglePerso * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_LOCAL);
        // Si on appuye sur les deux boutons de la souris en même temps
        if (mMouse->getMouseState().buttonDown(OIS::MB_Right)){
			mMouseBoth = true;
			Ogre::Quaternion quat(0,0,0,0);
			quat = mNodePersonnage->getChild("nodeCamera")->getOrientation();
			quat.x = 0;
			quat.z = 0;
			mMouvement.z = 1;
			mNodePersonnage->getChild("perso")->setOrientation(quat);
		}
    }
    else
    {
    	if (mKeyboard->isKeyDown(OIS::KC_Z))
    	{
    		Ogre::Real yawAngle = mNodePersonnage->getChild("perso")->getOrientation().y - mNodePersonnage->getChild("nodeCamera")->getOrientation().y;
    		if (yawAngle <= 0.02 && yawAngle >= -0.02)
    			mNodePersonnage->getChild("nodeCamera")->yaw(Ogre::Radian(yawAngle), Ogre::Node::TS_PARENT);
    		else
    			mNodePersonnage->getChild("nodeCamera")->yaw(Ogre::Radian(yawAngle) * 4 * mEvt->timeSinceLastFrame, Ogre::Node::TS_PARENT);
    	}
    	mMouseLeft = false;
        mNodePersonnage->rotate(Ogre::Quaternion(mAnglePerso * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_LOCAL);
        /*if (mNodePersonnage->getChild("nodeCamera")->getOrientation() != mNodePersonnage->getChild("perso")->getOrientation() && mKeyboard->isKeyDown(OIS::KC_W))
        {
            Ogre::Radian angle(2);
            mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(angle * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_PARENT);
        }*/
    }

    // Déplacement du personnage
    Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
    deplacement = mMouvement * mVitesse * evt.timeSinceLastFrame / 4;
    mNodePersonnage->translate(mNodePersonnage->getChild("perso")->getOrientation() * mNodePersonnage->getOrientation() * deplacement);
	if (mClick) {
		mAnimationState->addTime(evt.timeSinceLastFrame);
	}

	return mContinuer;
}

void InputListener::startOIS()
{
	// Ajout d'un message dans le log
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	// Ajout du paramètre caractéristique de la RenderWindow dans la liste
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	// Instanciation du gestionnaire d'entrée
	mInputManager = OIS::InputManager::createInputSystem(pl);

	// Creation des objets pour la gestion de la souris et du clavier (sans buffer -> sinon mettre à true)
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));

	// Car utilisation du buffer
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	// Permet de récupérer la taille de la fenêtre
	windowResized(mWindow);
	// Enregistre notre objet InputListener en tant que WindowEventListener pour cette fenêtre
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

void InputListener::windowResized(Ogre::RenderWindow* wnd)
{
    unsigned int width, height, depth;
    int left, top;
    // récupération de la géométrie de la fenêtre -> redimensionnement, etc
    wnd->getMetrics(width, height, depth, left, top);

    // Récupération de la position relative de la souris
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void InputListener::windowClosed(Ogre::RenderWindow* wnd)
{
    if( wnd == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
bool InputListener::mouseMoved(const OIS::MouseEvent &e)
{
    if (mMouseLeft)
    {
	mNodePersonnage->getChild("nodeCamera")->yaw(Ogre::Radian(-e.state.X.rel) * mEvt->timeSinceLastFrame / 4, Ogre::Node::TS_PARENT);
	mNodePersonnage->getChild("nodeCamera")->pitch(Ogre::Radian(e.state.Y.rel) * mEvt->timeSinceLastFrame / 4, Ogre::Node::TS_LOCAL);
	//mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(mAngleMouseX * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_PARENT);
	//mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(mAngleMouseY * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(1.0f, 0.0f, 0.0f)), Ogre::Node::TS_LOCAL);
    }
    return true;
}
bool InputListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}
bool InputListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if ((id == OIS::MB_Right || id == OIS::MB_Left) && mMouseBoth)
	{
		mMouvement.z = 0;
		mMouseBoth = false;
	}
    return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent &e)
{
	switch(e.key)
	{
		case OIS::KC_ESCAPE:
			mContinuer = false;
			break;
		case OIS::KC_Z:
			mMouvement.z = 1;
			break;
		case OIS::KC_S:
			mMouvement.z = -1;
			break;
		case OIS::KC_Q:
            mAnglePerso = 10;
			break;
		case OIS::KC_D:
            mAnglePerso = -10;
			break;
		case OIS::KC_LSHIFT:
			mVitesse *= 2;
			break;
		case OIS::KC_T:
			mClick = true;
			mAnimationState->setEnabled(true);
			break;
	}
    return mContinuer;
}
bool InputListener::keyReleased(const OIS::KeyEvent &e)
{
	switch(e.key)
	{
	    case OIS::KC_Z:
	        mMouvement.z = 0;
	        break;
	    case OIS::KC_S:
	        mMouvement.z = 0;
	        break;
	    case OIS::KC_Q:
            mAnglePerso = 0;
	        break;
	    case OIS::KC_D:
            mAnglePerso = 0;
	        break;
	    case OIS::KC_LSHIFT:
	        mVitesse /= 2;
	        break;
	    case OIS::KC_T:
	    	mClick = false;
	    	mAnimationState->setTimePosition(0);
	    	mAnimationState->setEnabled(false);
	    	break;
	}
	return true;
}
