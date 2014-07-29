//
//  Player.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 23/07/14.
//
//

#include "Player.h"

Player::Player(SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *node, Ogre::Vector3 position) : Object3D(scnMgr, name, path, node, position), mNodeMainPlayer(node), mMouseBoth(false), mMouseLeft(false), mMouseRight(false), mAngleRotation(0), mMove(false), mDeplacementLateral(false)
{
    mNodeGoal = mNodeMainPlayer->createChildSceneNode();
    mDirection = Ogre::Vector3::ZERO;
    mAnimBody = ANIM_IDLE_BASE;
    mAnimHead = ANIM_IDLE_TOP;
    mAnimTime = 0;
    
    setupAnimations();
}

void Player::setupAnimations()
{
    // this is very important due to the nature of the exported animations
    mOgreEntity->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);
    
    String animNames[] =
    {"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};
    
    // populate our animation list
    for (int i = 0; i < NBR_ANIM; i++)
    {
        mAnims[i] = mOgreEntity->getAnimationState(animNames[i]);
        mAnims[i]->setLoop(true);
    }
    
    // Met les mains en "relax" tant qu'il ne tient rien en main
    mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void Player::updatePlayer(const Ogre::Real deltaTime)
{
    // Rotation du personnage :
    // On calcule la direction finale vers où on doit arriver
    Ogre::Vector3 goalDirection = Ogre::Vector3::ZERO;
    
    goalDirection += mNodeGoal->getOrientation().zAxis() * (-1);
    goalDirection.normalise();
    
    // Calcul de la rotation à effectuer pour atteindre cette direction
    Ogre::Quaternion toGoal = mNode->getOrientation().zAxis().getRotationTo(goalDirection);
    
    // Calcul l'angle qu'il faut tourner pour faire face à cette direction
    Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();
    // Représente combien de degrés le personnage PEUT tourner à cette image
    Ogre::Real yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;
    
    // On vérifie qu'on ne tourne pas plus que ce qu'il faut
    if (yawToGoal < 0) yawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(yawToGoal, yawAtSpeed)); // Si angle négatif
    else if (yawToGoal > 0) yawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(yawToGoal, yawAtSpeed)); // Si angle positif
    
    // Enfin on fait la rotation
    mNode->yaw(Ogre::Degree(yawToGoal));
    
    // On déplace le personnage
    if (mMove)
    {
        Ogre::Vector3 deplacement = Ogre::Vector3::UNIT_Z * deltaTime * RUN_SPEED;
        mNodeMainPlayer->translate(mNode->getOrientation() * mNodeMainPlayer->getOrientation() * deplacement);
    }
    
    // Mise à jour des animations
    mAnimTime += deltaTime;
    
    // Animation à l'arrêt
    if (!mMove && mAnimBody != ANIM_IDLE_BASE)
    {
        mAnimTime = 0;
        mAnims[mAnimBody]->setEnabled(false);
        mAnims[mAnimHead]->setEnabled(false);
        
        mAnims[ANIM_IDLE_BASE]->setEnabled(true);
        mAnims[ANIM_IDLE_BASE]->setTimePosition(mAnimTime);
        mAnimBody = ANIM_IDLE_BASE;
        
        mAnims[ANIM_IDLE_TOP]->setEnabled(true);
        mAnims[ANIM_IDLE_TOP]->setTimePosition(mAnimTime);
        mAnimHead = ANIM_IDLE_TOP;
    }
    
    // Animation en marche
    else if (mMove && mAnimBody != ANIM_RUN_BASE)
    {
        mAnimTime = 0;
        mAnims[mAnimBody]->setEnabled(false);
        mAnims[mAnimHead]->setEnabled(false);
        
        mAnims[ANIM_RUN_BASE]->setEnabled(true);
        mAnims[ANIM_RUN_BASE]->setTimePosition(mAnimTime);
        mAnimBody = ANIM_RUN_BASE;
        
        mAnims[ANIM_RUN_TOP]->setEnabled(true);
        mAnims[ANIM_RUN_TOP]->setTimePosition(mAnimTime);
        mAnimHead = ANIM_RUN_TOP;
    }
    
    /*for (int i = ANIM_IDLE_BASE; i < NBR_ANIM; i++)
    {
        if (mAnimBody == static_cast<AnimID>(i))
        {
            mAnims[i]->setTimePosition(mAnimTime);
        }
        else if (mAnimHead == static_cast<AnimID>(i))
        {
            mAnims[i]->setTimePosition(mAnimTime);
        }
    }*/
    
    // Ajout du temps
    mAnims[mAnimBody]->addTime(deltaTime);
    mAnims[mAnimHead]->addTime(deltaTime);
}

