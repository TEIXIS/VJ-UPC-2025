#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

// Constants
#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 70
#define FALL_STEP 5
#define MOVE_SPEED 2

// Animation indices
enum PlayerAnims {
    STAND_RIGHT, STAND_LEFT,
    MOVE_LEFT, MOVE_RIGHT,
    LOOK_UP_R, LOOK_DOWN_R, LOOK_UP_L, LOOK_DOWN_L,
    ATK_LEFT_STANDING, ATK_RIGHT_STANDING,
    ATK_LEFT_MOVING, ATK_RIGHT_MOVING,
    ATK_LEFT_DOWN, ATK_RIGHT_DOWN,
    ATK_JUMPING_UP_R, ATK_JUMPING_DOWN_R, ATK_JUMPING_UP_L, ATK_JUMPING_DOWN_L, PLORANT_DRETA, PLORANT_ESQUERRA, 
    ESCALANT, ATK_ESCALANT_R, ATK_ESCALANT_L, COUNT
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    isAttacking = false;
    atacantAdalt = false;
    atacantAbaix = false;
    plorantTimer = 0;

    spritesheet.loadFromFile("images/whomp_em_usa.png", TEXTURE_PIXEL_FORMAT_RGBA);

    // Initialize main sprite
    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(COUNT);

    // Initialize weapon sprites
    lanza = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanza->setNumberAnimations(2);

    lanzaAbaix = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAbaix->setNumberAnimations(1);

    lanzaAdalt = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAdalt->setNumberAnimations(1);

	totemFoc = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	totemFoc->setNumberAnimations(2);

    // Weapon animations
    // Right
    lanza->setAnimationSpeed(0, 8);
    lanza->addKeyframe(0, glm::vec2(0.25f, 0.375f));
    lanza->addKeyframe(0, glm::vec2(0.5f, 0.375f));
    lanza->addKeyframe(0, glm::vec2(0.75f, 0.375f));

    // Left
    lanza->setAnimationSpeed(1, 8);
    lanza->addKeyframe(1, glm::vec2(0.375f, 0.375f));
    lanza->addKeyframe(1, glm::vec2(0.625f, 0.375f));
    lanza->addKeyframe(1, glm::vec2(0.875f, 0.375f));

    // Up
    lanzaAdalt->setAnimationSpeed(0, 8);
    lanzaAdalt->addKeyframe(0, glm::vec2(0.5f, 0.625f));

    // Down
    lanzaAbaix->setAnimationSpeed(0, 8);
    lanzaAbaix->addKeyframe(0, glm::vec2(0.625f, 0.625f));

	totemFoc->setAnimationSpeed(0, 8);
	totemFoc->addKeyframe(0, glm::vec2(0.5f, 0.5f));
	totemFoc->addKeyframe(0, glm::vec2(0.5f, 0.5625f));

	totemFoc->setAnimationSpeed(1, 8);
	totemFoc->addKeyframe(1, glm::vec2(0.5625f, 0.5f));
	totemFoc->addKeyframe(1, glm::vec2(0.5625f, 0.5625f));

