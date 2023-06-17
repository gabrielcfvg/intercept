#pragma once

// local
#include "entity.hpp"
#include "state.hpp"

// extern
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>



class Explosion : public Entity {

private:

    glm::u32vec2 m_pos;
    double m_current_radius;
    double m_target_radius;
    double m_speed;

public:

    Explosion(glm::u32vec2 pos, double target_radius, double speed):
        m_pos(pos), m_current_radius(0), m_target_radius(target_radius), m_speed(speed) {}

public:

    void update(StateManager& state, double delta) override {

        m_current_radius += m_speed * delta;
        state.explosion(m_pos, m_current_radius);

        if (m_current_radius >= m_target_radius)
            state.remove_entity(this->get_id());
    }

    void explosion(StateManager&, glm::u32vec2, double) override {}

    void render(sf::RenderWindow& window) const override {

        auto circle = sf::CircleShape{};
        circle.setPosition((float)m_pos.x - (float)m_current_radius, (float)m_pos.y - (float)m_current_radius);
        circle.setRadius((float)m_current_radius);
        circle.setFillColor(explosion_color);
        window.draw(circle);
    }
};


struct Missile : public Entity {

private:

    glm::u32vec2 m_origin;
    glm::dvec2 m_pos;
    glm::dvec2 m_dir;
    double m_flight_distance;
    MissileModel* m_model = nullptr;

public:

    Missile(glm::u32vec2 origin, glm::dvec2 dir, double flight_distance, MissileModel* model):
        m_origin(origin), m_pos(origin), m_dir(dir), m_flight_distance(flight_distance), m_model(model) {}

public:

    void update(StateManager& state, double delta) override {

        auto old_pos = m_pos;
        m_pos += m_dir * m_model->speed * delta;
        m_flight_distance -= glm::distance(old_pos, m_pos);

        if (m_flight_distance < 0 || (m_pos.y < ground_level && m_model->friendly == false))
            this->explode(state);
        else if (m_pos.x >= window_size.x || m_pos.y >= window_size.y)
            this->self_remove(state);
    }

    void explosion(StateManager& state, glm::u32vec2 position, double radius) override {

        if (glm::distance((glm::dvec2)position, m_pos) <= radius)
            state.remove_entity(this->get_id());
    }

    void render(sf::RenderWindow& window) const override {

        auto tail = sf::RectangleShape{};
        tail.setSize({(float)glm::distance((glm::dvec2)m_origin, m_pos), 1.f});
        tail.setPosition((float)m_origin.x, (float)m_origin.y);
        tail.setRotation(glm::degrees((float)atan2(m_dir.y, m_dir.x)));
        tail.setFillColor(m_model->tail_color);
        tail.setOutlineThickness(0);
        window.draw(tail);

        auto circle = sf::CircleShape{};
        circle.setRadius((float)missile_radius);
        circle.setPosition((float)m_pos.x - (float)missile_radius, (float)m_pos.y - (float)missile_radius);
        circle.setFillColor(m_model->color);
        circle.setOutlineThickness(0);
        window.draw(circle);
    }

private:

    void explode(StateManager& state) {

        auto explosion = std::make_unique<Explosion>(m_pos, m_model->explosion_size, m_model->explosion_speed);
        state.add_entity(std::move(explosion));
        this->self_remove(state);
    }

    void self_remove(StateManager& state) {

        state.remove_entity(this->get_id());
        this->dec_missile_counter(state);
    }

    void dec_missile_counter(StateManager& state) {

        if (m_model->friendly == true)
            state.dec_friendly_missiles();
        else
            state.dec_enemy_missiles();
    }
};
