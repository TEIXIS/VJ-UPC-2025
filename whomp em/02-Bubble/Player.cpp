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
    ATK_JUMPING_UP_R, ATK_JUMPING_DOWN_R, ATK_JUMPING_UP_L, ATK_JUMPING_DOWN_L
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
    bJumping = false;
    spritesheet.loadFromFile("images/whomp_em_usa.png", TEXTURE_PIXEL_FORMAT_RGBA);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(18);

    lanza = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanza->setNumberAnimations(2);

    lanzaAbaix = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAbaix->setNumberAnimations(1);

    lanzaAdalt = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
    lanzaAdalt->setNumberAnimations(1);

    //dreta
    lanza->setAnimationSpeed(0, 8);
    lanza->addKeyframe(0, glm::vec2(0.25f, 0.375f));
    lanza->addKeyframe(0, glm::vec2(0.5f, 0.375f));
    lanza->addKeyframe(0, glm::vec2(0.75f, 0.375f));

    //esquerra
    lanza->setAnimationSpeed(1, 8);
    lanza->addKeyframe(1, glm::vec2(0.375f, 0.375f));
    lanza->addKeyframe(1, glm::vec2(0.625f, 0.375f));
    lanza->addKeyframe(1, glm::vec2(0.875f, 0.375f));

    //adalt
    lanzaAdalt->setAnimationSpeed(0, 8);
    lanzaAdalt->addKeyframe(0, glm::vec2(0.5f, 0.625f));

    //abaix
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

    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

    lanza->changeAnimation(0);
    lanza->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

    lanzaAbaix->changeAnimation(0);
    lanzaAbaix->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
    lanzaAdalt->changeAnimation(0);
    lanzaAdalt->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
    lanza->update(deltaTime);
    lanzaAbaix->update(deltaTime);
    lanzaAdalt->update(deltaTime);


    // Handle horizontal movement
    handleHorizontalMovement();

    // Handle vertical keys
    handleVerticalKeys();

    // Handle jumping and falling
    handleJumpingAndFalling();

    if (!Game::instance().getKey(GLFW_KEY_X)) isAttacking = false;
    else {
        if (isRightFacing()) {
            lanza->changeAnimation(0);
            if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                posLanza = glm::vec2(posPlayer.x + 26, posPlayer.y + 8);
            }
            else posLanza = glm::vec2(posPlayer.x + 26, posPlayer.y);
        }

        else if (isLeftFacing()) {
            lanza->changeAnimation(1);
            if (Game::instance().getKey(GLFW_KEY_DOWN)) {
                posLanza = glm::vec2(posPlayer.x - 27, posPlayer.y + 8);
            }
            else posLanza = glm::vec2(posPlayer.x - 27, posPlayer.y);
        }
    }

    if (sprite->animation() == ATK_JUMPING_UP_R) {
        atacantAdalt = true;
        posLanza = glm::vec2(posPlayer.x + 6, posPlayer.y - 32);
    }
    else if (sprite->animation() == ATK_JUMPING_UP_L) {
        atacantAdalt = true;
        posLanza = glm::vec2(posPlayer.x, posPlayer.y - 32);
    }
    else atacantAdalt = false;

    if (sprite->animation() == ATK_JUMPING_DOWN_R) {
        atacantAbaix = true;
        posLanza = glm::vec2(posPlayer.x + 7, posPlayer.y + 25);
    }
    else if (sprite->animation() == ATK_JUMPING_DOWN_L) {
        atacantAbaix = true;
        posLanza = glm::vec2(posPlayer.x, posPlayer.y + 25);
    }
    else
    {
        atacantAbaix = false;
    }


    // Update sprite position
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

    lanza->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

    lanzaAdalt->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

    lanzaAbaix->setPosition(glm::vec2(float(tileMapDispl.x + posLanza.x), float(tileMapDispl.y + posLanza.y)));

}





