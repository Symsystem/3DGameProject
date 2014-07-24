//
//  Entity.cpp
//  3DGameProject
//
//  Created by Syméon del Marmol on 17/07/14.
//
//

#include "Entite.h"

Entite::Entite(const std::string &name)
{
    mName = name;
}

void Entite::setName(const std::string &name)
{
    mName = name;
}

std::string Entite::getName()
{
    return mName;
}