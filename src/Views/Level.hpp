#pragma once

#include "Renderer/Drawable.hpp"
#include "Renderer/Renderer.hpp"
#include "States/AppState.hpp"

#include "Views/Entities.hpp"
#include "Views/Plants.hpp"
#include "Views/Background.hpp"
#include "Views/Player.hpp"

#include "Util/Loop.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

namespace mario::views{

static auto render_water(const LevelState& level){
  static constexpr auto WaterTransparency = 0.5f;

  if (level.water_level == util::BigValue) return;

  static auto waves_offset = 0.f;
  waves_offset += window::delta_time * 40.f;
  if (waves_offset >= BlockBase::Size) waves_offset -= BlockBase::Size;

  for (int i = 0; i < level.max_size().x + 1; ++i){ //one more than level size because of animation
    auto block = BlockBase{};
    block.position = { i * BlockBase::Size - waves_offset, level.water_level * BlockBase::Size };
    block.size = glm::vec2(BlockBase::Size);
    block.texture = &textures::water_top;
    block.alpha = WaterTransparency;

    render_block(block, level);
  }

  const auto water_area_position = std::make_pair(0.f, level.water_level + 1.f);
  const auto water_area_size = std::make_pair(level.max_size().x, level.max_size().y);

  util::for_2d([&](int x, int y){
    auto block = BlockBase{};
    block.position = { x * BlockBase::Size, y * BlockBase::Size };
    block.size = glm::vec2(BlockBase::Size);
    block.texture = &textures::water_bottom;
    block.alpha = WaterTransparency;

    render_block(block, level);
  }, water_area_position, water_area_size);
}

static auto render_stats(const LevelState& level){
  const auto& stats = level.stats;

  auto body = std::ostringstream(); 
  body 
    << std::setw(6) << std::setfill('0') << stats.score << "  x"
    << std::setw(2) << std::setfill('0') << stats.coins << "  x"
    << std::setw(2) << std::setfill('0') << stats.stored_mushrooms << "  "
    << stats.level_major << '-' << stats.level_minor << "   "
    << std::setw(3) << std::setfill('0') << stats.time;

  const auto font_size = fonts::normal.size * 3.5f;
  static constexpr auto WindowWidth = config::FrameBufferSize.x;

  auto text = renderer::Text(&fonts::normal, "MARIO            WORLD TIME", font_size / fonts::normal.size);
  text.update();
  const auto center_x = WindowWidth / 2 - text.get_size().x / 2;
  const auto step_y = 2.f / 3.f * BlockBase::Size;

  text.position = { center_x, step_y + level.stats.position_y };
  text.update_position();
  renderer::print(text, glm::vec2(0));

  text.text = body.str();
  text.position += glm::vec2(0.f, step_y);
  text.update();
  renderer::print(text, glm::vec2(0));

  //Mini coin
  auto mini_coin = renderer::Drawable{};
  mini_coin.position = glm::vec2(center_x + 7 * font_size, step_y * 2 + level.stats.position_y);
  mini_coin.size = glm::vec2(font_size);
  mini_coin.texture = &textures::mini_coin;
  renderer::draw(mini_coin);

	//Mushroom:
  auto mushroom = renderer::Drawable{};
  mushroom.position = glm::vec2(center_x + 11.9f * font_size, step_y * 2 + level.stats.position_y - font_size * 0.1f);
  mushroom.size = glm::vec2(font_size * 1.f);
  mushroom.texture = &textures::mushroom;
  renderer::draw(mushroom);

  if (level.type == LevelState::Type::Boss && stats.boss_hp.current != nullptr){
    text.text = "BOSS";
    text.update();
    text.position = glm::vec2(WindowWidth / 2.f - text.get_size().x / 2.f, step_y * 4);
    text.update_position();

    renderer::print(text, glm::vec2(0));

    static constexpr auto MaxBossBarSize = glm::vec2(100.f * 6.f, 50.f);

    renderer::draw_plain(renderer::PlainDrawable{
      glm::vec2(WindowWidth / 2 - MaxBossBarSize.x / 2, step_y * 5),
      glm::vec2(*stats.boss_hp.current * MaxBossBarSize.x / stats.boss_hp.max, MaxBossBarSize.y),
      glm::vec4(1.f)
    });
  }
}

static auto render_loading_screen(const LevelState& level){
  const auto font_size = fonts::normal.size * 3.5f;

  //Background;
  auto background = renderer::Drawable{};

  background.position = { 0, 0 };
  background.size = config::FrameBufferSize;
  background.texture = &textures::black;

  renderer::draw(background);

  //Header:
  const auto& stats = level.stats;

  auto text = renderer::Text(
    &fonts::normal, 
    "WORLD " + std::to_string(stats.level_major) + "-" + std::to_string(stats.level_minor),
    font_size / fonts::normal.size
  );
  text.update();

  const auto center_pos = config::FrameBufferSize / 2.f - text.get_size() * 0.5f;

  text.position = center_pos - glm::vec2(0, font_size * 2.5f);
  text.update_position();

  renderer::print(text, glm::vec2(0));

  //Mario:
  auto mario = renderer::Drawable{};
  mario.position = glm::vec2(
    config::FrameBufferSize.x / 2 - BlockBase::Size * 1.5f, 
    center_pos.y - (BlockBase::Size - font_size * 1.f) 
  );
  mario.size = glm::vec2(BlockBase::Size);
  mario.texture = &textures::small_mario;

  renderer::draw(mario);

  //HP:
  text.text = "X " + std::to_string(stats.hp);
  text.position = glm::vec2(config::FrameBufferSize.x / 2, center_pos.y);
  text.update();

  renderer::print(text, glm::vec2(0));
}

static auto render_all_points_particles(const LevelState& level){
  level.game_objects.for_each_template<QBlockState>([&](const auto& block){
    using BlockType = std::decay_t<decltype(block)>;

    if constexpr (BlockType::PusherType::ContainsEntity){
      render_points_particles(block.pusher.entity.points_generator.items, level.camera_offset);
    }
    else if constexpr(!std::is_same_v<BlockType, QBlockState<FireFlowerPusherState>>){
      render_points_particles(block.pusher.points_generator.items, level.camera_offset);
    }
    else{
      render_points_particles(block.pusher.fire_flower.points_generator.items, level.camera_offset);
    }
  });

  level.game_objects.for_each_derived<MonsterState>([&](const auto& entity){
    render_points_particles(entity.points_generator.items, level.camera_offset);
  });

  level.game_objects.for_each_type<CannonState>([&](const auto& block){
		for (auto& bullet : block.bullet_generator.items){
			render_points_particles(bullet.points_generator.items, level.camera_offset);
		}
  });
}

static auto render_all_level_objects(const LevelState& level){
  renderer::draw_with_shadow([&]{
    level.game_objects.for_each_type<UpfireState>([&](auto& object){
      render_entity(object, level);
    });
  });

  renderer::draw_with_shadow([&]{
    level.game_objects.for_each([&](auto& object){
      render_background(object, level);
    });
  });

  renderer::draw_with_shadow([&]{
    level.game_objects.for_each([&](auto& object){
      render_plant(object, level);
    });
  });

  renderer::draw_with_shadow([&]{
    if (level.is_finished) render_player(level.player, level);
  });

  renderer::draw_with_shadow([&]{
    level.game_objects.for_each([&](auto& object){
      render_block(object, level);
    });
  });

  renderer::draw_with_shadow([&]{
    level.game_objects.for_each([&](auto& object){
			if constexpr (!std::is_convertible_v<decltype(object), UpfireState>){
				render_entity(object, level);
			}
    });
  });

  renderer::draw_with_shadow([&]{
    if (!level.is_finished) render_player(level.player, level);
  });
}

static auto render_level_background(const LevelState& level){
  auto background = renderer::Drawable{};

  background.position = { 0.f, 0.f };
  background.size = config::FrameBufferSize;
  background.texture = level.background_texture;

  if (level.is_level_in_castle()){
    background.alpha = level.background_opacity;
  }

  renderer::draw(background);
}

static auto render_darkness(const LevelState& level){
  static constexpr auto PlayerViewSizeInDarkness = 4.f * BlockBase::Size;
  const auto player_center = level.player.position + level.player.size / 2.f - level.camera_offset;

  const auto player_view_min = glm::vec2(
    player_center.x - PlayerViewSizeInDarkness,
    player_center.y - PlayerViewSizeInDarkness
  );

  const auto player_view_max = glm::vec2(
    player_center.x + PlayerViewSizeInDarkness,
    player_center.y + PlayerViewSizeInDarkness
  );

  auto darkness_view = renderer::Drawable{};
  darkness_view.position = player_view_min;
  darkness_view.size = glm::vec2(PlayerViewSizeInDarkness * 2.f);
  darkness_view.texture = &textures::darkness_view;
  renderer::draw(darkness_view);

  renderer::draw_plain(renderer::PlainDrawable{
    { 0, 0 },
    { config::FrameBufferSize.x, player_view_min.y },
    { 0.f, 0.f, 0.f, 1.f }
  });

  renderer::draw_plain(renderer::PlainDrawable{
    { 0, player_view_max.y },
    { config::FrameBufferSize.x, config::FrameBufferSize.y - player_view_max.y },
    { 0.f, 0.f, 0.f, 1.f }
  });

  renderer::draw_plain(renderer::PlainDrawable{
    { 0, player_view_min.y },
    { player_view_min.x, player_view_max.y - player_view_min.y },
    { 0.f, 0.f, 0.f, 1.f }
  });

  renderer::draw_plain(renderer::PlainDrawable{
    { player_view_max.x, player_view_min.y },
    { config::FrameBufferSize.x - player_view_max.x, player_view_max.y - player_view_min.y },
    { 0.f, 0.f, 0.f, 1.f }
  });

}

static auto render_level(const AppState& app){
	auto& level = app.current_level;

  render_level_background(level);
  render_all_level_objects(level);

  if (level.is_dark){
    render_darkness(level);
  }

  renderer::draw_with_shadow([&]{
    render_all_points_particles(level);  
  });

  render_water(level);

  if (level.load_delay > 0.f){
    render_loading_screen(level);
  }

  renderer::draw_with_shadow([&]{
    render_stats(level);
  });

	//Render final level textures:
	if (app.current_frame != AppState::Frame::Level76) return;

	//Peach:
	renderer::draw(renderer::Drawable{
		{},
		glm::vec2(41.f, 9.5f) * BlockBase::Size - level.camera_offset,
		glm::vec2(1.f, 1.5f) * BlockBase::Size,
		&textures::peach
	});

	if (level.player.position.x < 39.f * BlockBase::Size) return;

	renderer::draw_with_shadow([&]{
		auto text = renderer::Text(&fonts::normal, "Thank you mario!", 4.f);
		text.update();
		text.position = glm::vec2(40.f, 4.f) * BlockBase::Size - glm::vec2(text.get_text_width() / 2.f, 0.f) - level.camera_offset;
		text.update_position();

		renderer::print(text, glm::vec2(0));

		if (app.game_finish_timer < 2.f) return;

		text.text = "YOUR QUEST IS OVER";
		text.update();
		text.position = glm::vec2(40.f, 5.5f) * BlockBase::Size - glm::vec2(text.get_text_width() / 2.f, 0.f) - level.camera_offset;
		text.update_position();

		renderer::print(text, glm::vec2(0));

		if (app.game_finish_timer < 4.f) return;

		text.text = "THE END";
		text.font_scale = 6.f;
		text.update();
		text.position = glm::vec2(40.f, 7.f) * BlockBase::Size - glm::vec2(text.get_text_width() / 2.f, 0.f) - level.camera_offset;
		text.update_position();

		renderer::print(text, glm::vec2(0));
	});

}

} //namespace mario::views
