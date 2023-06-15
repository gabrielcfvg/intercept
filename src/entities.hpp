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


struct Missile : private Entity {

private:

    glm::dvec2 m_pos;
    glm::dvec2 m_dir;
    double m_flight_distance;
    double m_speed;

public:

    explicit Missile(Id id): Entity(id) {}
    ~Missile() override = default;

public:

    void update(StateManager& state) override {

        auto old_pos = this->m_pos;
        this->m_pos += this->m_dir * this->m_speed;
        this->m_flight_distance -= glm::distance(old_pos, this->m_pos);

        if (this->m_flight_distance < 0)
            state.remove_entity(this->get_id());
    }

    void render(sf::RenderWindow&) const override {

        // TODO: this
    }
};
