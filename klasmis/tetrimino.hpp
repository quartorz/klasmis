#pragma once

#include <array>

class tetrimino {
public:
	enum class shape {
		I, J, L, S, Z, T, O, max
	};

	using data_type = std::array<std::array<unsigned char, 5>, 5>;

private:
	shape shape_;
	data_type data_;

public:
	tetrimino(const tetrimino &) = default;
	tetrimino(shape s);

	void rotate_left();
	void rotate_right();

	const data_type &get_data() const;
	shape get_shape() const
	{
		return shape_;
	}
};
