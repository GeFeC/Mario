#pragma once

#include "States/AppState.hpp"
#include "States/MenuState.hpp"
#include "Window.hpp"
#include "Saves.hpp"
#include "Util/Enum.hpp"

namespace mario::menu_controller{

static auto run_player_controller(PlayerState& player){
	player.position.y = -2 * BlockBase::Size;
}

static auto run_input_controller(AppState& app){
	static auto down_key_cooldown = false;
	auto& menu = app.menu;
	const auto is_last_option = 
		(menu.current_option | util::as<int>) == menu.options.size() - 1;

	//Switching between menu options:
	if (window::is_key_pressed(GLFW_KEY_DOWN) && !is_last_option && !down_key_cooldown){
		menu.current_option = util::enum_add(menu.current_option, 1.f);
		down_key_cooldown = true;
	}
	if (!window::is_key_pressed(GLFW_KEY_DOWN)){
		down_key_cooldown = false;
	}

	static auto up_key_cooldown = false;
	const auto is_first_option = (menu.current_option | util::as<int>) == 0;

	if (window::is_key_pressed(GLFW_KEY_UP) && !is_first_option && !up_key_cooldown){
		menu.current_option = util::enum_add(menu.current_option, -1.f);
		up_key_cooldown = true;
	}
	if (!window::is_key_pressed(GLFW_KEY_UP)){
		up_key_cooldown = false;
	}
}

static auto run_main_menu_controller(AppState& app){
	auto& menu = app.menu;
	if (menu.current_option == MenuState::Option::NewGame){
		menu.state = MenuState::State::NewGame;
		menu.current_option = MenuState::Option::Easy;
		menu.options = menu.new_game_options();
	}

	if (menu.current_option == MenuState::Option::Continue){
		const auto save_state = saves::load();
		app.difficulty = save_state.difficulty;

		auto level_index = (save_state.world * config::LevelsInWorld);

		if (save_state.difficulty == AppState::Difficulty::Easy){
			level_index += save_state.level;
		}

		app.current_frame = level_index | util::as<AppState::Frame>;
	}

	if (menu.current_option == MenuState::Option::Exit){
		app.should_exit = true;
	}
}

static auto run_new_game_menu_controller(AppState& app){
	auto& menu = app.menu;

	if (menu.current_option == MenuState::Option::Back){
		menu.state = MenuState::State::Main;
		menu.options = menu.main_options();
		menu.current_option = MenuState::Option::NewGame;

		return;
	}

	switch(menu.current_option){
		case MenuState::Option::Easy:
			app.difficulty = AppState::Difficulty::Easy;
			break;
		case MenuState::Option::Normal:
			app.difficulty = AppState::Difficulty::Normal;
			break;
		case MenuState::Option::Hard:
			app.difficulty = AppState::Difficulty::Hard;
			break;

		default:;
	}

	app.current_frame = AppState::Frame::Level11;
}

static auto option_selected(){
	const auto enter_key_pressed = window::is_key_pressed(GLFW_KEY_ENTER);
	static auto enter_key_cooldown = false;

	if (enter_key_pressed && !enter_key_cooldown){
		enter_key_cooldown = true;
		return true;
	}

	if (!enter_key_pressed) enter_key_cooldown = false;
	return false;
};

static auto run(AppState& app){
	run_player_controller(app.current_level.player);
	run_input_controller(app);

	if (!option_selected()) return;

	switch(app.menu.state){
		case MenuState::State::Main:
			run_main_menu_controller(app);
			break;

		case MenuState::State::NewGame:
			run_new_game_menu_controller(app);
			break;

		default:;
	}
}

} //namespace mario::menu_controller
