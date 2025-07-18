﻿#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Hud.h"
#include "Platform1.h"
#include "Platform2.h"
#include "Llamarada.h"
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

#define SCREEN_X 32
#define SCREEN_Y 16

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
    if (soundEngine) {
        soundEngine->drop();
    }

}

void Scene::init()
{
    initShaders();
    map = TileMap::createTileMap("levels/Level.csv", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    player = new Player();
    soundEngine = irrklang::createIrrKlangDevice();

    jocComencat = false;
    showControls = false;
    spacePressed = false;
	showingCredits = false;
    counterW = -1;
    win = false;

	spritesheetCredits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pantallaCredits = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(1.f, 1.f), &spritesheetCredits, &texProgram);
	pantallaCredits->setNumberAnimations(1);
	pantallaCredits->setAnimationSpeed(0, 1);
	pantallaCredits->addKeyframe(0, glm::vec2(0.f, 0.f));
	pantallaCredits->setPosition(glm::vec2(0.f, 0.f));



	spritesheetWin.loadFromFile("images/winTitle.png", TEXTURE_PIXEL_FORMAT_RGBA);
    pantallaWin = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(1.f, 1.f), &spritesheetWin, &texProgram);
    pantallaWin->setNumberAnimations(1);
    pantallaWin->setAnimationSpeed(0, 1);
    pantallaWin->addKeyframe(0, glm::vec2(0.f, 0.f));
    pantallaWin->setPosition(glm::vec2(0.f, 0.f));

    spritesheet.loadFromFile("images/titol.png", TEXTURE_PIXEL_FORMAT_RGBA);
    pantallaTitol = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	pantallaTitol->setNumberAnimations(1);
	pantallaTitol->setAnimationSpeed(0, 1);
	pantallaTitol->addKeyframe(0, glm::vec2(0.f, 0.f));
    pantallaTitol->setPosition(glm::vec2(0.f, 0.f));

    spritesheetControls.loadFromFile("images/controls.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pantallaControles = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(1.f, 1.f), &spritesheetControls, &texProgram);
	pantallaControles->setNumberAnimations(1);
	pantallaControles->setAnimationSpeed(0, 1);
	pantallaControles->addKeyframe(0, glm::vec2(0.f, 0.f));
	pantallaControles->setPosition(glm::vec2(0.f, 0.f));



    glm::vec2 playerPos = player->getPosition();
    float centerX = SCREEN_WIDTH / 8.0f;
    glm::vec3 cameraPos = glm::vec3(playerPos.x - centerX, 1460, 0.0f); 
    view = glm::translate(glm::mat4(1.0f), -cameraPos);

    boss = new Boss();
    boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    boss->setTileMap(map);
    boss->setPosition(glm::vec2(3300, 300));


    for (int i = 0; i < 3; ++i) {
        Platform1* plataforma = new Platform1();
        glm::vec2 plataformaPos = glm::vec2((1 + 100 + i * 4) * 16, (98) * 16+i*2);
        plataforma->init(plataformaPos, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y));
        plataformas1.push_back(plataforma);
    }
    for (int i = 3; i < 5; ++i) {
        Platform1* plataforma = new Platform1();
        glm::vec2 plataformaPos = glm::vec2((1 + 104 + i * 4) * 16, (98-2) * 16 + i * 2);
        plataforma->init(plataformaPos, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y));
        plataformas1.push_back(plataforma);
    }


    for (int i = 0; i < 2; ++i) {
        Platform2* plataforma2 = new Platform2();
        glm::vec2 plataformaPos2 = glm::vec2((INIT_PLAYER_X_TILES + 158 + i * 9) * 16, (INIT_PLAYER_Y_TILES - 86) * 16);
        plataforma2->init(plataformaPos2, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), 60.f);
        plataformas2.push_back(plataforma2);
    }

    for (int i = 0; i < 2; ++i) {
        Platform2* plataforma2 = new Platform2();
        glm::vec2 plataformaPos2 = glm::vec2((INIT_PLAYER_X_TILES + 180 + i * 7) * 16, (INIT_PLAYER_Y_TILES - 86) * 16);
        plataforma2->init(plataformaPos2, texProgram, glm::ivec2(SCREEN_X, SCREEN_Y), 60.f);
        plataformas2.push_back(plataforma2);
    }
    
        Llamarada* flame = new Llamarada();
  
        glm::vec2 flamePos = glm::vec2((INIT_PLAYER_X_TILES + 136) * 16, (INIT_PLAYER_Y_TILES - 24) * 16);
        flame->init(flamePos, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Llamarada::UP);
        llamas.push_back(flame);

        Llamarada* flame1 = new Llamarada();
       
         flamePos = glm::vec2((INIT_PLAYER_X_TILES + 134) * 16, (INIT_PLAYER_Y_TILES - 27) * 16);
        flame1->init(flamePos, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Llamarada::UP);
        llamas.push_back(flame1);

        Llamarada* flame2 = new Llamarada();
   
         flamePos = glm::vec2((INIT_PLAYER_X_TILES + 132) * 16, (INIT_PLAYER_Y_TILES - 30) * 16);
        flame2->init(flamePos, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, Llamarada::DOWN);
        llamas.push_back(flame2);
    

    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 16, INIT_PLAYER_Y_TILES * 16));
    player->setTileMap(map);
    hud = new HUD();
    hud->init(texProgram, player);



	mag = new Mag();
	mag->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	mag2 = new Mag();
	mag2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	
    

	mag->setTileMap(map);
	mag2->setTileMap(map);
    

    projection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
    currentTime = 0.0f;

    for (int i = 0; i < 10; ++i) {
        Fenix* fen = new Fenix();
        fen->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
        fen->setTileMap(map);
        fenixes.push_back(fen);
    }

    for (int i = 0; i < 10; ++i) {
		Seta* seta = new Seta();
		seta->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		seta->setTileMap(map);
		setas.push_back(seta);
    }
}

