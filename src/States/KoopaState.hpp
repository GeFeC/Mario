#pragma once

#include "States/MonsterState.hpp"

struct KoopaState : ShellMonsterState{
  static constexpr auto WalkSpeed = 2.f;

protected:
  template<typename Entity>
  static auto make(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = Entity();
    koopa.position = position * config::BlockSize;
    koopa.size = glm::vec2(config::BlockSize, config::BlockSize * 1.5f);
    koopa.walk_speed = 2.f;
    koopa.shell_speed = 8.f;
    koopa.set_direction(direction);
    koopa.texture_flip = Drawable::Flip::UseFlip;
    koopa.reward_for_killing = 100.f;
    koopa.shell_height = config::BlockSize * 7.f / 8.f;

    return koopa;
  }

public:
  static auto make_green(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = make<KoopaState>(position, direction);
    koopa.current_texture = &textures::green_koopa_walk[0];

    return koopa;
  }

  static auto make_red(const glm::vec2& position, Direction direction = DirectionLeft){
    auto koopa = make<KoopaState>(position, direction);
    koopa.current_texture = &textures::red_koopa_walk[0];
    koopa.fall_from_edge = false;

    return koopa;
  }
};


