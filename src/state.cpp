// header
#include "state.hpp"

// local
#include "entities.hpp"

// extern
#include <glm/geometric.hpp>



void State::update_entities(double delta) {

    auto state_manager = StateManager{*this};
    for (auto& entity: m_entities)
        entity->update(state_manager, delta);

    state_manager.flush();
}

void State::render_entities(sf::RenderWindow& window) {

    for (auto& entity: m_entities)
        entity->render(window);
}

void State::new_missile(glm::u32vec2 orig, glm::u32vec2 target, MissileModel* model) {

    auto distance = glm::distance((glm::dvec2)orig, (glm::dvec2)target);
    auto dir = glm::normalize((glm::dvec2)target - (glm::dvec2)orig);

    auto missile = std::make_unique<Missile>(orig, dir, distance, model);
    this->create_entity(std::move(missile));

    if (model->friendly == true)
        m_alive_friendly_missiles += 1;
    else
        m_alive_enemy_missiles += 1;
}

void State::create_entity(std::unique_ptr<Entity> new_entity) {

    auto id = m_entities.push(std::move(new_entity));
    m_entities.get(id)->set_id(id);
}



void StateManager::add_entity(std::unique_ptr<Entity> new_entity) {

    auto new_command = new CreateEntityCommand{std::move(new_entity)};
    m_existential_commands.push(std::unique_ptr<CreateEntityCommand>{new_command});
}

void StateManager::remove_entity(Id id) {

    rb_runtime_assert(m_state.m_entities.contains(id));
    auto new_command = new RemoveEntityCommand{id};
    m_existential_commands.push(std::unique_ptr<RemoveEntityCommand>{new_command});
}

void StateManager::explosion(glm::u32vec2 pos, double radius) {

    auto new_command = new ExplosionCommand{*this, pos, radius};
    m_misc_commands.push(std::unique_ptr<ExplosionCommand>{new_command});
}

void StateManager::dec_friendly_missiles() {

    rb_assert(m_state.m_alive_friendly_missiles > 0);
    m_state.m_alive_friendly_missiles -= 1;
}

void StateManager::dec_enemy_missiles() {

    rb_assert(m_state.m_alive_enemy_missiles > 0);
    m_state.m_alive_enemy_missiles -= 1;
};
