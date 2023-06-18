#pragma once

// builtin
#include <random>
#include <utility>
#include <vector>

// extern
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>

// local
#include "defs.hpp"



inline std::pair<glm::u32vec2, glm::u32vec2> gen_enemy_missile_cord() {

    static auto gen_random = [] {
        static std::mt19937_64 engine(time(0));
        return engine() % window_size.x;
    };

    auto origin = glm::u32vec2{gen_random(), 0};
    auto target = glm::u32vec2{gen_random(), window_size.y};

    return {origin, target};
}

inline std::vector<std::pair<glm::u32vec2, glm::u32vec2>> gen_enemy_wave(uint64_t score) {

    auto missile_amount = (uint32_t)log10((double)score + 5) + 1;

    std::vector<std::pair<glm::u32vec2, glm::u32vec2>> output;
    for (size_t idx = 0; idx < missile_amount; ++idx)
        output.push_back(gen_enemy_missile_cord());

    return output;
}
