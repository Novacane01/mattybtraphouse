//#include "stdafx.h"
#include "GameManager.h"
#include "Chest.h"
#include <unistd.h>
unsigned WINDOW_LENGTH, WINDOW_WIDTH;
//Game Manager Constructor
GameManager::GameManager(int width, int length) {
	setWindowLength(length);
	setWindowWidth(width);
	if (!font.loadFromFile("Fonts/light_pixel-7.ttf")) {
		std::cout << "Could not load file" << std::endl;
	}
}

//Sets window length
void GameManager::setWindowLength(int value) {
	WINDOW_LENGTH = value;
}

//Sets window width
void GameManager::setWindowWidth(int value) {
	WINDOW_WIDTH = value;
}

//Starts The Game
void GameManager::Start() {
    //Creates Window with size WINDOW_WITDTH x WINDOW_LENGTH
	static sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_LENGTH), "Traphouse");

	//creates randomized map
    LinkedMap lmap(17);
	lmap.addRooms(17,lmap.head, window);

	window.setKeyRepeatEnabled(false); //Disables repeated keypresses
	window.setVerticalSyncEnabled(false); //Limits refresh rate to monitor

    //Initializes clock to record frames per second
	sf::Clock FPSclock;
	sf::Clock clickInterval;

	//Creates player object
	Player* player = createPlayer(window);

    //sets center of window at 0,0
    sf::View roomView;
    roomView.setCenter(0,0);
    window.setView(roomView);

	//Creates bounded Map rectangle object
	Floor *map = new Floor;

	bool centered = false;

	lmap.findCurrentRoom(lmap.head, player);
    //Main loop
	while (window.isOpen()) {
		float deltaTime = FPSclock.restart().asSeconds();
		//Records window events such as mouse movement, mouse clicks, and key strokes
		sf::Event event;
		while (window.pollEvent(event)) {
			if (clickInterval.getElapsedTime().asSeconds() > ((player->triggerhappy)?player->getCurrentWeapon().getAttackSpeed()/2.f:player->getCurrentWeapon().getAttackSpeed())) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					player->getCurrentWeapon().Shoot(player, window);
					clickInterval.restart();
				}
			}
			if (event.type == event.Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::W) {
					player->isMovingUp = true;
				}
				if (event.key.code == sf::Keyboard::A) {
					player->isMovingLeft = true;
				
				}
				if (event.key.code == sf::Keyboard::S) {
					player->isMovingDown = true;
					
				}
				if (event.key.code == sf::Keyboard::D) {
					player->isMovingRight = true;
					
				}
				if (event.key.code == sf::Keyboard::R&&player->getCurrentWeapon().getCurrentClip()<player->getCurrentWeapon().getMaxClip()) {
					player->getCurrentWeapon().bIsReloading = true;
				}
				if (event.key.code == sf::Keyboard::LShift) {
					player->bIsSprinting = true;
				}
				if (event.key.code == sf::Keyboard::Escape) {
					Pause(window);
				}
                if(event.key.code == sf::Keyboard::M){
				    DisplayMap(window, player, &lmap);
                }
				if (event.key.code == sf::Keyboard::X) {
					if (player->getPotions().size() > 0) {
						player->getCurrentPotion()->Use(player);
						std::cout << player->getCurrentPotion()->getName() << " used" << std::endl;
						player->getPotions().erase(player->getPotions().begin());
					}
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::W) {
					player->isMovingUp = false;
				}
				if (event.key.code == sf::Keyboard::A) {
					player->isMovingLeft = false;
				}
				if (event.key.code == sf::Keyboard::S) {
					player->isMovingDown = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					player->isMovingRight = false;
				}
				if (event.key.code == sf::Keyboard::LShift) {
					player->bIsSprinting = false;
				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				player->switchWeapons();
			}
		}
		//If not in current room (Between rooms/In hallways)
		if(!lmap.getCurrentRoom()->playerIsInside){

			lmap.findCurrentRoom(lmap.head, player);
			roomView.setCenter(player->getPlayer().getPosition());
			window.setView(roomView);
			centered = false;

		//if room is not cleared and map is not already centered, centers map and spawns enemies, only called once per room
		} else if(!centered && !lmap.getCurrentRoom()->isCleared){
			std::cout << "howdy partner\n";
			centered = true;
			roomView.setCenter(lmap.getCurrentRoom()->floor.getPosition());
			window.setView(roomView);
			spawnEnemies(&lmap);

			//if room is cleared and player is inside, locks window view to players position until unvisited room is found
		} else if(lmap.getCurrentRoom()->isCleared){
			lmap.findCurrentRoom(lmap.head, player);
			roomView.setCenter(player->getPlayer().getPosition());
			window.setView(roomView);
			centered = false;
		} else if(Enemy::getEnemies().size() == 0){ //If all enemies have been killed, room is cleared;
			lmap.getCurrentRoom()->isCleared = true;
		}

		window.clear(); //Clears window
		//map->Draw(window); //Draws map

        lmap.displayMap(lmap.head, window); //Draws Map

        //Draws Enemmies to screen
		for (unsigned i = 0; i < Enemy::getEnemies().size();i++) {
			Enemy::getEnemies()[i]->Update(player, deltaTime);
			Enemy::getEnemies()[i]->Draw(window);
			if (Enemy::getEnemies()[i]->isDead()) {
				Enemy::Destroy(Enemy::getEnemies().begin()+i);
			}
		}

		player->Update(window, deltaTime); //Updates player position
		player->Draw(window); //Draws player to screen
		for (unsigned i = 0;i < player->getWeapons().size();i++) {
			player->getWeapons()[i].Update(window, player, deltaTime); //Updates weapons and bullets
		}
		player->getCurrentWeapon().displayWeaponInfo(window);

		window.display(); //Displays all drawn objects
		if (player->isDead()) {
			GameOver(window);
		}
	}
}

