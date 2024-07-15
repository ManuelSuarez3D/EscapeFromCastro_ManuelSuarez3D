
#ifndef ESCAPEFROMCASTRO_SCENE_LOADING_H
#define ESCAPEFROMCASTRO_SCENE_LOADING_H

#pragma once
#include "Scene.h"

class Scene_Loading : public Scene
{
private:

	sf::View        m_worldView;

	bool			m_drawAABB{ false };
	float           m_elapsedTime = 0.0f;
	sf::Clock       m_timeFlashClock;

	void			loadLevel(const std::string& path);
	void			loadSelectedLevel(const std::string& filePath);
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

	Scene_Loading(GameEngine* gameEngine, const std::string& levelPath);
			
	void			loadCuba();
	void			loadBermuda();
	void			loadUSA();
	void			loadCredits();

};

#endif 