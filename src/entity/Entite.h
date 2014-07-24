//
//  Entity.h
//  3DGameProject
//
//  Created by Syméon del Marmol on 17/07/14.
//
//

#ifndef ___DGameProject__Entity__
#define ___DGameProject__Entity__

#define PLAYER_SPEED 10

#include <iostream>
#include <string>

class Entite {
    
public:
    Entite(const std::string &name);
    
    void setName(const std::string &name);
    
    std::string getName();
    
protected:
    
    std::string mName;
};

#endif /* defined(___DGameProject__Entity__) */
