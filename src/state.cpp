// header
#include "state.hpp"



void State::update_entities() {

    auto state_manager = StateManager{*this};
    for (auto& entity: m_entities)
        entity->update(state_manager);

    state_manager.flush();
}

void State::render_entities(sf::RenderWindow& window) {

    // TODO: this
}
void State::new_missile(glm::u32vec2 orig, glm::u32vec2 target, MissileModel model) {

    // TODO: this
}

void State::create_entity(std::unique_ptr<Entity> new_entity) {

    auto id = m_entities.push(std::move(new_entity));
    m_entities.get(id).value().get()->set_id(id);
}



Id StateManager::add_entity(std::unique_ptr<Entity> new_entity) {

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

    rb_assert(m_state.alive_friendly_missiles > 0);
    m_state.alive_friendly_missiles -= 1;
}

void StateManager::dec_enemy_missiles() {

    rb_assert(m_state.alive_enemy_missiles > 0);
    m_state.alive_enemy_missiles -= 1;
}
