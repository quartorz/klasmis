#pragma once

#include "main_window.hpp"

#include <quote/direct2d/scene.hpp>
#include <quote/direct2d/callback_flat_button.hpp>

#include "main_window.hpp"

class title_scene : public quote::direct2d::scene<> {
	main_window &w_;

	struct button : quote::direct2d::callback_flat_button {
		main_window &w_;

	public:
		button(main_window &w)
			: w_(w)
		{
			set_text_color(button::state::none, { 190, 190, 190 });
			set_text_color(button::state::hover, { 220, 220, 220 });
			set_text_color(button::state::push, { 180, 180, 180 });
			set_color(button::state::none, { 230, 230, 230 });
			set_color(button::state::hover, { 250, 250, 250 });
			set_color(button::state::push, { 210, 210, 210 });
		}

		void set_state(state s) override
		{
			quote::direct2d::callback_flat_button::set_state(s);
			w_.repaint();
		}
	}mode_buttons_[2];

public:
	title_scene(main_window &w)
		: w_(w)
		, mode_buttons_{ {w}, {w} }
	{

		this->register_object(&mode_buttons_[0]);
		mode_buttons_[0].set_text(L"Mode 1");
		mode_buttons_[0].set_position({ 175, 350 });
		mode_buttons_[0].set_size({ 200, 50 });
		mode_buttons_[0].set_callback([&]() {
			w_.select_game(0);
		});

		this->register_object(&mode_buttons_[1]);
		mode_buttons_[1].set_text(L"Mode 2");
		mode_buttons_[1].set_position({ 175, 450 });
		mode_buttons_[1].set_size({ 200, 50 });
		mode_buttons_[1].set_callback([&]() {
			w_.select_game(1);
		});

		this->add_timer_handler([&](unsigned id) {
			w_.repaint();
		}, 0);
	}

	~title_scene()
	{
		this->unregister_object(&mode_buttons_[0]);
		this->unregister_object(&mode_buttons_[1]);
	}
};
