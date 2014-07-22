//
//  Object3D.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 17/07/14.
//
//

#include "Object3D.h"

Object3D::Object3D(Ogre::SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *node, Ogre::Vector3 position)
{
    mOgreEnt = scnMgr->createEntity(name, path);
    mNode = node->createChildSceneNode("perso");
	mNode->attachObject(mOgreEnt);
    mNode->translate(position);
}