#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "SIR.hpp"
#include "graph.hpp"

namespace Con = Contagion;

int main() {
  /*std::cout << "The following parameters need to be inserted: \n "
               "S=susceptible, I=infectous, β (contagion parameter), γ "
               "(healing parameter) and duration. \n";
  int S = 0;
  int I = 0;
  int R = 0;
  double beta = 0.;
  double gamma = 0.;
  int duration = 0;
  std::cin >> S >> I >> beta >> gamma >> duration;
  for (; S < 0;) {
    std::cout << "Invalid input S, insert the value again:";
    std::cin >> S;
  }
  for (; I < 0;) {
    std::cout << "Invalid input I, insert the value again:";
    std::cin >> I;
  }
  // for(;R<0;) std::cin >> R;
  for (; beta < 0 || beta > 1.;) {
    std::cout << "Invalid input beta, insert the value again:";
    std::cin >> beta;
  }
  for (; gamma < 0 || gamma > 1.;) {
    std::cout << "Invalid input gamma, insert the value again:";
    std::cin >> gamma;
  }
  for (; duration < 0;) {
    std::cout << "Invalid input duration, insert the value again:";
    std::cin >> duration;
  }
  */

  Con::State s0{1000, 10, 0, 0.3, 0.1};
  Con::SIR pandemic{s0, 100};

  std::vector<Con::State> states = pandemic.Con::SIR::evolve();

  // Draw the graph
  constexpr int wind_size = 600;
  sf::RenderWindow window{sf::VideoMode(wind_size, wind_size), "SIR graphs",
                          sf::Style::Default};
  Con::Graph graph{window};

  if (window.isOpen()) {
    window.clear(sf::Color::White);
    // graph.draw(states);
    graph.draw_dots(states);
    // write the legend on display
    graph.write_leg("Legend \nS=black\nI=red\nR=green");
    window.display();
  }

  // game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
  }

  // disegniamo dei punti=rettangoli di un pixel
  /*sf::RenderWindow window2{sf::VideoMode(wind_size, wind_size), "SIR
  graphs_rect", sf::Style::Default};

  window2.clear(sf::Color::White);

  int const duration =states.size();
  for (int i = 0; i != duration ; ++i) {
    sf::RectangleShape rect (sf::Vector2f(2.f,2.f));
    rect.setFillColor(sf::Color::Black);
    float const x = (i / static_cast<float>(duration)) * window2.getSize().x;
    float const y = (1-states[i].S /
  static_cast<float>(pandemic.getPopulation())) * window2.getSize().y;
    rect.setPosition(sf::Vector2f(x, y));
    window2.draw(rect);
  }
  window2.display();

  sf::Event event;
    while (window2.waitEvent(event)) {
      if (event.type == sf::Event::Closed) window2.close();
    }
*/
}