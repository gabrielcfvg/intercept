#pragma once

// builtin
#include <cstdint>
#include <glm/ext/vector_uint2_sized.hpp>
#include <string>

// extern
#include <glm/vec2.hpp>



std::string const window_name = "Intercept";
glm::u32vec2 const window_size{512, 512};
uint32_t const max_ground_level = 96;
uint32_t const ground_level = (window_size.y > (max_ground_level * 5)) ? max_ground_level : window_size.y / 5;
glm::vec2 const gun_position{window_size.x / 2, ground_level};
uint32_t const framerate = 60;
double const ballistic_missile_speed = 1;
double const interceptor_speed = 1;
