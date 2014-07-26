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
#include "Entite.h"

class Object3D : public Entite {
    
public:
    
    Object3D(Ogre::SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *parentNode, Ogre::Vector3 position = Ogre::Vector3::ZERO);
    
    void setNode(Ogre::SceneNode *node);
    
    Ogre::Vector3 getPosition() const;
    Ogre::SceneNode* getNode() const;
    
    virtual float getTurnSpeed() const;
    virtual bool isMovable() const = 0;
    virtual bool isMoving() const;
    virtual void setIsMoving(const bool move) = 0;
    virtual void setOrientation(const Ogre::Quaternion &direction) = 0;

protected:
    
    Ogre::Entity *mOgreEntity;
    Ogre::SceneNode *mNode;
};

#endif /* defined(___DGameProject__Object3D__) */
