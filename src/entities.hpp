#pragma once

// local
#include "entity.hpp"
#include "state.hpp"

// extern
#include <glm/geometric.hpp>



class Explosion : private Entity {

private:

    glm::u32vec2 m_pos;
    double m_current_radius;
    double m_target_radius;
    double m_speed;

public:

    Explosion(glm::u32vec2 pos, double current_radius, double target_radius, double speed):
        m_pos(pos), m_current_radius(current_radius), m_target_radius(target_radius), m_speed(speed) {}

public:

    void update(StateManager& state) override {

        this->m_current_radius += this->m_speed;
        state.explosion(this->m_pos, this->m_current_radius);

        if (this->m_current_radius >= m_target_radius)
            state.remove_entity(this->get_id());
    }

    void render(sf::RenderWindow&) const override {

        // TODO: this
    }
};


enum class MissileType {
    Enemy,
    Friendly
};

struct Missile : private Entity {

private:

    glm::dvec2 m_pos;
    glm::dvec2 m_dir;
    double m_flight_distance;
    double m_speed;
    MissileType m_type;

public:

    Missile(glm::dvec2 pos, glm::dvec2 dir, double flight_distance, double speed, MissileType type):
        m_pos(pos), m_dir(dir), m_flight_distance(flight_distance), m_speed(speed), m_type(type) {}

    ~Missile() override = default;

public:

    void update(StateManager& state) override {

        auto old_pos = this->m_pos;
        this->m_pos += this->m_dir * this->m_speed;
        this->m_flight_distance -= glm::distance(old_pos, this->m_pos);

        if (this->m_flight_distance < 0) {
            state.remove_entity(this->get_id());

            switch (m_type) {
                case MissileType::Enemy:
                    state.dec_enemy_missiles();
                    break;
                case MissileType::Friendly:
                    state.dec_friendly_missiles();
                    break;
            }
        }
    }

    void render(sf::RenderWindow&) const override {

        // TODO: this
    }
};
