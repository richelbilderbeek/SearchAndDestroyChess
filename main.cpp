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
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      const sf::Vector2i p = sf::Mouse::getPosition(window);
      g.click_mouse(p.x, p.y);
    }
    if (sf::Joystick::isConnected(0))
    {
      if (sf::Joystick::hasAxis(0, sf::Joystick::X))
      {
        const double dx{sf::Joystick::getAxisPosition(0, sf::Joystick::X)};
        if (dx < -50.0) { g.add_command(command::left); }
        if (dx >  50.0) { g.add_command(command::right); }
      }
      if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
      {
        const double dy{sf::Joystick::getAxisPosition(0, sf::Joystick::Y)};
        if (dy < -50.0) { g.add_command(command::up); }
        if (dy >  50.0) { g.add_command(command::down); }
      }
      const int key_activate{0};
      const bool pressed{sf::Joystick::isButtonPressed(0,key_activate)};
      if (pressed)
      {
        g.add_command(command::select);
      }
      const int key_select{7};
      if (sf::Joystick::isButtonPressed(0,key_select))
      {
        g.add_command(command::ok);
      }
    }
    if (sf::Joystick::isConnected(1))
    {
      if (sf::Joystick::hasAxis(1, sf::Joystick::X))
      {
        const double dx{sf::Joystick::getAxisPosition(1, sf::Joystick::X)};
        if (dx < -50.0) { g.add_command(command::left); }
        if (dx >  50.0) { g.add_command(command::right); }
      }
      if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
      {
        const double dy{sf::Joystick::getAxisPosition(1, sf::Joystick::Y)};
        if (dy < -50.0) { g.add_command(command::up); }
        if (dy >  50.0) { g.add_command(command::down); }
      }
      const int key_activate{0};
      if (sf::Joystick::isButtonPressed(1,key_activate))
      {
        g.add_command(command::select);
      }
      const int key_select{7};
      if (sf::Joystick::isButtonPressed(1,key_select))
      {
        g.add_command(command::ok);
      }
    }
    // get global mouse position
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) g.add_command(command::ok);
    if (clock.getElapsedTime().asMilliseconds() < 200) continue;
    clock.restart();
    g.tick();
    window.clear();
    g.draw(window);
    window.display();
  }
}

/*

  if (sf::Joystick::isConnected(1))
  {
    //First move, then build
    RemoveKey(Key::up1);
    RemoveKey(Key::right1);
    RemoveKey(Key::down1);
    RemoveKey(Key::left1);
    if (sf::Joystick::hasAxis(1, sf::Joystick::X))
    {
      const double dx{sf::Joystick::getAxisPosition(1, sf::Joystick::X)};
      if (dx < -50.0) { AddKey(Key::left1 ); }
      if (dx >  50.0) { AddKey(Key::right1); }
    }
    if (sf::Joystick::hasAxis(1, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(1, sf::Joystick::Y)};
      if (dy < -50.0) { AddKey(Key::up1  );  }
      if (dy >  50.0) { AddKey(Key::down1);  }
    }


*/