void Scene::update(int deltaTime)
{
    glm::vec2 playerPos = player->getPosition();
    if (!jocComencat) {
        bool currentSpace = Game::instance().getKey(GLFW_KEY_SPACE);

        if (currentSpace && !spacePressed) {
            if (showControls) {
                jocComencat = true; 
                std::cout << "[DEBUG] Iniciando el nivel\n";
                if (soundEngine) soundEngine->play2D("media/sonidoCorrecto.wav", false);
                if (soundEngine) {
                    soundEngine->play2D("media/musica.wav", true);
                    soundEngine->play2D("media/volcan.wav", true); 
                }

            }
            else {
                showControls = true;  
                std::cout << "[DEBUG] Mostrando controles\n";
                if (soundEngine) soundEngine->play2D("media/sonidoCorrecto.wav", false); 
            }
        }


        spacePressed = currentSpace;

        return;  
    }
    else {
        if (Game::instance().getKey(GLFW_KEY_R)) {
            resetLevel();  
        }
        currentTime += deltaTime;
        mag->update(deltaTime);
        mag2->update(deltaTime);
        map->update(deltaTime / 1000.f);
        for (auto flame : llamas) {
            flame->update(deltaTime);
        }
        hud->update(deltaTime);

        for (auto& fenn : fenixes) {
            fenn->update(deltaTime);
        }
        for (auto& seta : setas) {
            seta->update(deltaTime);
        }

        
        int posNy = int(playerPos.y);
        int* ptr = &posNy;
        float deltaY = 0.f;
        bool collisionDetected = false;


        for (auto& plataforma : plataformas1) {
            if (plataforma->checkCollisionFromAbove(*player)) {
                if (!map->collisionMoveDown(playerPos, glm::ivec2(32, 32), ptr)) {
                    plataforma->moveWithCharacter(1.0f, *player);
                }
                else {
                    player->setPosition(glm::ivec2(playerPos.x, playerPos.y - 1.0f));
                }

                if (!player->isJumpingPlat()) {
                    player->stopJump();
                }

                player->setPlatform(true);
                collisionDetected = true;
            }
			else plataforma->returnToOriginalPosition(deltaTime); 
        }



        if (!collisionDetected) {
            bool anyCollision = false;
            for (auto& plataforma : plataformas1) {
                if (plataforma->checkCollisionFromAbove(*player)) {
                    anyCollision = true;
                    break;
                }
            }
            for (auto& plataforma : plataformas2) {
                if (plataforma->checkCollisionFromAbove(*player)) {
                    anyCollision = true;
                    break;
                }
            }

            if (!anyCollision) {
                for (auto& plataforma : plataformas1) plataforma->returnToOriginalPosition(deltaTime);
               

                if (!plataformas2.empty())
                    deltaY = plataformas2[0]->getLastDeltaY(); 
                player->setPlatform(false);
            }
        }

   
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
				playerPos = player->getPosition();
				if (map->collisionMoveDown(playerPos, glm::ivec2(32, 32), ptr)) {
					player->setPosition(glm::ivec2(playerPos.x, playerPos.y - 1.0f));
				}
                collisionDetected = true;
            }
			else plataforma->update(deltaTime);
        }

   

      
        if (!collisionDetected) {
            player->setPlatform(false);

        }


        player->update(deltaTime, setas, fenixes, *mag, *mag2, *boss);

        if (boss && boss->isAlive()) boss->update(deltaTime, *player);


        if (mag->spawnItem()) {
			spawnRandomCollectible(mag->getPositionSpawn());
        }
		if (mag2->spawnItem()) {
			spawnRandomCollectible(mag2->getPositionSpawn());
		}
		for (auto& seta : setas) {
			if (seta->spawnItem()) {
				spawnRandomCollectible(seta->getPositionSpawn());
			}
		}
		for (auto& fenix : fenixes) {
			if (fenix->spawnItem()) {
				spawnRandomCollectible(fenix->getPositionSpawn());
			}
		}
		for (auto& collectible : collectibles) {
			collectible->applyGravity(deltaTime,map);
			if (!collectible->isCollected() && collectible->collidesWith(*player)) {
				collectible->onCollect(*player);
                if (soundEngine) soundEngine->play2D("media/coin.wav", false);
			}
            collectible->update(deltaTime);
		}


        if (player->getLives() <= 0) {
            if (player->getLamps() == 0) resetLevel();
            else {
                player->healallLives();
                player->restaLamp();
            }
        }

        for (auto flame : llamas) {
            if (flame->collidesWithPlayer(*player) && !player->getCapaActiva() && !player->playerIsPlorant() && !player->isGod()) {
                player->setPlorantTimer();
                player->takeDamage(0.66f); 
            }
        }
        

        if (playerPos.x == 8 * 16) {
            mag->spawn(20, 99);

        }
        if (playerPos.x == 82 * 16) {
            fenixes[0]->spawn(94, 93);

        }
        if (playerPos.x == 86 * 16) {
            fenixes[1]->spawn(98, 93);
        }
        if (playerPos.x == 90 * 16) {
            fenixes[2]->spawn(100, 93);
        }
        if (playerPos.x == 94 * 16) {
            fenixes[3]->spawn(104, 93);
        }

        if (playerPos.x == 3040) {
            mag2->spawn(200, 9);
        }



        if (playerPos.y == 740) {
            setas[1]->spawn(133, 36);
        }
        if (playerPos.y == 574) {
            setas[2]->spawn(133, 30);
        }
        if (playerPos.y == 546) {
            setas[3]->spawn(136, 24);
        }
        if (playerPos.y == 442) {
            setas[4]->spawn(135, 17);
        }
        if (playerPos.y == 368) {
            setas[5]->spawn(135, 13);
            setas[6]->spawn(140, 13);
        }
        if (playerPos.y == 104) {
            fenixes[4]->spawn(141, 2);
        }
        if (playerPos.x == 2810) {
            fenixes[5]->spawn(185, 1);
            fenixes[6]->spawn(187, 3);
        }
        if (playerPos.x == 3214) {
            fenixes[7]->spawn(210, 1);
        }
        if (playerPos.x == 3278) {
            setas[7]->spawn(213, 2);
        }
		if (playerPos.x >= 3600) {
			player->isWithBoss(true);
		}
		else player->isWithBoss(false);
    }
    

    if (player->winGame()) {
        if (!win) {

            std::cout << "[DEBUG] winGame() = true" << std::endl;
            if (counterW == -1) {
                counterW = 2000;
                soundEngine->stopAllSounds();
                if (soundEngine) soundEngine->play2D("media/win.wav", false);
                std::cout << "[DEBUG] counterW initialized to 2000" << std::endl;
            }
            else if (counterW > 0) {
                counterW -= deltaTime;
            }
            else {
                win = true;  
                counterCredits = 5000;  
                std::cout << "[DEBUG] win set to true" << std::endl;
            }
        }
        else if (win && !showingCredits) {
            if (counterCredits > 0) {
                counterCredits -= deltaTime;
            }
            else {
                showingCredits = true;
                std::cout << "[DEBUG] Showing credits now\n";
            }
        }

        if (Game::instance().getKey(GLFW_KEY_R)) {
            resetLevel();  
        }

        return; 
    }



    float centerX = SCREEN_WIDTH / 8.0f;
    float centerY = SCREEN_HEIGHT / 8.0f;
    glm::vec3 cameraPos;
    if(playerPos.x < 2050)
        cameraPos = glm::vec3(playerPos.x - centerX, 1460, 0.0f);
    else if (playerPos.x >= 2050 && playerPos.x<2300) {
        if (playerPos.y > 136) cameraPos = glm::vec3(2150 - centerX, playerPos.y - centerY - 60, 0.0f);
        else cameraPos = glm::vec3(2150 - centerX, 45, 0.0f);
    }
	else if (playerPos.x >= 2300 && playerPos.x <3400)
		cameraPos = glm::vec3(playerPos.x - centerX, 20, 0.0f);
    else if (playerPos.x >=3400 && playerPos.x <3588)
        cameraPos = glm::vec3(3400 - centerX, 20, 0.0f);
    else if (playerPos.x >=3588)
        cameraPos = glm::vec3(3630 - centerX, 20, 0.0f);
	


    int mapWidth = SCREEN_WIDTH;
    int mapHeight = SCREEN_HEIGHT;



    view = glm::translate(glm::mat4(1.0f), -cameraPos);
}

