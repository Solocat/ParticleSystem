#include <SFML/Graphics.hpp>
#include <iostream>
#include "ParticleSystem.h"

/*sf::Color interpolateColor(sf::Color col1, sf::Color col2, float per)
{

}*/

sf::Color colorCycle(unsigned i)
{
	sf::Uint8 red = 0;
	sf::Uint8 green = 0;
	sf::Uint8 blue = 0;

	if (i < 256)
	{
		red = 255;
		green = i;
	}
	else if (i < 2 * 256)
	{
		green = 255;
		red = 2 * 256 - i - 1;
	}
	else if (i < 3 * 256)
	{
		green = 255;
		blue = i - 2 * 256;
	}
	else if (i < 4 * 256)
	{
		blue = 255;
		green = 4 * 256 - i - 1;
	}
	else if (i < 5 * 256)
	{
		blue = 255;
		red = i - 4 * 256;
	}
	else
	{
		red = 255;
		blue = 6 * 256 - i - 1;
	}

	return sf::Color(red, green, blue, 0xFF);
}

sf::Color pastelCycle(unsigned i)
{
	sf::Uint8 red = 0;
	sf::Uint8 green = 0;
	sf::Uint8 blue = 0;

	if (i < 128) //red to yellow
	{
		red = 255;
		blue = 128;
		green = 128 + i;
	}
	else if (i < 2 * 128) //yellow to green
	{
		green = 255;
		blue = 128;
		red = 3 * 128 - i - 1;
	}
	else if (i < 3 * 128) //green to cyan
	{
		green = 255;
		red = 128;
		blue = i - 128;
	}
	else if (i < 4 * 128) //cyan to blue
	{
		blue = 255;
		red = 128;
		green = 5 * 128 - i - 1;
	}
	else if (i < 5 * 128) //blue to magenta
	{
		blue = 255;
		green = 128;
		red = i - 3 * 128;
	}
	else //magenta to red
	{
		red = 255;
		green = 128;
		blue = 7 * 128 - i - 1;
	}

	return sf::Color(red, green, blue, 0xFF);
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML experiment", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	const double Tau = 6.28318530718;
	const double radius = 1.f;
	const double angleGap = Tau / 10;
	double angle = Tau / 4;

	sf::ConvexShape star;
	star.setPointCount(10);
	for (int i = 0; i < 10; i++)
	{
		float r = i % 2 ? radius : radius / 2;

		star.setPoint(i, sf::Vector2f(cos(angle) * r, sin(angle) * r));
		angle += angleGap;
	}

	RainGenerator3D rainer;
	rainer.x = 400.f;
	rainer.y = -20.f;
	rainer.maxParticles = 3000;
	rainer.velocityMin = 30.f;
	rainer.velocityMax = 30.f;
	rainer.scaleMin = 2.f;
	rainer.scaleMax = 2.f;
	rainer.spawnInterval = sf::milliseconds(2);
	rainer.particleShape = star;
	rainer.depthFactor = 6.f;
	rainer.width = 800.f;
	rainer.killBounds = sf::FloatRect(-30, -30, 860, 860);
	rainer.rotationSpeedMin = -100.f;
	rainer.rotationSpeedMax = 100.f;

	sf::Clock clock;

	unsigned color = 0;
	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		int frametime = std::max((int)elapsed.asMicroseconds(), 1);

		if (color >= 6 * 128) color = 0;

		rainer.color = pastelCycle(color++);
		rainer.countTime(elapsed);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		rainer.render(window);
		window.display();
	}

	return 0;
}