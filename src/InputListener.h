/*
 * InputListener.h
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#ifndef INPUTLISTENER_H_
#define INPUTLISTENER_H_

#include <Ogre.h>
#include <OIS.h>
#include "NewCamera.h"
#include "Player.h"

class InputListener : public Ogre::FrameListener, public Ogre::WindowEventListener, OIS::KeyListener, OIS::MouseListener {

public:
	InputListener(Ogre::RenderWindow *wnd, NewCamera *camera, Player *player);
	~InputListener();

	void startOIS();
    
	// Appelée pendant que l'image est en cours de rendu
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Prototypes des méthodes de WindowEventListener
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw); // Permet de détruire les objets créés avec la fenêtre

	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);

private:
	Ogre::RenderWindow* mWindow;
	NewCamera* 		mCamera;
	Player*	mPlayer;
    
    const Ogre::FrameEvent* mEvt;

	OIS::InputManager*  mInputManager;
	OIS::Mouse*			mMouse;
	OIS::Keyboard*		mKeyboard;

	bool mContinuer;
};

#endif /* INPUTLISTENER_H_ */
