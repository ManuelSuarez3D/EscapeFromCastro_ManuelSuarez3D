#ifndef ESCAPEFROMCASTRO_SCENE_MENU_H
#define ESCAPEFROMCASTRO_SCENE_MENU_H
#pragma once

#include "Scene.h"


struct LevelMenuConfig {

	float       scrollSpeed{ 100.f };

	std::map<std::string,
		std::vector<std::pair<float, sf::Time>>> directions;
};

class Scene_Menu : public Scene
{
private:
	std::vector<std::string>	m_levelPaths;
	std::vector<std::pair<std::string, bool>> m_menu_menu;
	int							m_menuIndex{ 0 };
	sf::View					m_worldView;
	sf::Vector2f				m_worldBounds;
	bool						m_drawAABB{ false };
	bool						m_isPlay{ false };
	bool						m_playSound{ false };
	bool						m_isGuide{ false };
	bool						m_isLevels{ false };
	bool						m_isCredits{ false };

	LevelMenuConfig             m_menuConfig;
	sf::FloatRect				getViewBounds();
	void loadMenu(const std::string& path);
	void init();
	void onEnd() override;

public:

	Scene_Menu(GameEngine* gameEngine);

	void			update(sf::Time dt) override;

	void			sRender() override;
	void			sDoAction(const Command& action) override;
	void            sUpdate(sf::Time dt);
	void			sMovement(sf::Time dt);
	bool			menuState(std::string tag);
	void			menuSelection(std::string tag, bool selection);

};

#endif 