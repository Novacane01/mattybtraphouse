#include "LinkedMap.h"


LinkedMap::LinkedMap(int rta, int level) { //constructor for linked map
	if (!font.loadFromFile("Fonts/light_pixel-7.ttf")) {
		std::cout << "Could not load file" << std::endl;
	}
	chest1 = new Chest; //one out of two chests
	chest2 = new Chest; //second
	this->level = level;
	if (!floorTexture.loadFromFile("Sprites/Map/DungeonFloorStone.png")) {
		std::cout << "Could not load file<<std" << std::endl;
	}
	srand(time(0)); //seed time
	roomsToAdd = rta; //how many rooms to add randomly
	head = new room;
	head->playerIsInside = true;
	head->bIsVisited = true;
    head->isCleared = false;
	count++;
	head->floor.setSize(sf::Vector2f(900, 900));
	head->wallTop.setSize(sf::Vector2f(head->floor.getGlobalBounds().width+100, 50));
	head->wallTop.setPosition(head->floor.getPosition().x-head->floor.getGlobalBounds().width/2-50, head->floor.getPosition().y - head->floor.getGlobalBounds().height/2-50);
	head->wallTop.setTexture(&floorTexture);
	head->wallRight.setSize(sf::Vector2f(50, head->floor.getGlobalBounds().height));
	head->wallRight.setPosition(head->floor.getPosition().x + head->floor.getGlobalBounds().width / 2, head->floor.getPosition().y - head->floor.getGlobalBounds().height / 2);
	head->wallRight.setTexture(&floorTexture);
	head->wallBottom.setSize(sf::Vector2f(head->floor.getGlobalBounds().width+100, 50));
	head->wallBottom.setPosition(head->floor.getPosition().x - head->floor.getGlobalBounds().width / 2-50, head->floor.getPosition().y + head->floor.getGlobalBounds().height / 2);
	head->wallBottom.setTexture(&floorTexture);
	head->wallLeft.setSize(sf::Vector2f(50, head->floor.getGlobalBounds().height));
	head->wallLeft.setPosition(head->floor.getPosition().x - head->floor.getGlobalBounds().width / 2-50, head->floor.getPosition().y - head->floor.getGlobalBounds().height / 2);
	head->wallLeft.setTexture(&floorTexture);
	head->floor.setOrigin(head->floor.getSize().x / 2, head->floor.getSize().y / 2);
	head->floor.setTexture(&floorTexture); // last lines set walls, floor, textures, etc. for head room
	head->floor.setPosition(0, 0);
	positions.push_back(head->floor);
	head->floor.setFillColor(sf::Color::Blue);
	head->neighbor1 = nullptr;
	head->neighbor2 = nullptr;
	head->neighbor3 = nullptr;
	head->previous = nullptr;
	head->name = "HEAD";
	std::cout << "Adding room 1" << std::endl;

	current = head;
}

