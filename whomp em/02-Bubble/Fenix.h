#ifndef FENIX_H
#define FENIX_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Fenix {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    void restarVida();
    void Fenix::getPosPlayer(glm::vec2 pos);

private:
    bool bJumping;
    bool movingRight;
    bool attacking;
    int vida;
    int groundTimer;
    glm::ivec2 tileMapDispl, posEnemy;
    glm::vec2 originalHeight;
    Sprite* sprite;
    Sprite* foc;
    TileMap* map;
    Texture spritesheet;
	glm::vec2 posPlayer;
    bool habaixat;
    bool focActiu;
};


#endif // FENIX_H
