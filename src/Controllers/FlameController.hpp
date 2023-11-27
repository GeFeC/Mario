#pragma once

#include "Controllers/CollisionController.hpp"
#include "Controllers/MonsterController.hpp"
#include "States/FlameState.hpp"
#include "States/LevelState.hpp"
#include "Controllers/EntityController.hpp"

namespace mario{

static auto flame_controller(FlameState& flame, LevelState& level, int particles_per_spawn = 10){
  //Burning:
  auto& particle_generator = flame.particle_generator;

  flame.particle_counter.run(window::delta_time, [&]{
    for (int i = 0; i < particles_per_spawn; ++i){
      particle_generator.make_item_if_needed();
      auto& new_item = particle_generator.item();
      new_item.opacity = 1.f;
      new_item.is_active = true;
      new_item.position = flame.position;
      new_item.angle = util::random_value(-90.f, 90.f);
    }
  });

  for (auto& particle : particle_generator.items){
    particle.position -= window::delta_time * 500.f * glm::vec2(
      -glm::sin(glm::radians(particle.angle)),
      glm::cos(glm::radians(particle.angle))
    ); 
    particle.opacity -= window::delta_time * 2.f * (1.f + glm::abs(particle.angle) / 30.f);

    if (particle.opacity <= 0.f) {
      particle.is_active = false;
      particle.position = glm::vec2(util::BigValue);
    }
  }

  for (auto& p : flame.particles()){
    if (p.opacity < 0.5f) continue;

    entity_controller::kill_player_on_touch(p, level);
  }

}

} //namespace mario