bool LinkedMap::doesIntersect(LinkedMap::room* current) {
	for (unsigned i = 0;i < positions.size();i++) {
		if (current->floor.getGlobalBounds().intersects(positions[i].getGlobalBounds())) {
			return true; //checks if floor intersects with another floor (should never)
		}
	}
	return false;
}
//Function determines collision for walls of rooms and hallways
void LinkedMap::doesCollide(Player *player) { //if it collides, move it somewhere else
	if (!current->isCleared) {
		if (current->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			player->canMoveUp = false;
		}
		else {
			player->canMoveUp = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallBottom.getGlobalBounds())) {
			player->canMoveDown = false;
		}
		else {
			player->canMoveDown = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallLeft.getGlobalBounds())) {
			player->canMoveLeft = false;
		}
		else {
			player->canMoveLeft = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallRight.getGlobalBounds())) {
			player->canMoveRight = false;
		}
		else {
			player->canMoveRight = true;
		}
	}
	else {
		if (current->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			player->canMoveUp = false;
		}
		else {
			player->canMoveUp = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallBottom.getGlobalBounds())) {
			player->canMoveDown = false;
		}
		else {
			player->canMoveDown = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallLeft.getGlobalBounds())) {
			player->canMoveLeft = false;
		}
		else {
			player->canMoveLeft = true;
		}
		if (player->getPlayer().getGlobalBounds().intersects(current->wallRight.getGlobalBounds())) {
			player->canMoveRight = false;
		}
		else {
			player->canMoveRight = true;
		}
		//Checks for the hallways of the current room's neighbors
		if (current->neighbor1 != nullptr) {
			//Checks if the neighbor is a right room
			if (current->neighbor1->floor.getPosition().x > current->floor.getPosition().x) {
				//Checks for collision of hallway walls
				if (current->neighbor1->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor1->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor1->hallway->floor.getPosition().y-current->neighbor1->hallway->floor.getSize().y/2&&player->getPlayer().getPosition().y < current->neighbor1->hallway->floor.getPosition().y + current->neighbor1->hallway->floor.getSize().y/2)
					player->canMoveRight = true;
			}
			//Checks if the neighbor is a left room
			if (current->neighbor1->floor.getPosition().x < current->floor.getPosition().x) {
				if (current->neighbor1->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor1->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor1->hallway->floor.getPosition().y - current->neighbor1->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->neighbor1->hallway->floor.getPosition().y + current->neighbor1->hallway->floor.getSize().y / 2)
					player->canMoveLeft = true;
			}
			//Checks if the neighbor is a up room
			if (current->neighbor1->floor.getPosition().y < current->floor.getPosition().y) {
				if (current->neighbor1->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor1->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor1->hallway->floor.getPosition().x - current->neighbor1->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor1->hallway->floor.getPosition().x + current->neighbor1->hallway->floor.getSize().x / 2)
					player->canMoveUp = true;
			}
			//Checks if neighbor is a down room
			if (current->neighbor1->floor.getPosition().y > current->floor.getPosition().y) {
				if (current->neighbor1->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor1->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor1->hallway->floor.getPosition().x - current->neighbor1->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor1->hallway->floor.getPosition().x + current->neighbor1->hallway->floor.getSize().x / 2)
					player->canMoveDown = true;
			}
		}
		if (current->neighbor2 != nullptr) {
			//Checks if neighbor is a right room
			if (current->neighbor2->floor.getPosition().x > current->floor.getPosition().x) {
				if (current->neighbor2->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor2->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor2->hallway->floor.getPosition().y - current->neighbor2->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->neighbor2->hallway->floor.getPosition().y + current->neighbor2->hallway->floor.getSize().y / 2)
					player->canMoveRight = true;
			}
			//Checks if neighbor is a left room
			if (current->neighbor2->floor.getPosition().x < current->floor.getPosition().x) {
				if (current->neighbor2->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor2->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor2->hallway->floor.getPosition().y - current->neighbor2->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->neighbor2->hallway->floor.getPosition().y + current->neighbor2->hallway->floor.getSize().y / 2)
					player->canMoveLeft = true;
			}
			//Checks if neighbor is a up room
			if (current->neighbor2->floor.getPosition().y < current->floor.getPosition().y) {
				if (current->neighbor2->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor2->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor2->hallway->floor.getPosition().x - current->neighbor2->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor2->hallway->floor.getPosition().x + current->neighbor2->hallway->floor.getSize().x / 2)
					player->canMoveUp = true;
			}
			//Checks if neighbor is a down room
			if (current->neighbor2->floor.getPosition().y > current->floor.getPosition().y) {
				if (current->neighbor2->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor2->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor2->hallway->floor.getPosition().x - current->neighbor2->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor2->hallway->floor.getPosition().x + current->neighbor2->hallway->floor.getSize().x / 2)
					player->canMoveDown = true;
			}
		}
		//Checks if current has a 3rd neighbor
		if (current->neighbor3 != nullptr){
			//Checks if neighbor is a right room
			if (current->neighbor3->floor.getPosition().x > current->floor.getPosition().x) {
				if (current->neighbor3->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor3->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor3->hallway->floor.getPosition().y - current->neighbor3->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->neighbor3->hallway->floor.getPosition().y + current->neighbor3->hallway->floor.getSize().y / 2)
					player->canMoveRight = true;
			}
			//Checks if neighbor is a left room
			if (current->neighbor3->floor.getPosition().x < current->floor.getPosition().x) {
				if (current->neighbor3->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->neighbor3->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->neighbor3->hallway->floor.getPosition().y - current->neighbor3->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->neighbor3->hallway->floor.getPosition().y + current->neighbor3->hallway->floor.getSize().y / 2)
					player->canMoveLeft = true;
			}
			//Checks if neighbor is an up room
			if (current->neighbor3->floor.getPosition().y < current->floor.getPosition().y) {
				if (current->neighbor3->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor3->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor3->hallway->floor.getPosition().x - current->neighbor3->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor3->hallway->floor.getPosition().x + current->neighbor3->hallway->floor.getSize().x / 2)
					player->canMoveUp = true;
			}
			//Checks if neighbor is a down room
			if (current->neighbor3->floor.getPosition().y > current->floor.getPosition().y) {
				if (current->neighbor3->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->neighbor3->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->neighbor3->hallway->floor.getPosition().x - current->neighbor3->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->neighbor3->hallway->floor.getPosition().x + current->neighbor3->hallway->floor.getSize().x / 2)
					player->canMoveDown = true;
			}
		}
		//Checks if current has a previous neighbor
		if (current->previous != nullptr&&current!=head) {
			//Checks if neighbor is a left Room
			if (current->floor.getPosition().x > current->previous->floor.getPosition().x) {
				if (current->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->hallway->floor.getPosition().y - current->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->hallway->floor.getPosition().y + current->hallway->floor.getSize().y / 2)
					player->canMoveLeft = true;
			}
			//Checks if neighbor is a right room
			if (current->floor.getPosition().x < current->previous->floor.getPosition().x) {
				if (current->hallway->wallTop.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveUp = false;
				}
				if (current->hallway->wallBottom.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveDown = false;
				}
				if (player->getPlayer().getPosition().y > current->hallway->floor.getPosition().y - current->hallway->floor.getSize().y / 2 && player->getPlayer().getPosition().y < current->hallway->floor.getPosition().y + current->hallway->floor.getSize().y / 2)
					player->canMoveRight = true;
			}
			//Checks if neighbor is an up rooom
			if (current->floor.getPosition().y < current->previous->floor.getPosition().y) {
				if (current->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->hallway->floor.getPosition().x - current->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->hallway->floor.getPosition().x + current->hallway->floor.getSize().x / 2)
					player->canMoveDown = true;
			}
			//Checks if neighbor is a down room
			if (current->floor.getPosition().y > current->previous->floor.getPosition().y) {
				if (current->hallway->wallLeft.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveLeft = false;
				}
				if (current->hallway->wallRight.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
					player->canMoveRight = false;
				}
				if (player->getPlayer().getPosition().x > current->hallway->floor.getPosition().x - current->hallway->floor.getSize().x / 2 && player->getPlayer().getPosition().x < current->hallway->floor.getPosition().x + current->hallway->floor.getSize().x / 2)
					player->canMoveUp = true;
			}
		}
	}
}

