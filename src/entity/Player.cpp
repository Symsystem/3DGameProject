//
//  Player.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 23/07/14.
//
//

#include "Player.h"

Player::Player(SceneManager *scnMgr, std::string const &name, std::string const &path, Ogre::SceneNode *node, Ogre::Vector3 position) : Object3D(scnMgr, name, path, node, position), mNodeMainPlayer(node), mMouseBoth(false), mMouseLeft(false), mMouseRight(false), mAngleRotation(0)
{
    mDirection = Ogre::Vector3::ZERO;
}

void Player::updatePlayer(const Ogre::Real deltaTime)
{    
    // Et ensuite on déplace le personnage
    Ogre::Vector3 deplacement =  mDirection * deltaTime * RUN_SPEED;
    mNodeMainPlayer->translate(mNode->getOrientation() * mNodeMainPlayer->getOrientation() * deplacement);
    mNode->rotate(Ogre::Quaternion(Ogre::Degree(mAngleRotation * deltaTime * TURN_SPEED), Ogre::Vector3(0.0f, 1.0f, 0.0f)));
}

void Player::keyDown(const OIS::KeyEvent &e)
{
    if ((e.key == OIS::KC_W || e.key == OIS::KC_UP) && mDirection.x == 0) mDirection.z = 1;
    else if ((e.key == OIS::KC_S || e.key == OIS::KC_DOWN) && mDirection.x == 0) mDirection.z = -1;
    else if (e.key == OIS::KC_A && mDirection.z == 0) mDirection.x = 1;
    else if (e.key == OIS::KC_D && mDirection.z == 0) mDirection.x = -1;
    else if (e.key == OIS::KC_LEFT) mAngleRotation = 1;
    else if (e.key == OIS::KC_RIGHT) mAngleRotation = -1;
}

void Player::keyUp(const OIS::KeyEvent &e)
{
    if ((e.key == OIS::KC_W || e.key == OIS::KC_UP) && mDirection.z == 1) mDirection.z = 0;
    else if ((e.key == OIS::KC_S || e.key == OIS::KC_DOWN) && mDirection.z == -1) mDirection.z = 0;
    else if (e.key == OIS::KC_A && mDirection.x == 1) mDirection.x = 0;
    else if (e.key == OIS::KC_D && mDirection.x == -1) mDirection.x = 0;
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

void Player::setDirection(const Ogre::Vector3 &direction)
{
    mDirection = direction;
}
