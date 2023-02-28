#pragma once

#include "States/MonsterState.hpp"

struct HammerState : MonsterState{
  HammerState(){
    position = glm::vec2(-config::BigValue);
    size = glm::vec2(config::BlockSize);
    current_texture = &textures::hammer[0];
    is_visible = false;
    walk_speed = 5.f;
    should_collide = false;
  }
};