//Create room called in add rooms, adds each room to linked map, checking if new square intersects any previous squares
bool LinkedMap::createRoom(LinkedMap::room* current,std::string dir,sf::RenderWindow &window) { //string determines if room placed up down left or right, random
	LinkedMap::room* temp = new room;
	LinkedMap::hallway* tempHallway = new hallway;
	int width = rand() % 350 + 500; //size of room is random between values
	int height = rand() % 350 + 500;
	//Creates a link between current and new neighbor for hallway
	temp->hallway = tempHallway;

	//sets 3 branches to null and previous branch to current
	temp->neighbor1 = nullptr;
	temp->neighbor2 = nullptr;
	temp->neighbor3 = nullptr;
	temp->previous = current;

	//Gives information about size, color and placement of room
	temp->playerIsInside = false;
	temp->bIsVisited = false;

	switch(level){	//floor changes shade colors as levels progress
		case 1:
			temp->hallway->floor.setFillColor(sf::Color::Yellow);
			temp->floor.setFillColor(sf::Color::Yellow);
			break;
		case 2:
			temp->hallway->floor.setFillColor(sf::Color::Red);
			temp->floor.setFillColor(sf::Color::Red);
			break;
		case 3:
			temp->hallway->floor.setFillColor(sf::Color::Magenta);
			temp->floor.setFillColor(sf::Color::Magenta);
			break;
		case 4:
			temp->hallway->floor.setFillColor(sf::Color::Green);
			temp->floor.setFillColor(sf::Color::Green);
		default:
			temp->hallway->floor.setFillColor(sf::Color::Blue);
			temp->floor.setFillColor(sf::Color::Blue);
	}
	//Up room
	if (dir == "Up") {

		temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
		temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
		temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y - (current->floor.getSize().y/2) - temp->hallway->floor.getSize().y/2));

		temp->floor.setSize(sf::Vector2f(width, height));
		temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
		temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y - 850 - ((current->floor.getSize().y + temp->floor.getSize().y)/2));

		//DOWN ROOM
		if (doesIntersect(temp)) {

			temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
			temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
			temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y + (current->floor.getSize().y/2) + temp->hallway->floor.getSize().y/2));

			temp->floor.setSize(sf::Vector2f(width, height));
			temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
			temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y + 850 + ((current->floor.getSize().y + temp->floor.getSize().y)/2));

			//LEFT ROOM
			if (doesIntersect(temp)) {

				temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
				temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
				temp->hallway->floor.setPosition(current->floor.getPosition().x - (current->floor.getSize().x/2) - (temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

				temp->floor.setSize(sf::Vector2f(width, height));
				temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
				temp->floor.setPosition(current->floor.getPosition().x - 850 - ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);

				//RIGHT ROOM
				if (doesIntersect(temp)) {

					temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
					temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
					temp->hallway->floor.setPosition((current->floor.getPosition().x + current->floor.getSize().x/2 + temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

					temp->floor.setSize(sf::Vector2f(width, height));
					temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
					temp->floor.setPosition(current->floor.getPosition().x + 850 + ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);


					if (doesIntersect(temp)) {
						std::cout << "Overlap" << std::endl;
						addRooms(roomsToAdd - count, current->previous, window);
						return false;
					}
				}
			}
		}
	}
	//Down room
	if (dir == "Down") {
		temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
		temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
		temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y + (current->floor.getSize().y/2) + temp->hallway->floor.getSize().y/2));

		temp->floor.setSize(sf::Vector2f(width, height));
		temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
		temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y + 850 + ((current->floor.getSize().y + temp->floor.getSize().y)/2));

		//UP ROOM
		if (doesIntersect(temp)) {
			temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
			temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
			temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y - (current->floor.getSize().y/2) - temp->hallway->floor.getSize().y/2));

			temp->floor.setSize(sf::Vector2f(width, height));
			temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
			temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y - 850 - ((current->floor.getSize().y + temp->floor.getSize().y)/2));
			//LEFT ROOM
			if (doesIntersect(temp)) {
				temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
				temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
				temp->hallway->floor.setPosition(current->floor.getPosition().x - (current->floor.getSize().x/2) - (temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

				temp->floor.setSize(sf::Vector2f(width, height));
				temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
				temp->floor.setPosition(current->floor.getPosition().x - 850 - ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);


				//RIGHT ROOM
				if (doesIntersect(temp)) {
					temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
					temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
					temp->hallway->floor.setPosition((current->floor.getPosition().x + current->floor.getSize().x/2 + temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

					temp->floor.setSize(sf::Vector2f(width, height));
					temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
					temp->floor.setPosition(current->floor.getPosition().x + 850 + ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);
					if (doesIntersect(temp)) {
						std::cout << "Overlap" << std::endl;
						addRooms(roomsToAdd - count, current->previous, window);
						return false;
					}
				}
			}
		}
	}
	//Left room
	if (dir == "Left") {
		temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
		temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
		temp->hallway->floor.setPosition(current->floor.getPosition().x - (current->floor.getSize().x/2) - (temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

		temp->floor.setSize(sf::Vector2f(width, height));
		temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
		temp->floor.setPosition(current->floor.getPosition().x - 850 - ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);

		//DOWN ROOM
		if (doesIntersect(temp)) {
			temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
			temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
			temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y + (current->floor.getSize().y/2) + temp->hallway->floor.getSize().y/2));

			temp->floor.setSize(sf::Vector2f(width, height));
			temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
			temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y + 850 + ((current->floor.getSize().y + temp->floor.getSize().y)/2));

			//UP ROOM
			if (doesIntersect(temp)) {
				temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
				temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
				temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y - (current->floor.getSize().y/2) - temp->hallway->floor.getSize().y/2));

				temp->floor.setSize(sf::Vector2f(width, height));
				temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
				temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y - 850 - ((current->floor.getSize().y + temp->floor.getSize().y)/2));

				//RIGHT ROOM
				if (doesIntersect(temp)) {
					temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
					temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
					temp->hallway->floor.setPosition((current->floor.getPosition().x + current->floor.getSize().x/2 + temp->hallway->floor.getSize().y/2),current->floor.getPosition().y);

					temp->floor.setSize(sf::Vector2f(width, height));
					temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
					temp->hallway->floor.setPosition((current->floor.getPosition().x + current->floor.getSize().x/2 + temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);
					if (doesIntersect(temp)) {
						std::cout << "Overlap" << std::endl;
						addRooms(roomsToAdd - count, current->previous, window);
						return false;
					}
				}
			}
		}
	}
	//Right room
	if (dir == "Right") {
		temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
		temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
		temp->hallway->floor.setPosition((current->floor.getPosition().x + current->floor.getSize().x/2 + temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

		temp->floor.setSize(sf::Vector2f(width, height));
		temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
		temp->floor.setPosition(current->floor.getPosition().x + 850 + ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);

		//LEFT ROOM
		if (doesIntersect(temp)) {
			temp->hallway->floor.setSize(sf::Vector2f(850, hallwayWidth));
			temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
			temp->hallway->floor.setPosition(current->floor.getPosition().x - (current->floor.getSize().x/2) - (temp->hallway->floor.getSize().x/2),current->floor.getPosition().y);

			temp->floor.setSize(sf::Vector2f(width, height));
			temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
			temp->floor.setPosition(current->floor.getPosition().x - 850 - ((current->floor.getSize().x + temp->floor.getSize().x)/2), current->floor.getPosition().y);

			//DOWN ROOM
			if (doesIntersect(temp)) {
				temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
				temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
				temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y + (current->floor.getSize().y/2) + temp->hallway->floor.getSize().y/2));

				temp->floor.setSize(sf::Vector2f(width, height));
				temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
				temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y + 850 + ((current->floor.getSize().y + temp->floor.getSize().y)/2));

				//UP ROOM
				if (doesIntersect(temp)) {
					temp->hallway->floor.setSize(sf::Vector2f(hallwayWidth/1.78, 850));
					temp->hallway->floor.setOrigin(temp->hallway->floor.getSize().x / 2, temp->hallway->floor.getSize().y / 2);
					temp->hallway->floor.setPosition(current->floor.getPosition().x, (current->floor.getPosition().y - (current->floor.getSize().y/2) - temp->hallway->floor.getSize().y/2));

					temp->floor.setSize(sf::Vector2f(width, height));
					temp->floor.setOrigin(temp->floor.getSize().x / 2, temp->floor.getSize().y / 2);
					temp->floor.setPosition(current->floor.getPosition().x, current->floor.getPosition().y - 850 - ((current->floor.getSize().y + temp->floor.getSize().y)/2));

					if (doesIntersect(temp)) {
						std::cout << "Overlap" << std::endl;
						addRooms(roomsToAdd - count, current->previous,window);
						return false;
					}
				}
			}
		}
	}
	temp->name = "room " + std::to_string(count++);
	temp->floor.setTexture(&floorTexture);

	//Walls adding to room and hallway after their creation
	temp->wallTop.setSize(sf::Vector2f(temp->floor.getGlobalBounds().width + 100, 50));
	temp->wallTop.setPosition(temp->floor.getPosition().x - temp->floor.getGlobalBounds().width / 2 - 50, temp->floor.getPosition().y - temp->floor.getGlobalBounds().height / 2 - 50);
	temp->wallTop.setTexture(&floorTexture);
	temp->wallRight.setSize(sf::Vector2f(50, temp->floor.getGlobalBounds().height));
	temp->wallRight.setPosition(temp->floor.getPosition().x + temp->floor.getGlobalBounds().width / 2, temp->floor.getPosition().y - temp->floor.getGlobalBounds().height / 2);
	temp->wallRight.setTexture(&floorTexture);
	temp->wallBottom.setSize(sf::Vector2f(temp->floor.getGlobalBounds().width + 100, 50));
	temp->wallBottom.setPosition(temp->floor.getPosition().x - temp->floor.getGlobalBounds().width / 2 - 50, temp->floor.getPosition().y + temp->floor.getGlobalBounds().height / 2);
	temp->wallBottom.setTexture(&floorTexture);
	temp->wallLeft.setSize(sf::Vector2f(50, temp->floor.getGlobalBounds().height));
	temp->wallLeft.setPosition(temp->floor.getPosition().x - temp->floor.getGlobalBounds().width / 2 - 50, temp->floor.getPosition().y - temp->floor.getGlobalBounds().height / 2);
	temp->wallLeft.setTexture(&floorTexture);
	temp->hallway->floor.setTexture(&floorTexture);
	if (temp->hallway->floor.getSize().x > temp->hallway->floor.getSize().y) {
		temp->hallway->wallTop.setSize(sf::Vector2f(temp->hallway->floor.getGlobalBounds().width, 50));
		temp->hallway->wallTop.setPosition(temp->hallway->floor.getPosition().x - temp->hallway->floor.getGlobalBounds().width / 2, temp->hallway->floor.getPosition().y - temp->hallway->floor.getGlobalBounds().height / 2 - 50);
		temp->hallway->wallTop.setTexture(&floorTexture);
		temp->hallway->wallBottom.setSize(sf::Vector2f(temp->hallway->floor.getGlobalBounds().width, 50));
		temp->hallway->wallBottom.setPosition(temp->hallway->floor.getPosition().x - temp->hallway->floor.getGlobalBounds().width / 2, temp->hallway->floor.getPosition().y + temp->hallway->floor.getGlobalBounds().height / 2);
		temp->hallway->wallBottom.setTexture(&floorTexture);
	}
	else if (temp->hallway->floor.getSize().x < temp->hallway->floor.getSize().y) {
		temp->hallway->wallRight.setSize(sf::Vector2f(50, temp->hallway->floor.getGlobalBounds().height));
		temp->hallway->wallRight.setPosition(temp->hallway->floor.getPosition().x + temp->hallway->floor.getGlobalBounds().width / 2, temp->hallway->floor.getPosition().y - temp->hallway->floor.getGlobalBounds().height / 2);
		temp->hallway->wallRight.setTexture(&floorTexture);
		temp->hallway->wallLeft.setSize(sf::Vector2f(50, temp->hallway->floor.getGlobalBounds().height));
		temp->hallway->wallLeft.setPosition(temp->hallway->floor.getPosition().x - temp->hallway->floor.getGlobalBounds().width / 2 - 50, temp->hallway->floor.getPosition().y - temp->hallway->floor.getGlobalBounds().height / 2);
		temp->hallway->wallLeft.setTexture(&floorTexture);
	}
	if (current->neighbor1 == nullptr) {
		current->neighbor1 = temp;
	}
	else if (current->neighbor2 == nullptr) {
		current->neighbor2 = temp;
	}
	else if (current->neighbor3 == nullptr) {
		current->neighbor3 = temp;
	}
	std::cout << "Adding room " << count << std::endl;
	positions.push_back(temp->floor);
	window.clear();
	window.display();
	return true;
}

