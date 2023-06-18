#pragma once

// builtin
#include <memory>
#include <queue>

// local
#include "assert.hpp"
#include "defs.hpp"
#include "entity.hpp"
#include "missile_generator.hpp"
#include "slotmap.hpp"



class State {
public:

    friend class StateManager;

private:

    uint64_t m_score = 0;
    uint64_t m_max_score = 0;
    double m_delta_accumulator = wave_delay;
    SlotMap<std::unique_ptr<Entity>> m_entities;

public:

    void new_missile(glm::u32vec2 orig, glm::u32vec2 target, MissileModel* model);
    void update_entities(double delta);
    void render_entities(sf::RenderWindow& window);

    uint64_t get_score() const;
    uint64_t get_max_score() const;

private:

    void create_entity(std::unique_ptr<Entity> new_entity);
    void update_missiles(double delta);
};


class StateManager {
private:

    struct Command {

        virtual void execute(State&) = 0;
        virtual ~Command() = default;
    };

    struct RemoveEntityCommand : public Command {

        Id m_entity_id;

        explicit RemoveEntityCommand(Id entity_id): m_entity_id(entity_id) {}

        void execute(State& state) override {

            rb_runtime_assert(state.m_entities.contains(m_entity_id));
            state.m_entities.remove(m_entity_id);
        }
    };

    struct CreateEntityCommand : public Command {

        std::unique_ptr<Entity> m_new_entity;

        explicit CreateEntityCommand(std::unique_ptr<Entity> new_entity): m_new_entity(std::move(new_entity)) {}

        void execute(State& state) override {

            state.create_entity(std::move(m_new_entity));
        }
    };

    struct ExplosionCommand : public Command {

        StateManager& m_state_manager;
        State& m_state;
        glm::u32vec2 m_pos;
        double m_radius;
        MissileModel* m_model;
        uint32_t m_shotdown_count = 0;

        ExplosionCommand(StateManager& state_manager, State& state, glm::u32vec2& pos, double radius,
                         MissileModel* model):
            m_state_manager(state_manager),
            m_state(state), m_pos(pos), m_radius(radius), m_model(model) {}

        void execute(State& state) override {

            for (auto& entity: state.m_entities) {

                bool shotdown = false;
                entity->explosion(m_state_manager, m_pos, m_radius, m_model, shotdown);

                if (shotdown == true)
                    m_shotdown_count += 1;
            }
        }

        ~ExplosionCommand() override {

            if (m_shotdown_count > 0) {

                m_state.m_score += (uint64_t)pow((double)2, (double)m_shotdown_count);

                if (m_state.m_score > m_state.m_max_score)
                    m_state.m_max_score = m_state.m_score;
            }
        }
    };

private:

    State& m_state;
    std::queue<std::unique_ptr<Command>> m_existential_commands;
    std::queue<std::unique_ptr<Command>> m_misc_commands;

public:

    explicit StateManager(State& _state): m_state{_state} {}

    void add_entity(std::unique_ptr<Entity> new_entity);
    void remove_entity(Id id);
    void explosion(glm::u32vec2 pos, double radius, MissileModel* model);
    void hit_ground(MissileModel* model);

private:

    friend State;

    void flush();
};
