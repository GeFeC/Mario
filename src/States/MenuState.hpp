#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace mario{

struct MenuStringState{
	std::string text;
	glm::vec3 color;
	float font_size = 4.f;

	explicit MenuStringState(const std::string& text, const glm::vec3& color = glm::vec3(1.f)) 
		: text(text), color(color) {}
};

struct MenuState{
	static constexpr auto MainMenuSize = glm::vec2(7.5f, 5.5f) * 95.f;
	static constexpr auto MainMenuPositionY = 470.f;
	static constexpr auto ControlNameMaxSize = 10;
	static constexpr auto ControlTextSize = 20;

	using Options = std::vector<MenuStringState>;

	Options options;
	glm::vec2 size;
	float position_y;
	bool controls_change_mode = false;

	enum class Option{
		First = 0,
		NewGame = 0,
		Continue = 1,
		Controls = 2,
		Exit = 3,

		Easy = 0,
		Normal = 1,
		Hard = 2,
		Back = 3,
	} current_option = Option::NewGame;

	enum class State{
		Main,
		NewGame,
		Controls
	} state = MenuState::State::Main;

	MenuState() {
		options = main_options();
		size = MainMenuSize;
		position_y = MainMenuPositionY;
	}

	auto main_options() const -> Options{
		return {
			MenuStringState("NEW GAME"),
			MenuStringState("CONTINUE"),
			MenuStringState("CONTROLS"),
			MenuStringState("EXIT")
		};
	}

	auto new_game_options() const -> Options{
		return {
			MenuStringState("EASY", glm::vec3(0.f, 1.f, 0.f)),
			MenuStringState("NORMAL", glm::vec3(1.f, 1.f, 0.f)),
			MenuStringState("HARD", glm::vec3(1.f, 0.f, 0.f)),
			MenuStringState("BACK")
		};
	}
};

} //namespace mario
