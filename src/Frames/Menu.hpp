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
		texture_groups::blue_cloud,
    &textures::mini_coin,
		&textures::mushroom,
		&textures::mushroom_bg,
		&textures::snow_bg,
		&textures::bg,
		&textures::underground_bg,
		&textures::sky_bg,
		&textures::water_top,
		&textures::water_bottom
  };

	auto& level = app.current_level;
	level = {};
	const auto setup = [&](auto& app){
    fonts::normal.allocate(16);
    LevelState::timer = 0.f;

		level.initialise_hitbox_grid();

		//Boss-level size needed
		level.type = LevelState::Type::Boss;
		level.load_delay = 0.f;
		level.player.position = glm::vec2(1.f, 10.f) * BlockBase::Size;
		level.player.is_controllable = false;

		const auto worlds_with_clouds = std::vector{ 1, 2, 3, 6 };
		auto menu_number = util::random_value(1, 6);

    level.background_texture = [&]{
			switch(menu_number){
				default: return &textures::bg;
				case 1: return &textures::mushroom_bg;
				case 3: return &textures::snow_bg;
				case 5: return &textures::underground_bg;
				case 6: return &textures::sky_bg;
			}
		}();

    level.cloud_textures = [&]{
			switch(menu_number){
				default: return &texture_groups::blue_cloud;
				case 1: return &texture_groups::red_cloud;
			}
		}();

		if (std::find(worlds_with_clouds.begin(), worlds_with_clouds.end(), menu_number) != worlds_with_clouds.end()){
			level_generator::generate_horizontal_level_clouds(level);
		}

    level_generator::generate_level(level, "menu" + std::to_string(menu_number) + "_1.csv");
    level_generator::generate_level(level, "menu" + std::to_string(menu_number) + "_2.csv");

		if (menu_number == 4){
			level.water_level = 0.f;

			level.game_objects.for_each_type<FishState>([](auto& fish){
				fish.walk_speed = 0.f;
				fish.set_direction(fish.direction);
			});
		}

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
