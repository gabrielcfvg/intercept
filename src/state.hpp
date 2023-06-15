#pragma once

// builtin
#include <memory>
#include <queue>

// local
#include "assert.hpp"
#include "defs.hpp"
#include "entity.hpp"
#include "slotmap.hpp"



class State {
public:

    friend class StateManager;

private:

    uint64_t alive_friendly_missiles = 0;
    uint64_t alive_enemy_missiles = 0;
    SlotMap<std::unique_ptr<Entity>> m_entities;

public:

    void new_missile(glm::u32vec2 orig, glm::u32vec2 target, MissileModel model);
    void update_entities();
    void render_entities(sf::RenderWindow& window);

private:

    void create_entity(std::unique_ptr<Entity> new_entity);
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
        glm::u32vec2 m_pos;
        double m_radius;

        ExplosionCommand(StateManager& state_manager, glm::u32vec2 pos, double radius):
            m_state_manager(state_manager), m_pos(pos), m_radius(radius) {}

        void execute(State& state) override {

            for (auto& entity: state.m_entities)
                entity->explosion(m_state_manager, m_pos, m_radius);
        }
    };

private:

    State& m_state;
    std::queue<std::unique_ptr<Command>> m_existential_commands;
    std::queue<std::unique_ptr<Command>> m_misc_commands;

public:

    explicit StateManager(State& _state): m_state{_state} {}

    Id add_entity(std::unique_ptr<Entity> new_entity);
    void remove_entity(Id id);
    void explosion(glm::u32vec2 pos, double radius);
    void dec_friendly_missiles();
    void dec_enemy_missiles();

private:

    friend State;

    void flush() {

    again:;

        while (m_existential_commands.empty() == false) {

            auto command = std::move(m_existential_commands.front());
            m_existential_commands.pop();

            command->execute(m_state);
        }

        if (m_misc_commands.empty() == false) {

            auto command = std::move(m_misc_commands.front());
            m_misc_commands.pop();

            command->execute(m_state);
            goto again;
        }
    }
};
