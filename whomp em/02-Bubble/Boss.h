#pragma once
#include <glm/glm.hpp>
#include "TileMap.h"
class Player;
#include "Sprite.h"
#include "ShaderProgram.h"
#include <irrKlang.h>

enum BossPhase {
    PHASE_INFINITE,
    PHASE_BOUNCE
};

class Boss {
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, Player& player);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;

    bool isAlive() const;
    void takeDamage(float dmg);
    int getCooldown() const;
	float getHealth() const;

private:
    glm::ivec2 tileMapDispl;
    glm::vec2 position;
    glm::vec2 velocity;

    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    irrklang::ISoundEngine* soundEngine;
    float health;
    bool alive;
    int attackCooldown;
    int phaseTimer;
    float timeAccum;
    BossPhase currentPhase;
	int inmortalTimer;
    int frameCount = 0;
    glm::vec2 infinityCenter;  // centro del movimiento ∞
};
