#include <cassert>
#include <iostream>
#include <vector>
#include <set>
#include <SFML/Graphics.hpp>

#include "command.h"
#include "game_dialog.h"
#include "game.h"

int main()
{

  const int window_width{640};
  const int window_height{640};

  game_dialog g(window_width, window_height);

  sf::RenderWindow window(
    sf::VideoMode(window_width, window_height),
    "SearchAndDestroyChess",
    sf::Style::Titlebar | sf::Style::Close
  );
  sf::Clock clock;
  while(window.isOpen())
  {
    sf::Event event;

    while(window.pollEvent(event))
    {
      switch(event.type)
      {
        case sf::Event::Closed:
          window.close();
          break;
        default:
          break;
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) g.add_command(command::up);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) g.add_command(command::right);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) g.add_command(command::down);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) g.add_command(command::left);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) g.add_command(command::select);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) g.add_command(command::select);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) g.add_command(command::up);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) g.add_command(command::right);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) g.add_command(command::down);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) g.add_command(command::left);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) g.add_command(command::select);
    if (clock.getElapsedTime().asMilliseconds() < 200) continue;
    clock.restart();
    g.tick();

    window.clear();
    g.draw(window);
    window.display();
  }
}
