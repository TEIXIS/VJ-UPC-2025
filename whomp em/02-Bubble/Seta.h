#ifndef SETA_H
#define SETA_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Seta
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    void restarVida();
    void spawn(int x, int y);
    void getPosPlayer(glm::vec2 pos);
    bool spawnItem();
    glm::vec2 getPositionSpawn() const;

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
    int vida = -1;
    glm::vec2 posPlayer;
    glm::ivec2 posSpawnItem;
    bool defeated = false;
};

#endif // SETA_H