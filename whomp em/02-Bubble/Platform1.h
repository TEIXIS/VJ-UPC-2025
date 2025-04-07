#ifndef PLATFORM1_H
#define PLATFORM1_H

#include <glm/glm.hpp>
#include "Sprite.h"
#include "Player.h"
#include "ShaderProgram.h"

class Platform1 {
public:
    // Constructor: se inicializa sin parámetros, luego se inicializa con init.
    Platform1();

    // Destructor: se encarga de liberar la memoria del sprite.
    ~Platform1();

    // Inicializa el sprite de la plataforma con un shader dado.
    void init(const glm::vec2& position, ShaderProgram& shaderProgram, const glm::ivec2& tileMapOffset);

    // Renderiza el sprite de la plataforma.
    void render();

    // Verifica si el jugador colisiona "por arriba" de la plataforma.
    // Se asume que el jugador es de 32x32 píxeles.
    // collisionThreshold define el margen vertical para considerar la colisión.
    bool checkCollisionFromAbove(const Player& player, float collisionThreshold = 5.0f);

    // Verifica si el jugador colisiona "por abajo" de la plataforma.
    // Esto evitará que el jugador se meta por debajo y acceda a la plataforma desde abajo.
    bool checkCollisionFromBelow(const Player& player, float collisionThreshold = 5.0f);

    // Mueve la plataforma junto con el jugador (deltaY es el desplazamiento vertical).
    void moveWithCharacter(float deltaY, Player& player);

    // Hace que la plataforma regrese gradualmente a su posición original.
    void returnToOriginalPosition(float deltaTime);

    // Retorna la posición actual de la plataforma.
    glm::vec2 getPosition() const;

private:
    glm::vec2 pos;  
    glm::ivec2 tileMapDispl;// Posición actual de la plataforma.
    glm::vec2 originalPos;  // Posición original de la plataforma.
    Sprite* sprite;         // Sprite que representa la plataforma (32x16).
    Texture spritesheet;    // Textura para el sprite de la plataforma.
};

#endif