void Player::keyDown(const OIS::KeyEvent &e)
{
    // change la direction ou l'angle de rotation en fonction des touches enfoncées
    if (e.key == OIS::KC_W || e.key == OIS::KC_UP)
    {
        mMove = true;
    }
    else if ((e.key == OIS::KC_S || e.key == OIS::KC_DOWN) && !mMouseBoth)
    {
        mMove = true;
        mNodeGoal->yaw(Ogre::Degree(180));
        mDeplacementLateral = true;
    }
    else if (e.key == OIS::KC_A && !mMouseBoth)
    {
        mMove = true;
        mNodeGoal->yaw(Ogre::Degree(90));
        mDeplacementLateral = true;
    }
    else if (e.key == OIS::KC_D && !mMouseBoth)
    {
        mMove = true;
        mNodeGoal->yaw(Ogre::Degree(-90));
        mDeplacementLateral = true;
    }
    else if (e.key == OIS::KC_LEFT) mAngleRotation = 1;
    else if (e.key == OIS::KC_RIGHT) mAngleRotation = -1;
}

void Player::keyUp(const OIS::KeyEvent &e)
{
    // remet la direction ou l'angle de rotation à 0 en fonction des touches relachées
    if (e.key == OIS::KC_W || e.key == OIS::KC_UP) mMove = false;
    else if ((e.key == OIS::KC_S || e.key == OIS::KC_DOWN) && mDeplacementLateral)
    {
        mMove = false;
        mNodeGoal->yaw(Ogre::Degree(-180));
        mDeplacementLateral = false;
    }
    else if (e.key == OIS::KC_A && mDeplacementLateral)
    {
        mMove = false;
        mNodeGoal->yaw(Ogre::Degree(-90));
        mDeplacementLateral = false;
    }
    else if (e.key == OIS::KC_D && mDeplacementLateral)
    {
        mMove = false;
        mNodeGoal->yaw(Ogre::Degree(90));
        mDeplacementLateral = false;
    }
    else if (e.key == OIS::KC_LEFT && mAngleRotation != -1) mAngleRotation = 0;
    else if (e.key == OIS::KC_RIGHT && mAngleRotation != 1) mAngleRotation = 0;
}

void Player::mouseMoved(const OIS::MouseEvent &e)
{
    
}

void Player::mouseDown(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        mMouseLeft = true;
    }
    else if (id == OIS::MB_Right)
    {
        mMouseRight = true;
    }
    if ((mMouseLeft && id == OIS::MB_Right) || (mMouseRight && id == OIS::MB_Left))
    {
        mMouseBoth = true;
    }
}

void Player::mouseUp(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if (id == OIS::MB_Left)
    {
        mMouseLeft = false;
    }
    else if (id == OIS::MB_Right)
    {
        mMouseRight = false;
    }
    if (mMouseBoth)
    {
        mMouseBoth = false;
        mDirection = Ogre::Vector3::ZERO;
    }
}

float Player::getTurnSpeed() const
{
    return TURN_SPEED;
}

bool Player::isMovable() const
{
    return true;
}

bool Player::isMoving() const
{
    return mMove;
}

void Player::setIsMoving(const bool move)
{
    mMove = move;
}

void Player::setOrientation(const Ogre::Quaternion &direction)
{
    mNodeGoal->setOrientation(direction);
}