    // Standing animations
    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.125f));

    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.125f));

    // Moving animations
    sprite->setAnimationSpeed(MOVE_LEFT, 8);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.125f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.750f, 0.125f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.875f, 0.125f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 8);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.125f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.125f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375, 0.125f));

    // Looking animations
    sprite->setAnimationSpeed(LOOK_UP_R, 8);
    sprite->addKeyframe(LOOK_UP_R, glm::vec2(0.f, 0.375f));

    sprite->setAnimationSpeed(LOOK_UP_L, 8);
    sprite->addKeyframe(LOOK_UP_L, glm::vec2(0.125f, 0.375f));

    sprite->setAnimationSpeed(LOOK_DOWN_R, 8);
    sprite->addKeyframe(LOOK_DOWN_R, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(LOOK_DOWN_L, 8);
    sprite->addKeyframe(LOOK_DOWN_L, glm::vec2(0.25f, 0.5f));

    // Attack animations
    sprite->setAnimationSpeed(ATK_LEFT_DOWN, 8);
    sprite->addKeyframe(ATK_LEFT_DOWN, glm::vec2(0.375f, 0.5f));

    sprite->setAnimationSpeed(ATK_RIGHT_DOWN, 8);
    sprite->addKeyframe(ATK_RIGHT_DOWN, glm::vec2(0.125f, 0.5f));

    sprite->setAnimationSpeed(ATK_RIGHT_MOVING, 8);
    sprite->addKeyframe(ATK_RIGHT_MOVING, glm::vec2(0.125f, 0.25f));
    sprite->addKeyframe(ATK_RIGHT_MOVING, glm::vec2(0.25f, 0.25f));
    sprite->addKeyframe(ATK_RIGHT_MOVING, glm::vec2(0.375f, 0.25f));

    sprite->setAnimationSpeed(ATK_LEFT_MOVING, 8);
    sprite->addKeyframe(ATK_LEFT_MOVING, glm::vec2(0.625f, 0.25f));
    sprite->addKeyframe(ATK_LEFT_MOVING, glm::vec2(0.75f, 0.25f));
    sprite->addKeyframe(ATK_LEFT_MOVING, glm::vec2(0.875f, 0.25f));

    sprite->setAnimationSpeed(ATK_RIGHT_STANDING, 8);
    sprite->addKeyframe(ATK_RIGHT_STANDING, glm::vec2(0.f, 0.25f));

    sprite->setAnimationSpeed(ATK_LEFT_STANDING, 8);
    sprite->addKeyframe(ATK_LEFT_STANDING, glm::vec2(0.5f, 0.25f));

    // Jumping attack animations
    sprite->setAnimationSpeed(ATK_JUMPING_UP_R, 8);
    sprite->addKeyframe(ATK_JUMPING_UP_R, glm::vec2(0.f, 0.625f));

    sprite->setAnimationSpeed(ATK_JUMPING_UP_L, 8);
    sprite->addKeyframe(ATK_JUMPING_UP_L, glm::vec2(0.25f, 0.625f));

    sprite->setAnimationSpeed(ATK_JUMPING_DOWN_R, 8);
    sprite->addKeyframe(ATK_JUMPING_DOWN_R, glm::vec2(0.125f, 0.625f));

    sprite->setAnimationSpeed(ATK_JUMPING_DOWN_L, 8);
    sprite->addKeyframe(ATK_JUMPING_DOWN_L, glm::vec2(0.375f, 0.625f));

    // Hurt animations
    sprite->setAnimationSpeed(PLORANT_DRETA, 8);
    sprite->addKeyframe(PLORANT_DRETA, glm::vec2(0.625f, 0.0f));

    sprite->setAnimationSpeed(PLORANT_ESQUERRA, 8);
    sprite->addKeyframe(PLORANT_ESQUERRA, glm::vec2(0.75f, 0.0f));


	sprite->setAnimationSpeed(ESCALANT, 8);
	sprite->addKeyframe(ESCALANT, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(ESCALANT, glm::vec2(0.125f, 0.75f));

	sprite->setAnimationSpeed(ATK_ESCALANT_R, 8);
	sprite->addKeyframe(ATK_ESCALANT_R, glm::vec2(0.25f, 0.75f));

	sprite->setAnimationSpeed(ATK_ESCALANT_L, 8);
	sprite->addKeyframe(ATK_ESCALANT_L, glm::vec2(0.375f, 0.75f));

    // Set initial states
    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

    lanza->changeAnimation(0);
    lanza->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

    lanzaAbaix->changeAnimation(0);
    lanzaAbaix->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

    lanzaAdalt->changeAnimation(0);
    lanzaAdalt->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    
	totemFoc->changeAnimation(0);
	totemFoc->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));



    godMode = false;
	frameCount = 0;
}

