#include "Platform1.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

// Constructor: inicializa el puntero del sprite en nullptr.
Platform1::Platform1()
    : sprite(nullptr)
{
}

// Destructor: libera la memoria asignada al sprite.
Platform1::~Platform1() {
    if (sprite)
        delete sprite;
}

// Inicializa el sprite de la plataforma.
// Se carga una textura (por ejemplo, "images/platform1.png") y se crea un sprite de 32x16 píxeles.
void Platform1::init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset) {
    pos = position;
    originalPos = position;
    tileMapDispl = tileMapOffset;

    if (!spritesheet.loadFromFile("images/platform1.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cerr << "Error al cargar la textura de la plataforma." << std::endl;
    }

    sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
    sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
}

// Renderiza el sprite de la plataforma.
void Platform1::render() {
    if (sprite)
        sprite->render();
}

// Verifica si el jugador colisiona "por arriba" de la plataforma.
// Se comprueba que:
// - Exista superposición horizontal.
// - La parte superior del jugador esté por encima de la plataforma.
// - La parte inferior del jugador esté en un rango (pos.y hasta pos.y + collisionThreshold).
bool Platform1::checkCollisionFromAbove(const Player& player, float collisionThreshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);
    //std::cout << "Jugador x: " << playerPos.x << " Plataforma x: " << pos.x << std::endl;


    bool horizontalOverlap = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 24);

    // Aseguramos que el jugador esté efectivamente arriba de la plataforma.
    bool verticalCollision = (playerPos.y < pos.y) &&
        (playerPos.y + playerSize.y >= pos.y) &&
        (playerPos.y + playerSize.y <= pos.y + collisionThreshold);
    return horizontalOverlap && verticalCollision;
}

// Verifica si el jugador colisiona "por abajo" de la plataforma.
// Se comprueba que:
// - Exista superposición horizontal.
// - La parte superior del jugador esté dentro de un rango cercano a la parte inferior de la plataforma.
bool Platform1::checkCollisionFromBelow(const Player& player, float collisionThreshold) {
    glm::vec2 playerPos = player.getPosition();
    glm::ivec2 playerSize(32, 32);

    bool horizontalOverlap = (playerPos.x + playerSize.x > pos.x) && (playerPos.x < pos.x + 32);
    // La parte superior del jugador (playerPos.y) se encuentra cerca de la parte inferior de la plataforma (pos.y + 16).
    bool verticalCollision = (playerPos.y <= pos.y + 16) &&
        (playerPos.y >= pos.y + 16 - collisionThreshold);
    return horizontalOverlap && verticalCollision;
}

// Mueve la plataforma en el eje Y según el delta recibido y actualiza la posición del sprite.
void Platform1::moveWithCharacter(float deltaY, Player& player) {
    pos.y += deltaY;
    glm::vec2 playerPos = player.getPosition();
    if (sprite) {
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
        player.setPosition(glm::vec2(playerPos.x, playerPos.y + deltaY));
        player.setPlatform(true);  // nueva función

    }
}

// Hace que la plataforma regrese gradualmente a su posición original.
// deltaTime se espera en milisegundos; se convierte a segundos para el cálculo.
void Platform1::returnToOriginalPosition(float deltaTime) {
    float dt = deltaTime / 1000.f;
    float returnSpeed = 50.0f; // píxeles por segundo
    if (pos.y > originalPos.y) {
        pos.y -= returnSpeed * dt;
        if (pos.y < originalPos.y)
            pos.y = originalPos.y;
    }
    else if (pos.y < originalPos.y) {
        pos.y += returnSpeed * dt;
        if (pos.y > originalPos.y)
            pos.y = originalPos.y;
    }
    if (sprite) {
        sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));

    }
}

// Retorna la posición actual de la plataforma.
glm::vec2 Platform1::getPosition() const {
    return pos;
}
