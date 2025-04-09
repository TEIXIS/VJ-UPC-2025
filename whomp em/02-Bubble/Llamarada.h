#pragma once
#include "Sprite.h"
#include "Player.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Llamarada {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    Llamarada();
    ~Llamarada();

    void init(const glm::vec2& position, const glm::ivec2& tileMapOffset, ShaderProgram& shader, Direction dir);
    void update(int deltaTime);
    void render();

    bool collidesWithPlayer(const Player& player) const;
    void setInvisibleTime(float seconds); 
    void reset();

private:
    void loadTexture(Direction dir);
    void updateAnimationFrame();

private:
    Sprite* sprite = nullptr;
    Texture spritesheet;
    glm::vec2 pos;
    glm::ivec2 tileMapDispl;
    glm::ivec2 size;
    Direction direction;

    enum Phase {
        INVISIBLE,
        CHARGING,
        ACTIVE
    } phase;

    float elapsedTime = 0.f;
    float invisibleTime = 2.f; 
    int totalFrames = 4;
};
