#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

#include "SIR.hpp"

namespace Con = Contagion;

// print S-I-R values on terminal
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

// print S-I-R values on file
void Print_file(std::vector<Con::State> const& s) {
  std::ofstream os{"SIR.txt"};
  if (!os) {
    std::runtime_error{"Failure to open the file"};
  };
  for (auto const& v : s) {
    os << std::setw(10) << v.S << std::setw(10);
    os << v.I << std::setw(10);
    os << v.R << std::setw(10) << '\n';
  }
}

int main() {
  try {
    std::cout << "The following parameters need to be inserted: \n "
                 "S=susceptible, I=infectious, β (contagion parameter), γ "
                 "(healing parameter) and duration. \n";

    int S = 0;
    int I = 0;
    int R = 0;
    double beta = 0.;
    double gamma = 0.;
    int duration = 0;
    std::cin >> S >> I >> beta >> gamma >> duration;

    std::cout << "Initial parameters \nS= " << S << '\n';
    std::cout << "I= " << I << '\n';
    std::cout << "beta= " << beta << '\n';
    std::cout << "gamma= " << gamma << '\n';
    std::cout << "duration= " << duration << '\n';

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
