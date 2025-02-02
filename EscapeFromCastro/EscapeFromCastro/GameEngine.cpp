#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Cuba.h"
#include "Scene_Menu.h"
#include "Command.h"
#include <fstream>
#include <memory>
#include <cstdlib>
#include <iostream>

GameEngine::GameEngine(const std::string& path)
{
	Assets::getInstance().loadFromFile("../config.txt");
	init(path);
}

void GameEngine::init(const std::string& path)
{
	unsigned int width;
	unsigned int height;
	loadConfigFromFile(path, width, height);

	sf::VideoMode fullscreenMode = sf::VideoMode::getDesktopMode();
	m_window.create(fullscreenMode, "Escape From Castro", sf::Style::Fullscreen);


	changeScene("MENU", std::make_shared<Scene_Menu>(this));

}

void GameEngine::loadConfigFromFile(const std::string& path, unsigned int& width, unsigned int& height) const {
	std::ifstream config(path);
	if (config.fail()) {
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);
	}
	std::string token{ "" };
	config >> token;
	while (!config.eof()) {
		if (token == "Window") {
			config >> width >> height;
		}
		else if (token[0] == '#') {
			std::string tmp;
			std::getline(config, tmp);
			std::cout << tmp << "\n";
		}

		if (config.fail()) {
			config.clear(); // clear error on stream
			std::cout << "*** Error reading config file\n";
		}
		config >> token;
	}
	config.close();
}

//void GameEngine::update()
//{
//
//	sUserInput();
//	currentScene()->update();
//
//}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			quit();

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (currentScene()->getActionMap().contains(event.key.code))
			{
				const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
				currentScene()->doAction(Command(currentScene()->getActionMap().at(event.key.code), actionType));
			}
		}
		if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
			if (currentScene()->getActionMap().contains(sf::Mouse::Left)) {
				const std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";
				currentScene()->doAction(Command(currentScene()->getActionMap().at(sf::Mouse::Left), actionType));
			}
		}
	}


}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap.at(m_currentScene);
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{


	if (endCurrentScene) {
		// remove scene from map
		m_sceneMap.erase(m_currentScene);
	}

	if (!m_sceneMap.contains(sceneName)) {
		// if scene not in map alrady put new one in
		// otherwise use existing scene arleady in map
		m_sceneMap[sceneName] = scene;
	}

	m_currentScene = sceneName;
}


void GameEngine::quit()
{
	m_window.close();
}


void GameEngine::run()
{
	const sf::Time SPF = sf::seconds(1.0f / 60.f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (isRunning())
	{
		sUserInput();								


		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > SPF)
		{
			currentScene()->update(SPF);			
			timeSinceLastUpdate -= SPF;
		}

		currentScene()->sRender();					

		window().display();
	}
}

void GameEngine::quitLevel() {
	changeScene("MENU", nullptr, true);
}

void GameEngine::backLevel() {
	changeScene("MENU", nullptr, false);

}


sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

sf::Vector2f GameEngine::windowSize() const {
	return sf::Vector2f{ m_window.getSize() };
}


bool GameEngine::isRunning()
{
	return (m_running && m_window.isOpen());
}
