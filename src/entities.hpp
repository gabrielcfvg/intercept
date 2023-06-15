#pragma once

// local
#include "entity.hpp"
#include "state.hpp"

// extern
#include <glm/geometric.hpp>



class Explosion : public Entity {

private:

    glm::u32vec2 m_pos;
    double m_current_radius;
    double m_target_radius;
    double m_speed;

public:

    Explosion(glm::u32vec2 pos, double current_radius, double target_radius, double speed):
        m_pos(pos), m_current_radius(current_radius), m_target_radius(target_radius), m_speed(speed) {}

public:

    void update(StateManager& state, double delta) override {

        m_current_radius += m_speed * delta;
        state.explosion(m_pos, m_current_radius);

        if (m_current_radius >= m_target_radius)
            state.remove_entity(this->get_id());
    }

    void explosion(StateManager&, glm::u32vec2, double) override {}

    void render(sf::RenderWindow&) const override {

        // TODO: this
    }
};


struct Missile : public Entity {

private:

    glm::u32vec2 origin;
    glm::dvec2 m_pos;
    glm::dvec2 m_dir;
    double m_flight_distance;
    MissileModel* m_model = nullptr;

public:

    Missile(glm::u32vec2 origin, glm::dvec2 pos, glm::dvec2 dir, double flight_distance, MissileModel* model):
        origin(origin), m_pos(pos), m_dir(dir), m_flight_distance(flight_distance), m_model(model) {}

public:

    void update(StateManager& state, double delta) override {

        auto old_pos = m_pos;
        m_pos += m_dir * m_model->speed * delta;
        m_flight_distance -= glm::distance(old_pos, m_pos);

        if (m_flight_distance < 0) {

            state.remove_entity(this->get_id());

            auto explosion = std::make_unique<Explosion>(m_pos, 0, m_model->explosion_size, m_model->explosion_speed);
            state.add_entity(std::move(explosion));

            if (m_model->friendly == true)
                state.dec_friendly_missiles();
            else
                state.dec_enemy_missiles();
        }
        else if (m_pos.x >= window_size.x || m_pos.y >= window_size.y)
            state.remove_entity(this->get_id());
    }

    void explosion(StateManager& state, glm::u32vec2 position, double radius) override {

        if (glm::distance((glm::dvec2)position, m_pos) <= radius)
            state.remove_entity(this->get_id());
    }

    void render(sf::RenderWindow&) const override {

        // TODO: this
    }
};
