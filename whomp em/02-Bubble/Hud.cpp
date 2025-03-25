#include "Hud.h"
#include "Game.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

HUD::HUD() {

}

enum HeartAnims
{
    FULL, MFULL, MEMPTY, EMPTY
};

void HUD::init(ShaderProgram& shaderProgram, Player* p)
{
    shader = &shaderProgram;
    player = p;

    heartTexture.loadFromFile("images/HUD.png", TEXTURE_PIXEL_FORMAT_RGBA);
    weaponTexture.loadFromFile("images/HUD.png", TEXTURE_PIXEL_FORMAT_RGBA); // arma actual

    heartSprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25f, 0.25f), &heartTexture, shader);
    weaponSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &weaponTexture, shader);
    heartSprite->setNumberAnimations(4);
    heartSprite->setAnimationSpeed(FULL, 8);
    heartSprite->addKeyframe(FULL, glm::vec2(0.f, 0.f));
    heartSprite->setAnimationSpeed(MFULL, 8);
    heartSprite->addKeyframe(MFULL, glm::vec2(0.25f, 0.f));
    heartSprite->setAnimationSpeed(MEMPTY, 8);
    heartSprite->addKeyframe(MEMPTY, glm::vec2(0.5f, 0.f));
    heartSprite->setAnimationSpeed(EMPTY, 8);
    heartSprite->addKeyframe(EMPTY, glm::vec2(0.75f, 0.f));

    weaponSprite->setNumberAnimations(1);
    weaponSprite->setAnimationSpeed(0, 8);
    weaponSprite->addKeyframe(0, glm::vec2(0.f, 0.25f));
}

void HUD::render()
{

    int numHearts = 4;
    float health = player->getLives();
    vector<int> x(numHearts);
    for (int i = 0; i < numHearts; ++i) {
        if (health >= 1.f) {
            x[i] = 1;
            health -= 1;
        }
        else if (health >= 0.50f && health <= 0.75) {
            x[i] = 2;
            health = 0.f;
        }
        else if (health >= 0.25f && health <= 0.40) {
            x[i] = 3;
            health = 0.f;
        }
        else x[i] = 4;
    }
    for (int i = 0; i < numHearts; ++i) {
        heartSprite->setPosition(glm::vec2(10.f, 29.f + i * 8.f));
        if(x[i] == 1) heartSprite->changeAnimation(FULL);
        if (x[i] == 2) heartSprite->changeAnimation(MFULL);
        if (x[i] == 3) heartSprite->changeAnimation(MEMPTY);
        if (x[i] == 4) heartSprite->changeAnimation(EMPTY);
        heartSprite->render();
    }

    // arma al lado de los corazones (alineada al primero)
    weaponSprite->setPosition(glm::vec2(10.f, 10.f));
    weaponSprite->changeAnimation(0);
    weaponSprite->render();
}
