#include "Scene_Menu.h"
#include "Physics.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include "Scene.h"
#include "Scene_Credits.h"
#include "Scene_Cuba.h"
#include "Scene_Loading.h"
#include "Scene_Bermuda.h"
#include "Scene_USA.h"
#include <memory>

#pragma region SceneLoad
Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine),
	 m_worldBounds(gameEngine->window().getSize())
{
	loadMenu("../assets/menu.txt");
	init();

	MusicPlayer::getInstance().play("menuTheme");
	MusicPlayer::getInstance().setVolume(50);

}
void Scene_Menu::init() {

	m_menu_menu.push_back(std::make_pair("START", false));
	m_menu_menu.push_back(std::make_pair("LEVELS", false));
	m_menu_menu.push_back(std::make_pair("CONTROLS", false));
	m_menu_menu.push_back(std::make_pair("CREDITS", false));
	m_menu_menu.push_back(std::make_pair("CHAPTER1", false));
	m_menu_menu.push_back(std::make_pair("CHAPTER2", false));
	m_menu_menu.push_back(std::make_pair("CHAPTER3", false));
	m_menu_menu.push_back(std::make_pair("BACK", false));
	m_menu_menu.push_back(std::make_pair("QUIT", false));

	m_levelPaths.push_back("../assets/loading.txt");

	writeToScoreFile(0);
	writeToInventoryFile(0);

	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Mouse::Left, "MOUSE_CLICK");

}

void Scene_Menu::update(sf::Time dt)
{
	sUpdate(dt);
	m_entityManager.update();
}
void Scene_Menu::onEnd()
{
	m_game->window().close();
}
#pragma endregion

#pragma region Utility
sf::FloatRect Scene_Menu::getViewBounds() {
	auto view = m_game->window().getView();
	return sf::FloatRect(
		(view.getCenter().x - view.getSize().x / 2.f), (view.getCenter().y - view.getSize().y / 2.f),
		view.getSize().x, view.getSize().y);
}
#pragma endregion

