#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "SIR.hpp"

namespace Con = Contagion;

TEST_CASE("Testing N <= 0"){
    int duration{};
    Con::State init_state{};
    Con::SIR sim{init_state, duration};
    CHECK_THROWS(sim.evolve());
}

//TEST_CASE("Testing N <= 0"){
//    int duration{1};
//    Con::State init_state{};
//    int N = init_state.S + init_state.I + init_state.R;
//    Con::SIR sim{init_state, duration};
//    sim.evolve();
//    CHECK(N == 1000);
//} //non funziona forse la condizione su N va fuori da evolve?

TEST_CASE("Testing failure of pandemic#1" ) {
    int duration{1000};
    Con::State init_state{1000,1,0,0.1,0.2};
    Con::SIR sim{init_state, duration};
    auto next = sim.evolve();
    CHECK(doctest::Approx(next[duration-1].S).epsilon(0.001) == init_state.S);
    CHECK(doctest::Approx(next[duration-1].I).epsilon(0.001) == 0 );
    //CHECK(doctest::Approx(next[duration-1].R).epsilon(0.001) == 1 );
    //check su N o diamo per scontato che vada bene per assert? 
}


TEST_CASE("Testing failure of pandemic#2" ) {
    int duration{1000};
    Con::State init_state{10000,10,0,0.1,0.2};
    Con::SIR sim{init_state, duration};
    auto next = sim.evolve();
    CHECK(doctest::Approx(next[duration-1].S).epsilon(0.001) == init_state.S);
    CHECK(doctest::Approx(next[duration-1].I).epsilon(0.001) == 0 );
//    CHECK(doctest::Approx(next[duration-1].R).epsilon(0.001) == 1 );
    //check su N o diamo per scontato che vada bene per assert? 
}

//manca test su un esempio