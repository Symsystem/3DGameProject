/*
 * InputListener.cpp
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#include "InputListener.h"

InputListener::InputListener(Ogre::RenderWindow *wnd, NewCamera *camera, Player *player)
{
	mWindow = wnd;
	mCamera = camera;
    mPlayer = player;
    
	mContinuer = true;

	// démarrage d'OIS
	startOIS();
}
InputListener::~InputListener()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
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

bool InputListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;
    
    if (mKeyboard)
        mKeyboard->capture();
    if (mMouse)
        mMouse->capture();

    mCamera->updateCamera(evt.timeSinceLastFrame);
    mPlayer->updatePlayer(evt.timeSinceLastFrame);

	return mContinuer;
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
    mCamera->mouseMoved(e);
    mPlayer->mouseMoved(e);
    
    return true;
}
bool InputListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    mCamera->mouseDown(e, id);
    mPlayer->mouseDown(e, id);
    return true;
}
bool InputListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    mCamera->mouseUp(e, id);
    mPlayer->mouseUp(e, id);
    return true;
}

bool InputListener::keyPressed(const OIS::KeyEvent &e)
{
    if (e.key == OIS::KC_ESCAPE)
        mContinuer = false;
    
    mCamera->keyDown(e);
    mPlayer->keyDown(e);
    return mContinuer;
}
bool InputListener::keyReleased(const OIS::KeyEvent &e)
{
    mCamera->keyUp(e);
    mPlayer->keyUp(e);
	return true;
}
