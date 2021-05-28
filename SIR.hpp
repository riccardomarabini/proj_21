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
    long int const max_int = std::numeric_limits<int>::max();
    return !(S < 0 || S >= max_int || I < 0 || I >= max_int || R < 0 ||
             R >= max_int || beta < 0 || beta > 1 || gamma < 0 || gamma > 1);
  }
};

class SIR {
  State s0{};
  int duration = 0;
  int N = 0;

 public:
  SIR(State const& s, int d) : s0{s}, duration{d}, N{s.S + s.I + s.R} {
    if (!s.state_is_valid()) {
      throw std::runtime_error{"Invalid initialization of SIR with State"};
    } else if (s.S + s.I + s.R == 0) {
      throw std::runtime_error { "not enough people" };
    } else if (d < 0) {
      throw std::runtime_error{"invalid duration"};
    }
  };

  // func evolve() produces a vector of N="duration" States with the
  // corresponding S I R values, so that it can be printed on terminal
  
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

  int getPopulation() const noexcept { return N; }
};

}  // namespace Contagion

#endif