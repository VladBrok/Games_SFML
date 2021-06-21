#include <SFML/Graphics.hpp>
#include <forward_list>

/**
 * Don't let the cats get to the left side of the screen!
 */

using namespace sf;

const int TIME_TO_SPAWN = 47;
const int MAX_HP = 10;

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(640, 480), "Cat do(d)ge");
	window.setFramerateLimit(60);

	// Cats (enemies)
	Texture catTexture;
	Sprite cat;

	if (!catTexture.loadFromFile("Textures/cat.png"))
		throw "Could not load cat.png.";

	cat.setTexture(catTexture);
	cat.setScale(0.1f, 0.1f);
	std::forward_list<Sprite> cats;
	int spawnTimer = 0;
	
	// Doge (player)
	Texture dogeTexture;
	Sprite doge;

	if (!dogeTexture.loadFromFile("Textures/doge.png")) 
		throw "Could not load doge.png.";

	doge.setTexture(dogeTexture);
	doge.setScale(0.21f, 0.21f);

	// HP and HP bar
	int hp = MAX_HP;
	RectangleShape hpBar(Vector2f(hp * 20.f, 20.f));
	hpBar.setPosition( (window.getSize().x - hpBar.getSize().x) / 2, 5.f) ;
	hpBar.setFillColor(Color::Red);

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
		
		// Updating doge
		if (Mouse::getPosition(window).y - doge.getGlobalBounds().height / 2 > 0 &&
			Mouse::getPosition(window).y < window.getSize().y - doge.getGlobalBounds().height / 2)
		{
			doge.setPosition(5.f, Mouse::getPosition(window).y - doge.getGlobalBounds().height / 2);
		}

		// Updating cats
		if (spawnTimer < TIME_TO_SPAWN)
			++spawnTimer;
		if (spawnTimer >= TIME_TO_SPAWN)
		{
			spawnTimer = 0;
			cat.setPosition(window.getSize().x, rand() % static_cast<int>(
						   (window.getSize().y - cat.getGlobalBounds().height)));
			cats.push_front(cat);
		}

		// Collisions
		auto prev_enemy = cats.before_begin();
		for (auto enemy = cats.begin(); enemy != cats.end(); ++enemy)
		{
			(enemy->getPosition().x > doge.getGlobalBounds().width) ? 
			 enemy->move(-4.4f, 0.f) : enemy->move(-6.f, 0.f); // Making cats that passed the doge move faster
			if (enemy->getPosition().x + enemy->getGlobalBounds().width < 0)
			{
				cats.erase_after(prev_enemy);
				--hp;
				hpBar.setSize(Vector2f(hp * 20.f, hpBar.getSize().y));
				
				// GAME OVER
				if (hp == 0)
					window.close();
				break;
			}

			// Collision of doge and cat
			else if (enemy->getGlobalBounds().intersects(doge.getGlobalBounds())) 
			{
				cats.erase_after(prev_enemy);
				break;
			}
			prev_enemy = enemy;
		}

		// Clear, draw, display
		window.clear();

		window.draw(doge);
		window.draw(hpBar);
		for (auto c = cats.begin(); c != cats.end(); ++c)
		{
			window.draw(*c);
		}

		window.display();
	}

	return 0;
}













