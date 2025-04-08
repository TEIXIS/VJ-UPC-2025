#ifndef PLATFORM2_H
#define PLATFORM2_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "Player.h"
#include "ShaderProgram.h"

class Platform2 {
public:
    Platform2();
    ~Platform2();

    void init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset, float maxOffset);
    void update(int deltaTime);
    void render();

    bool checkCollisionFromAbove(const Player& player, float collisionThreshold = 5.0f);
    bool checkCollisionFromBelow(const Player& player, float collisionThreshold = 5.0f);
    void moveWithCharacter(float deltaY, Player& player);
    float getLastDeltaY() const;
    glm::vec2 getPosition() const;

private:
    glm::vec2 pos;
    glm::vec2 originalPos;
    glm::ivec2 tileMapDispl;
    float maxOffset;
    float speed;
    bool movingUp;
    float lastDeltaY; // Guarda el último delta de movimiento vertical

    Sprite* sprite;
    Texture spritesheet;
};

#endif