Player* GameManager::createPlayer(sf::RenderWindow &window) {
	window.setKeyRepeatEnabled(true);
	bool created = false;

	//Creates textbox
	sf::RectangleShape textBox(sf::Vector2f(250, 50));
	textBox.setOrigin(textBox.getSize().x/2,textBox.getSize().y/2);
	textBox.setPosition(window.getView().getCenter().x , window.getView().getCenter().y);
	textBox.setFillColor(sf::Color::Transparent);
	textBox.setOutlineColor(sf::Color::Red);
	textBox.setOutlineThickness(2);

	sf::Text text; //Creates text object for name to be drawn to screen
	text.setFont(font);
	text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
	text.setPosition(textBox.getPosition().x, textBox.getPosition().y -16);
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);

	sf::Text namePrompt; //Creates text object for user prompt
	namePrompt.setFont(font);
	namePrompt.setString("Enter your name:");
    namePrompt.setOrigin(namePrompt.getGlobalBounds().width/2, namePrompt.getGlobalBounds().height/2);
	namePrompt.setPosition(window.getView().getCenter().x, window.getView().getCenter().y - 100);
	namePrompt.setFillColor(sf::Color::White);

	std::string name; //Player name

	
	//Draw Loop
	while (window.isOpen()&&!created) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::BackSpace&&name.size()>0) {
					name = name.substr(0, name.size() - 1);
					text.setString(name);
					text.setPosition(text.getPosition().x + (textBox.getSize().x / (text.getCharacterSize()*1.15f)), text.getPosition().y);
				}
				if (name.size() > 0 && event.key.code == sf::Keyboard::Return) {
					created = true;
					break;
				}
			}
			if (event.type == sf::Event::TextEntered&&(char)event.text.unicode!='\b'&&name.size()<10) {
				name += (char)event.text.unicode;
				text.setString(name);
				text.setPosition(text.getPosition().x - (textBox.getSize().x/ (text.getCharacterSize()*1.15f)), text.getPosition().y);
			}

			if (event.type == event.Closed) {
                window.close();
            }
		}



		window.clear();
		window.draw(text);
		window.draw(namePrompt);
		window.draw(textBox);
		window.display();
	}

	window.setKeyRepeatEnabled(false);

	//Creating player
	Player *player = new Player(name);
	return player;
}

//Game Manager Destructor
GameManager::~GameManager() {

}

void GameManager::GameOver(sf::RenderWindow &window) {
	std::cout << "You have died" << std::endl;
	Enemy::getEnemies().clear();

    sf::Event event;
    sf::Text restartButton;
    restartButton.setFont(font);
    restartButton.setCharacterSize(48);
    restartButton.setString("Restart");
    restartButton.setOrigin(restartButton.getGlobalBounds().width/2, restartButton.getGlobalBounds().height/2);
    restartButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 100);

    //Adds quit button to menu screen
    sf::Text exitButton;
    exitButton.setFont(font);
    exitButton.setCharacterSize(48);
    exitButton.setString("Quit");
    exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
    exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y + 100);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setCharacterSize(80);
    gameOverText.setString("Game Over");
    gameOverText.setOrigin(gameOverText.getGlobalBounds().width/2, gameOverText.getGlobalBounds().height/2);
    gameOverText.setPosition(window.getView().getCenter().x - window.getView().getSize().x, window.getView().getCenter().y - 350);

    sf::RectangleShape fade;
    fade.setSize(window.getView().getSize());
    fade.setFillColor(sf::Color(0,0,0,1));
    fade.setOrigin(fade.getSize().x/2,fade.getSize().y/2);
    fade.setPosition(window.getView().getCenter().x,window.getView().getCenter().y);

    //Fades into GameOver
	for(int i = 2; i < 250; i++){
		usleep(10000);
		window.draw(fade);
		window.display();
		fade.setFillColor(sf::Color(0,0,0,i));
	}
	while(gameOverText.getPosition().x != window.getView().getCenter().x){
		gameOverText.setPosition(gameOverText.getPosition().x + 1, gameOverText.getPosition().y);
		usleep(2000);
		window.clear();
		window.draw(fade);
		window.draw(gameOverText);
		window.display();
	}
	while (window.isOpen()) {
        //Activates buttons if pressed, respectively
        if(restartButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
            restartButton.setCharacterSize(58);//enlarges text when mouse is hovering over
            restartButton.setOrigin(restartButton.getGlobalBounds().width/2, restartButton.getGlobalBounds().height/2);
            restartButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 100);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                Start();
            }
        } else{
            restartButton.setCharacterSize(48);
            restartButton.setOrigin(restartButton.getGlobalBounds().width/2, restartButton.getGlobalBounds().height/2);
            restartButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 100);
        }
        if(exitButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            exitButton.setCharacterSize(58); //enlarges text when mouse is hovering over
            exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
            exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y + 100);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                window.close();
            }
        } else {
            exitButton.setCharacterSize(48);
            exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
            exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y + 100);
        }
        while (window.pollEvent(event)) {

            if (event.type == event.Closed) {
                window.close();
            }
        }
		window.clear();
        window.draw(fade);
        window.draw(gameOverText);
        window.draw(restartButton);
        window.draw(exitButton);
        window.display();
    }
	Start();
}