void Player::update(int deltaTime, vector<Seta*>& setas, vector<Fenix*>& fenixes, Mag& mag, Mag& mag2)
{
    // Update all sprites
    sprite->update(deltaTime);
    lanza->update(deltaTime);
    lanzaAbaix->update(deltaTime);
    lanzaAdalt->update(deltaTime);
	totemFoc->update(deltaTime);
    
    this->checkImmortalTimer(deltaTime);


    static bool levitateKey = false;

    if (Game::instance().getKey(GLFW_KEY_L)) {
        if (!levitateKey) {
            levitar = !levitar;
            levitateKey = true;
        }
    }
    else {
        levitateKey = false;
    }

    static bool healKey = false;

    if (Game::instance().getKey(GLFW_KEY_H)) {
        if (!healKey) {
			lives = hMax;
            lamps = 2;
            healKey = true;
        }
    }
    else {
        healKey = false;
    }


    static bool totemKey = false;

    if (Game::instance().getKey(GLFW_KEY_T)) {
        if (!totemKey) {
            totemFocActiu = !totemFocActiu;
            std::cout << "Totem: " << (totemFocActiu ? "ON" : "OFF") << std::endl;
            totemKey = true;
        }
    }
    else {
        totemKey = false;
    }


    static bool godModeKeyPressed = false;

    if (Game::instance().getKey(GLFW_KEY_G)) {
        if (!godModeKeyPressed) {
            godMode = !godMode;
            std::cout << "God mode: " << (godMode ? "ON" : "OFF") << std::endl;
            godModeKeyPressed = true;
        }
    }
    else {
        godModeKeyPressed = false;
    }

    // Handle hurt state
    if (plorantTimer > 0) {
        plorantTimer -= deltaTime;
        if (plorantTimer <= 0) {
            plorantTimer = 0;
        }

        // Basic movement still allowed when hurt
        if (Game::instance().getKey(GLFW_KEY_LEFT)) {
            posPlayer.x -= MOVE_SPEED;
            if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) {
                posPlayer.x += MOVE_SPEED;
            }
        }
        else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
            posPlayer.x += MOVE_SPEED;
            if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
                posPlayer.x -= MOVE_SPEED;
            }
        }

        // Handle jumping/falling when hurt
        if (bJumping) {
            saltarPlata = false;
            handleJumpingAnimations();

            jumpAngle += JUMP_ANGLE_STEP;
            if (jumpAngle == 180) {
                bJumping = false;
                posPlayer.y = startY;
            }
            else {
                posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

                if (jumpAngle < 90) {
                    if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
                        bJumping = false;
                        jumpAngle = 180;
                    }
                }
                else {
                    bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
                }
            }
        }
        else if (platform) {
            // 🔹 Si está sobre una plataforma, no aplicar gravedad
            if (Game::instance().getKey(GLFW_KEY_Z)) {
                platform = false;
                cout << "Salto\n";
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
                saltarPlata = true;
            }
        }
        else {
            // 🔹 Aplica gravedad normalmente
            posPlayer.y += FALL_STEP;
            if (map->collisionLava(posPlayer,glm::ivec2(32.f,32.f))) {
                lava = true;
            }
            else lava = false;
            if (lava && !godMode && inmortalTimer <= 0) {
                if (plorantTimer <= 0) {
                    plorantTimer = 1000;
                    this->takeDamage(0.33f);
                }
            }
            if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
                if (Game::instance().getKey(GLFW_KEY_Z)) {
                    bJumping = true;
                    jumpAngle = 0;
                    startY = posPlayer.y;
                    saltarPlata = false;
                }
                saltarPlata = false;
            }
        }
    }
    else {
        // Normal gameplay when not hurt

        // Handle horizontal movement
        
        if (map->isOnLadder(posPlayer, glm::ivec2(32,32)) && !map->collisionMoveDown(posPlayer, glm::ivec2(32, 33), &posPlayer.y)) {
			
            handleLadderMovement();
        }
        
        else {
			if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
                if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                    posPlayer.y += 5;
                }
                else if (Game::instance().getKey(GLFW_KEY_UP)) {
                    posPlayer.y -= 5;
                }
			}
            


            if (Game::instance().getKey(GLFW_KEY_W)) {
                posPlayer.y -= MOVE_SPEED;
            }
            else if (Game::instance().getKey(GLFW_KEY_S)) {
                posPlayer.y += MOVE_SPEED;
            }

            //cout << "PosPlayer: " << posPlayer.x << ", " << posPlayer.y << endl;

            // Handle jumping and falling




            // Handle weapon state
            if (!Game::instance().getKey(GLFW_KEY_X)) {
                isAttacking = false;
            }
            else {
                isAttacking = true;
                if (isRightFacing()) {
                    if (totemFocActiu) {
                        lanza->changeAnimation(0);
                        if (totemFoc->animation() != 1) totemFoc->changeAnimation(1);
                        offsetTotem = 8;
                    }
                    else {
                        if (lanza->animation() != 0) lanza->changeAnimation(0);
                    }

                    posLanza = glm::vec2(posPlayer.x + 26, sprite->animation() == ATK_RIGHT_DOWN ? posPlayer.y + 8 : posPlayer.y);
                }
                else if (isLeftFacing()) {
                    if (totemFocActiu) {
                        lanza->changeAnimation(1);
                        if (totemFoc->animation() != 0) totemFoc->changeAnimation(0);

                        offsetTotem = 8;
                    }
                    else {
                        if (lanza->animation() != 1) lanza->changeAnimation(1);
                    }

                    posLanza = glm::vec2(posPlayer.x - 27, sprite->animation() == ATK_LEFT_DOWN ? posPlayer.y + 8 : posPlayer.y);
                }
            }

            handleHorizontalMovement();

            // Handle vertical keys
            handleVerticalKeys();

            handleJumpingAndFalling();

            // Handle special attack positions
            if (sprite->animation() == ATK_JUMPING_UP_R) {
                atacantAdalt = true;
                posLanza = glm::vec2(posPlayer.x + 6, posPlayer.y - 32);
            }
            else if (sprite->animation() == ATK_JUMPING_UP_L) {
                atacantAdalt = true;
                posLanza = glm::vec2(posPlayer.x, posPlayer.y - 32);
            }
            else {
                atacantAdalt = false;
            }

            if (sprite->animation() == ATK_JUMPING_DOWN_R) {
                atacantAbaix = true;
                posLanza = glm::vec2(posPlayer.x + 7, posPlayer.y + 25);
            }
            else if (sprite->animation() == ATK_JUMPING_DOWN_L) {
                atacantAbaix = true;
                posLanza = glm::vec2(posPlayer.x, posPlayer.y + 25);
            }
            else {
                atacantAbaix = false;
            }
        }

        

        // Check collisions with enemy
        for (int i = 0; i < setas.size();i++) {
            Seta& seta = *setas[i];
			//cout << inmortalTimer << endl;
            if (checkCollision(seta.getPosition(), glm::ivec2(16, 16)) && !godMode && inmortalTimer <= 0) {
                cout << "Collision with seta" << endl;
                if (isRightFacing()) {
                    sprite->changeAnimation(PLORANT_DRETA);
                }
                else if (isLeftFacing()) {
                    sprite->changeAnimation(PLORANT_ESQUERRA);
                }
				if (plorantTimer <= 0) {
					plorantTimer = 1000;
					this->takeDamage(0.66f);
				}
                
            }
        }

        

        for (int i = 0; i < fenixes.size(); i++) {
			Fenix& fenix = *fenixes[i];
            if (checkCollision(fenix.getPosition(), glm::ivec2(32, 16)) && !godMode && inmortalTimer <= 0) {
                cout << "Collision with fenix" << endl;
                if (isRightFacing()) {
                    sprite->changeAnimation(PLORANT_DRETA);
                }
                else if (isLeftFacing()) {
                    sprite->changeAnimation(PLORANT_ESQUERRA);
                }
                if (plorantTimer <= 0) {
                    plorantTimer = 1000;
                    this->takeDamage(0.66f);
                }
            }

            if (checkCollision(fenix.getPosFoc(), glm::ivec2(16, 16)) && !godMode && inmortalTimer <= 0) {
                cout << "Collision with fenix fire" << endl;
                if (isRightFacing()) {
                    sprite->changeAnimation(PLORANT_DRETA);
                }
                else if (isLeftFacing()) {
                    sprite->changeAnimation(PLORANT_ESQUERRA);
                }
                if (plorantTimer <= 0) {
                    plorantTimer = 1000;
                    this->takeDamage(0.33f);
                }
            }
        }
        

		if (checkCollision(mag.getPosition(), glm::ivec2(32, 32)) && !godMode && inmortalTimer <= 0) {
			cout << "Collision with mag" << endl;
			if (isRightFacing()) {
				sprite->changeAnimation(PLORANT_DRETA);
			}
			else if (isLeftFacing()) {
				sprite->changeAnimation(PLORANT_ESQUERRA);
			}
            if (plorantTimer <= 0) {
                plorantTimer = 1000;
                this->takeDamage(0.66f);
            }
		}

		if (checkCollision(mag2.getPosition(), glm::ivec2(32, 32)) && !godMode && inmortalTimer <= 0) {
			cout << "Collision with mag2" << endl;
			if (isRightFacing()) {
				sprite->changeAnimation(PLORANT_DRETA);
			}
			else if (isLeftFacing()) {
				sprite->changeAnimation(PLORANT_ESQUERRA);
			}
            if (plorantTimer <= 0) {
                plorantTimer = 1000;
                this->takeDamage(0.66f);
            }
		}

		if (checkCollision(mag.getPosProjectile(), glm::ivec2(8, 8)) && !godMode && inmortalTimer <= 0) {
			cout << "Collision with mag projectile" << endl;
			if (isRightFacing()) {
				sprite->changeAnimation(PLORANT_DRETA);
			}
			else if (isLeftFacing()) {
				sprite->changeAnimation(PLORANT_ESQUERRA);
			}
            if (plorantTimer <= 0) {
                plorantTimer = 1000;
                this->takeDamage(0.33f);
            }
		}

		if (checkCollision(mag2.getPosProjectile(), glm::ivec2(8, 8)) && !godMode && inmortalTimer <= 0) {
			cout << "Collision with mag2 projectile" << endl;
			if (isRightFacing()) {
				sprite->changeAnimation(PLORANT_DRETA);
			}
            else if (isLeftFacing()) {
                sprite->changeAnimation(PLORANT_ESQUERRA);
            }
            if (plorantTimer <= 0) {
                plorantTimer = 1000;
                this->takeDamage(0.33f);
            }
        }


        if (isAttacking || atacantAbaix || atacantAdalt) {
            for (int i = 0; i < setas.size();i++) {
                Seta& seta = *setas[i];
                if (checkCollisionLanza(seta.getPosition(), glm::ivec2(16, 16))) {
                    seta.restarVida();
                }
            }

            
			for (int i = 0; i < fenixes.size(); i++) {
				Fenix& fenix = *fenixes[i];
				if (checkCollisionLanza(fenix.getPosition(), glm::ivec2(16, 16))) {
					fenix.restarVida();
				}
			}
            if (checkCollisionLanza(mag.getPosition(), glm::ivec2(32, 32))) {
                mag.restarVida();
            }
			if (checkCollisionLanza(mag2.getPosition(), glm::ivec2(32, 32))) {
				mag2.restarVida();
			}


        }
    }

    for (int i = 0; i < fenixes.size(); i++) {
        Fenix& fenix = *fenixes[i];
        fenix.getPosPlayer(posPlayer);
    }
    mag.getPosPlayer(posPlayer);
	mag2.getPosPlayer(posPlayer);
    for (int i = 0; i < setas.size(); i++) {
        Seta& seta = *setas[i];
		seta.getPosPlayer(posPlayer);
    }

    // Update sprite positions
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    lanza->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    lanzaAdalt->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    lanzaAbaix->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
	totemFoc->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x + offsetTotem), float(tileMapDispl.y + posLanza.y + 10)));
}

