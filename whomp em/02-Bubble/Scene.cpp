#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Hud.h"

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


    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    player->setTileMap(map);
    hud = new HUD();
    hud->init(texProgram, player);

	enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	//enemy->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 26 * map->getTileSize()));
    enemy->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
	enemy->setTileMap(map);

    projection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
    currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
    currentTime += deltaTime;
    player->update(deltaTime, *enemy);
	enemy->update(deltaTime);
    map->update(deltaTime / 1000.f);  // convertir ms → s


    glm::vec2 playerPos = player->getPosition();


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
	enemy->render();
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
