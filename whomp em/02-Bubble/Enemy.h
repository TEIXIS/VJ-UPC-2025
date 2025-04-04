#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Enemy
{
public:
    void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);


private:
    glm::ivec2 tileMapDispl, posEnemy;
    int startY;
    bool bJumping;
	bool movingRight;
    int jumpAngle;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;
};

#endif // ENEMY_H
