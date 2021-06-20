#include <SFML/Graphics.hpp>

/**
 * This minigame consists of a square that can move and change color!
 * Use keys W, A, S and D to move.
 * Use the left mouse button to change the color of the square.
 * To exit, press ESC button or closing button.
 */

using namespace sf;

void update(RenderWindow& window, RectangleShape& square, int& keyTime);
void draw(RenderWindow& window, RectangleShape& square);

int main()
{
	RenderWindow window(VideoMode(640, 480), "Moving square");
	window.setFramerateLimit(60);
	int keyTime = 0;

	RectangleShape square(Vector2f(50.f, 50.f));
	square.setFillColor(Color::Red);
	square.setPosition( (window.getSize().x - square.getSize().x) / 2, 
						(window.getSize().y - square.getSize().y) / 2);

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

		update(window, square, keyTime);
		draw(window, square);
	}

	return 0;
}

void update(RenderWindow& window, RectangleShape& square, int& keyTime)
{
	if(keyTime < 10)
		++keyTime;

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}

	if (Mouse::isButtonPressed(Mouse::Left) && keyTime >= 10)
	{
		keyTime = 0;
		if (square.getFillColor() == Color::Red)
			square.setFillColor(Color::Blue);
		else
			square.setFillColor(Color::Red);
	}
	if (Keyboard::isKeyPressed(Keyboard::A) && square.getPosition().x > 0)
	{
		square.move(-5.f, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::D) && 
		square.getPosition().x + square.getSize().x < window.getSize().x)
	{
		square.move(5.f, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::W) && square.getPosition().y > 0)
	{
		square.move(0.f, -5.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::S) &&
		square.getPosition().y + square.getSize().y < window.getSize().y)
	{
		square.move(0.f, 5.f);
	}
}

void draw(RenderWindow& window, RectangleShape& square)
{
	window.clear(Color::White);
	
	window.draw(square);

	window.display();
}


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