void LinkedMap::addRooms(int rooms, room* current, sf::RenderWindow &window) {
	//if no more rooms to create, return
	if (rooms <= 0||current == nullptr) {
		return;
	}
	//each room has random number of neighbors
	int neighbors = rand() % 3 + 1;

    if (current->neighbors == 0) {
		current->neighbors = neighbors;

        for (int i = 0;i < neighbors&&rooms>0;i++) {

            int r = rand() % 4;
			switch (r) {
			case 0:
				if (!createRoom(current, "Up", window))
					return;
				break;
			case 1:
				if (!createRoom(current, "Left", window))
					return;
				break;
			case 2:
				if (!createRoom(current, "Down", window))
					return;
				break;
			case 3:
				if (!createRoom(current, "Right", window))
					return;
			default:
				break;
			}
			rooms--;
		}
		int r = 0;
		if (neighbors == 1) {
			addRooms(rooms, current->neighbor1, window);
		}
		else if (neighbors == 2) {
			if (rooms == 1) {
				r = 1;
			}
			else if (rooms <= 0) {
				r = 0;
			}
			else {
				r = rand() % (rooms - 1) + 1;
			}
			addRooms(r, current->neighbor1, window);
			addRooms(rooms - r, current->neighbor2, window);
		}
		else if (neighbors == 3) {
			if (rooms == 1) {
				r = 1;
			}
			else if (rooms <= 0) {
				r = 0;
			}
			else {
				r = rand() % (rooms - 1) + 1;
			}
			addRooms(r, current->neighbor1, window); //recursively calls a random number of rooms to be added to neighboring room
			rooms -= r;
			if (rooms == 1) {
				r = 1;
			}
			else if (rooms <= 0) {
				r = 0;
			}
			else {
				r = rand() % (rooms - 1) + 1;
			}
			addRooms(r, current->neighbor2, window); //adds another random amount of rooms
			addRooms(rooms - r, current->neighbor3, window); //adds rest of rooms
		}
	}
	else if(current->neighbors<3) {
		neighbors = 3-current->neighbors;
		for (int i = 0;i < neighbors&&rooms>0;i++) {
			int r = rand() % 3;
			switch (r) {
			case 0:
				if (!createRoom(current, "Up", window))
					return;
				break;
			case 1:
				if (!createRoom(current, "Left", window))
					return;
				break;
			case 2:
				if (!createRoom(current, "Down", window))
					return;
				break;
			case 3:
				if (!createRoom(current, "Right", window))
					return;
			default:
				break;
			}
			rooms--;
		}
		int r = 0;
		if (neighbors == 1) {
			addRooms(rooms, current->neighbor3, window);
		}
		else if (neighbors == 2) {
			if (rooms == 1) {
				r = 1;
			}
			else if (rooms <= 0) {
				r = 0;
			}
			else {
				r = rand() % (rooms - 1) + 1;
			}

            addRooms(r, current->neighbor2, window); //adds random amount of rooms to neighbor 2
			addRooms(rooms - r, current->neighbor3, window); //adds remaining rooms to neighbor 3
		}
		else {
			std::cout << "THIS SHOULDN'T HAPPEN" << std::endl;
		}
	}
	else {
		return;
	}
}

