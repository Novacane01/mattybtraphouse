#pragma once
#include "SFML/Graphics.hpp"
#include "Floor.h"
#include "Enemy.h"
#include "Player.h"
#include <random>
#include <iostream>
#include <string>
#include <vector>
#include "LinkedMap.h"

extern unsigned WINDOW_LENGTH, WINDOW_WIDTH;

class GameManager
{
public:
	GameManager(int, int);
	void setWindowWidth(int);
	void setWindowLength(int);
	void Start();
	Player* createPlayer(sf::RenderWindow &);
	void GameOver(sf::RenderWindow &);
	void Pause(sf::RenderWindow &);
	void DisplayMap(sf::RenderWindow &, Player *, LinkedMap*);

	void spawnEnemies(LinkedMap*);
	~GameManager();
private:
	sf::Font font;
	sf::View view;

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