void Player::handleHorizontalMovement()
{
    if (Game::instance().getKey(GLFW_KEY_LEFT))
    {
        if (Game::instance().getKey(GLFW_KEY_X)) {
            isAttacking = true;
            if (sprite->animation() != ATK_LEFT_MOVING)
                sprite->changeAnimation(ATK_LEFT_MOVING);
        }
        else if (sprite->animation() != MOVE_LEFT)
            sprite->changeAnimation(MOVE_LEFT);

        posPlayer.x -= MOVE_SPEED;
        if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
        {
            posPlayer.x += MOVE_SPEED;
            sprite->changeAnimation(STAND_LEFT);
        }
    }
    else if (Game::instance().getKey(GLFW_KEY_RIGHT))
    {
        if (Game::instance().getKey(GLFW_KEY_X)) {
            isAttacking = true;
            if (sprite->animation() != ATK_RIGHT_MOVING)
                sprite->changeAnimation(ATK_RIGHT_MOVING);
        }
        else if (sprite->animation() != MOVE_RIGHT)
            sprite->changeAnimation(MOVE_RIGHT);

        posPlayer.x += MOVE_SPEED;
        if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
        {
            posPlayer.x -= MOVE_SPEED;
            sprite->changeAnimation(STAND_RIGHT);
        }
    }
    else if (Game::instance().getKey(GLFW_KEY_X))
    {
        isAttacking = true;
        if (sprite->animation() == STAND_RIGHT || sprite->animation() == ATK_RIGHT_MOVING)
            sprite->changeAnimation(ATK_RIGHT_STANDING);
        else if (sprite->animation() == STAND_LEFT || sprite->animation() == ATK_LEFT_MOVING)
            sprite->changeAnimation(ATK_LEFT_STANDING);
    }
    else if (!Game::instance().getKey(GLFW_KEY_UP) && !Game::instance().getKey(GLFW_KEY_DOWN))
    {
        setIdleAnimation();
    }
}

void Player::handleVerticalKeys()
{
    if (Game::instance().getKey(GLFW_KEY_UP))
    {
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
    else if (Game::instance().getKey(GLFW_KEY_DOWN))
    {
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
    if (bJumping)
    {
        handleJumpingAnimations();

        jumpAngle += JUMP_ANGLE_STEP;
        if (jumpAngle == 180)
        {
            bJumping = false;
            posPlayer.y = startY;
        }
        else
        {
            posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

            if (jumpAngle < 90)
            {
                if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
                {
                    bJumping = false;
                    jumpAngle = 180; // fuerza que baje directo
                }
            }
            else
            {
                bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
            }
        }
    }
    else
    {
        // Apply gravity
        posPlayer.y += FALL_STEP;
        if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
        {
            if (Game::instance().getKey(GLFW_KEY_Z))
            {
                bJumping = true;
                jumpAngle = 0;
                startY = posPlayer.y;
            }
        }
        else
        {
            // We're falling, set appropriate animations
            handleFallingAnimations();
        }
    }
}

void Player::handleJumpingAnimations()
{
    if (Game::instance().getKey(GLFW_KEY_UP))
    {
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_L);
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN))
    {
        if (sprite->animation() == LOOK_DOWN_R)
            sprite->changeAnimation(ATK_JUMPING_DOWN_R);
        else if (sprite->animation() == LOOK_DOWN_L)
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
    if (Game::instance().getKey(GLFW_KEY_UP))
    {
        if (isRightFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_R);
        else if (isLeftFacing())
            sprite->changeAnimation(ATK_JUMPING_UP_L);
    }
    else if (Game::instance().getKey(GLFW_KEY_DOWN))
    {
        if (sprite->animation() == LOOK_DOWN_R)
            sprite->changeAnimation(ATK_JUMPING_DOWN_R);
        else if (sprite->animation() == LOOK_DOWN_L)
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
        anim == ATK_RIGHT_DOWN || anim == ATK_JUMPING_UP_R || anim == ATK_JUMPING_DOWN_R;
}

bool Player::isLeftFacing() const
{
    int anim = sprite->animation();
    return anim == STAND_LEFT || anim == MOVE_LEFT || anim == LOOK_UP_L ||
        anim == LOOK_DOWN_L || anim == ATK_LEFT_STANDING || anim == ATK_LEFT_MOVING ||
        anim == ATK_LEFT_DOWN || anim == ATK_JUMPING_UP_L || anim == ATK_JUMPING_DOWN_L;
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