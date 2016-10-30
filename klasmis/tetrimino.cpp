#include <cassert>

#include "tetrimino.hpp"

namespace {
	tetrimino::data_type shape_data[7] = {
		{
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
			1, 1, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 1, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 0, 0, 1, 0,
			0, 1, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 0, 1, 1, 0,
			0, 1, 1, 0, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 1, 1, 0, 0,
			0, 0, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 1, 1, 1, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
		{
			0, 0, 0, 0, 0,
			0, 1, 1, 0, 0,
			0, 1, 1, 0, 0,
			0, 0, 0, 0, 0,
			0, 0, 0, 0, 0,
		},
	};

	tetrimino::data_type &
	get_shape_data(tetrimino::shape s)
	{
		int v = static_cast<int>(s);

		assert(v >= 0 && v < 7);

		return shape_data[v];
	}
}

tetrimino::tetrimino(tetrimino::shape s)
	: shape_(s)
	, data_(get_shape_data(s))
{
}

void tetrimino::rotate_left()
{
	data_type new_data = {};

	if (shape_ == shape::I) {
		for (unsigned i = 0; i < 4; ++i) {
			for (unsigned j = 0; j < 4; ++j) {
				new_data[i][j] = data_[j][i];
			}
		}
	} else if (shape_ == shape::O) {
		for (unsigned i = 0; i < 4; ++i) {
			for (unsigned j = 0; j < 4; ++j) {
				new_data[i][j] = data_[j][3 - i];
			}
		}
	} else {
		for (unsigned i = 0; i < 5; ++i) {
			for (unsigned j = 0; j < 5; ++j) {
				new_data[i][j] = data_[j][4 - i];
			}
		}
	}

	data_ = new_data;
}

void tetrimino::rotate_right()
{
	data_type new_data = {};

	if (shape_ == shape::I) {
		for (unsigned i = 0; i < 4; ++i) {
			for (unsigned j = 0; j < 4; ++j) {
				new_data[i][j] = data_[j][i];
			}
		}
	} else if (shape_ == shape::O) {
		for (unsigned i = 0; i < 4; ++i) {
			for (unsigned j = 0; j < 4; ++j) {
				new_data[i][j] = data_[3 - j][i];
			}
		}
	} else {
		for (unsigned i = 0; i < 5; ++i) {
			for (unsigned j = 0; j < 5; ++j) {
				new_data[i][j] = data_[4 - j][i];
			}
		}
	}

	data_ = new_data;
}

const tetrimino::data_type &tetrimino::get_data() const
{
	return data_;
}
