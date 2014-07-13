/*
 * AppDemarrage.h
 *
 *  Created on: 8 mai 2014
 *      Author: sym
 */

#ifndef APPDEMARRAGE_H_
#define APPDEMARRAGE_H_

#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
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
    void createTerrain();

private:
	Ogre::Root *mRoot;
	Ogre::RenderWindow *mWindow;
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;

    Ogre::Light* mLight;
	Ogre::SceneNode *mNode;

	Ogre::SceneNode *mRobotNode;
	Ogre::Entity *mRobot;
	Ogre::AnimationState* mAnimationState;
    
    Ogre::Terrain* mTerrain;
    Ogre::TerrainGlobalOptions* mTerrainOptions;

	InputListener* mInputListener;
};

#endif /* APPDEMARRAGE_H_ */
