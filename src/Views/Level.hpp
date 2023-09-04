#pragma once

#include "States/LevelState.hpp"
#include "Renderer/Renderer.hpp"
#include "Controllers/LevelController.hpp"

#include "Views/Components.hpp"

#include "Util/Util.hpp"
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

  const auto water_area = util::make_pair_from_vec2(level.max_size());

  util::for_2d([&](int x, int y){
    auto block = BlockBase{};
    block.position = { x * BlockBase::Size, y * BlockBase::Size };
    block.size = glm::vec2(BlockBase::Size);
    block.texture = &textures::water_bottom;
    block.alpha = WaterTransparency;

    render_block(block, level);
  }, std::make_pair(0.f, level.water_level + 1.f), water_area);
}

static auto render_stats(const LevelState& level){
  const auto& stats = level.stats;

  auto body = std::ostringstream(); 
  body 
    << std::setw(6) << std::setfill('0') << stats.score << "    x"
    << std::setw(2) << std::setfill('0') << stats.coins << "    "
    << stats.level_major << '-' << stats.level_minor << "     "
    << std::setw(3) << std::setfill('0') << stats.time;

  const auto font_size = fonts::normal.size * 3.5f;
  static constexpr auto WindowWidth = config::FrameBufferSize.x;

  auto text = renderer::Text(&fonts::normal, "MARIO           WORLD   TIME", font_size / fonts::normal.size);
  text.update();
  const auto center_x = WindowWidth / 2 - text.get_size().x / 2;
  const auto step_y = 2.f / 3.f * BlockBase::Size;

  text.position = { center_x, step_y };
  text.update_position();
  renderer::print(text, glm::vec2(0));

  text.text = body.str();
  text.position += glm::vec2(0.f, step_y);
  text.update();
  renderer::print(text, glm::vec2(0));

  //Mini coin
  renderer::draw(renderer::Drawable{
    glm::vec2(center_x + 9 * font_size, step_y * 2),
    glm::vec2(font_size),
    &textures::mini_coin
  });

  if (level.type == LevelState::Type::Boss && stats.boss_hp != nullptr){
    text.text = "BOSS";
    text.update();
    text.position = glm::vec2(WindowWidth / 2.f - text.get_size().x / 2.f, step_y * 4);
    text.update_position();

    renderer::print(text, glm::vec2(0));

    static constexpr auto MaxBossBarSize = glm::vec2(100.f * 6.f, 50.f);

    renderer::draw_plain(renderer::PlainDrawable{
      glm::vec2(WindowWidth / 2 - MaxBossBarSize.x / 2, step_y * 5),
      glm::vec2(*stats.boss_hp * MaxBossBarSize.x / stats.max_boss_hp, MaxBossBarSize.y),
      glm::vec4(1.f)
    });
  }
}

static auto render_loading_screen(const LevelState& level){
  const auto font_size = fonts::normal.size * 3.5f;

  //Background;
  renderer::draw(renderer::Drawable{
    glm::vec2(0, 0),
    config::FrameBufferSize,
    &textures::black

  });

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
  renderer::draw(renderer::Drawable{
    glm::vec2(
      config::FrameBufferSize.x / 2 - BlockBase::Size * 1.5f, 
      center_pos.y - (BlockBase::Size - font_size * 1.f) 
    ),
    glm::vec2(BlockBase::Size),
    &textures::small_mario
  });

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

  level.game_objects.for_each_derived<EntityState>([&](const auto& entity){
    render_points_particles(entity.points_generator.items, level.camera_offset);
  });
}

static auto render_level(const LevelState& level){
  renderer::draw(renderer::Drawable{
    glm::vec2(0, 0),
    config::FrameBufferSize,
    level.background_texture
  });

  //Rendering game objects

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
      render_entity(object, level);
    });
  });

  renderer::draw_with_shadow([&]{
    if (!level.is_finished) render_player(level.player, level);
  });

  render_water(level);

  //Darkness

  if (level.is_dark){

    static constexpr auto PlayerViewSizeInDarkness = 3.f * BlockBase::Size;
    const auto player_center = level.player.position + level.player.size / 2.f - level.camera_offset;

    const auto player_view_min = glm::vec2(
      player_center.x - PlayerViewSizeInDarkness,
      player_center.y - PlayerViewSizeInDarkness
    );

    const auto player_view_max = glm::vec2(
      player_center.x + PlayerViewSizeInDarkness,
      player_center.y + PlayerViewSizeInDarkness
    );

    renderer::draw(renderer::Drawable{
      player_view_min,
      glm::vec2(PlayerViewSizeInDarkness * 2.f),
      &textures::darkness_view
    });

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

  //Stats and loading screen

  renderer::draw_with_shadow([&]{
    if (level.load_delay > 0.f){
      render_loading_screen(level);
    }

    render_stats(level);
  });

  //Points particles
  renderer::draw_with_shadow([&]{
    render_all_points_particles(level);  
  });

}

} //namespace mario::views
