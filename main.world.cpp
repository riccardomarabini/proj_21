#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <random>

#include "environment.hpp"
#include "graph.hpp"

namespace Con = Contagion;

int main() {
  try {
    int constexpr upper_display_limit = 150;
    int constexpr lower_display_limit = 20;
    std::cout << "Insert the side of the environment: \n";
    int side;
    std::cin >> side;
    if (side < lower_display_limit || side > upper_display_limit) {
      std::cout << "Resetting environmetal side to 75\n";
      side = upper_display_limit / 2;
    }
    Con::Environment env{side};

    // initialising the environment with 2% of void and arbitrary percentage of
    // infectious people
    double constexpr void_percent = 0.02;
    double constexpr def_inf_percent = 0.01;
    double inf_percent;
    std::cout << "Insert the percentage of infectious people -between (0-1):\n";
    std::cin >> inf_percent;
    if (inf_percent <= 0 || inf_percent >= 1) {
      std::cout << "Resetting percentage of infectious to 0.01\n";
      inf_percent = def_inf_percent;
    }
    int const n_void = void_percent * (side * side);
    int n_infetious = inf_percent * (side * side);

    // random distribution of void cells and infectious people
    {
      std::default_random_engine gen{std::random_device{}()};
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

    // display the evolution of the environment as contagion spreads out for a definite
    // number of days

    int constexpr rect_dim = 6;
    int const window_size = rect_dim * side;

    int duration = 0;
    double beta;
    double gamma;
    double mi;

    std::cout << "Insert the time span of the simulation:\n";
    std::cin >> duration;
    if (duration < 1 || duration > 120) {
      std::cout << "Resetting duration at 60 days\n";
      duration = 60;
    }
    std::cout << "Insert β (contagion parameter), γ (convalescence parameter) "
                 "and μ (mortality parameter)- between ]0-1[ :\n";
    std::cin >> beta >> gamma >> mi;
    if (beta <= 0 || beta > 1) {
      throw std::runtime_error("Invalid beta input");
    }
    if (gamma <= 0 || gamma > 1) {
      throw std::runtime_error("Invalid gamma input");
    }
    if (mi <= 0 || mi > 1) {
      throw std::runtime_error("Invalid mi input");
    }

    /*if(!std::cin.good()){
      throw std::runtime_error("Incorrect parameters initialisation");
    }*/

    sf::RenderWindow window(sf::VideoMode(window_size, window_size), "pandemic",
                            sf::Style::Default);
    window.setFramerateLimit(60);
    Con::Graph draw(window);

    // game loop with the evolution of the environment

    std::default_random_engine gen{std::random_device{}()};

    using namespace std::chrono_literals;

    auto last = std::chrono::steady_clock::now();

    int i = 0;

    bool toggle = true;

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        } else if (event.type == sf::Event::KeyPressed) {
          toggle = !toggle;
        }
      }

      window.clear(sf::Color::White);
      draw.draw(env);
      window.display();

      if (i < duration && toggle) {
        if (auto now = std::chrono::steady_clock::now(); now > last + 1s) {
          env = evolve(env, beta, gamma, mi, gen);
          last = now;
          i += 1;
        }
      }
    }

  } catch (std::exception const& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception\n";
    return EXIT_FAILURE;
  }
}