#pragma once

#include <random>

#include <quote/direct2d/scene.hpp>
#include <quote/direct2d/base_types.hpp>
#include <quote/direct2d/solid_brush.hpp>
#include <quote/direct2d/text.hpp>

#include "tetrimino.hpp"
#include "klasmimino.hpp"
#include "field.hpp"

#include "main_window.hpp"

class game_scene : public quote::direct2d::scene<> 
{
public:
	enum class mode {
		user_changeable,
		random,
	};

private:
	main_window &w_;

	std::mt19937_64 random_{ std::random_device{}() };
	std::uniform_int_distribution<> shape_random_{ 0, static_cast<int>(tetrimino::shape::max) - 1 };
	std::uniform_int_distribution<> change_shape_random_{ 0, 99 };
	std::uniform_int_distribution<> size_random_{ 0, 99 };

	mode mode_ = mode::random;

	bool game_over_ = false;

	field field_;
	klasmimino current_klasmimino_;
	field::coord_type current_coord_;
	klasmimino next_klasmimino_;
	unsigned score_;

	quote::direct2d::solid_brush brush_;
	quote::direct2d::text score_text_;

	const quote::direct2d::color bg_colors_[4] = {
		{ 0.97f, 0.97f, 0.97f },
		{ 0.95f, 0.95f, 0.95f },
		{ 0.90f, 0.90f, 0.90f },
		{ 0.92f, 0.92f, 0.92f },
	};
	const quote::direct2d::color klasmimino_colors_[3] = {
		{ 170, 170, 170 },
		{ 190, 190, 190 },
		{ 211, 211, 211 },
	};

	const float big_cell_size_ = 24.0f;

	void rotate_klasmimino_size(bool increase)
	{
		auto old_size = current_klasmimino_.get_size();
		auto s = old_size;

		if (increase) {
			switch (s) {
			case klasmimino::size::s:
				s = klasmimino::size::m;
				break;
			case klasmimino::size::m:
				s = klasmimino::size::l;
				break;
			case klasmimino::size::l:
				s = klasmimino::size::s;
				break;
			}
		} else {
			switch (s) {
			case klasmimino::size::s:
				s = klasmimino::size::l;
				break;
			case klasmimino::size::m:
				s = klasmimino::size::s;
				break;
			case klasmimino::size::l:
				s = klasmimino::size::m;
				break;
			}
		}

		current_klasmimino_.set_size(s);

		auto shape = current_klasmimino_.get_shape();
		int diff = static_cast<int>(s) - static_cast<int>(old_size);

		if (shape == tetrimino::shape::I || shape == tetrimino::shape::O) {
			std::get<0>(current_coord_) -= 2 * diff;
			std::get<1>(current_coord_) -= 2 * diff;
		} else {
			std::get<0>(current_coord_) -= 3 * diff;
			std::get<1>(current_coord_) -= 3 * diff;
		}
	}

	klasmimino::size generate_klasmimino_size()
	{
		auto v = size_random_(random_);

		if (v < 50) {
			return klasmimino::size::s;
		} else if (v < 90) {
			return klasmimino::size::m;
		} else {
			return klasmimino::size::l;
		}
	}

	void setup_game()
	{
		game_over_ = false;

		field_.clear();
		score_ = 0;

		set_score_text();

		auto size = klasmimino::size::l;

		if (mode_ == mode::random) {
			size = generate_klasmimino_size();
		}

		next_klasmimino_ = {
			static_cast<tetrimino::shape>(shape_random_(random_)),
			size
		};

		generate_next_klasmimino();
	}

	void generate_next_klasmimino()
	{
		current_klasmimino_ = next_klasmimino_;
		std::get<0>(current_coord_) = 3 * 3;
		std::get<1>(current_coord_) = -3 * 3;

		auto size = klasmimino::size::l;

		if (mode_ == mode::random) {
			if (change_shape_random_(random_) < 50) {
				size = generate_klasmimino_size();
			}
		}

		next_klasmimino_ = {
			static_cast<tetrimino::shape>(shape_random_(random_)),
			size
		};
	}

