#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>

#include "SIR.hpp"

namespace Contagion {

class Graph {
  sf::RenderWindow& window;
  sf::Font font;

 public:
  Graph(sf::RenderWindow& w)
      : window{w} {  // qui è possibile scaricare i font e le textures
    if (!font.loadFromFile("font.ttf")) {
      if (!font.loadFromFile(
              "/usr/share/fonts/truetype/msttcorefonts/arialbi.ttf")) {
        throw std::runtime_error{"cannot load font"};
      }
    }
  }

  void draw(std::vector<Contagion::State> const& states) {
    // quello che dobbiamo fare è creare tre convexshape in cui la posizione dei
    // punti è determinata in x dal numero di giorni trascorsi e in y dal valore
    // di ciascuna componente di state
    // quindi in entrambe le posioni dobbiamo trovare quella percentuale sulla
    // durata della pandemia e sul numero totale di popolazione esaminata

    sf::ConvexShape graph_S(states.size());
    // graph_S.setPointCount(states.size());

    // adesso dobbiamo segnare la posizone di ciascun punto, quindi utilizziamo
    // un ciclo che scorra tutti gli states in modo da accere al valore di S in
    // ciascun state

    const int duration = states.size();
    const int population = states[0].S + states[0].I + states[0].R;

    for (int i = 0; i != duration; ++i) {
      float const x = (i / static_cast<float>(duration)) * (window.getSize().x);
      float const y_S = (1 - states[i].S / static_cast<float>(population)) *
                        window.getSize().y;

      assert(i < (int)graph_S.getPointCount());
      graph_S.setPoint(i, sf::Vector2f{x, y_S});
    }

    // selezionamo i colori per la linea esterna e lo spessore
    graph_S.setOutlineColor(sf::Color::Blue);
    graph_S.setOutlineThickness(2.f);

    // infine disegniamo la convex sulla finestra
    window.draw(graph_S);
  }

  void draw_dots(std::vector<Contagion::State> const& states) {
    const int duration = states.size();
    const int population = states[0].S + states[0].I + states[0].R;
    sf::RectangleShape rect(sf::Vector2f(2.f, 2.f));

    for (int i = 0; i != duration; ++i) {
      float x = (i / static_cast<float>(duration)) * window.getSize().x;
      rect.setFillColor(sf::Color::Black);
      float y_S = (1 - states[i].S / static_cast<float>(population)) *
                  window.getSize().y;
      rect.setPosition(sf::Vector2f(x, y_S));
      window.draw(rect);
      rect.setFillColor(sf::Color::Red);
      float y_I = (1 - states[i].I / static_cast<float>(population)) *
                  window.getSize().y;
      rect.setPosition(sf::Vector2f(x, y_I));
      window.draw(rect);
      rect.setFillColor(sf::Color::Green);
      float y_R = (1 - states[i].R / static_cast<float>(population)) *
                  window.getSize().y;
      rect.setPosition(sf::Vector2f(x, y_R));
      window.draw(rect);
    }
  }

  // funzione che disegna la legenda
  void write_leg(std::string const& legend) {
    sf::Text text;
    text.setFont(font);
    text.setString(legend);

    float const x = window.getSize().x * 0.8;
    float const y = window.getSize().y * 0.1;
    text.setPosition(sf::Vector2f(x, y));

    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(15);
    window.draw(text);
  }
};

}  // namespace Contagion

#endif
