#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#include "SIR.hpp"

namespace Con = Contagion;

void Print(std::vector<Con::State> const& s) {
  std::cout << "+-------------------+-------------------+------------------+\n"
            << "|          S        |          I        |          R       |\n"
            << "+-------------------+-------------------+------------------+\n";
  for (auto const& v : s) {
    std::cout << "|" << std::setw(10) << v.S << std::setw(10) << "|"
              << std::setw(10) << v.I << std::setw(10) << "|" << std::setw(10)
              << v.R << std::setw(10) << "|\n";
  }

  std::cout << "+-------------------+-------------------+------------------+\n";
}

void Print_file(std::vector<Con::State> const& s) {
  std::ofstream os{"SIR.txt"};
  if (!os) {
    std::runtime_error{"couldn't properly open the file"};
  };
  for (auto const& v : s) {
    os << std::setw(10) << v.S << std::setw(10);
    os << v.I << std::setw(10);
    os << v.R << std::setw(10) << '\n';
  }
}

// L'input del programma è costituito dai parametri del modello β e
// γ, dai valori iniziali di S, I e R e dalla durata in giorni della
// simulazione. I parametri vanno presi o da standard input o da riga di
// comando.

int main() {
  try {
    std::cout << "The following parameters need to be inserted: \n "
                 "S=susceptible, I=infectous, β (contagion parameter), γ "
                 "(healing parameter) and duration. \n";

    int constexpr int_limit = std::numeric_limits<int>::max();
    int S = 0;
    int I = 0;
    int R = 0;
    double beta = 0.;
    double gamma = 0.;
    int duration = 0;
    std::cin >> S >> I >> beta >> gamma >> duration;
    for (; S < 0 || S > int_limit;) {
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
    for (; duration < 0 || duration > int_limit;) {
      std::cout << "Invalid input duration, insert the value again:";
      std::cin >> duration;
    }

    if (!std::cin.good()) {
      throw std::runtime_error{"Invalid parameter(s) input\n"};
    }

    Con::State s0{S, I, R, beta, gamma};
    Con::SIR pandemic{s0, duration};

    std::vector<Con::State> states = pandemic.Con::SIR::evolve();
    Print(states);
    Print_file(states);

  } catch (std::exception const& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unkown exception\n";
    return EXIT_FAILURE;
  }
}