//#include <SFML/Graphics.hpp>
//#include <list>
//
///**
// * Simple shooter
// * Goal: kill all these stupid squares!
// */
//
//using namespace sf;
//
//int main()
//{
//	srand(time(0));
//	RenderWindow window(VideoMode(640, 480), "Ball vs squares");
//	window.setFramerateLimit(60);
//
//	CircleShape player(40.f);
//	player.setFillColor(Color::White);
//	player.setPosition(window.getSize().x / 2 - player.getRadius(), window.getSize().y - player.getRadius() * 2 - 10.f);
//
//	CircleShape projectile(8.f);
//	projectile.setFillColor(Color::Red);
//	std::list<CircleShape> projectiles;
//	int shotTimer = 0;
//
//	RectangleShape enemy(Vector2f(38.f, 38.f));
//	enemy.setFillColor(Color::Magenta);
//	std::list<RectangleShape> enemies;
//	int spawnTimer = 0;
//
//	while (window.isOpen())
//	{
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//			{
//				window.close();
//			}
//		}
//		if (Keyboard::isKeyPressed(Keyboard::Escape))
//			window.close();
//
//		// Update player and projectiles
//		if (Mouse::getPosition(window).x - player.getRadius() >= 0 && 
//			Mouse::getPosition(window).x <= window.getSize().x - player.getRadius())
//		{
//			player.setPosition(Mouse::getPosition(window).x - player.getRadius(), player.getPosition().y);
//		}
//		if (shotTimer < 10)
//			++shotTimer;
//		if (Mouse::isButtonPressed(Mouse::Left) && shotTimer >= 10)
//		{
//			projectiles.push_back(projectile);
//			projectiles.back().setPosition(player.getPosition().x + player.getRadius() - projectile.getRadius(), 
//										   player.getPosition().y + player.getRadius() - projectile.getRadius());
//			shotTimer = 0;
//		}
//
//		// Making projectiles move
//		for (auto proj = projectiles.begin(); proj != projectiles.end(); ++proj)
//		{
//			proj->move(0.f, -10.f);
//			if (proj->getPosition().y <= 0) // removing projectile which reaches the end of window
//			{
//				projectiles.erase(proj);
//				break;
//			}
//		}
//
//		// Update enemies
//		if (spawnTimer < 60)
//			++spawnTimer;
//		if (spawnTimer >= 60)
//		{
//			enemies.push_back(enemy);
//			enemies.back().setPosition(rand() % (int)(window.getSize().x - enemy.getSize().x - player.getRadius()), 0.5f);
//			spawnTimer = 0;
//		}
//
//		// Making enemies move
//		for (auto foe = enemies.begin(); foe != enemies.end(); ++foe)
//		{
//			foe->move(0.f, 5.f);
//			if (foe->getPosition().y >= window.getSize().y - enemy.getSize().x)
//			{
//				enemies.erase(foe);
//				break;
//			}
//		}
//
//		// Collision
//		bool collided = false;
//		for (auto proj = projectiles.begin(); proj != projectiles.end(); ++proj)
//		{
//			for (auto enem = enemies.begin(); enem != enemies.end(); ++enem)
//			{
//				if ( proj->getGlobalBounds().intersects(enem->getGlobalBounds()) )
//				{
//					collided = true;
//					projectiles.erase(proj);
//					enemies.erase(enem);
//					break;
//				}
//			}
//			if (collided)
//				break;
//		}
//
//		// Clear, draw, display
//		window.clear();
//
//		window.draw(player);
//		for (auto it = enemies.begin(); it != enemies.end(); ++it)
//			window.draw(*it);
//		for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
//			window.draw(*it);
//
//		window.display();
//	}
//
//	return 0;
//}









//#include <SFML/Graphics.hpp>
//
//using namespace sf;
//
//int main()
//{
//	RenderWindow window(VideoMode(640, 480), "Moving square");
//	window.setFramerateLimit(60);
//
//	CircleShape hoop(35.f);
//	hoop.setFillColor(Color::Black);
//	hoop.setOutlineThickness(3.f);
//	hoop.setOutlineColor(Color::Blue);
//	hoop.setPosition(0.f, 10.f);
//	int hoop_direction = 0; // 0 - left, 1 - right
//
//	CircleShape ball(20.f);
//	ball.setFillColor(Color::Red);
//	ball.setPosition(10.f, window.getSize().y - ball.getRadius() * 2 - 10.f);
//	bool isShot = 0;
//
//	while (window.isOpen())
//	{
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//			{
//				window.close();
//			}
//		}
//
//		// Update
//		if (hoop.getPosition().x <= 0)
//		{
//			hoop_direction = 1; // right
//		}
//		else if (hoop.getPosition().x + hoop.getRadius() * 2 >= window.getSize().x)
//		{
//			hoop_direction = 0; // left
//		}
//
//		switch (hoop_direction)
//		{
//		case 0:
//			hoop.move(-5.f, 0.f);
//			break;
//		case 1:
//			hoop.move(5.f, 0.f);
//			break;
//		}
//
//		if (Mouse::isButtonPressed(Mouse::Left))
//			isShot = true;
//		if (!isShot)
//		{
//			if (Mouse::getPosition(window).x >= 0 && 
//				Mouse::getPosition(window).x <= window.getSize().x - ball.getRadius() * 2)
//			{
//				ball.setPosition(Mouse::getPosition(window).x, ball.getPosition().y);
//			}
//		}
//		else
//		{
//			ball.move(0.f, -5.f);
//		}
//		// Collision
//		if (ball.getPosition().y <= 0 || ball.getGlobalBounds().intersects(hoop.getGlobalBounds()))
//		{
//			isShot = false;
//			ball.setPosition(10.f, window.getSize().y - ball.getRadius() * 2 - 10.f);
//		}
//
//		window.clear(Color::White);
//
//		// Draw
//		window.draw(hoop);
//		window.draw(ball);
//		window.display();
//	}
//
//	return 0;
//}










