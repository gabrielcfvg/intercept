#pragma once

// builtin
#include <cstdint>
#include <memory>
#include <functional>

// local
#include "assert.hpp"
#include "slotmap.hpp"

// extern
#include <SFML/Graphics/RenderWindow.hpp>
#include <glm/ext/vector_double2.hpp>
#include <glm/ext/vector_uint2_sized.hpp>
#include <glm/geometric.hpp>
#include <optional>



class StateAccess;
class Entity;
using Id = SlotMap<std::unique_ptr<Entity>>::Key;

class Entity {

    public:

        Id id;

    public:
    
        virtual std::optional<std::function<void(StateAccess&)>> update(StateAccess&) = 0;
        virtual void explosion(glm::u32vec2 pos, double radius) = 0;
        virtual void render(sf::RenderWindow&) = 0;
        
        Id get_id() {

            return this->id;
        }

    public:
        
        Entity(Id _id): id(_id) {}
        virtual ~Entity() = default;
};

class State {

    private:

        SlotMap<std::unique_ptr<Entity>> entities;

    public:

        friend class StateAccess;

    public:

        void render_entities(sf::RenderWindow& window) {

            rb_runtime_assert(window.isOpen());
        }

};

class StateAccess {

    private:

        State& state;

    public:

        StateAccess(State& _state): state{_state} {}

        Id add_entity(std::unique_ptr<Entity> new_entity) {

            return this->state.entities.push(std::move(new_entity));
        }

        void remove_entity(Id id) {

            rb_runtime_assert(this->state.entities.contains(id));
            this->state.entities.remove(id);
        }

        void explosion(glm::u32vec2 pos, double radius) {


        }
};


class Explosion: private Entity {

    private:

        glm::u32vec2 pos;
        double current_radius;
        double target_radius;
        double speed;

    public:

        std::optional<std::function<void(StateAccess&)>> update(StateAccess& state) override {

        }

        void render(sf::RenderWindow& window) override {

        }

    public:

        virtual ~Explosion() = default;
};


struct Missile: private Entity {

    private:

        glm::dvec2 pos;
        glm::dvec2 dir;
        double flight_distance;
        double speed;

    public:

        Missile(Id id): Entity(id) {}
        virtual ~Missile() = default;

    public:

        std::optional<std::function<void(StateAccess&)>> update(StateAccess& state) override {

            auto old_pos = this->pos;
            this->pos += this->dir * this->speed;
            this->flight_distance -= glm::distance(old_pos, this->pos);

            if (this->flight_distance < 0)
                return [=](StateAccess& state){ state.remove_entity(this->id); };
            else
                return std::nullopt;
        }
};