//if room has been visited, draws it on map when M is pressed by user, also draws hallways to unexplored rooms
void LinkedMap::displayMap(room* current, sf::RenderWindow &window) {
	if (current == head) {
		window.draw(current->floor);
	}
	if (current->neighbor1 != nullptr) {
		if(current->neighbor1->bIsVisited) {
			window.draw(current->neighbor1->floor);
			window.draw(current->neighbor1->hallway->floor);

		} else if (current->bIsVisited){
			window.draw(current->neighbor1->hallway->floor);
		}
		if (current->neighbor1->neighbors>0) {
			displayMap(current->neighbor1, window);
		}
	}
	if (current->neighbor2 != nullptr){
		if(current->neighbor2->bIsVisited) {
			window.draw(current->neighbor2->floor);
			window.draw(current->neighbor2->hallway->floor);
		} else if(current->bIsVisited){
			window.draw(current->neighbor2->hallway->floor);
		}

		if (current->neighbor2->neighbors>0) {
			displayMap(current->neighbor2, window);
		}

	}
	if (current->neighbor3 != nullptr) {
		if(current->neighbor1->bIsVisited) {
			window.draw(current->neighbor3->floor);
			window.draw(current->neighbor3->hallway->floor);

		} else if(current->bIsVisited){
			window.draw(current->neighbor3->hallway->floor);
		}

		if (current->neighbor3->neighbors>0) {
			displayMap(current->neighbor3, window);
		}
	}
}

