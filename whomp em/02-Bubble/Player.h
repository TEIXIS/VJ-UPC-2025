#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Enemy.h"

class Player
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, Enemy& enemy);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    glm::vec2 getPosition() const;
    float getLives() const;
    glm::vec2 getLanzaPosition() const;
    glm::ivec2 getLanzaSize() const;
    bool checkCollision(const glm::vec2& pos, const glm::ivec2& size) const;
    bool checkCollisionLanza(const glm::vec2& pos, const glm::ivec2& size) const;
    void takeDamage(float damage);

private:
    // Helper methods for cleaner code organization
    void handleHorizontalMovement();
    void handleVerticalKeys();
    void handleJumpingAndFalling();
    void handleJumpingAnimations();
    void handleFallingAnimations();

    // State checking methods
    bool isRightFacing() const;
    bool isLeftFacing() const;
    void setIdleAnimation();

    bool bJumping;
    glm::ivec2 tileMapDispl, posPlayer, posLanza;
    int jumpAngle, startY;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    float lives = 2.67;
    bool isAttacking, atacantAdalt, atacantAbaix;
    Sprite* lanza;
    Sprite* lanzaAdalt;
    Sprite* lanzaAbaix;
    int plorantTimer;
};

#endif // _PLAYER_INCLUDE