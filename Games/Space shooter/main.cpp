#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <list>
#include <cstdlib>

using namespace sf;

class Bullet
{
	Texture* texture;

public:

	Sprite shape;

	Bullet(const Texture const* texture)
	{
		shape.setTexture(*texture);
		shape.setScale(0.07f, 0.07f);
	}
};

class Creature
{
	int hp;
	int maxHp;

public:

	Sprite shape;
	Text text;

	Creature(const Texture const* texture, const Font const* textFont, int maxHp)
	{
		shape.setTexture(*texture);
		text.setFont(*textFont);
		text.setCharacterSize(14u);
		text.setStyle(Text::Bold);
		this->maxHp = maxHp;
		hp = maxHp;
	}

	const int getHp() const 
	{
		return hp;
	}

	const int getMaxHp() const
	{
		return maxHp;
	}

	void reduceHp(int points)
	{
		hp -= points;
	}

	void drawHpInfo(RenderWindow& window, const Vector2f& textPosition)
	{
		text.setString(std::to_string((long long)hp) + "/" + std::to_string((long long)maxHp));
		text.setPosition(textPosition);
		window.draw(text);
	}
};

class Player : public Creature
{
public:

	std::list<Bullet> bullets;

	Player(const Texture const* texture, const Font const* font)
		: Creature(texture, font, 10)
	{
		shape.setTexture(*texture);
		shape.setScale(0.1f, 0.1f);
	}
};

class Enemy : public Creature
{
public:

	Enemy(const Texture const* texture, const Font const* font)
		: Creature(texture, font, rand() % 5 + 1)
	{
		shape.setTexture(*texture);
		shape.setScale(0.2f, 0.2f);
	}
};

const int TIME_TO_SHOOT = 20;
const int TIME_TO_SPAWN = 50;

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(720, 480), "Space brawl");
	window.setFramerateLimit(60);

	// UI
	Font font;
	if (!font.loadFromFile("Fonts/Metropolian-Display.ttf"))
		throw std::runtime_error("Could not load font.");
	Text gameOverText("GAME OVER!", font, 40);
	gameOverText.setColor(Color::Red);
	gameOverText.setStyle(Text::Bold);
	gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2,
							 window.getSize().y / 2 - gameOverText.getGlobalBounds().height);

	// Player
	Texture playerTexture;
	if (!playerTexture.loadFromFile("Textures/player.png"))
		throw std::runtime_error("Could not load texture for player.");
	Player player(&playerTexture, &font);
	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(17);
	scoreText.setPosition(10.f, 10.f);
	scoreText.setStyle(Text::Bold);

	// Bullet
	Texture bulletTexture;
	if (!bulletTexture.loadFromFile("Textures/missileRight.png"))
		throw std::runtime_error("Could not load texture for bullet.");
	Bullet bullet(&bulletTexture);
	int shootTimer = 0;

	// Enemy
	Texture enemyTexture;
	if (!enemyTexture.loadFromFile("Textures/enemy.png"))
		throw std::runtime_error("Could not load texture for enemy.");
	std::list<Enemy> enemies;
	int spawnTimer = 0;


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();		
		
		if (player.getHp() > 0)
		{
			// UPDATE

			// Player update

			// Movement
			if (Keyboard::isKeyPressed(Keyboard::W))
				player.shape.move(0.f, -10.f);
			if (Keyboard::isKeyPressed(Keyboard::A))
				player.shape.move(-10.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::S))
				player.shape.move(0.f, 10.f);
			if (Keyboard::isKeyPressed(Keyboard::D))
				player.shape.move(10.f, 0.f);

			// Preventing the player from going outside the window
			if (player.shape.getPosition().y < 0)
				player.shape.setPosition(player.shape.getPosition().x, 0.f);
			if (player.shape.getPosition().x < 0)
				player.shape.setPosition(0.f, player.shape.getPosition().y);
			if (player.shape.getPosition().y > window.getSize().y - player.shape.getGlobalBounds().height)
				player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);
			if (player.shape.getPosition().x > window.getSize().x - player.shape.getGlobalBounds().width)
				player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width,  player.shape.getPosition().y);


			// Enemies update
			if (spawnTimer < TIME_TO_SPAWN)
				++spawnTimer;
			if (spawnTimer >= TIME_TO_SPAWN)
			{
				spawnTimer = 0;
				Enemy enemy(&enemyTexture, &font);
				enemy.shape.setPosition(window.getSize().x - enemy.shape.getGlobalBounds().width, rand() % 
										static_cast<int>(window.getSize().y - enemy.shape.getGlobalBounds().height));
				enemies.push_back(enemy);
			}

			for (auto enem = enemies.begin(); enem != enemies.end(); ++enem)
			{
				enem->shape.move(-1.f, 0.f);

				// Out of window bounds
				if (enem->shape.getPosition().x < 0 - enem->shape.getGlobalBounds().width)
				{
					enemies.erase(enem);
					break;
				}

				// Collision with player
				if (enem->shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enem);
					player.reduceHp(1);
					break;
				}
			}


			// Bullets update
			if (shootTimer < TIME_TO_SHOOT)
				++shootTimer;
			if (Keyboard::isKeyPressed(Keyboard::Space) && shootTimer >= TIME_TO_SHOOT)
			{
				shootTimer = 0;
				bullet.shape.setPosition(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2,
										 player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2 - 
										 bullet.shape.getGlobalBounds().height / 2);
				player.bullets.push_back(bullet);
			}

			bool collided = false;
			for (auto projectile = player.bullets.begin(); projectile != player.bullets.end(); ++projectile)
			{
				projectile->shape.move(20.f, 0.f);
				if (projectile->shape.getPosition().x > window.getSize().x)
				{
					player.bullets.erase(projectile);
					break;
				}

				// Collision of enemy and bullet
				for (auto enem = enemies.begin(); enem != enemies.end(); ++enem)
				{
					if (projectile->shape.getGlobalBounds().intersects(enem->shape.getGlobalBounds()))
					{
						if (enem->getHp() <= 1)
						{
							score += enem->getMaxHp();
							enemies.erase(enem);
						}
						else 
							enem->reduceHp(1);
						player.bullets.erase(projectile);
						collided = true;
						break;
					}
				}
				if (collided)
					break;
			}
		}
		scoreText.setString("Score: " + std::to_string((long long)score));

		// CLEAR, DRAW, DISPLAY
		window.clear();

		window.draw(player.shape);
		player.drawHpInfo(window, Vector2f(player.shape.getPosition().x,
										   player.shape.getPosition().y - player.text.getGlobalBounds().height * 1.5));
		if (player.getHp() <= 0)
		{
			player.text.setColor(Color::Red);
			window.draw(gameOverText);
		}
		else
		{
			for (auto enem = enemies.begin(); enem != enemies.end(); ++enem)
			{
				window.draw(enem->shape);
				enem->drawHpInfo(window, Vector2f(enem->shape.getPosition().x, enem->shape.getPosition().y + 10.f));
			}
		}
		for (auto bul = player.bullets.begin(); bul != player.bullets.end(); ++bul)
			window.draw(bul->shape);
		window.draw(scoreText);

		window.display();
	}

	return 0;
}