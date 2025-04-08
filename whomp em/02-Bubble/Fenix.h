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
    void getPosPlayer(glm::vec2 pos);
    glm::vec2 getPosFoc() const;
	void spawn(int x, int y);

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
    Sprite* foc2;
    TileMap* map;
    Texture spritesheet;
	glm::vec2 posPlayer;
    glm::ivec2 posFoc;
    glm::ivec2 posFoc2;
    bool habaixat;
    bool focActiu;
	bool spawnB;
};


#endif // FENIX_H
