#pragma once

#include "States/AppState.hpp"
#include "Frames/LevelBase.hpp"
#include "res/textures.hpp"

namespace mario::frames{

static auto run_level72(AppState& app){
  auto level = LevelFrameSharedData{};
  level.frame = AppState::Frame::Level72;
  level.type = LevelState::Type::Horizontal;
  level.number = { 7, 2 };
  level.extra_textures = {
    &textures::castle_bg,
		&textures::platform,
		&textures::upfire,
		texture_groups::flame_flying_koopa,
		texture_groups::flame_koopa,
		texture_groups::lava
  };

  run_levelbase(app, level, [](AppState& app){
    auto& level = app.current_level;
    level.background_texture = &textures::castle_bg;

		level.game_objects.push(LavaState{ glm::vec2(3.f, 10.f), glm::vec2(101.f, 2.f) });
		level.game_objects.push(PlatformState(glm::vec2(4.f, 9.f), glm::vec2(95.f, 0.f), 10));
		level.game_objects.push(FireBarState(glm::vec2(11.f, 9.f), 4));
		level.game_objects.push(FireBarState(glm::vec2(11.f, 9.f), 4)).initial_rotation = 180.f;
		level.game_objects.push(UpfireState(glm::vec2(16.f, 11.f)));
		level.game_objects.push(FireBarState(glm::vec2(16.f, 4.f), 3));
		level.game_objects.push(FireBarState(glm::vec2(16.f, 4.f), 3)).initial_rotation = 180.f;
		level.game_objects.push(FlameFlyingKoopaState::make(glm::vec2(26.f, 6.f), glm::vec2(0.f, 2.f)));
		level.game_objects.push(UpfireState(glm::vec2(35.f, 11.f)));
		level.game_objects.push(UpfireState(glm::vec2(37.f, 11.f))).counter.value = 5.f;
		level.game_objects.push(UpfireState(glm::vec2(50.f, 11.f), 30.f));

		for (int i = 0; i < 3; ++i){
			level.game_objects.push(FireBarState(glm::vec2(50.f, 8.f), 3)).initial_rotation = i * 120.f;
		}

		for (int i = 0; i < 2; ++i){
			auto& bar = level.game_objects.push(FireBarState(glm::vec2(72.f, 6.f), 2));
			bar.spacing = 4.f;
			bar.initial_rotation = i * 180.f;
		}

		level.game_objects.push(FireBarState(glm::vec2(85.f, 10.f), 3));
		level.game_objects.push(FireBarState(glm::vec2(85.f, 10.f), 3)).initial_rotation = 180.f;

		level_generator::put_q_block_with_coins(level, glm::vec2(101.f, 5.f), 10);

		level.game_objects.push(FireBarState(glm::vec2(108.f, 3.f), 4));
		level.game_objects.push(FireBarState(glm::vec2(114.f, 3.f), 4)).initial_rotation = 90.f;
		level.game_objects.push(FireBarState(glm::vec2(120.f, 3.f), 4)).initial_rotation = 180.f;

		level.game_objects.push(LavaState{ glm::vec2(126.f, 10.f), glm::vec2(25.f, 2.f) });

		const auto put_platform = [&](float x, float y, int direction){
			level.game_objects.push(PlatformState(glm::vec2(x,y), glm::vec2(0.f, 4.f * direction), 6))
			.is_active = true;
		};

		put_platform(127.f, 8.f, -1);
		put_platform(132.f, 4.f, 1);
		put_platform(137.f, 8.f, -1);
		put_platform(142.f, 4.f, 1);
		put_platform(147.f, 8.f, -1);

		for (int i = 0; i < 8; ++i){
			level.game_objects.push(UpfireState(glm::vec2(128.f + i * 3.f, 11.f), 30.f))
				.counter.value = (11 - i) * 0.8f;
		}

		level.game_objects.push(LavaState{ glm::vec2(155.f, 10.f), glm::vec2(12.f, 2.f) });

		level.game_objects.push(FireBarState(glm::vec2(161.f, 9.f), 8));
		level.game_objects.push(UpfireState(glm::vec2(157.f, 11.f), 30.f)).counter.limit = 20.f;
		level.game_objects.push(UpfireState(glm::vec2(164.f, 11.f), 30.f)).counter.limit = 20.f;

		level.game_objects.push(LavaState{ glm::vec2(171.f, 10.f), glm::vec2(25.f, 2.f) });

		for (int i = 0; i < 4; ++i){
			level.game_objects.push(UpfireState(glm::vec2(176.f + i * 5, 11.f), 30.f))
				.counter.value = i * 2.f;
		}

		level.game_objects.push(FlameJumpingKoopaState::make(glm::vec2(189.f, 2.f)))
			.flip_gravity();

		level.game_objects.push(FlameJumpingKoopaState::make(glm::vec2(193.f, 2.f)))
			.flip_gravity();

    level_generator::generate_level(level, "level72_1.csv");
    level_generator::generate_level(level, "level72_2.csv");
  });
}

} //namespace mario::frames