#pragma region System
void Scene_Menu::sDoAction(const Command& action) {

	if (!m_isPlay) {
		if (action.type() == "START") {

			//if (action.name() == "TOGGLE_COLLISION") { m_drawAABB = !m_drawAABB; }
			if (action.name() == "MOUSE_CLICK") {
				if (menuState("START")) {

					m_isCuba = true;
					writeToLoadingFile("CUBA");
					m_isPlay = true;

				}
				else if (menuState("LEVELS")) {
					m_isLevels = true;

					if (menuState("CHAPTER1")) {

						m_isCuba = true;
						writeToLoadingFile("CUBA");
						m_isPlay = true;

					}
					else if (menuState("CHAPTER2")) {

						m_isBermuda = true;
						writeToLoadingFile("BERMUDA");
						m_isPlay = true;

					}
					else if (menuState("CHAPTER3")) {

						m_isUSA = true;
						writeToLoadingFile("USA");
						m_isPlay = true;

					}
					else if (menuState("BACK")) {
						menuSelection("ALL", false);
						m_isLevels = false;
					}
				}
				else if (menuState("CONTROLS")) {
					m_isGuide = true;

					if (menuState("BACK")) {
						menuSelection("ALL", false);
						m_isGuide = false;
					}
				}
				else if (menuState("CREDITS")) {
					m_isCredits = true;

					if (menuState("BACK")) {
						menuSelection("ALL", false);
						m_isCredits = false;
					}
				}
				else if (menuState("QUIT")) {
					onEnd();
				}
			}
		}
	}
}
void Scene_Menu::sMovement(sf::Time dt) {

	if (m_isPlay) {

		for (auto& e : m_entityManager.getEntities("curtaintop")) {
			auto ebb = e->getComponent<CBoundingBox>();
			if (e->hasComponent<CTransform>()) {
				auto& tfm = e->getComponent<CTransform>();
				if ((tfm.pos.y + ebb.size.y) < 17.f) {
					tfm.pos.y += 50.f * dt.asSeconds();
				}
				else {
					for (auto& e : m_entityManager.getEntities("curtain")) {
						auto ebb = e->getComponent<CBoundingBox>();
						if (e->hasComponent<CTransform>()) {
							auto& tfm = e->getComponent<CTransform>();
							if ((tfm.pos.y + ebb.size.y) < 512.f) {
								tfm.pos.y += 145.f * dt.asSeconds();
							}
							else {
								tfm.pos.y = 0 - 512.f;
								m_isCuba = false;
								m_isBermuda = false;
								m_isUSA = false;
								m_isPlay = false;
								m_isLevels = false;
								m_isGuide = false;
								m_isCredits = false;
								m_game->changeScene("LOADING", std::make_shared<Scene_Loading>(m_game, m_levelPaths[0]));
							}
						}
					}
				}
			}
		}
	}
}
void Scene_Menu::sRender()
{

	sf::RenderWindow& window = m_game->window();

	sf::View view1;
	view1.reset(sf::FloatRect(0.f, 0.f, 900.f, 512.f));
	m_game->window().setView(view1);
	m_game->window().setMouseCursorVisible(false);

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f viewMousePos = window.mapPixelToCoords(mousePos, view1);

	for (auto& e : m_entityManager.getEntities("bkg")) {
		if (e->getComponent<CSprite>().has) {
			auto& sprite = e->getComponent<CSprite>().sprite;
			auto& tfm = e->getComponent<CTransform>();

			m_game->window().draw(sprite);
		}
	}

	for (auto& e1 : m_entityManager.getEntities()) {

		if (m_drawAABB) {
			if (e1->hasComponent<CBoundingBox>()) {
				auto box = e1->getComponent<CBoundingBox>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f{ box.size.x, box.size.y });
				centerOrigin(rect);
				rect.setPosition(e1->getComponent<CTransform>().pos);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color{ 0, 255, 0 });
				rect.setOutlineThickness(2.f);
				m_game->window().draw(rect);
			}
		}
	}

	if (!m_isGuide && !m_isLevels && !m_isCredits) {
		for (auto& e1 : m_entityManager.getEntities("start1")) {

			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto& tfm1 = e1->getComponent<CTransform>();

				for (auto e2 : m_entityManager.getEntities("start2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;
						auto& tfm = e2->getComponent<CTransform>();

						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {

							if (!m_playSound) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}

							menuSelection("START", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("START", false);
							m_game->window().draw(sprite1);
						}
					}
				}
			}
		}
		for (auto& e1 : m_entityManager.getEntities("levels1")) {
		
			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto test = e1->getComponent<CTransform>();
		
		
				for (auto e2 : m_entityManager.getEntities("levels2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;
		
						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {
		
							if (!m_playSound) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}
		
							menuSelection("LEVELS", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("LEVELS", false);
							m_game->window().draw(sprite1);
						}
					}
				}
			}
		}
		for (auto& e1 : m_entityManager.getEntities("ctrls1")) {
		
			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto test = e1->getComponent<CTransform>();
		
		
				for (auto e2 : m_entityManager.getEntities("ctrls2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;
		
						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {
		
							if (!m_playSound) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}
							menuSelection("CONTROLS", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("CONTROLS", false);
							m_game->window().draw(sprite1);
						}
					}
				}
			}
		}
		for (auto& e1 : m_entityManager.getEntities("credits1")) {
		
			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto test = e1->getComponent<CTransform>();
		
		
				for (auto e2 : m_entityManager.getEntities("credits2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;
		
						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {
		
							if (!m_playSound) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}
		
							menuSelection("CREDITS", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("CREDITS", false);
							m_game->window().draw(sprite1);
						}
					}
				}
			}
		}
		for (auto& e1 : m_entityManager.getEntities("quit1")) {
		
			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto test = e1->getComponent<CTransform>();
		
				for (auto e2 : m_entityManager.getEntities("quit2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;
		
						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {
		
							if (!m_playSound) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}
							menuSelection("QUIT", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("QUIT", false);
							m_game->window().draw(sprite1);
						}
					}
				}
			}
		}
	}
	else {

		if (m_isLevels) {
			for (auto& e1 : m_entityManager.getEntities("chapt1a")) {

				if (e1->hasComponent<CSprite>()) {
					auto& sprite1 = e1->getComponent<CSprite>().sprite;
					auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
					auto test = e1->getComponent<CTransform>();


					for (auto e2 : m_entityManager.getEntities("chapt1b")) {
						if (e2->hasComponent<CSprite>()) {
							auto& sprite2 = e2->getComponent<CSprite>().sprite;

							if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {

								if (!m_playSound && menuState("LEVELS")) {
									SoundPlayer::getInstance().play("menuPing");
									m_playSound = true;
								}

								menuSelection("CHAPTER1", true);
								m_game->window().draw(sprite2);
							}
							else {
								menuSelection("CHAPTER1", false);
								m_game->window().draw(sprite1);
							}
						}
					}
				}
			}
			for (auto& e1 : m_entityManager.getEntities("chapt2a")) {

				if (e1->hasComponent<CSprite>()) {
					auto& sprite1 = e1->getComponent<CSprite>().sprite;
					auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
					auto test = e1->getComponent<CTransform>();


					for (auto e2 : m_entityManager.getEntities("chapt2b")) {
						if (e2->hasComponent<CSprite>()) {
							auto& sprite2 = e2->getComponent<CSprite>().sprite;

							if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {

								if (!m_playSound && menuState("LEVELS")) {
									SoundPlayer::getInstance().play("menuPing");
									m_playSound = true;
								}

								menuSelection("CHAPTER2", true);
								m_game->window().draw(sprite2);
							}
							else {
								menuSelection("CHAPTER2", false);
								m_game->window().draw(sprite1);
							}
						}
					}
				}
			}
			for (auto& e1 : m_entityManager.getEntities("chapt3a")) {

				if (e1->hasComponent<CSprite>()) {
					auto& sprite1 = e1->getComponent<CSprite>().sprite;
					auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
					auto test = e1->getComponent<CTransform>();


					for (auto e2 : m_entityManager.getEntities("chapt3b")) {
						if (e2->hasComponent<CSprite>()) {
							auto& sprite2 = e2->getComponent<CSprite>().sprite;

							if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {

								if (!m_playSound && menuState("CHAPTER3")) {
									SoundPlayer::getInstance().play("menuPing");
									m_playSound = true;
								}

								menuSelection("CHAPTER3", true);
								m_game->window().draw(sprite2);
							}
							else {
								menuSelection("CHAPTER3", false);
								m_game->window().draw(sprite1);
							}
						}
					}
				}
			}
		}
			
		if (m_isGuide) {
			for (auto& e : m_entityManager.getEntities("instruction")) {
				if (e->getComponent<CSprite>().has) {
					auto& sprite = e->getComponent<CSprite>().sprite;
					m_game->window().draw(sprite);
				}
			}
		}
		if (m_isCredits) {
			for (auto& e : m_entityManager.getEntities("byme")) {
				if (e->getComponent<CSprite>().has) {
					auto& sprite = e->getComponent<CSprite>().sprite;
					m_game->window().draw(sprite);
				}
			}
		}
		for (auto& e1 : m_entityManager.getEntities("back1")) {

			if (e1->hasComponent<CSprite>()) {
				auto& sprite1 = e1->getComponent<CSprite>().sprite;
				auto overlap = Physics::getOverlapMouse(viewMousePos, e1);
				auto test = e1->getComponent<CTransform>();

				for (auto e2 : m_entityManager.getEntities("back2")) {
					if (e2->hasComponent<CSprite>()) {
						auto& sprite2 = e2->getComponent<CSprite>().sprite;

						if (overlap.x > 0 && overlap.y > 0 && !m_isPlay) {

							if (!m_playSound && menuState("BACK")) {
								SoundPlayer::getInstance().play("menuPing");
								m_playSound = true;
							}
							menuSelection("BACK", true);
							m_game->window().draw(sprite2);
						}
						else {
							menuSelection("BACK", false);
							m_game->window().draw(sprite1);

						}
					}
				}
			}
		}
	}

	sf::Sprite spriteCursor;
	spriteCursor.setTexture(Assets::getInstance().getTexture("Cursor"));

	spriteCursor.setPosition(viewMousePos);
	m_game->window().draw(spriteCursor);

	if (m_isPlay) {
		for (auto& e : m_entityManager.getEntities("curtain")) {

			if (e->getComponent<CSprite>().has) {
				auto& sprite = e->getComponent<CSprite>().sprite;
				if (e->hasComponent<CTransform>()) {
					auto& tfm = e->getComponent<CTransform>();
					sprite.setPosition(tfm.pos);
					sprite.setRotation(tfm.angle);
				}
				m_game->window().draw(sprite);
			}
		}
		for (auto& e : m_entityManager.getEntities("curtaintop")) {

			if (e->getComponent<CSprite>().has) {
				auto& sprite = e->getComponent<CSprite>().sprite;
				if (e->hasComponent<CTransform>()) {
					auto& tfm = e->getComponent<CTransform>();
					sprite.setPosition(tfm.pos);
					sprite.setRotation(tfm.angle);
				}
				m_game->window().draw(sprite);
			}
		}
	}
	if(menuState("ALL"))
		m_playSound = false;
	else if (m_isLevels && !menuState("CHAPTER1")
			&& !menuState("CHAPTER1")
			&& !menuState("CHAPTER2")
			&& !menuState("CHAPTER3")
			&& !menuState("BACK")) {
		m_playSound = false;
	}
	else if (m_isGuide && !menuState("BACK")) {
		m_playSound = false;
	}
	else if (m_isCredits && !menuState("BACK")) {
		m_playSound = false;
	}

}
void Scene_Menu::sUpdate(sf::Time dt) {

	sMovement(dt);
}
#pragma endregion

