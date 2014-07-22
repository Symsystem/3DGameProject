//
//  Object3D.h
//  3DGameProject
//
//  Created by Syméon del Marmol on 17/07/14.
//
//

#ifndef ___DGameProject__Object3D__
#define ___DGameProject__Object3D__

#include <iostream>
#include <Ogre.h>
#include <string>
#include "Entity.h"

class Object3D : public Entity {
    
public:
    
    Object3D(Ogre::SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *node, Ogre::Vector3 position);
    
private:
    
    Ogre::Entity *mOgreEnt;
    Ogre::SceneNode *mNode;

};

#endif /* defined(___DGameProject__Object3D__) */
