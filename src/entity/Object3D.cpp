//
//  Object3D.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 17/07/14.
//
//

#include "Object3D.h"

Object3D::Object3D(Ogre::SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *parentNode, Ogre::Vector3 position):
Entite(name)
{
    mOgreEntity = scnMgr->createEntity(name, path);
    
    mNode = parentNode->createChildSceneNode("node_" + name);
	mNode->attachObject(mOgreEntity);
    mNode->translate(position);
}

void Object3D::setNode(Ogre::SceneNode *node)
{
    mNode = node;
}

Ogre::Vector3 Object3D::getPosition() const
{
    return mNode->getPosition();
}

Ogre::SceneNode* Object3D::getNode() const
{
    return mNode;
}

float Object3D::getTurnSpeed() const
{
    return 0;
}