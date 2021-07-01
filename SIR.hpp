#ifndef SIR_HPP
#define SIR_HPP

#include <cassert>
#include <limits>
#include <stdexcept>
#include <vector>

namespace Contagion {
struct State {
  int S = 0;
  int I = 0;
  int R = 0;

  double beta = 0.;
  double gamma = 0.;

  bool state_is_valid() const {
    return (S >= 0 && I >= 0 && R >= 0 && beta >= 0 && beta <= 1 &&
            gamma >= 0 && gamma <= 1);
  }
};

class SIR {
  State s0{};
  int duration = 0;
  int N = 0;

 public:
  SIR(State const& s, int d) : s0{s}, duration{d}, N{s.S + s.I + s.R} {
    if (!s.state_is_valid()) {
      throw std::runtime_error{"Invalid initialisation of SIR with State"};
    } else if (s.S + s.I + s.R == 0) {
      throw std::runtime_error{"Not enough people"};
    } else if (d < 0) {
      throw std::runtime_error{"Invalid duration"};
    }
  };

  SIR() = default;

  // evolve() produces a vector of N=1+"duration" States with the
  // corresponding S I R values, so that they can be printed on terminal

  std::vector<State> evolve() {
    std::vector<State> evo{s0};
    State s = s0;

    assert(N > 0);
    for (int i = 0; i != duration; ++i) {
      State const& previous = evo.back();
      s.S = previous.S * (1 - (previous.beta * previous.I) / N);
      s.I = previous.I *
            (1 + ((previous.beta * previous.S) / N) - previous.gamma);
      s.R = N - (s.S + s.I);
      assert(s.S >= 0 && s.I >= 0 && s.R >= 0);
      assert(s.S + s.R + s.I == N);
      evo.push_back(s);
    }

    return evo;
  }

  // return the total number of population
  int getPopulation() const noexcept { return N; }
};

}  // namespace Contagion

#endif