#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Seta.h"
#include "Fenix.h"
#include "Mag.h"

class Player
{
public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime, vector<Seta*>& setas, vector<Fenix*>& fenixes, Mag& mag, Mag& mag2);
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
    void renderHitbox(const glm::vec2& position, const glm::ivec2& size);
    void stopJump();
    void setPlatform(bool a);
    bool isJumping();
    bool isJumpingPlat();
    void heal1Live();
    void healallLives();
    int getHmax();
    void actCalabaza();

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
    float hMax = 4;
    bool isAttacking, atacantAdalt, atacantAbaix;
    Sprite* lanza;
    Sprite* lanzaAdalt;
    Sprite* lanzaAbaix;
    Sprite* totemFoc;
    int plorantTimer;
    bool godMode;
    bool attackKeyPressed = false;
    int frameCount;
    bool platform = false;
    bool saltarPlata = false;
	bool totemFocActiu = false;
	int offsetTotem = 0;
    int calabazas = 1;
	
};

#endif // _PLAYER_INCLUDE