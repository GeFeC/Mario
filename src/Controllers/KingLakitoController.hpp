#pragma once

#include "Controllers/BossController.hpp"
#include "Controllers/EntityController.hpp"
#include "Controllers/LakitoController.hpp"
#include "States/BossState.hpp"
#include "States/LevelState.hpp"
#include "Util/Random.hpp"
#include "Window.hpp"
#include "res/textures.hpp"

namespace mario{

static auto run_controller(KingLakitoState& boss, LevelState& level){
  boss_controller::run(boss, level);
  boss_controller::react_when_hit_by_fireball(boss, level);

  boss.timer += window::delta_time;
  if (boss.hp == 0){
    boss.gravity_flip = util::Flip::no_flip();
    entity_controller::handle_gravity(boss, level);
    return;
  }

  //Movement:
  boss.position = KingLakitoState::InitialPosition + glm::vec2(
    glm::sin(boss.timer * 1.4f) * 7.5 * BlockBase::Size,
    glm::sin(boss.timer * 0.65f) * 1.f * BlockBase::Size
  );

  //Throwing Spike:
  static constexpr auto ThrownSpikeMoveSpeed = 500.f;
  boss.extra_thrown_spikes[0].position.x += window::delta_time * ThrownSpikeMoveSpeed;
  boss.extra_thrown_spikes[1].position.x -= window::delta_time * ThrownSpikeMoveSpeed;

  lakito_controller::run_thrown_spike_controller(boss.thrown_spike, level);
  for (auto& spike : boss.extra_thrown_spikes){
    lakito_controller::run_thrown_spike_controller(spike, level);
  }

  boss.throw_counter.run(window::delta_time);

  auto counter_limit = boss.throw_counter.limit;
  if (boss.throw_counter.value - counter_limit == util::in_range(-3, 0)){
    boss.current_texture = &textures::lakito_hidden;

    boss.should_throw_spike = true;
  }
  else{
    boss.current_texture = &textures::lakito_shown;

    if (boss.should_throw_spike){
      boss.throw_spike(25.f);
      boss.throw_spike(boss.extra_thrown_spikes[0], 22.f);
      boss.throw_spike(boss.extra_thrown_spikes[1], 22.f);

      boss.should_throw_spike = false;
      boss.throw_counter.limit = util::random_value(10, 14);
    }
  }
}

} //namespace mario
