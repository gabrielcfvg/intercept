// header
#include "state.hpp"

// local
#include "entities.hpp"

// extern
#include <glm/geometric.hpp>



void State::update_entities(double delta) {

    this->update_missiles(delta);

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
}

void State::create_entity(std::unique_ptr<Entity> new_entity) {

    auto id = m_entities.push(std::move(new_entity));
    m_entities.get(id)->set_id(id);
}

void State::update_missiles(double delta) {

    m_delta_accumulator += delta;
    if (m_delta_accumulator < wave_delay)
        return;

    m_delta_accumulator = 0;
    auto wave = gen_enemy_wave(m_score);

    for (auto& cords: wave)
        this->new_missile(cords.first, cords.second, &enemy_missile);
}

uint64_t State::get_score() const {

    return m_score;
}

uint64_t State::get_max_score() const {

    return m_max_score;
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

void StateManager::explosion(glm::u32vec2 pos, double radius, MissileModel* model) {

    auto new_command = new ExplosionCommand{*this, m_state, pos, radius, model};
    m_misc_commands.push(std::unique_ptr<ExplosionCommand>{new_command});
}

void StateManager::flush(){

    while ((m_existential_commands.empty() == false) || (m_misc_commands.empty() == false)) {

        while (m_existential_commands.empty() == false) {

            auto command = std::move(m_existential_commands.front());
            m_existential_commands.pop();
            command->execute(m_state);
        }

        if (m_misc_commands.empty() == false) {

            auto command = std::move(m_misc_commands.front());
            m_misc_commands.pop();
            command->execute(m_state);
        }
    }
}

void StateManager::hit_ground(MissileModel* model) {

    if (model->friendly == true)
        return;

    auto dec = m_state.m_max_score / 2;
    if (m_state.m_score >= dec)
        m_state.m_score -= dec;
    else {
        m_state.m_score = 0;
        m_state.m_max_score = 0;
    }
}
