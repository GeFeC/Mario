#pragma once

#include "States/AppState.hpp"
#include "Renderer/Renderer.hpp"

#include <sstream>
#include <iomanip>

namespace mario::views{

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

static auto render_loading_screen(const LevelState& level, const AppState::Difficulty& difficulty){
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
    config::FrameBufferSize.x / 2 - BlockBase::Size * 1.6f, 
    center_pos.y - (BlockBase::Size - font_size * 1.f) 
  );
  mario.size = glm::vec2(BlockBase::Size);
  mario.texture = &textures::small_mario;

  renderer::draw(mario);

  //HP:
  text.text = "X " + std::to_string(stats.hp);
  text.position = glm::vec2(config::FrameBufferSize.x / 2 - BlockBase::Size * 0.2f, center_pos.y);
  text.update();

  renderer::print(text, glm::vec2(0));

	//Difficulty:
	using Difficulty = AppState::Difficulty;
	switch(difficulty){
		case Difficulty::Easy:
			text.text = "EASY";
			text.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
			break;

		case Difficulty::Normal:
			text.text = "NORMAL";
			text.color = glm::vec4(1.f, 1.f, 0.f, 1.f);
			break;

		case Difficulty::Hard:
			text.text = "HARD";
			text.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			break;
	}

	text.update();

	text.position.x = config::FrameBufferSize.x / 2.f - text.get_text_width() / 2.f;
	text.position.y += BlockBase::Size * 1.2f;

	text.update_position();

	renderer::print(text, glm::vec2(0));
}

} //namespace mario::views
