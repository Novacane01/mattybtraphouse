#pragma once
#include "SFML/Graphics.hpp"

#include "Enemy.h"
#include "LinkedMap.h"
#include "Player.h"
#include <random>
#include <iostream>
#include <string>
#include <vector>


extern unsigned WINDOW_LENGTH, WINDOW_WIDTH;

class GameManager
{
public:
	GameManager(int, int);
	void setWindowWidth(int);
	void setWindowLength(int);
	void Start();
	void LoadingScreen();
	Player* createPlayer();
	void spawnEnemies(LinkedMap*);

	void GameOver();
	void Pause();
	void DisplayMap(Player *, LinkedMap*);
	void levelUp(LinkedMap* lmap);
	void changeView(LinkedMap*);
private:
	sf::Font font;
	sf::View view;
	sf::RenderWindow window;
	Player *player;
	int level;
	sf::View roomView;
	Weapon defaultKnife;
	Weapon defaultPistol;
	Weapon heavyPistol;
	Weapon boltSniper;
	Weapon semiAuto;
	Weapon shotgun;
	Weapon assaultRifle;
	Weapon minigun;
	Weapon semiSniper;
	Weapon submachine;
};
