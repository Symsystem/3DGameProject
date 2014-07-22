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

	mKeyboard->capture();
	mMouse->capture();

    mEvt = &evt;
    
    std::cout << "Test" << std::endl;
    
    Ogre::Vector3 deplacement = Ogre::Vector3::ZERO;
	deplacement = mMouvement * mVitesse * evt.timeSinceLastFrame / 4;
    deplacement.normalise();
    mNodePersonnage->translate(mNodePersonnage->getChild("perso")->getOrientation() * mNodePersonnage->getOrientation() * deplacement);
    if (mMouse->getMouseState().buttonDown(OIS::MB_Left))
    {
        mNodePersonnage->getChild("perso")->rotate(Ogre::Quaternion(mAnglePerso * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_LOCAL);
    }
    else
    {
        mNodePersonnage->rotate(Ogre::Quaternion(mAnglePerso * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_LOCAL);
        if (mNodePersonnage->getChild("nodeCamera")->getOrientation() != mNodePersonnage->getChild("perso")->getOrientation() && mKeyboard->isKeyDown(OIS::KC_W))
        {
            Ogre::Radian angle(2);
            mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(angle * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_PARENT);
        }
    }

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
	if (e.state.buttonDown(OIS::MB_Left)){
        mAngleMouseX = -e.state.X.rel;
        mAngleMouseY = e.state.Y.rel;
        mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(mAngleMouseX * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(0.0f, 1.0f, 0.0f)), Ogre::Node::TS_PARENT);
        mNodePersonnage->getChild("nodeCamera")->rotate(Ogre::Quaternion(mAngleMouseY * mEvt->timeSinceLastFrame / 4, Ogre::Vector3(1.0f, 0.0f, 0.0f)), Ogre::Node::TS_LOCAL);
    }
	return true;
}
bool InputListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}
bool InputListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    mAngleMouseX = 0;
    mAngleMouseY = 0;
    return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent &e)
{
	switch(e.key)
	{
		case OIS::KC_ESCAPE:
			mContinuer = false;
			break;
		case OIS::KC_W:
			mMouvement.z += 1;
			break;
		case OIS::KC_S:
			mMouvement.z -= 1;
			break;
		case OIS::KC_A:
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
	    case OIS::KC_W:
	        mMouvement.z -= 1;
	        break;
	    case OIS::KC_S:
	        mMouvement.z += 1;
	        break;
	    case OIS::KC_A:
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