void Player::handleHorizontalMovement()
{
    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
        if (Game::instance().getKey(GLFW_KEY_X)) {
            isAttacking = true;
            if (sprite->animation() != ATK_LEFT_MOVING)
                sprite->changeAnimation(ATK_LEFT_MOVING);
        }
        else if (sprite->animation() != MOVE_LEFT)
            sprite->changeAnimation(MOVE_LEFT);

        posPlayer.x -= MOVE_SPEED;
        if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) {
            posPlayer.x += MOVE_SPEED;
            sprite->changeAnimation(STAND_LEFT);
        }
    }
    else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
        if (Game::instance().getKey(GLFW_KEY_X)) {
            isAttacking = true;
            if (sprite->animation() != ATK_RIGHT_MOVING)
                sprite->changeAnimation(ATK_RIGHT_MOVING);
        }
        else if (sprite->animation() != MOVE_RIGHT)
            sprite->changeAnimation(MOVE_RIGHT);

        posPlayer.x += MOVE_SPEED;
        if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
            posPlayer.x -= MOVE_SPEED;
            sprite->changeAnimation(STAND_RIGHT);
        }
    }
    else if (Game::instance().getKey(GLFW_KEY_X)) {
        isAttacking = true;
        if (sprite->animation() == STAND_RIGHT || sprite->animation() == ATK_RIGHT_MOVING)
            sprite->changeAnimation(ATK_RIGHT_STANDING);
        else if (sprite->animation() == STAND_LEFT || sprite->animation() == ATK_LEFT_MOVING)
            sprite->changeAnimation(ATK_LEFT_STANDING);
    }
    else if (!Game::instance().getKey(GLFW_KEY_UP) && !Game::instance().getKey(GLFW_KEY_DOWN)) {
        setIdleAnimation();
    }
}

