#pragma once

#include "States/AppState.hpp"
#include "Renderer/Renderer.hpp"

namespace mario::views{

static auto render_menu_frame(const glm::vec2& position, const glm::vec2& size){
	static constexpr auto Thickness = 6.f;
	static constexpr auto Padding = Thickness * 2.f;

	const auto horizontal_line_size = glm::vec2(size.x - Padding * 2.f, Thickness);
	//Upper:
	renderer::draw_plain(renderer::PlainDrawable{
		position + Padding,
		horizontal_line_size,
		glm::vec4(1.f)
	});

	//Lower:
	renderer::draw_plain(renderer::PlainDrawable{
		position + Padding + glm::vec2(0.f, size.y - Padding * 2.f - Thickness),
		horizontal_line_size,
		glm::vec4(1.f)
	});

	const auto vertical_line_size = glm::vec2(Thickness, size.y - Padding * 2.f);
	//Left:
	renderer::draw_plain(renderer::PlainDrawable{
		position + Padding,
		vertical_line_size,
		glm::vec4(1.f)
	});

	//Right:
	renderer::draw_plain(renderer::PlainDrawable{
		position + Padding + glm::vec2(size.x - Padding * 2.f - Thickness, 0.f),
		vertical_line_size,
		glm::vec4(1.f)
	});
}

static auto make_menu_background(const glm::vec2& position, const glm::vec2& size){
	auto background = renderer::PlainDrawable{};
	background.position = position;
	background.size = size;
	background.color = glm::vec4(0.f, 0.f, 0.f, 0.9f);

	return background;
}

static auto render_menu_title(const AppState& app){
	auto text = renderer::Text(&fonts::normal, "SUPER MARIO", 6.f);
	text.update();

	auto title_background = make_menu_background(
		glm::vec2(BlockBase::Size),
		glm::vec2(
			text.get_text_width() + BlockBase::Size * 2.f,
			250.f + BlockBase::Size
		)
	);

	title_background.position.x = config::FrameBufferSize.x / 2.f - title_background.size.x / 2.f;

	text.position = title_background.position + BlockBase::Size;
	text.update_position();

	renderer::draw_plain(title_background);

	renderer::draw_with_shadow([&]{
		renderer::print(text, glm::vec2(0.f));
	});

	const auto prev_text_width = text.get_text_width();

	text.text = "BOSS WORLDS";
	text.position.y += BlockBase::Size * 0.9f;
	text.font_scale = 4.f;
	text.update();

	text.position.x += prev_text_width / 2.f - text.get_text_width() / 2.f;
	text.update_position();

	renderer::draw_with_shadow([&]{
		renderer::print(text, glm::vec2(0.f));
	});

	renderer::draw_with_shadow([&]{
		render_menu_frame(title_background.position, title_background.size);
	});
}

static auto render_menu_options(const MenuState& menu, const glm::vec2& background_size){
	const auto background_position = glm::vec2(
		config::FrameBufferSize.x / 2.f - background_size.x / 2.f,
		BlockBase::Size * 2.3f + 250.f
	);

	renderer::draw_plain(make_menu_background(background_position, background_size));

	renderer::draw_with_shadow([&]{
		render_menu_frame(background_position, background_size);
	});

	for (int i = 0; i < menu.options.size(); ++i){
		auto& option = menu.options[i];

		auto text = renderer::Text(&fonts::normal, option.text, option.font_size);
		text.color = glm::vec4(option.color, 0.5f);

		const auto current_option = menu.current_option | util::as<int>;
		if (current_option == i){
			text.color.a = 1.f;
		}

		text.update();

		text.position = background_position + glm::vec2(
			background_size.x / 2.f - text.get_text_width() / 2.f,
			BlockBase::Size * (i + 0.9)
		);

		text.update_position();

		renderer::draw_with_shadow([&]{
			renderer::print(text, glm::vec2(0.f));
		});
	}
}

static auto render_menu(const AppState& app){
	render_menu_title(app);

	render_menu_options(app.menu, glm::vec2(7.5f, 5.5f) * BlockBase::Size);
}

} //namespace mario::views