	void set_score_text()
	{
		score_text_.set_text((L"Score: " + std::to_wstring(score_)).c_str());
	}

public:
	game_scene(main_window &w)
		: w_(w)
		, current_klasmimino_(tetrimino::shape::I, klasmimino::size::l)
		, next_klasmimino_(tetrimino::shape::I, klasmimino::size::l)
	{
		this->add_timer_handler([&](unsigned id) {
			if (id != 0 || game_over_) {
				return;
			}

			auto coord = current_coord_;
			++std::get<1>(coord);

			if (field_.can_place_klasmimino(current_klasmimino_, coord)) {
				current_coord_ = coord;
			} else {
				field_.place_klasmimino(current_klasmimino_, current_coord_);
				score_ += field_.erase_lines();
				set_score_text();
				generate_next_klasmimino();

				if (!field_.can_place_klasmimino(current_klasmimino_, current_coord_)) {
					game_over_ = true;

					auto r = ::MessageBoxW(w_.get_hwnd(), L"もう一度プレイしますか？", L"ゲームオーバー", MB_YESNO);

					if (r == IDYES) {
						this->setup_game();
					} else {
						w_.select_scene(0);
					}
				}
			}

			w_.repaint();
		}, 0);

		this->add_keyboard_handler([&](unsigned key, bool push) {
			if (!push) {
				return;
			}

			if (mode_ == mode::user_changeable && ::GetKeyState(VK_SHIFT) < 0) {
				switch (key) {
				case VK_UP:
					rotate_klasmimino_size(true);
					if (!field_.can_place_klasmimino(current_klasmimino_, current_coord_)) {
						rotate_klasmimino_size(false);
					}
					break;
				case VK_DOWN:
					rotate_klasmimino_size(false);
					if (!field_.can_place_klasmimino(current_klasmimino_, current_coord_)) {
						rotate_klasmimino_size(true);
					}
					break;
				}
			} else if (key == VK_UP) {
				current_klasmimino_.rotate_right();
				if (!field_.can_place_klasmimino(current_klasmimino_, current_coord_)) {
					current_klasmimino_.rotate_left();
				}
			} else {
				auto coord = current_coord_;

				switch (key) {
				case VK_LEFT:
					--std::get<0>(coord);
					break;
				case VK_RIGHT:
					++std::get<0>(coord);
					break;
				case VK_DOWN:
					++std::get<1>(coord);
					break;
				}

				if (field_.can_place_klasmimino(current_klasmimino_, coord)) {
					current_coord_ = coord;
				}
			}

			w_.repaint();
		}, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN);

		this->register_resource(&brush_);
		this->register_object(&score_text_);

		score_text_.get_font().set_size(18.0f);
		score_text_.set_position({ 100.0f, 70.0f + 24.0f * 20.0f + 5.0f });
		score_text_.set_size({ 400.0f, 20.0f });
	}

	~game_scene()
	{
		this->unregister_resource(&brush_);
		this->unregister_object(&score_text_);
	}

	void set_mode(mode m)
	{
		mode_ = m;
	}

	void on_show()
	{
		setup_game();
	}

	void on_hide()
	{
	}
	
	void draw(const quote::direct2d::paint_params &pp) override
	{
		using quote::direct2d::point;
		using quote::direct2d::size;
		using quote::direct2d::rect;

		const point offset(100.0f, 70.0f);
		const size cell_size(big_cell_size_ / 3.0f, big_cell_size_ / 3.0f);

		this->quote::direct2d::scene<>::draw(pp);

		// 背景
		for (unsigned i = 0; i < 10; ++i) {
			for (unsigned j = 0; j < 20; ++j) {
				if ((i + j) % 2 == 1) {
					brush_.set_color(bg_colors_[0]);
				} else {
					brush_.set_color(bg_colors_[2]);
				}

				rect rc(point(i * big_cell_size_, j * big_cell_size_) + offset, cell_size);

				pp.fill_rect(brush_, rc);
				pp.fill_rect(brush_, rc + point(2 * cell_size.width, 0.0f));
				pp.fill_rect(brush_, rc + point(cell_size.width, cell_size.height));
				pp.fill_rect(brush_, rc + point(0.0f, 2 * cell_size.height));
				pp.fill_rect(brush_, rc + point(2 * cell_size.width, 2 * cell_size.height));

				if ((i + j) % 2 == 1) {
					brush_.set_color(bg_colors_[1]);
				} else {
					brush_.set_color(bg_colors_[3]);
				}

				pp.fill_rect(brush_, rc + point(cell_size.width, 0.0f));
				pp.fill_rect(brush_, rc + point(0.0f, cell_size.height));
				pp.fill_rect(brush_, rc + point(2 * cell_size.width, cell_size.height));
				pp.fill_rect(brush_, rc + point(cell_size.width, 2 * cell_size.height));
			}
		}

		// 落ちたklasmimino
		{
			for (unsigned i = 0; i < 10 * 3; ++i) {
				for (unsigned j = 0; j < 20 * 3; ++j) {
					auto d = field_.get_datum(i, j);
					if (d != 0) {
						brush_.set_color(klasmimino_colors_[d - 1]);
						pp.fill_rect(brush_, {
							point(i * cell_size.width, j * cell_size.height) + offset,
							cell_size
						});
					}
				}
			}
		}

		// 落ちてる途中のklasmimino
		{
			auto size = current_klasmimino_.get_size();
			auto draw_size = cell_size * (static_cast<float>(size) + 1.0f);
			auto &data = current_klasmimino_.get_data();

			brush_.set_color(klasmimino_colors_[static_cast<int>(size)]);

			for (unsigned i = 0; i < 5; ++i) {
				for (unsigned j = 0; j < 5; ++j) {
					if (data[j][i] == 1) {
						pp.fill_rect(brush_, {
							point(
								i * draw_size.width + std::get<0>(current_coord_) * cell_size.width,
								j * draw_size.height + std::get<1>(current_coord_) * cell_size.height
							) + offset,
							draw_size
						});
					}
				}
			}
		}

		// 次のklasmimino
		{
			auto size = next_klasmimino_.get_size();
			auto draw_size = cell_size * (static_cast<float>(size) + 1.0f);
			auto &data = next_klasmimino_.get_data();

			brush_.set_color(klasmimino_colors_[static_cast<int>(size)]);

			for (unsigned i = 0; i < 5; ++i) {
				for (unsigned j = 0; j < 5; ++j) {
					if (data[j][i] == 1) {
						pp.fill_rect(brush_, {
							point(i * draw_size.width, j * draw_size.height) + point(370, 100),
							draw_size
						});
					}
				}
			}
		}
	}
};