void Player::handleVerticalKeys()
{
    if (Game::instance().getKey(GLFW_KEY_UP)) {
		if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y -= MOVE_SPEED;
			//animacio
		}
		else {
			if (Game::instance().getKey(GLFW_KEY_X)) {
				if (sprite->animation() == LOOK_UP_R)
					sprite->changeAnimation(ATK_RIGHT_STANDING);
				else if (sprite->animation() == LOOK_UP_L)
					sprite->changeAnimation(ATK_LEFT_STANDING);
			}
			else if (Game::instance().getKey(GLFW_KEY_Z)) {
				if (sprite->animation() == LOOK_UP_R)
					sprite->changeAnimation(ATK_JUMPING_UP_R);
				else if (sprite->animation() == LOOK_UP_L)
					sprite->changeAnimation(ATK_JUMPING_UP_L);
			}
			else {
				if (isRightFacing())
					sprite->changeAnimation(LOOK_UP_R);
				else if (isLeftFacing())
					sprite->changeAnimation(LOOK_UP_L);
			}
		}
        
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y += MOVE_SPEED;
            //animacio
        }
        else {
            if (Game::instance().getKey(GLFW_KEY_X)) {
                if (sprite->animation() == LOOK_DOWN_R)
                    sprite->changeAnimation(ATK_RIGHT_DOWN);
                else if (sprite->animation() == LOOK_DOWN_L)
                    sprite->changeAnimation(ATK_LEFT_DOWN);
            }
            else if (Game::instance().getKey(GLFW_KEY_Z)) {
                if (sprite->animation() == LOOK_DOWN_R)
                    sprite->changeAnimation(ATK_JUMPING_DOWN_R);
                else if (sprite->animation() == LOOK_DOWN_L)
                    sprite->changeAnimation(ATK_JUMPING_DOWN_L);
            }
            else {
                if (isRightFacing())
                    sprite->changeAnimation(LOOK_DOWN_R);
                else if (isLeftFacing())
                    sprite->changeAnimation(LOOK_DOWN_L);
            }
        }
       
    }
}