void LinkedMap::displayCurrentRoom(room * curr, sf::RenderWindow &window, bool cleared) {
	if (cleared) {
		if (curr != nullptr) { //draws floor and then hallways
			window.draw(curr->floor);
			window.draw(curr->wallTop);
			window.draw(curr->wallRight);
			window.draw(curr->wallLeft);
			window.draw(curr->wallBottom);
			if (curr != head) {
				window.draw(curr->hallway->floor);
				window.draw(curr->hallway->wallTop);
				window.draw(curr->hallway->wallRight);
				window.draw(curr->hallway->wallLeft);
				window.draw(curr->hallway->wallBottom);
			}
		}
	}

	if (curr->neighbor1 != nullptr) {
		displayCurrentRoom(curr->neighbor1, window, cleared);
	}
	if (curr->neighbor2 != nullptr) {
		displayCurrentRoom(curr->neighbor2,window,cleared);
	}
	if (curr->neighbor3 != nullptr) {
		displayCurrentRoom(curr->neighbor3, window, cleared);
	}

	if (!cleared) {	//draws floor and then hallways
		if (current != nullptr) {
			if (current != head) {
				window.draw(current->hallway->floor);
				window.draw(current->hallway->wallTop);
				window.draw(current->hallway->wallRight);
				window.draw(current->hallway->wallLeft);
				window.draw(current->hallway->wallBottom);
			}
			window.draw(current->floor);
			window.draw(current->wallTop);
			window.draw(current->wallRight);
			window.draw(current->wallLeft);
			window.draw(current->wallBottom);
		}
	}
}

