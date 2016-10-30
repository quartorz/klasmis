#pragma once


#include <quote/win32/window.hpp>
#include <quote/win32/procs.hpp>
#include <quote/win32/creator.hpp>
#include <quote/win32/object_processor.hpp>

#include <quote/direct2d/painter.hpp>
#include <quote/direct2d/traits.hpp>
#include <quote/direct2d/scene_manager.hpp>

class game_scene;
class title_scene;

class main_window :
	public quote::win32::window<
		main_window,
		quote::win32::quit_on_close<main_window>,
		quote::win32::timer<main_window>,
		quote::win32::keyboard<main_window>,
		quote::win32::resizer<main_window>,
		quote::win32::object_processor<main_window, quote::direct2d::traits>,
		quote::direct2d::painter<main_window>
	>,
	public quote::win32::creator<main_window>,
	public quote::direct2d::scene_manager<main_window>
{
	std::unique_ptr<title_scene> title_scene_;
	std::unique_ptr<game_scene> game_scene_;

public:
	static const wchar_t *get_class_name()
	{
		return L"klasmis main_window";
	}

	main_window(const main_window &) = delete;
	main_window(main_window &&) = delete;
	main_window() = default;

	bool initialize();
	void uninitialize();

	void select_game(int mode);
};

#include "game_scene.hpp"
#include "title_scene.hpp"

inline bool main_window::initialize()
{
	title_scene_ = std::make_unique<title_scene>(*this);
	game_scene_ = std::make_unique<game_scene>(*this);

	this->add_scene(0, title_scene_.get());
	this->add_scene(1, game_scene_.get());

	this->select_scene(0);

	// game_scene用のタイマー
	this->set_timer(0, 250);

	return true;
}

inline void main_window::uninitialize()
{
	this->remove_scene(0);
	this->remove_scene(1);

	title_scene_.reset();
	game_scene_.reset();
}

inline void main_window::select_game(int mode)
{
	game_scene_->set_mode(static_cast<game_scene::mode>(mode));
	this->select_scene(1);
}
