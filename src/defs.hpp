#pragma once

// builtin
#include <cstdint>
#include <glm/ext/vector_uint2_sized.hpp>
#include <string>

// extern
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>



inline std::string const window_name = "Intercept";
inline glm::u32vec2 const window_size{512, 512}; // TODO: suportar HIDPI
inline uint32_t const max_ground_level = 96;
inline uint32_t const ground_level = (window_size.y > (max_ground_level * 5)) ? max_ground_level : window_size.y / 5;
inline glm::vec2 const gun_position{window_size.x / 2, ground_level};
inline uint32_t const framerate = 60;

struct MissileModel {
    glm::u8vec3 color;
    double speed;
    double explosion_size;
    bool friendly;
};

inline auto friendly_missile = MissileModel{.color = {0, 0, 255}, .speed = 2, .explosion_size = 2, .friendly = true};
inline auto enemy_missile = MissileModel{.color = {255, 0, 0}, .speed = 1, .explosion_size = 3, .friendly = false};
