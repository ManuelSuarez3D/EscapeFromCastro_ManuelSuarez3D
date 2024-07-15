
#ifndef ESCAPEFROMCASTRO_SCENE_CREDITS_H
#define ESCAPEFROMCASTRO_SCENE_CREDITS_H

#pragma once
#include "Scene.h"

class Scene_Credits : public Scene
{
private:
	std::vector<std::pair<std::string, bool>> m_menu_credits;
	sf::View        m_worldView;
	int			    m_finalScore{ 0 };
	int				m_special{ 0 };
	
	bool			m_playSound{ false };
	bool			m_isCredits{ false };
	bool			m_drawAABB{ false };
	float           m_elapsedTime = 0.0f;
	sf::Clock       m_timeFlashClock;
	sf::Text		m_score_text;

	void			loadLevel(const std::string& path);
	void            init();

	void            sMovement(sf::Time dt);
	void            sUpdate(sf::Time dt);
	void            sAnimation(sf::Time dt);
	void            sState(sf::Time dt);
	void		    sRender() override;

	bool            timeState(sf::Time dt);
	void            gameState();
	void			update(sf::Time dt) override;
	void			sDoAction(const Command& command) override;
	void			onEnd() override;
	void            renderUI();

public:

	Scene_Credits(GameEngine* gameEngine, const std::string& levelPath);

	bool            menuSound(bool check = false);
	bool            menuState(std::string tag);
	void            menuSelection(std::string tag, bool selection);
    void            loadScore(const std::string& filePath);
    void            loadInventory(const std::string& filePath);

};

#endif 