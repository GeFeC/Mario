#pragma once

#include "States/BossState.hpp"
#include "Controllers/BossController.hpp"
#include "Controllers/MonsterController.hpp"
#include "config.hpp"

namespace mario::king_cheep_controller{

static auto minion_random_position_from_right(){
  return glm::vec2(
    LevelState::BossLevelSize.x + FishState::random_value(0, 10),
    FishState::random_value(2, LevelState::BossLevelSize.y - 2)
  );
};

static auto minion_random_position_from_left(){
  return glm::vec2(
    FishState::random_value(-11, -1),
    FishState::random_value(2, LevelState::BossLevelSize.y - 2)
  );
};

} //namespace mario::king_cheep_controller

namespace mario{

static auto run_controller(KingCheepState& boss, LevelState& level){
  boss.timer += window::delta_time;

  if (boss.hp > 0){
    monster_controller::run_movement_animation(boss, textures::red_fish_swim);
  }
  else{
    entity_controller::handle_gravity(boss, level);

    level.game_objects.for_each_type<FishState>([](FishState& fish){
      //Freeze fish:
      fish.is_active = false;
      fish.max_offset = 0.f;
    });
  }

  boss_controller::react_when_hit_by_fireball(boss, level);
  boss_controller::run(boss, level);

  if (boss.hp == 0) return;
  
  //Movement:

  static constexpr auto MovementOrigin = config::FrameBufferSize / 2.f;
  static constexpr auto MovementRadius = glm::vec2(config::FrameBufferSize.x / 3.f, 0.f);
  const auto timer_sin = glm::sin(-boss.timer);
  const auto timer_cos = glm::cos(-boss.timer);

  boss.position.x = -timer_sin * MovementRadius.x;
  boss.position.y = timer_cos * MovementRadius.x / 2.f;

  boss.position += MovementOrigin - boss.size / 2.f;
  //Texture rotation
  boss.rotation = -boss.timer + glm::radians(180.f);

  //Minions logic:
  level.game_objects.for_each_type<FishState>([&](FishState& fish){
    const auto was_fish_killed = fish.position.y > config::FrameBufferSize.y;

    if (was_fish_killed){
      fish.was_hit = false;
      fish.vertical_flip = util::Flip::no_flip();
    }

    if (fish.direction.is_left()){
      if (fish.position.x < -BlockBase::Size || was_fish_killed){
        fish.position = king_cheep_controller::minion_random_position_from_right() * BlockBase::Size;
      }
    }              

    if (fish.direction.is_right()){
      if (fish.position.x > config::FrameBufferSize.x || was_fish_killed){
        fish.position = king_cheep_controller::minion_random_position_from_left() * BlockBase::Size;
      }
    }              
  });
}

} //namespace mario
