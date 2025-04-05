#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Enemy.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, const Enemy& enemy);
	//void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() const;
	float getLives() const;
	string getWeapon() const;
	glm::vec2 Player::getLanzaPosition() const;
	glm::ivec2 getLanzaSize() const;
	bool checkCollision(const glm::vec2& pos, const glm::ivec2& size) const;
	bool checkCollisionLanza(const glm::vec2& pos, const glm::ivec2& size) const;

private:
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
	string weapon = "lanza";
	bool isAttacking, atacantAdalt, atacantAbaix;
	Sprite* lanza;
	Sprite* lanzaAdalt;
	Sprite* lanzaAbaix;
	int plorantTimer;


};


#endif // _PLAYER_INCLUDE


