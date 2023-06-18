
// builtin
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <string>
#include <thread>

// extern
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/vec2.hpp>

// local
#include "defs.hpp"
#include "entities.hpp"
#include "slotmap.hpp"
#include "state.hpp"



class ContextManager {
private:

    sf::RenderWindow m_window;
    State m_state;
    glm::u32vec2 m_mouse_position = {0, 0};

    sf::Font m_font;

public:

    ContextManager() {

        auto flags = sf::Style::Close | sf::Style::Titlebar;
        m_window.create(sf::VideoMode(window_size.x, window_size.y), window_name, flags);
        rb_runtime_assert(m_font.loadFromFile("roboto_mono.ttf"), "erro ao ler a fonte");
    }

    void run() {

        double delta = 0;
        bool should_continue = true;
        while (should_continue) {

            auto start = std::chrono::high_resolution_clock::now();
            this->cycle(delta, should_continue);
            auto end = std::chrono::high_resolution_clock::now();

            auto const min_time = (1'000 / framerate) * 1'000'000;
            auto cycle_time = ((end - start)).count();

            if (cycle_time < min_time) {
                auto wait_time = min_time - cycle_time;
                std::this_thread::sleep_for(std::chrono::nanoseconds(wait_time));
            }

            delta = (double)glm::max((uint64_t)cycle_time, (uint64_t)min_time) / (double)1'000'000'000;
        }
    }

private:

    void cycle(double delta, bool& should_continue) {

        if (this->m_window.isOpen() == false)
            should_continue = false;

        this->input();
        this->update(delta);
        this->render();
    }

    void input() {

        sf::Event event{};
        while (this->m_window.pollEvent(event)) {

            switch (event.type) {

                case sf::Event::Closed:
                    this->m_window.close();
                    break;

                case sf::Event::MouseButtonPressed:

                    if (m_mouse_position.y < ground_level)
                        m_state.new_missile(gun_position, m_mouse_position, &friendly_missile);
                    break;

                case sf::Event::MouseMoved:
                    m_mouse_position = {event.mouseMove.x, event.mouseMove.y};
                    break;

                default:
                    break;
            }
        }
    }

    void update(double delta) {

        m_state.update_entities(delta);
    }

    void render() {

        this->m_window.clear(sf::Color::Black);

        // render ground
        auto brown_color = sf::Color{61, 40, 22};
        auto ground_rectangle = sf::RectangleShape{};
        ground_rectangle.setSize({(float)window_size.x, (float)window_size.y - (float)ground_level});
        ground_rectangle.setFillColor(brown_color);
        ground_rectangle.setPosition(0, (float)ground_level);
        m_window.draw(ground_rectangle);

        // render state
        m_state.render_entities(m_window);

        // render score
        sf::Text score_display{fmt::format("score {}", m_state.get_score()), m_font, 25};
        score_display.setPosition(10, 10);
        m_window.draw(score_display);

        // render_max_score
        sf::Text max_score_display{fmt::format("max score {}", m_state.get_max_score()), m_font, 25};
        max_score_display.setPosition(10, 35);
        m_window.draw(max_score_display);

        m_window.display();
    }
};


int main() {

    fmt::print("foo bar\n");
    ContextManager ctx_manager{};
    ctx_manager.run();
}
