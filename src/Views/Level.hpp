#pragma once

#include "States/LevelState.hpp"
#include "Renderer/Renderer.hpp"

#include "Views/Components.hpp"

#include "Util.hpp"
#include "config.hpp"
#include "res/fonts.hpp"
#include "res/textures.hpp"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

static auto render_clouds(float screen_scroll){
  auto clouds = std::vector<std::pair<glm::vec2, int>>();
  clouds.reserve(32);

  for (int i = 0; i < 8; ++i){
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 1, 2), 3));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 7, 1), 2));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 12, 2), 1));
    clouds.push_back(std::make_pair(glm::vec2(i * 18 + 15, 1), 1));
  }

  static auto cloud_offset = 0.f;
  cloud_offset += window::delta_time;
  if (cloud_offset > 18.f) cloud_offset -= 18.f;

  for (const auto [position, cloud_size] : clouds){
    const auto x = position.x - cloud_offset;
    const auto y = position.y;

    render_block(BlockState({ x * 60, y * 60 }, &textures::red_cloud_top_left), screen_scroll);
    render_block(BlockState({ x * 60, (y + 1) * 60 }, &textures::red_cloud_bottom_left), screen_scroll);

    for (int i = 0; i < cloud_size; ++i){
      render_block(BlockState({ (x + i + 1) * 60, y * 60 }, &textures::red_cloud_top_center), screen_scroll);
      render_block(BlockState({ (x + i + 1) * 60, (y + 1) * 60 }, &textures::red_cloud_bottom_center), screen_scroll);
    }

    render_block(BlockState( { (x + cloud_size + 1) * 60, y * 60 }, &textures::red_cloud_top_right ), screen_scroll);
    render_block(BlockState( { (x + cloud_size + 1) * 60, (y + 1) * 60 }, &textures::red_cloud_bottom_right ), screen_scroll);
  }
}

static auto render_stats(const StatsState& stats){
  auto body = std::ostringstream(); 
  body 
    << std::setw(6) << std::setfill('0') << stats.score << "    x"
    << std::setw(2) << std::setfill('0') << stats.coins << "    "
    << stats.level_major << '-' << stats.level_major << "     "
    << std::setw(3) << std::setfill('0') << stats.time;

  auto text = Text(&fonts::medium, "MARIO           WORLD   TIME");
  text.set_position(glm::vec2(154, 40));
  renderer::print(text);

  text.set_text(body.str());
  text.set_position(text.get_position() + glm::vec2(0.f, 40));
  renderer::print(text);

  renderer::draw(Drawable{
    glm::vec2(440, 80),
    glm::vec2(32),
    &textures::mini_coin
  });
}

static auto render_entities(const LevelState& level_state, float screen_scroll){
  for (const auto& goomba : level_state.goombas){
    render_entity(goomba, screen_scroll);
  }
}

static auto render_loading_screen(const LevelState& level_state){
  //Background;
  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::black
  });

  //Header:

  const auto window_width = config::InitialWindowWidth;

  const auto& stats = level_state.stats_state;
  auto text = Text(&fonts::medium, "WORLD " + std::to_string(stats.level_major) + "-" + std::to_string(stats.level_minor));
  text.set_position(glm::vec2(460, 250));

  renderer::print(text);

  //Mario:
  renderer::draw(Drawable{
    glm::vec2(window_width / 2 - 90, 320),
    glm::vec2(60),
    &textures::small_mario
  });

  //HP:
  text.set_text("X " + std::to_string(stats.hp));
  text.set_position(text.get_position() + glm::vec2(140, 100));

  renderer::print(text);
}

static auto render_level(const LevelState& level_state){
  auto screen_scroll = 0.f;
  if (level_state.should_screen_scroll){
    screen_scroll = level_state.player_state.position.x - config::PlayerPositionToScroll;
  }

  renderer::draw(Drawable{
    glm::vec2(0, 0),
    glm::vec2(config::InitialWindowWidth, config::InitialWindowHeight),
    &textures::mushroom_bg
  });

  renderer::draw_with_shadow([&]{
    render_clouds(screen_scroll);
  });

  renderer::draw_with_shadow([&]{
    for (const auto& block : level_state.blocks){
      render_block(block, screen_scroll);
    }
  });

  renderer::draw_with_shadow([&]{
    render_entities(level_state, screen_scroll);
    render_entity(level_state.player_state, screen_scroll);

    if (level_state.load_delay > 0.f){
      render_loading_screen(level_state);
    }
    render_stats(level_state.stats_state);

    for (const auto& particle : level_state.points_particles){
      renderer::print(particle.text, screen_scroll);
    }
  });
}
