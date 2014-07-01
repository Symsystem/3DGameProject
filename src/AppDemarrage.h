/*
 * AppDemarrage.h
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#ifndef APPDEMARRAGE_H_
#define APPDEMARRAGE_H_

#include <Ogre.h>
#include "InputListener.h"

class AppDemarrage {
public:
	AppDemarrage();
	~AppDemarrage();

	bool start();

	void createCamera();
	void createViewports();
	void createScene();
	void createFrameListener();

private:
	Ogre::Root *mRoot;
	Ogre::RenderWindow *mWindow;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;

	Ogre::SceneNode *node;

	InputListener* mInputListener;
};

#endif /* APPDEMARRAGE_H_ */
