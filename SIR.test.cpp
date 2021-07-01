#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "SIR.hpp"

#include "doctest.h"

namespace Con = Contagion;

TEST_CASE("Testing N = 0") {
  int duration{};
  Con::State init_state{};
  CHECK_THROWS(Con::SIR(init_state, duration));
}

TEST_CASE("Testing d <= 0") {
  SUBCASE("d < 0") {
    int duration = -3;
    Con::State init_state{1000, 1, 0, 0.1, 0.2};
    CHECK_THROWS(Con::SIR(init_state, duration));
  }
  SUBCASE("d = 0") {
    int duration{};
    Con::State init_state{1000, 1, 0, 0.1, 0.2};
    Con::SIR sim{init_state, duration};
    auto next = sim.evolve();
    CHECK((next[duration].S) == init_state.S);
    CHECK((next[duration].I) == init_state.I);
    CHECK((next[duration].R) == init_state.R);
  }
}

TEST_CASE("Testing function state_is_valid") {
  SUBCASE("inacceptable value for S") {
    int duration{};
    Con::State init_state{-3, 1, 0, 0.7, 0.3};
    CHECK(init_state.state_is_valid() == false);
    CHECK_THROWS(Con::SIR(init_state, duration));
  }
  SUBCASE("inacceptable value for I") {
    int duration{};
    Con::State init_state{1000, -5, 0, 0.7, 0.3};
    CHECK(init_state.state_is_valid() == false);
    CHECK_THROWS(Con::SIR(init_state, duration));
  }
  SUBCASE("inacceptable value for beta") {
    int duration{};
    Con::State init_state{1000, 1, 0, -1, 0.3};
    CHECK(init_state.state_is_valid() == false);
    CHECK_THROWS(Con::SIR(init_state, duration));
  }
  SUBCASE("inacceptable value for gamma") {
    int duration{};
    Con::State init_state{1000, 1, 0, 0, 1.3};
    CHECK(init_state.state_is_valid() == false);
    CHECK_THROWS(Con::SIR(init_state, duration));
  }
}

TEST_CASE("Testing failure of pandemic for beta < gamma") {
  SUBCASE("#1") {
    int duration{1000};
    Con::State init_state{1000, 1, 0, 0.1, 0.2};
    Con::SIR sim{init_state, duration};
    auto next = sim.evolve();
    int Ni = init_state.S + init_state.I + init_state.R;
    int Nf = next[duration - 1].S + next[duration - 1].I + next[duration - 1].R;

    CHECK(doctest::Approx(next[duration].S).epsilon(0.001) == init_state.S);
    CHECK(doctest::Approx(next[duration].I).epsilon(0.001) == 0);
    CHECK(doctest::Approx(next[duration].R).epsilon(init_state.I) ==
          init_state.I);
    CHECK(Ni == Nf);
  }

  SUBCASE("#2") {
    int duration{1000};
    Con::State init_state{10000, 10, 0, 0.1, 0.2};
    Con::SIR sim{init_state, duration};
    auto next = sim.evolve();
    int Ni = init_state.S + init_state.I + init_state.R;
    int Nf = next[duration - 1].S + next[duration - 1].I + next[duration - 1].R;

    CHECK(doctest::Approx(next[duration].S).epsilon(0.001) == init_state.S);
    CHECK(doctest::Approx(next[duration].I).epsilon(0.001) == 0);
    CHECK(doctest::Approx(next[duration].R).epsilon(init_state.I) ==
          init_state.I);
    CHECK(Ni == Nf);
  }
}

TEST_CASE("Testing pandemic") {
  int duration{20};
  Con::State init_state{100, 10, 0, 0.4, 0.2};
  Con::SIR sim{init_state, duration};
  auto next = sim.evolve();
  int Ni = init_state.S + init_state.I + init_state.R;
  int Nf = next[duration].S + next[duration].I + next[duration].R;

  CHECK(doctest::Approx(next[1].S).epsilon(0.001) == 96);
  CHECK(doctest::Approx(next[1].I).epsilon(0.001) == 11);
  CHECK(doctest::Approx(next[1].R).epsilon(init_state.I) == 3);

  CHECK(doctest::Approx(next[duration].S).epsilon(0.001) == 31);
  CHECK(doctest::Approx(next[duration].I).epsilon(0.001) == 7);
  CHECK(doctest::Approx(next[duration].R).epsilon(init_state.I) == 72);
  CHECK(Ni == Nf);
}