void Scene::render()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!jocComencat) {
        glm::mat4 titleProjection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
        texProgram.use();
        texProgram.setUniformMatrix4f("projection", titleProjection);
        texProgram.setUniformMatrix4f("view", glm::mat4(1.0f));
        texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
        texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);  
        if (showControls)
            pantallaControles->render();
        else
            pantallaTitol->render();
        return;
	}
    if (showingCredits) {
        glm::mat4 titleProjection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
        texProgram.use();
        texProgram.setUniformMatrix4f("projection", titleProjection);
        texProgram.setUniformMatrix4f("view", glm::mat4(1.0f));
        texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
        texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
        pantallaCredits->render();
        return;
    }
	if (win) {
        cout << "Nashe\n";
        glm::mat4 titleProjection = glm::ortho(0.f, float(SCREEN_WIDTH) / 2, float(SCREEN_HEIGHT) / 2, 0.f);
        texProgram.use();
        texProgram.setUniformMatrix4f("projection", titleProjection);
        texProgram.setUniformMatrix4f("view", glm::mat4(1.0f));
        texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
        texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
        texProgram.setUniform2f("texCoordDispl", 0.f, 0.f); 
		pantallaWin->render();
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

	for (auto& collectible : collectibles) {
		collectible->render();
	}

    for (auto flame : llamas) {
        flame->render();
    }

    
    


	mag->render();
	mag2->render();
    player->render();
    if (boss && boss->isAlive()) boss->render();


	for (auto& seta : setas) seta->render();
	for (auto& fenn : fenixes) fenn->render();
    for (auto* p : plataformas1) p->render();
    for (auto* p : plataformas2) p->render();




    glm::mat4 hudProjection = glm::ortho(0.f, float(SCREEN_WIDTH)/2, float(SCREEN_HEIGHT)/2, 0.f);
    texProgram.setUniformMatrix4f("projection", hudProjection);
    texProgram.setUniformMatrix4f("view", glm::mat4(1.0f));
    texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
    if (player->esta())hud->setBoss(true);
    else hud->setBoss(false);
	float hB = boss->getHealth();
	hud->setHBoss(hB);
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

void Scene::resetLevel()
{

    delete player;
    player = nullptr;

    delete map;
    map = nullptr;

    delete hud;
    hud = nullptr;

    delete mag;
    delete mag2;
    mag = mag2 = nullptr;

    for (auto* f : fenixes) delete f;
    fenixes.clear();

    for (auto* s : setas) delete s;
    setas.clear();

    for (auto* p : plataformas1) delete p;
    plataformas1.clear();

    for (auto* p : plataformas2) delete p;
    plataformas2.clear();

	for (auto* f : llamas) delete f;
	llamas.clear();

	for (auto* c : collectibles) delete c;
	collectibles.clear();

    delete boss;
    boss = nullptr;


    delete cor1;
    delete cor2;
    delete calabaza1;
    delete lamp;
    delete capa;
    delete flam;

    cor1 = nullptr; cor2 = nullptr; calabaza1 = nullptr; lamp = nullptr; capa = nullptr; flam = nullptr;


    currentTime = 0.0f;
    jocComencat = false;
	showControls = false;


    init();
}

void Scene::spawnRandomCollectible(const glm::vec2& position) {

    int r = rand() % 5;
    Collectible* item = nullptr;

    

    switch (r) {
    case 0: {
        LittleHeart* heart = new LittleHeart();
        heart->init(position, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/heart1.png");
        item = heart;
        break;
    }
    case 1: {
        BigHeart* bigHeart = new BigHeart();
        bigHeart->init(position, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/heart2.png");
        item = bigHeart;
        break;
    }
    case 2: {
        Calabaza* pumpkin = new Calabaza();
        pumpkin->init(position, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/calabaza1.png");
        item = pumpkin;
        break;
    }
    case 3: {
        Lampara* lamp = new Lampara();
        lamp->init(position, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/lamp.png");
        item = lamp;
        break;
    }
	case 4: {
		Capa* cape = new Capa();
		cape->init(position, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/capa.png");
		item = cape;
		break;
	}
    default:
        break;
    }

    if (item != nullptr) {
        collectibles.push_back(item);
    }
}