void Player::handleJumpingAndFalling()
{
    if (bJumping) {
        saltarPlata = false;
        handleJumpingAnimations();

        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180) {
            bJumping = false;
            posPlayer.y = startY;
        }
        else {
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

            if (jumpAngle < 90) {
                if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
                    bJumping = false;
                    jumpAngle = 180;
                }
            }
            else {
                bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
            }
        }
    }
    else if (platform) {
        // 🔹 Si está sobre una plataforma, no aplicar gravedad
        if (Game::instance().getKey(GLFW_KEY_Z)) {
            platform = false;
            cout << "Salto\n";
            bJumping = true;
            jumpAngle = 0;
            startY = posPlayer.y;
            saltarPlata = true;
        }
    }
    else if (!levitar) {
        // 🔹 Aplica gravedad normalmente
        posPlayer.y += FALL_STEP;
        if (map->collisionLava(posPlayer, glm::ivec2(32.f, 32.f))) {
            lava = true;
        }
        else lava = false;
            if (lava && !godMode && inmortalTimer <= 0) {
                if (plorantTimer <= 0) {
                    plorantTimer = 1000;
                    this->takeDamage(0.33f);
                }
            }
        if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
            if (Game::instance().getKey(GLFW_KEY_Z)) {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
                saltarPlata = false;
            }
            saltarPlata = false;
        }
        else {
            handleFallingAnimations();
            saltarPlata = false;
        }
    }
}


void Player::handleJumpingAnimations()
{
    if (Game::instance().getKey(GLFW_KEY_UP)) {
		if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y -= MOVE_SPEED;
		}
		else {
            if (isRightFacing())
                sprite->changeAnimation(ATK_JUMPING_UP_R);
            else if (isLeftFacing())
                sprite->changeAnimation(ATK_JUMPING_UP_L);
		}
       
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
		if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y += MOVE_SPEED;
		}
		else {
			if (isRightFacing())
				sprite->changeAnimation(ATK_JUMPING_DOWN_R);
			else if (isLeftFacing())
				sprite->changeAnimation(ATK_JUMPING_DOWN_L);
		}
        
	}
	else if (Game::instance().getKey(GLFW_KEY_X)) {
		if (isRightFacing())
			sprite->changeAnimation(ATK_RIGHT_DOWN);
		else if (isLeftFacing())
			sprite->changeAnimation(ATK_LEFT_DOWN);
	}
    else {
        if (isLeftFacing())
            sprite->changeAnimation(LOOK_DOWN_L);
        else if (isRightFacing())
            sprite->changeAnimation(LOOK_DOWN_R);
    }
}

void Player::handleFallingAnimations()
{
    if (Game::instance().getKey(GLFW_KEY_UP)) {
		if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y -= MOVE_SPEED;
		}
		else {
			if (isRightFacing())
				sprite->changeAnimation(ATK_JUMPING_UP_R);
			else if (isLeftFacing())
				sprite->changeAnimation(ATK_JUMPING_UP_L);
		}
        
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
		if (map->isOnLadder(posPlayer, glm::ivec2(32, 32))) {
			posPlayer.y += MOVE_SPEED;
		}
		else {
            if (isRightFacing())
                sprite->changeAnimation(ATK_JUMPING_DOWN_R);
            else if (isLeftFacing())
                sprite->changeAnimation(ATK_JUMPING_DOWN_L);
		}
       
    }
	else if (Game::instance().getKey(GLFW_KEY_X)) {
		if (isRightFacing())
			sprite->changeAnimation(ATK_RIGHT_DOWN);
		else if (isLeftFacing())
			sprite->changeAnimation(ATK_LEFT_DOWN);
	}
    else {
        if (isLeftFacing())
            sprite->changeAnimation(LOOK_DOWN_L);
        else if (isRightFacing())
            sprite->changeAnimation(LOOK_DOWN_R);
    }
}

