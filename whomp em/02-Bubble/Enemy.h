#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Enemy
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    void restarVida();

private:
    glm::ivec2 tileMapDispl, posEnemy;
    int startY;
    bool bJumping;
    bool movingRight;
    int jumpAngle;
    int groundTimer;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    int vida = 1;
};

#endif // ENEMY_H