//gets end room for stairs to be placed to go to next level
void LinkedMap::findStairRoom(room* current){
    if (current->neighbor2 != nullptr) {
        if(current->neighbor2->neighbors == 0){
        	endRoomCount++;
            levelUpRoom = current->neighbor2;
        } else{
            findStairRoom(current->neighbor2);
        }
    }
    if (current->neighbor3 != nullptr) {
		if(current->neighbor3->neighbors == 0){
			endRoomCount++;
			levelUpRoom = current->neighbor3;

		} else{
			findStairRoom(current->neighbor3);
		}
    }
    if (current->neighbor1 != nullptr) {
    	if(current->neighbor1->neighbors == 0){
    		endRoomCount++;
			levelUpRoom = current->neighbor1;
		} else{
			findStairRoom(current->neighbor1);
		}
    }
}
//places stairs in stair room found in last method
void LinkedMap::placeStairs(){
	if (stairTexture.loadFromFile("Sprites/Map/Stairs.png")) {
		stairs.setTexture(stairTexture);

	}
	stairs.setOrigin(stairs.getGlobalBounds().width/2,stairs.getGlobalBounds().height/2);
    stairs.setPosition(levelUpRoom->floor.getPosition());

}
//level up text displayed when player walks over stairs
void LinkedMap::placeLevelUpText(){
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(24);
	levelUpText.setString("Press 'F' to Descend");
	levelUpText.setPosition(stairs.getPosition().x + 40,stairs.getPosition().y - 40);

}
//finds up to 2 available end rooms (rooms with no neighbors) to create chests in
void LinkedMap::findChestRoom(room* current){
	if(endRoomCount > 3){
		endRoomCount = 3;
	}
	if(endRoomCount >= 2){
		if (current->neighbor2 != nullptr) {
			if(current->neighbor2->neighbors == 0 && current->neighbor2 != levelUpRoom){
				if(chestRoom == nullptr){
					endRoomCount--;
					chestRoom = current->neighbor2;
				} else if(chestRoom2 == nullptr){
					endRoomCount--;
					chestRoom2 = current->neighbor2;
				} else{
					return;
				}

			} else{
				findChestRoom(current->neighbor2);
			}
		}
		if (current->neighbor3 != nullptr) {
			if(current->neighbor3->neighbors == 0 && current->neighbor3 != levelUpRoom){
				if(chestRoom == nullptr){
					endRoomCount--;
					chestRoom = current->neighbor3;
				} else if(chestRoom2 == nullptr){
					endRoomCount--;
					chestRoom2 = current->neighbor3;
				} else{
					return;
				}

			} else{
				findChestRoom(current->neighbor3);
			}
		}
		if (current->neighbor1 != nullptr) {
			if(current->neighbor1->neighbors == 0 && current->neighbor1 != levelUpRoom){
				if(chestRoom == nullptr){
					endRoomCount--;
					chestRoom = current->neighbor1;
				} else if(chestRoom2 == nullptr){
					endRoomCount--;
					chestRoom2 = current->neighbor1;
				} else{
					return;
				}
			} else{
				findChestRoom(current->neighbor1);
			}
		}
	}
}
//places between 1 and 2 chests in chest rooms
void LinkedMap::placeChests() {

	if (chestRoom != nullptr) {
		chest1->setData(chestRoom->floor.getPosition().x,chestRoom->floor.getPosition().y);
	}

	if (chestRoom2 != nullptr) {
		chest2->setData(chestRoom2->floor.getPosition().x,chestRoom2->floor.getPosition().y);
		}
	}
//text appears when near chest to open it
void LinkedMap::placeChestText(){
	chestText.setFont(font);
	chestText.setCharacterSize(24);
	chestText.setString("Press 'F' to Open");
}
//draws stairs if you're in stair room
bool LinkedMap::displayStairs(sf::RenderWindow &window, Player* player){
    window.draw(stairs);
    if(player->getPlayer().getGlobalBounds().intersects(stairs.getGlobalBounds())){
		window.draw(levelUpText);
		return true;
    }
    return false;
}
//draws chest one if player is in chest 1 room
bool LinkedMap::displayChest1(sf::RenderWindow &window, Player* player, bool isLookedAt){
	if(chest1 != nullptr) {
		window.draw(chest1->getChestSprite());

		if (player->getPlayer().getGlobalBounds().intersects(chest1->getChestSprite().getGlobalBounds()) && !chest1->getIsOpen() && !isLookedAt) {
			chestText.setPosition(chest1->getChestSprite().getPosition());
			window.draw(chestText);
			return true;
		}
	}
	return false;
}
//draws chest two if player is in chest 2 room
bool LinkedMap::displayChest2(sf::RenderWindow &window, Player* player, bool isLookedAt){
	if(chest2 != nullptr) {
		window.draw(chest2->getChestSprite());
		if (player->getPlayer().getGlobalBounds().intersects(chest2->getChestSprite().getGlobalBounds()) && !chest2->getIsOpen() && !isLookedAt) {
			chestText.setPosition(chest2->getChestSprite().getPosition());
			window.draw(chestText);
			return true;
		}
	}
	return false;
}

