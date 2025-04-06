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
    ATK_JUMPING_UP_R, ATK_JUMPING_DOWN_R, ATK_JUMPING_UP_L, ATK_JUMPING_DOWN_L, PLORANT_DRETA, PLORANT_ESQUERRA
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
    sprite->setNumberAnimations(20);

    // Initialize weapon sprites
    lanza = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanza->setNumberAnimations(2);

    lanzaAbaix = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAbaix->setNumberAnimations(1);

    lanzaAdalt = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAdalt->setNumberAnimations(1);

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

    godMode = false;
}

void Player::update(int deltaTime, Enemy& enemy)
{
    // Update all sprites
    sprite->update(deltaTime);
    lanza->update(deltaTime);
    lanzaAbaix->update(deltaTime);
    lanzaAdalt->update(deltaTime);

    static bool godModeKeyPressed = false;

    if (Game::instance().getKey(GLFW_KEY_G)) {
        if (!godModeKeyPressed) {
            godMode = !godMode;
            std::cout << "God mode: " << (godMode ? "ON" : "OFF") << std::endl;
            godModeKeyPressed = true;
        }
    } else {
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
        else {
            // Apply gravity
            posPlayer.y += FALL_STEP;
            if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
                if (Game::instance().getKey(GLFW_KEY_Z)) {
                    bJumping = true;
                    jumpAngle = 0;
                    startY = posPlayer.y;
                }
            }
        }
    }
    else {
        // Normal gameplay when not hurt

        // Handle horizontal movement
        handleHorizontalMovement();

        // Handle vertical keys
        handleVerticalKeys();

        // Handle jumping and falling
        handleJumpingAndFalling();

        // Handle weapon state
        if (!Game::instance().getKey(GLFW_KEY_X)) {
            isAttacking = false;
        }
        else {
            isAttacking = true;
            if (isRightFacing()) {
                if (lanza->animation() != 0) lanza->changeAnimation(0);
                posLanza = glm::vec2(posPlayer.x + 26, Game::instance().getKey(GLFW_KEY_DOWN) ? posPlayer.y + 8 : posPlayer.y);
            }
            else if (isLeftFacing()) {
                if (lanza->animation() != 1) lanza->changeAnimation(1);
                posLanza = glm::vec2(posPlayer.x - 27, Game::instance().getKey(GLFW_KEY_DOWN) ? posPlayer.y + 8 : posPlayer.y);
            }
        }

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

        // Check collisions with enemy
        if (checkCollision(enemy.getPosition(), glm::ivec2(16, 16)) && !godMode) {
            if (isRightFacing()) {
                sprite->changeAnimation(PLORANT_DRETA);
            }
            else if (isLeftFacing()) {
                sprite->changeAnimation(PLORANT_ESQUERRA);
            }
            plorantTimer = 500;
        }

        if (checkCollisionLanza(enemy.getPosition(), glm::ivec2(16, 16))) {
            enemy.restarVida();
        }
    }

    // Update sprite positions
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    lanza->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    lanzaAdalt->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    lanzaAbaix->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
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
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
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

void Player::handleJumpingAndFalling()
{
    if (bJumping) {
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
    else {
        // Apply gravity
        posPlayer.y += FALL_STEP;
        if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y)) {
            if (Game::instance().getKey(GLFW_KEY_Z)) {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
            }
        }
        else {
            handleFallingAnimations();
        }
    }
}

void Player::handleJumpingAnimations()
{
    if (Game::instance().getKey(GLFW_KEY_UP)) {
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_L);
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_DOWN_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_DOWN_L);
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
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_L);
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_DOWN_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_DOWN_L);
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
        anim == ATK_RIGHT_DOWN || anim == ATK_JUMPING_UP_R || anim == ATK_JUMPING_DOWN_R || anim == PLORANT_DRETA;
}

bool Player::isLeftFacing() const
{
    int anim = sprite->animation();
    return anim == STAND_LEFT || anim == MOVE_LEFT || anim == LOOK_UP_L ||
        anim == LOOK_DOWN_L || anim == ATK_LEFT_STANDING || anim == ATK_LEFT_MOVING ||
        anim == ATK_LEFT_DOWN || anim == ATK_JUMPING_UP_L || anim == ATK_JUMPING_DOWN_L || anim == PLORANT_ESQUERRA;
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
    sprite->render();
    if (isAttacking) {
        lanza->render();
    }
    if (atacantAdalt) {
        lanzaAdalt->render();
    }
    if (atacantAbaix) {
        lanzaAbaix->render();
    }
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