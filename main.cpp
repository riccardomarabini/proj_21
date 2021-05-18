#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include "SIR.hpp"

namespace Con = Contagion;

void Print(std::vector<Con::State> s) {
  std::cout << "+---------------+---------------+--------------+\n"
            << "|      S        |       I       |       R      |\n"
            << "+---------------+---------------+--------------+\n";
  for (auto const& v : s) {
    // std::cout << "\033[2J";
    std::cout << "|" << std::setw(8) << v.S << std::setw(8) << "|"
              << std::setw(8) << v.I << std::setw(8) << "|" << std::setw(8)
              << v.R << std::setw(8) << "|\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }

  std::cout << "+---------------+---------------+--------------+\n";
}

// GUARDA SU CPPREFERENCE LA CONVERSIONE DA STRINGA AD INTERO PER I FLOATING
// POINT !! STD::ATOI NON VA BENE!
/*int main(int argc, char* argv[]) {
  int S0 = argc > 1 ? std::atoi(argv[1]) : 0;
  int I0 = argc > 2 ? std::atoi(argv[2]) : 0;
  int R0 = argc > 3 ? std::atoi(argv[3]) : 0;
  double beta = argc > 4 ? std::atoi(argv[4]) : 0.1;
  double gamma = argc > 5 ? std::atoi(argv[5]) : 0.2;
  int duration = argc > 6 ? std::atoi(argv[6]) : 0;
std::cout << beta <<"  "<< gamma <<'\n';
  Con::State init_state{S0, I0, R0, beta, gamma}; // PERCHE' BETA E GAMMAVENGONO
MESSE A ZERO ? Con::SIR pandemy{init_state, duration}; std::vector<Con::State> s
= pandemy.Con::SIR::evolve(); Print(s);
}
*/

// L'input del programma è costituito dai parametri del modello β\betaβ e
// γ\gammaγ, dai valori iniziali di S, I e R e dalla durata in giorni della
// simulazione TTT. I parametri vanno presi o da standard input o da riga di
// comando.
int main() {
  std::cout << "The following parameters need to be inserted: \n "
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

  if (!std::cin.good()) std::cerr << "invalid input \n";

  Con::State s0{S, I, R, beta, gamma};
  Con::SIR pandemic{s0, duration};

  std::vector<Con::State> s = pandemic.Con::SIR::evolve();
  Print(s);
}
