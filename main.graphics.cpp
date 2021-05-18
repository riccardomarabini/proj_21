#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "environment.hpp"
#include "graph.hpp"

namespace Con = Contagion;

int main() {
  std::cout << "Insert dimesion of the environment: \n";
  int side;
  std::cin >> side;
  int constexpr display_limit = 1000;
  if (side < 1 || side > display_limit) {
    std::cout << "Resetting environmetal dimesion to 1000";
    side = display_limit;
  }
  Con::Environment env{side};

  // initialising the environment with 2% of void and 1% of infectious people
  double constexpr void_percent = 0.02;
  double constexpr inf_percent = 0.01;
  int const n_void = void_percent * (side * side);
  int const n_infetious = inf_percent * (side * side);

  {
    std::mt19937 gen;
    std::uniform_int_distribution<> dist{0, side - 1};

    for (int i = 0; i != n_void; ++i) {
      int r = dist(gen);
      int c = dist(gen);
      for (; env.condition(r, c) == Con::Person::Void;
           r = dist(gen), c = dist(gen))
        ;
      env.condition(r, c) = Con::Person::Void;
    }

    for (int i = 0; i != n_infetious; ++i) {
      int r = dist(gen);
      int c = dist(gen);
      for (; env.condition(r, c) == Con::Person::Infectious ||
             env.condition(r, c) == Con::Person::Void;
           r = dist(gen), c = dist(gen))
        ;
      env.condition(r, c) = Con::Person::Infectious;
    }
  }

  // display the evolution of env as contagion spreads out for a definite number
  // of days
  int constexpr rect_dim = 4;
  int const window_size = rect_dim * side;
  int duration = 0;
  double beta;
  double gamma;
  std::cout << "Insert the time span of the simulation:\n";
  std::cin >> duration;
  if (duration < 1 || duration > 100) {
    std::cout << "Resetting duration at 50 days";
    duration = 50;
  }
  std::cout << "Insert β (contagion parameter) and γ (healing parameter):\n";
  std::cin >> beta >> gamma;
  if (beta <= 0 || beta > 1) std::cerr << "invalid beta input";
  if (gamma <= 0 || gamma > 1) std::cerr << "invalid gamma input";

  sf::RenderWindow window(sf::VideoMode(window_size, window_size), "pandemic",
                          sf::Style::Default);
  Con::Graph draw(window);

  // event handling 1: clear window and display the initial environment

  if (window.isOpen()) {
    window.clear(sf::Color::White);
    draw.draw(env);
    window.display();

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed)
        break;
      else if (event.type == sf::Event::Closed)
        window.close();
    }
  }

  // bool start_stop = true;

  while (window.isOpen()) {
    sf::Event event;
    for (int i = 0; i != duration; ++i) {
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
      }
      env = evolve(env, beta, gamma);
      window.clear(sf::Color::White);
      draw.draw(env);
      window.display();
    }
    while (window.waitEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
  }
}