bool Player::isRightFacing() const
{
    int anim = sprite->animation();
    return anim == STAND_RIGHT || anim == MOVE_RIGHT || anim == LOOK_UP_R ||
        anim == LOOK_DOWN_R || anim == ATK_RIGHT_STANDING || anim == ATK_RIGHT_MOVING ||
		anim == ATK_RIGHT_DOWN || anim == ATK_JUMPING_UP_R || anim == ATK_JUMPING_DOWN_R || anim == PLORANT_DRETA || anim == ATK_ESCALANT_R;
}

bool Player::isLeftFacing() const
{
    int anim = sprite->animation();
    return anim == STAND_LEFT || anim == MOVE_LEFT || anim == LOOK_UP_L ||
        anim == LOOK_DOWN_L || anim == ATK_LEFT_STANDING || anim == ATK_LEFT_MOVING ||
		anim == ATK_LEFT_DOWN || anim == ATK_JUMPING_UP_L || anim == ATK_JUMPING_DOWN_L || anim == PLORANT_ESQUERRA || anim == ATK_ESCALANT_L;
}

void Player::setIdleAnimation()
{
    if (isLeftFacing())
        sprite->changeAnimation(STAND_LEFT);
    else if (isRightFacing())
        sprite->changeAnimation(STAND_RIGHT);
}

void Player::render()
{
    // Render hitboxes
    renderHitbox(posPlayer, glm::ivec2(32, 32)); // Player hitbox
    renderHitbox(posLanza, glm::ivec2(32, 32));
	frameCount++;
    // Render the player sprite
    if (plorantTimer <= 0 || (frameCount % 2 == 1) || (inmortalTimer<=0 && plorantTimer<=0)) {
        renderHitbox(posPlayer, glm::ivec2(32, 32)); // Player hitbox
        sprite->render();
    }

    // Render the weapon sprites if attacking
    if (atacantAdalt) {
        lanzaAdalt->render();
        renderHitbox(posLanza, glm::ivec2(32, 32));  // Lanza hitbox
    }
    else if (atacantAbaix) {
        lanzaAbaix->render();
        renderHitbox(posLanza, glm::ivec2(32, 32));  // Lanza hitbox
    }
    else if (isAttacking) {
        renderHitbox(posLanza, glm::ivec2(32, 32));
        lanza->render();
		if (totemFocActiu) totemFoc->render();
    }



    

}


//no funciona ns pq
void Player::renderHitbox(const glm::vec2& position, const glm::ivec2& size)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(tileMapDispl.x+position.x, tileMapDispl.y+position.y);
    glVertex2f(tileMapDispl.x+position.x + size.x, tileMapDispl.y+position.y);
    glVertex2f(tileMapDispl.x+position.x + size.x, tileMapDispl.y+position.y + size.y);
    glVertex2f(tileMapDispl.x+position.x, tileMapDispl.y+position.y + size.y);
    glEnd();

    glEnable(GL_TEXTURE_2D);
}


void Player::setTileMap(TileMap* tileMap)
{
    map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    posLanza = glm::vec2(posPlayer.x + 32, posPlayer.y);
}

glm::vec2 Player::getPosition() const
{
    return glm::vec2(posPlayer.x, posPlayer.y);
}

float Player::getLives() const {
    return lives;
}

glm::vec2 Player::getLanzaPosition() const
{
    return posLanza;
}

glm::ivec2 Player::getLanzaSize() const
{
    return glm::ivec2(32, 32);
}

bool Player::checkCollision(const glm::vec2& pos, const glm::ivec2& size) const
{
    glm::ivec2 playerSize(32, 32);
    return (posPlayer.x < pos.x + size.x &&
        posPlayer.x + playerSize.x > pos.x &&
        posPlayer.y < pos.y + size.y &&
        posPlayer.y + playerSize.y > pos.y);
}

bool Player::checkCollisionLanza(const glm::vec2& pos, const glm::ivec2& size) const
{
    glm::ivec2 lanzaSize(32, 32);
    return (posLanza.x < pos.x + size.x &&
        posLanza.x + lanzaSize.x > pos.x &&
        posLanza.y < pos.y + size.y &&
        posLanza.y + lanzaSize.y > pos.y);
}

