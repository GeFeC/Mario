#pragma once

#include "Window.hpp"
#include <string>
#include <unordered_map>
#include <array>

namespace mario::input{

inline auto key_names = std::unordered_map<int, std::string>{
	{ GLFW_KEY_UP, "UP" },
	{ GLFW_KEY_DOWN, "DOWN" },
	{ GLFW_KEY_LEFT, "LEFT" },
	{ GLFW_KEY_RIGHT, "RIGHT" },
	{ GLFW_KEY_ESCAPE, "ESC" },
	{ GLFW_KEY_TAB, "TAB" },
	{ GLFW_KEY_CAPS_LOCK, "CAPSLOCK" },
	{ GLFW_KEY_LEFT_SHIFT, "LSHIFT" },
	{ GLFW_KEY_LEFT_CONTROL, "LCONTROL" },
	{ GLFW_KEY_LEFT_ALT, "LEFT ALT" },
	{ GLFW_KEY_RIGHT_SHIFT, "RSHIFT" },
	{ GLFW_KEY_RIGHT_CONTROL, "RCONTROL" },
	{ GLFW_KEY_RIGHT_ALT, "RIGHT ALT" },
	{ GLFW_KEY_ENTER, "ENTER" },
	{ GLFW_KEY_BACKSLASH, "BACKSLASH" },
	{ GLFW_KEY_BACKSPACE, "BACKSPACE" },
	{ GLFW_KEY_SPACE, "SPACE" },
	{ GLFW_KEY_INSERT, "INSERT" },
	{ GLFW_KEY_HOME, "HOME" },
	{ GLFW_KEY_DELETE, "DELETE" },
	{ GLFW_KEY_END, "END" },
	{ GLFW_KEY_PAGE_DOWN, "PG DOWN" },
	{ GLFW_KEY_PAGE_UP, "PG UP" },
	{ GLFW_KEY_COMMA, "COMMA" },
	{ GLFW_KEY_PERIOD, "PERIOD" },
	{ GLFW_KEY_SLASH, "SLASH" },
	{ GLFW_KEY_SEMICOLON, "SEMICOLON" },
	{ GLFW_KEY_APOSTROPHE, "APOSTR" },
	{ GLFW_KEY_EQUAL, "EQUAL" },
	{ GLFW_KEY_MINUS, "MINUS" }
};

struct Key{
	int code = 0;
	bool clickable = true;

	constexpr Key(int code) : code(code) {}

	auto is_down() const{
		return window::is_key_pressed(code);
	}

	auto clicked(){
		if (is_down() && clickable) {
			clickable = false;

			return true;
		}

		if (!is_down()) clickable = true;

		return false;
	}
};

inline auto key_up = Key(GLFW_KEY_UP);
inline auto key_down = Key(GLFW_KEY_DOWN);
inline auto key_enter = Key(GLFW_KEY_ENTER);
inline auto key_escape = Key(GLFW_KEY_ESCAPE);

enum ControlName{
	Jump,
	Squat,
	GoLeft,
	GoRight,
	Sprint,
	Shoot,
	UseMushroom,
	ItemsCount
};

static auto controls = std::array{
	Key(GLFW_KEY_UP),
	Key(GLFW_KEY_DOWN),
	Key(GLFW_KEY_LEFT),
	Key(GLFW_KEY_RIGHT),
	Key(GLFW_KEY_LEFT_SHIFT),
	Key(GLFW_KEY_Z),
	Key(GLFW_KEY_SPACE)
};

static auto init(){
	for (int i = 0; i < 10; ++i){
		key_names.insert({ GLFW_KEY_0 + i, std::to_string(i) });
	}

	for (int i = GLFW_KEY_A; i <= GLFW_KEY_Z; ++i){
		key_names.insert({ i, std::string(1, i - GLFW_KEY_A + 'A') });
	}
}

} //namespace mario::input
