#pragma once

#include "States/AppState.hpp"
#include "res/textureGroups.hpp"
#include "LevelGenerator/Textures.hpp"
#include "Views/Level.hpp"
#include "Views/Menu.hpp"
#include "Frame.hpp"
#include "Controllers/LevelController.hpp"
#include "Controllers/MenuController.hpp"

namespace mario::frames{

static auto run_menu(AppState& app){
  auto textures = std::vector<renderer::TextureGroup>{
    texture_groups::mario,
		texture_groups::red_cloud,
    &textures::mini_coin,
		&textures::mushroom,
		&textures::mushroom_bg
  };

	auto& level = app.current_level;
	const auto setup = [&](auto& app){
    fonts::normal.allocate(16);
    LevelState::timer = 0.f;

		level.initialise_hitbox_grid();

    level.background_texture = &textures::mushroom_bg;
    level.cloud_textures = &texture_groups::red_cloud;
		level.load_delay = 0.f;
		level.player.position = glm::vec2(1.f, 10.f) * BlockBase::Size;
		level.player.is_controllable = false;
    
    level_generator::generate_horizontal_level_clouds(level);
    level_generator::generate_level(level, "level11_1.csv");
    level_generator::generate_level(level, "level11_2.csv");
    level_generator::generate_level(level, "level11_3.csv");

    level_generator::put_q_block_with_coins(level, { 77, 7 }, 5);
    level_generator::put_q_block_with_coins(level, { 187, 3 }, 5);
    level_generator::put_q_block_with_coins(level, { 189, 3 }, 5);

    for (const auto texture : level_generator::allocated_textures){
      textures.push_back(level_generator::id_to_texture.at(texture));
    }
	};

	const auto loop = [&](auto& app){
		level_controller::run(app);

		menu_controller::run(app);
		views::render_level(app);
		views::render_menu(app);
	};

	run_frame(app, AppState::Frame::Menu, textures, setup, loop);
}

} //namespace mario
