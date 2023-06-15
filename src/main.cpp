
// builtin
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <string>
#include <thread>

// extern
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

public:

    ContextManager() {

        m_window.create(sf::VideoMode(window_size.x, window_size.y), window_name);
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

            delta = (double)cycle_time / (double)1'000'000'000;
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

            if (event.type == sf::Event::Closed)
                this->m_window.close();
        }
    }

    void update(double delta) {}

    void render() {

        this->m_window.clear(sf::Color::Black);
        this->m_window.display();
    }
};


int main() {

    fmt::print("foo bar\n");

    ContextManager ctx_manager{};
    ctx_manager.run();
}
