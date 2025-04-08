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
#define INIT_PLAYER_X_TILES 1
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



    spritesheet.loadFromFile("images/titol.png", TEXTURE_PIXEL_FORMAT_RGBA);
    pantallaTitol = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	pantallaTitol->setNumberAnimations(1);
	pantallaTitol->setAnimationSpeed(0, 1);
	pantallaTitol->addKeyframe(0, glm::vec2(0.f, 0.f));
    pantallaTitol->setPosition(glm::vec2(-500.f, -500.f));
	jocComencat = false;

    // Suponiendo que texProgram, SCREEN_X, SCREEN_Y están definidos y accesibles aquí

// Crear varias plataformas del tipo Platform1
    for (int i = 0; i < 5; ++i) {
        Platform1* plataforma = new Platform1();
        glm::vec2 plataformaPos = glm::vec2((1 + 100 + i * 4) * 16, (98) * 16+i*2);
        plataforma->init(plataformaPos, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y));
        plataformas1.push_back(plataforma);
    }

    // Crear varias plataformas del tipo Platform2
    for (int i = 0; i < 2; ++i) {
        Platform2* plataforma2 = new Platform2();
        glm::vec2 plataformaPos2 = glm::vec2((INIT_PLAYER_X_TILES + 150 + i * 6) * 16, (INIT_PLAYER_Y_TILES - 80) * 16);
        plataforma2->init(plataformaPos2, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), 40.f);
        plataformas2.push_back(plataforma2);
    }

    cor1 = new LittleHeart();
    cor1->init(glm::vec2((INIT_PLAYER_X_TILES + 10) * 16, (INIT_PLAYER_Y_TILES-5) * 16), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/heart1.png");

    cor2 = new BigHeart();
    cor2->init(glm::vec2((INIT_PLAYER_X_TILES + 20) * 16, (INIT_PLAYER_Y_TILES-5) * 16), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/heart2.png");

    calabaza1 = new Calabaza();
    calabaza1->init(glm::vec2((INIT_PLAYER_X_TILES + 25) * 16, (INIT_PLAYER_Y_TILES - 5) * 16), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/calabaza1.png");

    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    player->setTileMap(map);
    hud = new HUD();
    hud->init(texProgram, player);

	seta = new Seta();
	seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	fenix = new Fenix();
	fenix->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	mag = new Mag();
	mag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    
	//enemy->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), 26 * map->getTileSize()));
    seta->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    seta->setTileMap(map);

    //fenix->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 8) * 16, (INIT_PLAYER_Y_TILES) * 16));
    fenix->setTileMap(map);

	//mag->setPosition(glm::vec2((INIT_PLAYER_X_TILES) * 16, (INIT_PLAYER_Y_TILES + 2) * 16));
	mag->setTileMap(map);
    

    projection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
    currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	if (!jocComencat) {
		if (Game::instance().getKey(GLFW_KEY_SPACE)) {
			jocComencat = true;
		}
		return;
	}
    currentTime += deltaTime;
    seta->update(deltaTime);
	fenix->update(deltaTime);
	mag->update(deltaTime);
    map->update(deltaTime / 1000.f);  
	hud->update(deltaTime);
    
    glm::vec2 playerPos = player->getPosition();
    int posNy = int(playerPos.y);
    int* ptr = &posNy;  
    float deltaY = 0.f;
    bool collisionDetected = false;

    // Revisar colisiones desde arriba con plataformas1
    for (auto& plataforma : plataformas1) {
        if (plataforma->checkCollisionFromAbove(*player)) {
            if (!map->collisionMoveDown(playerPos, glm::ivec2(32, 32), ptr)) {
                plataforma->moveWithCharacter(1.0f, *player);
            }
            else {
                player->setPosition(glm::ivec2(playerPos.x, playerPos.y - 1.0f));
            }

            if (!player->isJumpingPlat()) {
                cout << "Me poso\n";
                player->stopJump();
            }

            player->setPlatform(true);
            collisionDetected = true;
        }
    }

    // Revisar colisiones desde abajo con plataformas1
    if (!collisionDetected) {
        for (auto& plataforma : plataformas1) {
            if (plataforma->checkCollisionFromBelow(*player)) {
                plataforma->returnToOriginalPosition(deltaTime);
                player->stopJump();
                cout << "DJG\n";
                collisionDetected = true;
                break;
            }
        }
    }

    // Si no hay colisión con ninguna plataforma1 ni plataforma2
    if (!collisionDetected) {
        bool anyCollision = false;
        for (auto& plataforma : plataformas1) {
            if (plataforma->checkCollisionFromAbove(*player) || plataforma->checkCollisionFromBelow(*player)) {
                anyCollision = true;
                break;
            }
        }
        for (auto& plataforma : plataformas2) {
            if (plataforma->checkCollisionFromAbove(*player) || plataforma->checkCollisionFromBelow(*player)) {
                anyCollision = true;
                break;
            }
        }

        if (!anyCollision) {
            for (auto& plataforma : plataformas1) plataforma->returnToOriginalPosition(deltaTime);
            for (auto& plataforma : plataformas2) plataforma->update(deltaTime);

            if (!plataformas2.empty())
                deltaY = plataformas2[0]->getLastDeltaY(); // Podrías sumar todos los deltaY si hay más
            player->setPlatform(false);
        }
    }

    // Revisar colisiones desde arriba con plataformas2
    for (auto& plataforma : plataformas2) {
        if (plataforma->checkCollisionFromAbove(*player)) {
            if (!player->isJumpingPlat()) {
                player->stopJump();
            }
            player->setPlatform(true);
            glm::vec2 currentPos = player->getPosition();
            plataforma->update(deltaTime);
            deltaY = plataforma->getLastDeltaY();
            player->setPosition(currentPos + glm::vec2(0.f, deltaY));
            collisionDetected = true;
        }
    }

    // Revisar colisiones desde abajo con plataformas2
    if (!collisionDetected) {
        for (auto& plataforma : plataformas2) {
            if (plataforma->checkCollisionFromBelow(*player)) {
                player->stopJump();
                for (auto& plataformaaux : plataformas2) plataformaaux->update(deltaTime);
                collisionDetected = true;
                break;
            }
        }
    }

    // Si no pasó nada con plataformas2, igual se actualizan
    if (!collisionDetected) {
        player->setPlatform(false);
        for (auto& plataforma : plataformas2) plataforma->update(deltaTime);
    }
    player->update(deltaTime, *seta, *fenix, *mag);
    cor1->applyGravity(deltaTime, map);
    if (!cor1->isCollected() && cor1->collidesWith(*player)) {
        cor1->onCollect(*player);
    }
    cor1->update(deltaTime);
    cor2->applyGravity(deltaTime, map);
    if (!cor2->isCollected() && cor2->collidesWith(*player)) {
        cor2->onCollect(*player);
    }
    cor2->update(deltaTime);
    calabaza1->applyGravity(deltaTime, map);
    if (!calabaza1->isCollected() && calabaza1->collidesWith(*player)) {
        calabaza1->onCollect(*player);
    }
    calabaza1->update(deltaTime);

    if (playerPos.x == 8*16) {
        mag->spawn();
        
    }
	if (playerPos.x == 78*16) {
		fenix->spawn();
		
	}
    if (playerPos.x == 0 * 16) {
        //seta->spawn();
    }

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
	if (!jocComencat) {
		pantallaTitol->render();
		return;
	}

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
    cor1->render();
    cor2->render();
    calabaza1->render();
    // HUD usa coordenadas de pantalla
    
    


	mag->render();
    player->render();
    seta->render();
	fenix->render();
    for (auto* p : plataformas1) p->render();
    for (auto* p : plataformas2) p->render();




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
