#pragma once

// builtin
#include <cstdint>
#include <glm/ext/vector_uint2_sized.hpp>
#include <string>

// extern
#include <SFML/Graphics/Color.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



inline std::string const window_name = "Intercept";
inline glm::u32vec2 const window_size{512, 512}; // TODO: suportar HIDPI
inline uint32_t const max_ground_level = 96;
inline uint32_t const ground_level = (window_size.y > (max_ground_level * 5)) ? max_ground_level : window_size.y / 5;
inline glm::vec2 const gun_position{window_size.x / 2, window_size.y - ground_level};
inline uint32_t const framerate = 60;
inline uint32_t const cursor_width = 10;
inline sf::Color explosion_color = {255, 50, 50};
inline double const missile_radius = 2;

struct MissileModel {
    sf::Color color;
    sf::Color tail_color;
    double speed;
    bool friendly;
    double explosion_size;
    double explosion_speed;
};

inline auto friendly_missile = MissileModel{.color = {0, 0, 255},
                                            .tail_color = {255, 255, 255},
                                            .speed = 200,
                                            .friendly = true,
                                            .explosion_size = 20,
                                            .explosion_speed = 100};

inline auto enemy_missile = MissileModel{.color = {255, 0, 0},
                                         .tail_color = {255, 0, 0},
                                         .speed = 100,
                                         .friendly = false,
                                         .explosion_size = 30,
                                         .explosion_speed = 100};
