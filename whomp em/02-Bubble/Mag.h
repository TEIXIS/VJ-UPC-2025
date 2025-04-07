#ifndef MAG_H
#define MAG_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

class Mag {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();
    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    void restarVida();
    void getPosPlayer(glm::vec2 pos);
    

private:
    bool bJumping;
    bool movingRight;
    bool attacking;
    int vida;
    int atacTimer;
    glm::ivec2 tileMapDispl, posEnemy;
    glm::vec2 posPlayer;
    Sprite* sprite;
    
    TileMap* map;
    Texture spritesheet;
    
};


#endif // MAG_H

