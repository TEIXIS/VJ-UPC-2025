#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
//#include "Player.h"

class Enemy
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    //void update(int deltaTime, const Player& player);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    //bool checkCollision(const glm::vec2& pos, const glm::ivec2& size) const;
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
    int vida;
};

#endif // ENEMY_H
