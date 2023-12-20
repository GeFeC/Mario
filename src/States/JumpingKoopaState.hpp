#pragma once

#include "States/KoopaState.hpp"

namespace mario{

struct JumpingKoopaState : KoopaState{
  static constexpr auto JumpForce = -20.f;
  bool has_wings = true;

	template<typename T = JumpingKoopaState>
	static auto make(const glm::vec2& position){
		auto koopa = KoopaState::make<T>(position);
		koopa.gravity_boost = 0.7f;

		return koopa;
	}

  static auto make_green(const glm::vec2& position){
    auto koopa = make(position);
    koopa.type = Type::Green;

    koopa.current_texture = &textures::green_flying_koopa_walk[0];

    return koopa;
  }

  static auto make_red(const glm::vec2& position){
    auto koopa = make(position);
    koopa.type = Type::Red;

    koopa.current_texture = &textures::red_flying_koopa_walk[0];

    return koopa;
  }

  static auto make_purple(const glm::vec2& position){
    auto koopa = make(position);
    koopa.type = Type::Purple;

    koopa.current_texture = &textures::purple_flying_koopa_walk[0];

    return koopa;
  }
};

} //namespace mario
