#pragma once

#include "States/MonsterState.hpp"

struct HammerState : MonsterState{
  HammerState(){
    position = glm::vec2(-util::BigValue);
    size = glm::vec2(BlockBase::Size);
    current_texture = &textures::hammer[0];
    is_visible = false;
    walk_speed = 5.f;
    should_collide = false;
  }
};
