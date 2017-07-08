/* 
This is an asteroids like the game asteroids (but worse in every single way)
It is not fun to play and I would not reccomend it.
*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define PI 3.14159265
float dt = 0.0166666667;

//Bullet class
class Bullet
{
public:
	Bullet(sf::Vector2f playerPos, sf::Vector2f mosuePos, sf::Vector2f currentVel);
	~Bullet();

	void updatePosition();
	sf::Vector2f checkBounds();
	float fr=300;

	float vx=0;
	float vy=0;

	float px=0;
	float py=0;

	sf::CircleShape bulletShape;


private:
	
	float mass=1;
	float dpx = 0;
	float dpy = 0;
	float angle = 0;
	
};
Bullet::Bullet(sf::Vector2f playerPos,sf::Vector2f mousePos,sf::Vector2f currentVel)
{
	px = playerPos.x;
	dpx = (px - mousePos.x);
	py = playerPos.y;
	dpy = (mousePos.y-py);
	angle = atan2(dpy, dpx)-(PI/2);
	vy += currentVel.y;
	vx += currentVel.x;
	bulletShape.setPosition(px, py);
	bulletShape.setRadius(5);
	bulletShape.setFillColor(sf::Color::Yellow);
}
void Bullet::updatePosition() {
	vy += (fr*cos(angle) / mass)*dt;
	py += vy*dt;

	vx += (fr*sin(angle) / mass)*dt;
	px += vx*dt;

	bulletShape.setPosition(checkBounds());

	fr = 0;
}
sf::Vector2f Bullet :: checkBounds() {
	
		if (px > 1280) {
			vx = vx*0.5;		
			px = 0;
			//delete this;
		}
		
		if (px < 0) {
			vx = vx*0.5;
			px = 1280;
			//delete this;
		}
		if (py > 960) {
			vy = vy*0.5;
			py = 0;
			//delete this;
		}
		if (py < 0) {
			vy = vy*0.5;
			py = 960;
			//delete this;
		}
		
		sf::Vector2f vector(px, py);
		return vector;
}
Bullet::~Bullet()
{
	
}

//Enemy class
class Enemy
{
public:
	sf::CircleShape enemyShape;
	void updatePosition(sf::Vector2f playerPosition);

	Enemy();
	~Enemy();
	sf::Vector2f force = { 0,0 };
	sf::Vector2f vel = { 0,0 };
	sf::Vector2f pos = { 0,0 };

private:
	int mass = 1;
	sf::Vector2f checkBounds();

};
Enemy::Enemy()
{
	srand(time(NULL));
	pos.y = rand()%980;
	pos.x = rand() % 500;
	enemyShape.setPosition(pos);
	enemyShape.setRadius(10);
	enemyShape.setFillColor(sf::Color::Red);
}
void Enemy::updatePosition(sf::Vector2f playerPosition ) {
	
	if (playerPosition.y < pos.y) {
		force.y = -0.1;
	}
	else {
		force.y = 0.1;
	}
	vel.y += (force.y / mass)*dt;
	pos.y += vel.y*dt;

	if (playerPosition.x < pos.x) {
		force.x = -0.1;
	}
	else {
		force.x = 0.1;
	}
	vel.x += (force.x / mass)*dt;
	pos.x += vel.x*dt;

	enemyShape.setPosition(checkBounds());
}
sf::Vector2f Enemy::checkBounds() {

	if (pos.x > 1280) {
		vel.x = vel.x*0.01;
		pos.x = 0;
	}
	if (pos.x < 0) {
		vel.x = vel.x*0.01;
		pos.x = 1280;
	}
	if (pos.y > 960) {
		vel.y = vel.y*0.01;
		pos.y = 0;
	}
	if (pos.y < 0) {
		vel.y = vel.y*0.01;
		pos.y = 960;
	}
	return pos;
}
Enemy::~Enemy()
{
}

//Player class
class Player
{
public:
	sf::RectangleShape playerShape;
	void updatePosition();

	Player();
	~Player();
	sf::Vector2f force = { 0,0 };

	sf::Vector2f velocity={ 0,0 };

	sf::Vector2f position= { 0,0 };

private:
	int mass = 1;
	sf::Vector2f checkBounds();

};
Player::Player()
{
	playerShape.setSize(sf::Vector2f(10, 10));
	playerShape.setOutlineColor(sf::Color::Red);
	playerShape.setOutlineThickness(5);
	playerShape.setPosition(10, 20);
}
void Player::updatePosition() {
	velocity.y += (force.y / mass)*dt;
	position.y += velocity.y*dt;

	velocity.x += (force.x / mass)*dt;
	position.x += velocity.x*dt;

	playerShape.setPosition(checkBounds());

	force.x = 0;
	force.y = 0;

}
sf::Vector2f Player::checkBounds() {

	if (position.x > 1280) {
		velocity.x = velocity.x*0.5;
		position.x = 0;
	}
	if (position.x < 0) {
		velocity.x = velocity.x*0.5;
		position.x = 1280;
	}
	if (position.y > 960) {
		velocity.y = velocity.y*0.5;
		position.y = 0;
	}
	if (position.y < 0) {
		velocity.y = velocity.y*0.5;
		position.y = 960;
	}
	return position;
}
Player::~Player()
{
}

std::vector<Enemy> EnemyArray;
std::vector<Bullet> BulletArray;

void death(sf::RenderWindow* window) {
	sf::Text deathtext;
	sf::Font font;
	sf::Vector2f pos = { 500,500 };
	font.loadFromFile("AvenirNextLTPro-Cn.otf");
	deathtext.setString("You died");
	deathtext.setFont(font);
	deathtext.setPosition(pos);
	deathtext.setCharacterSize(100);
	window->draw(deathtext);
	window->display();
	system("pause");
}

int checkCollisions(sf::RenderWindow * window, sf::RectangleShape playerShape) {
	for (int i = 0; i < EnemyArray.size(); i++) {
		for (int j = 0; j < BulletArray.size(); j++) {
			if (EnemyArray[i].enemyShape.getGlobalBounds().intersects(BulletArray[j].bulletShape.getGlobalBounds())) {
				EnemyArray.erase(EnemyArray.begin() + i);
				BulletArray.erase(BulletArray.begin() + j);
			}
		}
		if (EnemyArray[i].enemyShape.getGlobalBounds().intersects(playerShape.getGlobalBounds())) {
			death(window);
			return 0;
		}

	}
}

void spawner() {
	while (EnemyArray.size() < 5) {
		Enemy newEnemy;
		EnemyArray.push_back(newEnemy);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 970), "Game");
	Player player;
	sf::Mouse mouse;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::W) {
					player.force.y = -50;
				}
				if (event.key.code == sf::Keyboard::A) {
					player.force.x = -50;

				}
				if (event.key.code == sf::Keyboard::S) {
					player.force.y = +50;
				}
				if (event.key.code == sf::Keyboard::D) {
					player.force.x = +50;
				}
				if (event.key.code == sf::Keyboard::Space) {
					if (BulletArray.size() < 20) {
						sf::Vector2f mousePos = window.mapPixelToCoords(mouse.getPosition(window));
						Bullet bullet(player.position, mousePos, player.velocity);
						BulletArray.push_back(bullet);
					}
				}
			}
		}

		if (checkCollisions(&window, player.playerShape) == 0) {
			return 0;
		}

		spawner();

		window.clear();

		player.updatePosition();
		window.draw(player.playerShape);

		for (int j = 0; j < EnemyArray.size(); j++) {
			EnemyArray[j].updatePosition(player.position);
			window.draw(EnemyArray[j].enemyShape);

		}
		for (int z = 0; z < BulletArray.size(); z++) {
			BulletArray[z].updatePosition();
			window.draw(BulletArray[z].bulletShape);
		}
		window.display();
	}
	return 0;
}