void GameManager::Pause(sf::RenderWindow &window) {
	sf::Event event;
	sf::Text resumeButton;
	resumeButton.setFont(font);
	resumeButton.setCharacterSize(48);
	resumeButton.setString("Resume");
	resumeButton.setOrigin(resumeButton.getGlobalBounds().width/2, resumeButton.getGlobalBounds().height/2);
	resumeButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 200);

    //Adds quit button to menu screen
    sf::Text exitButton;
    exitButton.setFont(font);
    exitButton.setCharacterSize(48);
    exitButton.setString("Quit");
    exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
    exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y);

	while (window.isOpen()) {
		//Activates buttons if pressed, respectively
	    if(resumeButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
	        resumeButton.setCharacterSize(58);//enlarges text when mouse is hovering over
            resumeButton.setOrigin(resumeButton.getGlobalBounds().width/2, resumeButton.getGlobalBounds().height/2);
            resumeButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 200);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
	    		return;
            }
        } else{
	        resumeButton.setCharacterSize(48);
            resumeButton.setOrigin(resumeButton.getGlobalBounds().width/2, resumeButton.getGlobalBounds().height/2);
	        resumeButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y - 200);
	    }
	    if(exitButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            exitButton.setCharacterSize(58); //enlarges text when mouse is hovering over
            exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
            exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                window.close();
            }
        } else {
            exitButton.setCharacterSize(48);
            exitButton.setOrigin(exitButton.getGlobalBounds().width/2, exitButton.getGlobalBounds().height/2);
            exitButton.setPosition(window.getView().getCenter().x,window.getView().getCenter().y);
        }
        while (window.pollEvent(event)) {
			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Escape){
				    return;
				}
			}
	    	if (event.type == event.Closed) {
			    window.close();
            }
		}
		window.clear();
		window.draw(resumeButton);
		window.draw(exitButton);
        window.display();
	}
}

void GameManager::DisplayMap(sf::RenderWindow &window, Player* player, LinkedMap* linkedMap){
    sf::Event event;
    sf::View mapView;
    sf::View roomView;
    bool windowMoving = false;
    sf::Vector2f oldMousePos;
    sf::Vector2f newMousePos;
    sf::Vector2f diffPosition;


    roomView = window.getView();
	//Shows large portion of map
	mapView.setSize(10000,10000);
    //Centers view of map around player
    mapView.setCenter(player->getPlayer().getPosition().x,player->getPlayer().getPosition().y);
    window.setView(mapView);

    while(window.isOpen())
    {

    	while (window.pollEvent(event)) {

        	/*
        	 * Methods For changing window location by clicking and dragging mouse
        	 */
    	    if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == 0){
					oldMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					windowMoving = true;
				}
			} else if(event.type == sf::Event::MouseButtonReleased){
				if(event.mouseButton.button == 0){
					windowMoving = false;
				}

			} else if(event.type == sf::Event::MouseMoved){
				if(windowMoving){
					newMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
					diffPosition = oldMousePos - newMousePos;
					mapView.setCenter(mapView.getCenter() + diffPosition);
					window.setView(mapView);

					//Sets old position = new position, based on new window, for the next time the mouse is moved
					oldMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				}
			}
			if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::M){
                    window.setView(roomView);
                    return;
                }
            }
            if (event.type == event.Closed) {
                window.close();
            }
        }
        window.clear();
        linkedMap->displayMap(linkedMap->head,window);
        player->Draw(window);
        window.display();

    }

}

void GameManager::spawnEnemies(LinkedMap* linkedMap){

    //Spawning monsters
    int numOfEnemies = rand() % 5 + 1;

    for(int i=0;i < numOfEnemies;i++){
        Enemy::Spawn(new Skeleton(), linkedMap->getCurrentRoom());


    }



    numOfEnemies = rand() % 5 + 1;
    for(int i=0;i < numOfEnemies;i++){
        Enemy::Spawn(new Spider(), linkedMap->getCurrentRoom());
    }

}