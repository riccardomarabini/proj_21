#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "SIR.hpp"
#include "graph.hpp"

namespace Con = Contagion;

int main(int argc, char* argv[]) {
  try {
    int S0 = argc > 1 ? std::atoi(argv[1]) : 0;
    int I0 = argc > 2 ? std::atoi(argv[2]) : 0;
    double beta = argc > 3 ? std::atof(argv[3]) : 0.;
    double gamma = argc > 4 ? std::atof(argv[4]) : 0.;
    int duration = argc > 5 ? std::atoi(argv[5]) : 0;
    int R0 = 0;
    Con::State init_state{S0, I0, R0, beta, gamma};
    Con::SIR pandemic{init_state, duration};
    std::vector<Con::State> states = pandemic.Con::SIR::evolve();

    // Draw the discrete graph
    constexpr int wind_size = 600;
    sf::RenderWindow window{sf::VideoMode(wind_size, wind_size),
                            "SIR graph; discrete", sf::Style::Default};
    window.setFramerateLimit(60);
    Con::Graph graph{window};

    if (window.isOpen()) {
      window.clear(sf::Color::White);
      // plot the values:discrete
      graph.draw_dots(states);
      // write the legend on display
      std::string parameters = "N=" + std::to_string(pandemic.getPopulation()) +
                               "\npar1=" + std::to_string(beta) +
                               "\npar2=" + std::to_string(gamma);
      graph.write_leg("Legend", "S=blue", "I=red", "R=green", parameters);
      window.display();
    }

    // Draw the continous graph
    sf::RenderWindow window2{sf::VideoMode(wind_size, wind_size),
                             "SIR graph; continuous", sf::Style::Default};
    window2.setFramerateLimit(60);
    Con::Graph graph2{window2};

    if (window2.isOpen()) {
      window2.clear(sf::Color::White);
      // plot the values: continuous
      graph2.draw_continuous(states);
      // write the legend on display
      std::string parameters = "N=" + std::to_string(pandemic.getPopulation()) +
                               "\npar1=" + std::to_string(beta) +
                               "\npar2=" + std::to_string(gamma);
      graph2.write_leg("Legend", "S=blue", "I=red", "R=green", parameters);
      window2.display();
    }

    // game loop
    while (window2.isOpen() || window.isOpen()) {
      sf::Event event;

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
      }

      while (window2.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window2.close();
        }
      }
    }

  } catch (std::exception const& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unkown exception\n";
    return EXIT_FAILURE;
  }
}