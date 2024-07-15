#include <fstream>
#include <iostream>

#include "Scene_Loading.h"
#include "Scene_Cuba.h"
#include "Scene_Bermuda.h"
#include "Scene_USA.h"
#include "Scene_Credits.h"

#include "Components.h"
#include "Physics.h"
#include "Scene_Menu.h"
#include "Utilities.h"
#include "MusicPlayer.h"
#include "Assets.h"
#include "SoundPlayer.h"
#include <random>
#include <unordered_set>


namespace {
    std::random_device rd;
    std::mt19937 rng(rd());
}

#pragma region SceneLoad
Scene_Loading::Scene_Loading(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine),
    m_worldView(gameEngine->window().getView()) {

    loadLevel(levelPath);
    init();
}
void Scene_Loading::loadLevel(const std::string& path) {

    std::ifstream config(path);
    if (config.fail()) {
        std::cerr << "Open file " << path << " failed\n";
        config.close();
        exit(1);
    }

    std::string token{ "" };
    config >> token;
    while (!config.eof()) {
        if (token == "CurtainFull") {
            std::string name;
            sf::Vector2f pos;

            config >> name >> pos.x >> pos.y;
            auto e = m_entityManager.addEntity("bkg");

            auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            sprite.setOrigin(0.f, 0.f);
            sprite.setPosition(pos);

        }
        else if (token == "LoadMap") {
            std::string name;
            sf::Vector2f pos;

            config >> name >> pos.x >> pos.y;
            auto e = m_entityManager.addEntity("loadCuba");

            e->addComponent<CTransform>(pos);
            auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            sprite.setOrigin(0.f, 0.f);
            sprite.setPosition(pos);

            auto e2 = m_entityManager.addEntity("loadBermuda");
            e2->addComponent<CTransform>(sf::Vector2f{ 450.f, 206.f });
            auto& sprite2 = e2->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            e2->addComponent<CAnimation>(Assets::getInstance().getAnimation("Bermuda_Loading"));
            sprite2.setOrigin(450.f, 206.f);
            sprite2.setPosition(pos);

            auto e3 = m_entityManager.addEntity("loadUSA");
            e3->addComponent<CTransform>(sf::Vector2f{ 450.f, 206.f });
            auto& sprite3 = e3->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            e3->addComponent<CAnimation>(Assets::getInstance().getAnimation("USA_Loading"));
            sprite3.setOrigin(450.f, 206.f);
            sprite3.setPosition(pos);

            auto e4 = m_entityManager.addEntity("loadCredits");
            e4->addComponent<CTransform>(sf::Vector2f{ 450.f, 206.f });
            auto& sprite4= e4->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
            e4->addComponent<CAnimation>(Assets::getInstance().getAnimation("Credits_Loading"));
            sprite4.setOrigin(450.f, 206.f);
            sprite4.setPosition(pos);
        }
        else if (token[0] == '#') {
            std::cout << token;
        }

        config >> token;
    }

    config.close();
}
void Scene_Loading::loadSelectedLevel(const std::string& filePath) {

    std::ifstream config(filePath);
    if (config.fail()) {
        std::cerr << "Open file " << filePath << " failed\n";
        config.close();
        exit(1);
    }
    std::string token;
    while (config >> token) {
        if (token == "isCuba") {

            m_isCuba = true;
        }
        else if (token == "isBermuda") {

            m_isBermuda = true;
        }
        else if (token == "isUSA") {

            m_isUSA = true;
        }
        else if (token == "isOver") {

            m_isOver = true;
        }
        else if (token[0] == '#') {
            std::cout << token;
            std::string tmp;
            std::getline(config, tmp);
        }
    }

    config.close();
}
void Scene_Loading::init() {

   MusicPlayer::getInstance().play("loadingTheme");
   MusicPlayer::getInstance().setVolume(40);
   loadSelectedLevel("../assets/levelSelected.txt");

}
void Scene_Loading::onEnd() {

}
#pragma endregion