void Player::takeDamage(float damage) {
    if (lives <= 0) {
        lives = 0;
        return;
    }
    lives -= damage;
    if (lives < 0) lives = 0;
}

void Player::stopJump() {
    bJumping = false;
    jumpAngle = 180;
}

void Player::setPlatform(bool a) {
    platform = a;
}

bool Player::isJumping() {
    return bJumping;
}

bool Player::isJumpingPlat() {
    return saltarPlata;
}

void Player::heal1Live() {
    lives += 1;
    lives = min(lives, hMax);
}

void Player::healallLives() {
    lives = hMax;
}

int Player::getHmax() {
    return hMax;
}

void Player::actCalabaza() {
    calabazas += 1;
    if (calabazas == 2) {
        hMax++;
        this->heal1Live();
    }
    else if (calabazas == 4) {
        hMax++;
        this->heal1Live();
    }
    else if (calabazas == 6) {
        hMax++;
        this->heal1Live();
    }
}

int Player::getLamps() {
    return lamps;
}

void Player::actLamp() {
    lamps += 1;
}

void Player::restaLamp() {
	lamps -= 1;
}

void Player::setImmortalTimer(float t) {
    inmortalTimer = t;
}

void Player::checkImmortalTimer(int deltaTime) {
    if (inmortalTimer > 0) {
        inmortalTimer -= deltaTime;
    }
    else {
        inmortalTimer = 0;
		this->setCapaActiva(false);
    }
}

void Player::setCapaActiva(bool capa) {
	capaActiva = capa;
    if(capaActiva) inmortalTimer = 5000;
	else inmortalTimer = 0;
}

bool Player::getCapaActiva() const {
	return capaActiva;
}

void Player::setPlorantTimer() {
	plorantTimer = 1000;
}


bool Player::playerIsPlorant() {
	if (plorantTimer > 0) return true;
	return false;
}

void Player::handleLadderMovement()
{
    // When on ladder, player can only move up and down
    if (Game::instance().getKey(GLFW_KEY_UP)) {
        // Move up on ladder
        posPlayer.y -= MOVE_SPEED;
        if (sprite->animation() != ESCALANT)
            sprite->changeAnimation(ESCALANT);
        // Set appropriate animation

    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        // Move down on ladder
        posPlayer.y += MOVE_SPEED;
        if (sprite->animation() != ESCALANT)
            sprite->changeAnimation(ESCALANT);
        // Set appropriate animation

    }
    else {
        // If not moving, use idle animation
        sprite->changeAnimation(ESCALANT);
    }

    // Allow attacking to the sides while on ladder
    if (Game::instance().getKey(GLFW_KEY_X)) {
        isAttacking = true;

        sprite->changeAnimation(ATK_ESCALANT_R);
        posLanza = glm::vec2(posPlayer.x + 26, posPlayer.y);

        if (totemFocActiu) {
            lanza->changeAnimation(0);
            if (totemFoc->animation() != 1) totemFoc->changeAnimation(1);
            offsetTotem = 8;
        }
        else {
            if (lanza->animation() != 0) lanza->changeAnimation(0);
        }

        posLanza = glm::vec2(posPlayer.x + 26, sprite->animation() == ATK_RIGHT_DOWN ? posPlayer.y + 8 : posPlayer.y);




    }
    else {
        isAttacking = false;
    }


    if (Game::instance().getKey(GLFW_KEY_LEFT)) {
        if (Game::instance().getKey(GLFW_KEY_X)) {
            /*isAttacking = true;

            sprite->changeAnimation(ATK_ESCALANT_L);
            if (totemFocActiu) {
                lanza->changeAnimation(1);
                if (totemFoc->animation() != 0) totemFoc->changeAnimation(0);

                offsetTotem = 8;
            }
            else {
                if (lanza->animation() != 1) lanza->changeAnimation(1);
            }

            posLanza = glm::vec2(posPlayer.x - 27, sprite->animation() == ATK_LEFT_DOWN ? posPlayer.y + 8 : posPlayer.y);*/

            isAttacking = true;

            sprite->changeAnimation(ATK_ESCALANT_L);
            if (totemFocActiu) {
                lanza->changeAnimation(0);
                if (totemFoc->animation() != 0) totemFoc->changeAnimation(0);
                offsetTotem = 8;
            }
            else {
                if (lanza->animation() != 1) lanza->changeAnimation(1);

            }

            posLanza = glm::vec2(posPlayer.x - 27, sprite->animation() == ATK_RIGHT_DOWN ? posPlayer.y + 8 : posPlayer.y);



        }
        else {
            isAttacking = false;
        }

    }
}
bool Player::isGod() {
	return godMode;
}