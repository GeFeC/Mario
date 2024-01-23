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
	using Options = std::vector<MenuStringState>;
	Options options;

	enum class Option{
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
		Continue,
		Exit
	} state = MenuState::State::Main;

	MenuState() {
		options = main_options();
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
