#pragma once

#include "States/AppState.hpp"
#include "States/MenuState.hpp"
#include "Saves.hpp"
#include "Input.hpp"
#include "Util/Enum.hpp"

namespace mario::menu_controller{

static auto run_player_controller(PlayerState& player){
	player.position.y = -2 * BlockBase::Size;
}

static auto run_input_controller(AppState& app){
	auto& menu = app.menu;
	const auto is_last_option = 
		(menu.current_option | util::as<int>) == menu.options.size() - 1;

	if (input::key_down.clicked() && !is_last_option){
		menu.current_option = util::enum_add(menu.current_option, 1.f);
	}

	const auto is_first_option = (menu.current_option | util::as<int>) == 0;

	if (input::key_up.clicked() && !is_first_option){
		menu.current_option = util::enum_add(menu.current_option, -1.f);
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

	if (menu.current_option == MenuState::Option::Controls){
		menu.state = MenuState::State::Controls;
		menu.current_option = MenuState::Option::First;
		menu.size = glm::vec2(1100.f, 900.f);
		menu.position_y = 100.f;

		auto& options = menu.options;
		options = {
			MenuStringState("JUMP"),
			MenuStringState("SQUAT"),
			MenuStringState("LEFT"),
			MenuStringState("RIGHT"),
			MenuStringState("SPRINT"),
			MenuStringState("SHOOT"),
			MenuStringState("MUSHROOM"),
		};

		for (auto& option : options){
			option.font_size = 3.f;
			option.text.resize(MenuState::ControlNameMaxSize, ' ');
		}

		for (int i = 0; i < 7; ++i){
			options[i].text += input::key_names.at(input::controls[i].code);
		}

		for (auto& option : options){
			option.text.resize(MenuState::ControlTextSize, ' ');
		}

		options.push_back(MenuStringState("BACK"));

		options.shrink_to_fit();
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

static auto run_controls_menu_controller(AppState& app){
	auto& menu = app.menu;
	auto current_option_index = menu.current_option | util::as<int>;

	const auto option_back = menu.options.size() - 1;
	if (current_option_index == option_back){
		menu.state = MenuState::State::Main;
		menu.options = menu.main_options();
		menu.current_option = MenuState::Option::NewGame;

		menu.size = MenuState::MainMenuSize;
		menu.position_y = MenuState::MainMenuPositionY;

		return;
	}

	menu.controls_change_mode = true;
	window::last_pressed_key = -1;
	menu.options[current_option_index].color.b = 0.5f;
}

static auto update_controls(AppState& app){
	auto& menu = app.menu;
	auto current_option_index = menu.current_option | util::as<int>;
	auto& current_option = menu.options[current_option_index];

	current_option.text.resize(MenuState::ControlNameMaxSize, ' ');

	//Ignore first key detection, because this key is an enter from previous controls selection
	static auto enter_pressed = false;
	if (window::last_pressed_key == GLFW_KEY_ENTER && !enter_pressed){
		enter_pressed = true;
		window::last_pressed_key = -1;
	}

	if (window::last_pressed_key != -1 && enter_pressed){
		if (input::key_names.find(window::last_pressed_key) == input::key_names.end()){
			current_option.text.resize(MenuState::ControlTextSize, ' ');

			return;
		}

		if (window::last_pressed_key == GLFW_KEY_DOWN){
			input::key_down.clickable = false;
		}

		menu.controls_change_mode = false;
		current_option.text += input::key_names.at(window::last_pressed_key);
		current_option.color = glm::vec3(1.f);

		input::controls[current_option_index].code = window::last_pressed_key;
		enter_pressed = false;

		auto state = saves::load();
		std::transform(input::controls.begin(), input::controls.end(), state.controls.begin(), [](const input::Key& key){
			return key.code;
		});
		saves::save(state);
	}

	current_option.text.resize(MenuState::ControlTextSize, ' ');
}

static auto run(AppState& app){
	run_player_controller(app.current_level.player);

	if (app.menu.controls_change_mode){
		update_controls(app);
		return;
	}
	else{
		run_input_controller(app);
	}

	if (!input::key_enter.clicked()) return;

	switch(app.menu.state){
		case MenuState::State::Main:
			run_main_menu_controller(app);
			break;

		case MenuState::State::NewGame:
			run_new_game_menu_controller(app);
			break;

		case MenuState::State::Controls:
			run_controls_menu_controller(app);
			break;

		default:;
	}
}

} //namespace mario::menu_controller
