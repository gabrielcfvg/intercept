
// builtin
#include <SFML/Window/VideoMode.hpp>
#include <string>

// extern
#include <fmt/format.h>
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/vec2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// local
#include "slotmap.hpp"
#include "defs.hpp"





class ContextManager {

    private:

        sf::RenderWindow window;

    public:

        ContextManager() {

            window.create(sf::VideoMode(window_size.x, window_size.y), window_name);
            window.setFramerateLimit(framerate);
        }

        void run() {

            while (this->main_loop());
        }

    private:

        bool main_loop() {

            if (this->window.isOpen() == false)
                return false;

            // graphic

            this->window.clear(sf::Color::Black);

            this->window.display();
            
            // input
            sf::Event event;
            while (this->window.pollEvent(event)) {
                
                if (event.type == sf::Event::Closed)
                    this->window.close();
            }

            return true;
        }

};


int main() {

    fmt::print("foo bar\n");
    
    ContextManager ctx_manager{};
    ctx_manager.run();
}