#pragma region System
void Scene_Loading::sDoAction(const Command& action) {

}
void Scene_Loading::sState(sf::Time dt) {

    SoundPlayer::getInstance().removeStoppedSounds();
    m_entityManager.update();

    if (!timeState(dt))
        return;

    gameState();
}
void Scene_Loading::sRender() {

    m_game->window().setView(m_worldView);



    renderUI();
}
void Scene_Loading::sMovement(sf::Time dt) {

    //sf::FloatRect view = getViewBounds();
    //m_worldView.move(m_config.scrollSpeed * dt.asSeconds() * 1, 0.f);

    //for (auto& e : m_entityManager.getEntities("player")) {
    //    if (e->hasComponent<CTransform>()) {
    //        auto& tfm = e->getComponent<CTransform>();

    //        tfm.pos += tfm.vel * dt.asSeconds();
    //    }
    //}
} // To remove?
void Scene_Loading::sUpdate(sf::Time dt) {

    sState(dt);
    sAnimation(dt);
    sMovement(dt);
}
void Scene_Loading::sAnimation(sf::Time dt) {
    for (auto e : m_entityManager.getEntities()) {

        if (e->hasComponent<CAnimation>()) {
            auto& anim = e->getComponent<CAnimation>();
            anim.animation.update(dt);
        }
    }
}
#pragma endregion

#pragma region sRender
void Scene_Loading::renderUI() {

    sf::RenderWindow& window = m_game->window();
    sf::View view1;
    view1.reset(sf::FloatRect(0.f, 0.f, 900.f, 512.f));
    m_game->window().setView(view1);

    sf::View view = window.getView();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewMousePos = window.mapPixelToCoords(mousePos, view);

    for (auto& e : m_entityManager.getEntities("bkg")) {
        auto& sprite = e->getComponent<CSprite>().sprite;
        auto tfm = e->getComponent<CTransform>();
        sprite.setPosition(tfm.pos);
        sprite.setRotation(tfm.angle);

        m_game->window().draw(sprite);
    }

    if (m_isCuba) {
        for (auto& e : m_entityManager.getEntities("loadCuba")) {

            auto& sprite = e->getComponent<CSprite>().sprite;
            auto tfm = e->getComponent<CTransform>();
            sprite.setPosition(tfm.pos);
            sprite.setRotation(tfm.angle);

            m_game->window().draw(sprite);
            
        }
    }
    else if (m_isBermuda) {
        for (auto& e : m_entityManager.getEntities("loadBermuda")) {

            auto& anim = e->getComponent<CAnimation>().animation;
            auto& tfm = e->getComponent<CTransform>();
            anim.getSprite().setPosition(tfm.pos);
            anim.getSprite().setRotation(tfm.angle);
            m_game->window().draw(anim.getSprite());

        }
    }
    else if (m_isUSA) {
        for (auto& e : m_entityManager.getEntities("loadUSA")) {

            auto& anim = e->getComponent<CAnimation>().animation;
            auto& tfm = e->getComponent<CTransform>();
            anim.getSprite().setPosition(tfm.pos);
            anim.getSprite().setRotation(tfm.angle);
            m_game->window().draw(anim.getSprite());

        }
    }
    else if (m_isOver) {
        for (auto& e : m_entityManager.getEntities("loadCredits")) {

            auto& anim = e->getComponent<CAnimation>().animation;
            auto& tfm = e->getComponent<CTransform>();
            anim.getSprite().setPosition(tfm.pos);
            anim.getSprite().setRotation(tfm.angle);
            m_game->window().draw(anim.getSprite());

        }
    }

}
#pragma endregion

#pragma region Utility
void Scene_Loading::update(sf::Time dt) {
    sUpdate(dt);
}
void Scene_Loading::loadCuba() {
    m_game->changeScene("LEVEL1", std::make_shared<Scene_Cuba>(m_game, "../assets/level1.txt"), true);
}
void Scene_Loading::loadBermuda() {
    m_game->changeScene("LEVEL2", std::make_shared<Scene_Bermuda>(m_game, "../assets/level2.txt"), true);
}
void Scene_Loading::loadUSA() {
    m_game->changeScene("LEVEL3", std::make_shared<Scene_USA>(m_game, "../assets/level3.txt"), true);
}
void Scene_Loading::loadCredits() {
    m_game->changeScene("CREDITS", std::make_shared<Scene_Credits>(m_game, "../assets/credits.txt"), true);
}

#pragma endregion

#pragma region sState
bool Scene_Loading::timeState(sf::Time dt) {

    const float flashDuration = 6.f;
    m_elapsedTime = m_timeFlashClock.getElapsedTime().asSeconds();

    if (m_elapsedTime > flashDuration) {
        return true;
    }
    return false;
}
void Scene_Loading::gameState() {

    if (m_isCuba)
        loadCuba();
    else if (m_isBermuda)
        loadBermuda();
    else if (m_isUSA)
        loadUSA();
    else if (m_isOver)
        loadCredits();
}
#pragma endregion