#pragma region Menu
void Scene_Menu::menuSelection(std::string tag, bool selection) {

	if (tag == "ALL") {
		for (auto& item : m_menu_menu) {
			item.second = selection;
		}
	}

	for (auto& item : m_menu_menu) {
		if (item.first == tag) {
			item.second = selection;
		}
	}
}
bool Scene_Menu::menuState(std::string tag) {

	if (tag == "ALL") {
		return std::all_of(m_menu_menu.begin(), m_menu_menu.end(), [](const auto& item) {
			return !item.second;
		});
	}
	for (auto& item : m_menu_menu) {
		if (item.first == tag) {
			return item.second;
		}
	}
}
void Scene_Menu::loadMenu(const std::string& path) {
	std::ifstream config(path);

	if (config.fail()) {
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);
	}

	std::string token{ "" };
	config >> token;

	while (!config.eof()) {
		if (token == "Bkg") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("bkg");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Strt1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("start1");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Strt2") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("start2");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;

			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Lvls1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("levels1");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Lvls2") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("levels2");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;

			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Chapt1a") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt1a");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Chapt1b") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt1b");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			auto spriteSize = sprite.getLocalBounds().getSize();

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Chapt2a") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt2a");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Chapt2b") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt2b");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			auto spriteSize = sprite.getLocalBounds().getSize();

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Chapt3a") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt3a");
			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Chapt3b") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("chapt3b");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			e->addComponent<CBoundingBox>(sprite.getLocalBounds().getSize());
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Ctrls1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("ctrls1");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Ctrls2") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("ctrls2");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			auto spriteSize = sprite.getLocalBounds().getSize();

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

		}
		else if (token == "Creds1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("credits1");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

			}
		else if (token == "Creds2") {
				std::string name;
				sf::Vector2f pos;

				config >> name >> pos.x >> pos.y;
				auto e = m_entityManager.addEntity("credits2");

				auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
				auto spriteSize = sprite.getLocalBounds().getSize();

				e->addComponent<CBoundingBox>(spriteSize);
				sf::Vector2f spriteOrigin = sprite.getOrigin();

				sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

				e->addComponent<CTransform>(boundingBoxPosition);
				sprite.setOrigin(0.f, 0.f);
				sprite.setPosition(pos);

				}
		else if (token == "Qut1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("quit1");
			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Qut2") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("quit2");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			e->addComponent<CBoundingBox>(sprite.getLocalBounds().getSize());
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Bck1") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("back1");
			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			sf::Vector2f spriteSize(195, 35);

			e->addComponent<CBoundingBox>(spriteSize);
			sf::Vector2f spriteOrigin = sprite.getOrigin();

			spriteOrigin.y -= 3.f;
			sf::Vector2f boundingBoxPosition = pos + spriteOrigin;

			e->addComponent<CTransform>(boundingBoxPosition);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Bck2") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("back2");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			e->addComponent<CBoundingBox>(sprite.getLocalBounds().getSize());
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "Instrc") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("instruction");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			e->addComponent<CBoundingBox>(sprite.getLocalBounds().getSize());
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
		}
		else if (token == "ByMe") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("byme");

			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			e->addComponent<CBoundingBox>(sprite.getLocalBounds().getSize());
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);
			}
		else if (token == "CurtainTop") {
			std::string name;
			sf::Vector2f pos;

			config >> name >> pos.x >> pos.y;
			auto e = m_entityManager.addEntity("curtaintop");
			sf::Vector2f adjustedSpawn{ pos.x , pos.y - 17.f };
			e->addComponent<CTransform>(adjustedSpawn);
			auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
			auto spriteSize = sprite.getLocalBounds().getSize();
			e->addComponent<CBoundingBox>(spriteSize);

			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(pos);

			}
		else if (token == "Curtain") {
				std::string name;
				sf::Vector2f pos;

				config >> name >> pos.x >> pos.y;
				auto e = m_entityManager.addEntity("curtain");
				sf::Vector2f adjustedSpawn{ pos.x , pos.y - 512.f };
				e->addComponent<CTransform>(adjustedSpawn);
				auto& sprite = e->addComponent<CSprite>(Assets::getInstance().getTexture(name)).sprite;
				auto spriteSize = sprite.getLocalBounds().getSize();
				e->addComponent<CBoundingBox>(spriteSize);

				sprite.setOrigin(0.f, 0.f);
				sprite.setPosition(pos);

				}
		else if (token[0] == '#') {
			std::cout << token;
		}

		config >> token;
	}

	config.close();
}
#pragma endregion