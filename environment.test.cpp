#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "environment.hpp"

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

#include "doctest.h"

namespace Con = Contagion;

TEST_CASE("Testing invalid side input") {
  std::vector<int> wrong_side{-10, -2};
  for (auto const& side : wrong_side) {
    CHECK_THROWS(Con::Environment(side));
  }
}

TEST_CASE("Testing condition() functions") {
  Con::Environment env(10);
  for (int i = 0; i != 12; i += 2) {
    for (int j = 0; j != 12; j += 2) {
      env.condition(i, j) = Con::Person::Infectious;
    }
  }
  for (int i = 0; i != 12; i += 2) {
    for (int j = 0; j != 12; j += 2) {
      CHECK(env.condition(i, j) == Con::Person::Infectious);
    }
  }
}

TEST_CASE("Testing side() function") {
  Con::Environment env(30);
  CHECK(env.side() == 30);
}

TEST_CASE("Testing N_inf() function") {
  Con::Environment env(30);
  for (int p : {7, 16, 20}) {
    for (int i : {-1, 1}) {
      for (int j : {-1, 1}) {
        env.condition(p + i, p + j) = Con::Person::Infectious;
        env.condition(p, p) = Con::Person::Infectious;
      }
    }
  }

  for (int p : {7, 16, 20}) {
    CHECK(Con::N_Inf(env, p, p) == 4);
  }
}

TEST_CASE("Testing N_inf() function") {
  constexpr int side = 50;
  constexpr int n_void = (side * side) * 0.3;
  constexpr int n_infectious = (side * side) * 0.05;
  Con::Environment env(side);
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

  for (int i = 0; i != n_infectious; ++i) {
    int r = dist(gen);
    int c = dist(gen);
    for (; env.condition(r, c) == Con::Person::Infectious ||
           env.condition(r, c) == Con::Person::Void;
         r = dist(gen), c = dist(gen))
      ;
    env.condition(r, c) = Con::Person::Infectious;
  }

  Con::State state_0 = count_SIR(env);
  CHECK(state_0.S == 1625);
  CHECK(state_0.I == 125);
  CHECK(state_0.R == 0);
}
