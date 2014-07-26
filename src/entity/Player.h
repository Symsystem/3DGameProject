//
//  Player.h
//  3DGameProject
//
//  Created by Syméon del Marmol on 23/07/14.
//
//

#ifndef ___DGameProject__Player__
#define ___DGameProject__Player__

#define TURN_SPEED 700.0f // en degrés par seconde
#define RUN_SPEED 20 // En unité ogre par seconde
#define NBR_ANIM 13 // Nombre d'animations

#include <iostream>
#include <string>
#include <Ogre.h>
#include <OIS.h>
#include "Object3D.h"

using namespace Ogre;

class Player : public Object3D {

private:
    // La liste des animations possible du modèle 3D
    enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};
public:
    
    Player(SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *node, Ogre::Vector3 position);
    
    void setupAnimations();
    void updatePlayer(const Ogre::Real deltaTime);
    
    void keyDown(const OIS::KeyEvent &e);
    void keyUp(const OIS::KeyEvent &e);
    void mouseMoved(const OIS::MouseEvent &e);
    void mouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void mouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    
    virtual float getTurnSpeed() const;
    virtual bool isMovable() const;
    virtual bool isMoving() const;
    virtual void setIsMoving(const bool move);
    virtual void setOrientation(const Ogre::Quaternion &direction);
    
private:
    
    Ogre::SceneNode *mNodeMainPlayer;
    Ogre::SceneNode *mNodeGoal;
    
    bool mMouseBoth, mMouseLeft, mMouseRight;
    bool mDeplacementLateral; // True si le personnage s'est déplacé à droite, gauche ou en arrière par rapport à sa direction
    
    // Déplacement
    bool mMove;
    Ogre::Vector3 mDirection;
    int mAngleRotation;
    
    // Animations
    Ogre::AnimationState *mAnims[NBR_ANIM];
    AnimID mAnimBody, mAnimHead;
    Ogre::Real mAnimTime;
    
};

#endif /* defined(___DGameProject__Player__) */
