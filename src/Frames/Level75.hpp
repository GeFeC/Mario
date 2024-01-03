#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level75(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level75;
  level.type = LevelState::Type::Vertical;
  level.number = { 7, 5 };
  level.extra_textures = {
    &textures::castle_bg,
		&textures::platform,
		texture_groups::lava,
		texture_groups::flame_koopa
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(7.f, 149.f), glm::vec2(6.f, 1.f) });
		for (int i = 0; i < 2; ++i){
			level.game_objects.push(PlatformState(glm::vec2(4.f + i * 8.f, 147.f - i * 2.f), glm::vec2(0.f, -80.f)));
		}

		const auto put_fire_bar = [&](const glm::vec2& position){
			level.game_objects.push(FireBarState(position, 5.f));
			level.game_objects.push(FireBarState(position, 5.f)).initial_rotation = 180.f;
		};

		put_fire_bar(glm::vec2(14.f, 84.f));
		put_fire_bar(glm::vec2(7.f, 79.f));
		put_fire_bar(glm::vec2(14.f, 74.f));
		put_fire_bar(glm::vec2(7.f, 69.f));

		level.game_objects.push(LavaState{ glm::vec2(1.f, 49.f), glm::vec2(18.f, 2.f) });

    level_generator::generate_level(level, "level75_1.csv");
    level_generator::generate_level(level, "level75_2.csv");
  });
}

} //namespace mario::frames
