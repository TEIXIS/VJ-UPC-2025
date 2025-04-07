#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Hud.h"
#include "Platform1.h"
#include "Platform2.h"

#define SCREEN_X 32
#define SCREEN_Y 16

//#define INIT_PLAYER_X_TILES 2
#define INIT_PLAYER_X_TILES 16
#define INIT_PLAYER_Y_TILES 97

Scene::Scene()
{
    map = NULL;
    player = NULL;
}

Scene::~Scene()
{
    if (map != NULL)
        delete map;
    if (player != NULL)
        delete player;
}

void Scene::init()
{
    initShaders();
    map = TileMap::createTileMap("levels/Level.csv", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    player = new Player();
    plataforma = new Platform1();
    glm::vec2 plataformaPos = glm::vec2((INIT_PLAYER_X_TILES + 5) * 16, (INIT_PLAYER_Y_TILES ) * 16);
    plataforma->init(plataformaPos, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y));

    plataforma2 = new Platform2();
    glm::vec2 plataformaPos2 = glm::vec2((INIT_PLAYER_X_TILES + 10) * 16, (INIT_PLAYER_Y_TILES-1) * 16);
    plataforma2->init(plataformaPos2, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), 40.f);


    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    player->setTileMap(map);
    hud = new HUD();
    hud->init(texProgram, player);

	seta = new Seta();
	seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	fenix = new Fenix();
	fenix->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    
	//enemy->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 26 * map->getTileSize()));
    seta->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    seta->setTileMap(map);

    fenix->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 8) * 16, (INIT_PLAYER_Y_TILES) * 16));
    fenix->setTileMap(map);

    projection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
    currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
    currentTime += deltaTime;
    seta->update(deltaTime);
	fenix->update(deltaTime);
    map->update(deltaTime / 1000.f);  // convertir ms → s
    glm::vec2 playerPos = player->getPosition();
    int posNy = int(playerPos.y);
    int* ptr = &posNy;  // puntero que apunta a la variable
    float deltaY = plataforma2->getLastDeltaY();
    if (plataforma->checkCollisionFromAbove(*player)) {
        cout << "coll\n";
        cout << player->isJumpingPlat() << endl;
        // Si el jugador está encima, se mueve la plataforma hacia abajo.
        if (!map->collisionMoveDown(playerPos, glm::ivec2(32, 32), ptr)) plataforma->moveWithCharacter(1.0f, *player);
        else {
            player->setPosition(glm::ivec2(playerPos.x, playerPos.y-1.0f));
        }
        //player->stopJump();// Puedes ajustar el valor según tu lógica.
        //cout << "Down\n";
        if (!player->isJumpingPlat()) {
            cout << "Me poso\n";
            player->stopJump();
        }

        player->setPlatform(true);
    }
    else if (plataforma->checkCollisionFromBelow(*player)) {
        // Si se detecta colisión desde abajo, puedes evitar que el jugador suba o bloquear el movimiento.
        // Aquí podrías, por ejemplo, evitar que el jugador avance hacia arriba.
        // Esta parte dependerá de cómo gestiones la física y el movimiento del jugador.
        //cout << "Up\n";
        plataforma->returnToOriginalPosition(deltaTime);
        player->stopJump();
        cout << "DJG\n";
        //player->setPlatform(false);
    }
    else if(!plataforma->checkCollisionFromBelow(*player) && !plataforma->checkCollisionFromAbove(*player) && (!plataforma2->checkCollisionFromAbove(*player) && !plataforma2->checkCollisionFromBelow(*player))) {
        // Si no hay colisión, la plataforma vuelve gradualmente a su posición original.
        plataforma->returnToOriginalPosition(deltaTime);
        player->setPlatform(false);
    }
    else if (plataforma2->checkCollisionFromAbove(*player)) {
        if (!player->isJumpingPlat()) {
            player->stopJump();
            cout << "nepe\n";
        }
        player->setPlatform(true);
        glm::vec2 playerPos = player->getPosition();
        deltaY *= 0.8;
        player->setPosition(playerPos + glm::vec2(0.f, deltaY));
    }
    else if (plataforma2->checkCollisionFromBelow(*player)) {
        player->stopJump();
    }
    else {
        player->setPlatform(false);
    }
    plataforma2->update(deltaTime);
    player->update(deltaTime, *seta, *fenix);


    float centerX = SCREEN_WIDTH / 8.0f;
    float centerY = SCREEN_HEIGHT / 8.0f;

    glm::vec3 cameraPos = glm::vec3(playerPos.x - centerX, 1460, 0.0f); //canviar

	//printf("Player position: %f, %f\n", playerPos.x, playerPos.y);

    int mapWidth = SCREEN_WIDTH;
    int mapHeight = SCREEN_HEIGHT;

    //cameraPos.x = glm::max(0.0f, cameraPos.x);
   // cameraPos.y = glm::max(0.0f, cameraPos.y);

   // cameraPos.x = glm::min(float(mapWidth - SCREEN_WIDTH / 2), cameraPos.x);
   // cameraPos.y = glm::min(float(mapHeight - SCREEN_HEIGHT / 2), cameraPos.y);

    view = glm::translate(glm::mat4(1.0f), -cameraPos);
}

void Scene::render()
{
    glm::mat4 modelview;

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniformMatrix4f("view", view);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    map->render();
    texProgram.use();
    // HUD usa coordenadas de pantalla
    
    
    player->render();
    seta->render();
	fenix->render();
    plataforma->render();
    plataforma2->render();


    glm::mat4 hudProjection = glm::ortho(0.f, float(SCREEN_WIDTH)/2, float(SCREEN_HEIGHT)/2, 0.f);
    texProgram.setUniformMatrix4f("projection", hudProjection);
    texProgram.setUniformMatrix4f("view", glm::mat4(1.0f));
    texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
    hud->render();
}

void Scene::initShaders()
{
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled())
    {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled())
    {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked())
    {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}