void LinkedMap::printRoomNames(room* current) {
	if (current->neighbor2 != nullptr) {
		std::cout << current->neighbor2->name<<std::endl;

		if (current->neighbor2->neighbors>0) {
			printRoomNames(current->neighbor2);
		}
	}
	if (current->neighbor3 != nullptr) {
		std::cout << current->neighbor3->name << std::endl;
		if (current->neighbor3->neighbors>0) {
			printRoomNames(current->neighbor3);
		}
	}
	if (current->neighbor1 != nullptr) {
	    std::cout << current->neighbor1->name << std::endl;
		if (current->neighbor1->neighbors>0) {
			printRoomNames(current->neighbor1);
		}
	}
}

//Method determines which rooms have been visited, which room the player is currently in and whether or not a room is cleared
void LinkedMap::findCurrentRoom(room* checkedRoom, Player* player){

    if(checkedRoom == head){

		if (checkedRoom->floor.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			if (!checkedRoom->bIsVisited) {
				checkedRoom->bIsVisited = true;
				checkedRoom->isCleared = false;
			}
			checkedRoom->playerIsInside = true;
			current = checkedRoom;
		}
		else {
			checkedRoom->playerIsInside = false;
		}
	}

	if (checkedRoom->neighbor1 != nullptr) {

		if (checkedRoom->neighbor1->floor.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			if (!checkedRoom->neighbor1->bIsVisited) {
				checkedRoom->neighbor1->bIsVisited = true;
				checkedRoom->neighbor1->isCleared = false;
			}
			checkedRoom->neighbor1->playerIsInside = true;
			current = checkedRoom->neighbor1;
			return;
		}
		else {
			checkedRoom->neighbor1->playerIsInside = false;
		}
		if (checkedRoom->neighbor1->neighbors>0) {
			findCurrentRoom(checkedRoom->neighbor1, player);
		}
	}

	if (checkedRoom->neighbor2 != nullptr) {
		if (checkedRoom->neighbor2->floor.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			if (!checkedRoom->neighbor2->bIsVisited) {
				checkedRoom->neighbor2->bIsVisited = true;
				checkedRoom->neighbor2->isCleared = false;
			}
			checkedRoom->neighbor2->playerIsInside = true;
			current = checkedRoom->neighbor2;
			return;
		}
		else {
			checkedRoom->neighbor2->playerIsInside = false;
		}

        if (checkedRoom->neighbor2->neighbors>0) {
            findCurrentRoom(checkedRoom->neighbor2, player);
        }
    }
    if (checkedRoom->neighbor3 != nullptr) {

		if (checkedRoom->neighbor3->floor.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())) {
			if (!checkedRoom->neighbor3->bIsVisited) {
				checkedRoom->neighbor3->bIsVisited = true;
				checkedRoom->neighbor3->isCleared = false;
			}
			checkedRoom->neighbor3->playerIsInside = true;
			current = checkedRoom->neighbor3;
			return;
		}
		else {
			checkedRoom->neighbor3->playerIsInside = false;
		}

        if (checkedRoom->neighbor3->neighbors>0) {
            findCurrentRoom(checkedRoom->neighbor3, player);
        }
    }
    if (checkedRoom->neighbor1 != nullptr) {

        if(checkedRoom->neighbor1->floor.getGlobalBounds().intersects(player->getPlayer().getGlobalBounds())){
            current = checkedRoom->neighbor1;
            checkedRoom->neighbor1->bIsVisited = true;
            checkedRoom->neighbor1->playerIsInside = true;
            return;
        } else{
            checkedRoom->neighbor1->playerIsInside = false;
        }
        if (checkedRoom->neighbor1->neighbors>0) {
            findCurrentRoom(checkedRoom->neighbor1, player);
        }
    }
}
//returns current room
LinkedMap::room* LinkedMap::getCurrentRoom(){
    return(current);
}

LinkedMap::room* LinkedMap::getHead(){
    return(head);
}

LinkedMap::room* LinkedMap::getLevelUpRoom(){
    return(levelUpRoom);
}

LinkedMap::room* LinkedMap::getChestRoom1(){
	return(chestRoom);
}

LinkedMap::room* LinkedMap::getChestRoom2(){
	return(chestRoom2);
}

Chest* LinkedMap::getChest1(){
	return chest1;
}
Chest* LinkedMap::getChest2(){
	return chest2;
}

void LinkedMap::displayChestUI(Player* player, sf::RenderWindow& window){

    if(chest1 != nullptr){
        if(current == chestRoom && chest1->getIsOpen()){
            chest1->setUI(player,window);
        }

    }
    if(chest2 != nullptr){
       if(current == chestRoom2 && chest2->getIsOpen()){
           chest2->setUI(player, window);
       }
    }

}