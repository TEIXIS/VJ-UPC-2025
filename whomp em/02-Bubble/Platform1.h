#ifndef PLATFORM1_H
#define PLATFORM1_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "Player.h"
#include "ShaderProgram.h"

class Platform1 {
public:
  
    Platform1();


    ~Platform1();


    void init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset);

    void render();

    bool checkCollisionFromAbove(const Player& player, float collisionThreshold = 5.0f);


    bool checkCollisionFromBelow(const Player& player, float collisionThreshold = 5.0f);


    void moveWithCharacter(float deltaY, Player& player);


    void returnToOriginalPosition(float deltaTime);


    glm::vec2 getPosition() const;

private:
    glm::vec2 pos;  
    glm::ivec2 tileMapDispl;
    glm::vec2 originalPos;  
    Sprite* sprite;         
    Texture spritesheet;    
};

#endif