//#include <SFML/Graphics.hpp>
//
///**
// * This minigame consists of a square that can move and change color!
// * Use keys W, A, S and D to move.
// * Use the left mouse button to change the color of the square.
// * To exit, press ESC button or closing button.
// */
//
//using namespace sf;
//
//void update(RenderWindow& window, RectangleShape& square, int& keyTime);
//void draw(RenderWindow& window, RectangleShape& square);
//
//int main()
//{
//	RenderWindow window(VideoMode(640, 480), "Moving square");
//	window.setFramerateLimit(60);
//	int keyTime = 0;
//
//	RectangleShape square(Vector2f(50.f, 50.f));
//	square.setFillColor(Color::Red);
//	square.setPosition( (window.getSize().x - square.getSize().x) / 2, 
//						(window.getSize().y - square.getSize().y) / 2);
//
//	while (window.isOpen())
//	{
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//			{
//				window.close();
//			}
//		}
//
//		update(window, square, keyTime);
//		draw(window, square);
//	}
//
//	return 0;
//}
//
//void update(RenderWindow& window, RectangleShape& square, int& keyTime)
//{
//	if(keyTime < 10)
//		++keyTime;
//
//	if (Keyboard::isKeyPressed(Keyboard::Escape))
//	{
//		window.close();
//	}
//
//	if (Mouse::isButtonPressed(Mouse::Left) && keyTime >= 10)
//	{
//		keyTime = 0;
//		if (square.getFillColor() == Color::Red)
//			square.setFillColor(Color::Blue);
//		else
//			square.setFillColor(Color::Red);
//	}
//	if (Keyboard::isKeyPressed(Keyboard::A) && square.getPosition().x > 0)
//	{
//		square.move(-5.f, 0.f);
//	}
//	if (Keyboard::isKeyPressed(Keyboard::D) && 
//		square.getPosition().x + square.getSize().x < window.getSize().x)
//	{
//		square.move(5.f, 0.f);
//	}
//	if (Keyboard::isKeyPressed(Keyboard::W) && square.getPosition().y > 0)
//	{
//		square.move(0.f, -5.f);
//	}
//	if (Keyboard::isKeyPressed(Keyboard::S) &&
//		square.getPosition().y + square.getSize().y < window.getSize().y)
//	{
//		square.move(0.f, 5.f);
//	}
//}
//
//void draw(RenderWindow& window, RectangleShape& square)
//{
//	window.clear(Color::White);
//	
//	window.draw(square);
//
//	window.display();
//}


//#include <SFML/Graphics.hpp>
//
//using namespace sf;
//    
//int main()
//{
//	srand(time(0));
//
//	// Declare and create a new window
//	RenderWindow window(VideoMode(640, 480), "SFML Works!");
//
//	// Limit the framerate to 60 frames per second (this step is optional)
//	window.setFramerateLimit(60);
//
//	CircleShape circle(50.f);
//	circle.setPosition(0.f, 0.f);
//	RectangleShape rectangle(Vector2f(100.f, 100.f));
//	rectangle.setPosition(400.f, 200.f);
//
//	CircleShape triangle(40.f);
//	triangle.setPosition(0.f, 0.f);
//	triangle.setPointCount(3);
//	triangle.setFillColor(Color::Cyan);
//	triangle.setOutlineColor(Color::Red);
//	triangle.setOutlineThickness(3.f);
//	
//	Vertex line[] = { Vertex(Vector2f(50.f, 100.f)), Vertex(Vector2f(50.f, 200.f)) };
//
//	int colors[] = {200, 250, 175};
//
//	// The main loop - ends as soon as the window is closed
//	while (window.isOpen())
//	{
//		// Event processing
//		Event event;
//		while (window.pollEvent(event))
//		{
//			// Request for closing the window
//			if (event.type == Event::Closed)
//			{
//				window.close();
//			}
//		}
//	
//		// Update
//
//		if (Keyboard::isKeyPressed(Keyboard::Escape))
//			window.close();
//		if (Mouse::isButtonPressed(Mouse::Left))
//			triangle.move(5.f, 0.f);
//		if (Mouse::isButtonPressed(Mouse::Right))
//			triangle.move(-5.f, 0.f);
//		//circle.move(0.9f, 0.f);
//		//rectangle.move(-0.5f, 0.f);
//		//circle.setFillColor(Color(colors[rand() % 3], colors[rand() % 3], colors[rand() % 3], colors[rand() % 3]));
//		//circle.rotate(2.f);
//		//rectangle.rotate(3.f);
//		
//
//		// Clear, draw and display
//
//		window.clear(Color::Black);
//
//		//window.draw(circle);
//		//window.draw(rectangle);
//		window.draw(triangle);
//		window.draw(line, 2, Lines);
//
//		window.display();
//	}
// 
//	return 0;
//}