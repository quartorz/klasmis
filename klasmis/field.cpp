#include <cassert>

#include "field.hpp"

namespace {
	unsigned size_to_score[3] = {
		300, 200, 100
	};
}

void field::clear()
{
	for (unsigned i = 0; i < 10 * 3; ++i) {
		for (unsigned j = 0; j < 20 * 3 + 5 * 3; ++j) {
			data_[i + 1][j] = 0;
		}
	}
}

bool field::can_place_klasmimino(const klasmimino &k, const coord_type &c) const
{
	unsigned x = std::get<0>(c) + 1;
	unsigned y = std::get<1>(c) + 5 * 3;

	unsigned scale = static_cast<int>(k.get_size()) + 1;
	auto &data = k.get_data();

	for (unsigned i = 0; i < 5; ++i) {
		for (unsigned j = 0; j < 5; ++j) {
			if (data[j][i] == 1) {
				for (unsigned k = 0; k < scale; ++k) {
					for (unsigned l = 0; l < scale; ++l) {
						if (data_[x + i * scale + k][y + j * scale + l] != 0) {
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

void field::place_klasmimino(const klasmimino &k, const coord_type &c)
{
	unsigned x = std::get<0>(c) + 1;
	unsigned y = std::get<1>(c) + 5 * 3;

	unsigned scale = static_cast<int>(k.get_size()) + 1;
	auto &data = k.get_data();

	for (unsigned i = 0; i < 5; ++i) {
		for (unsigned j = 0; j < 5; ++j) {
			if (data[j][i] == 1) {
				for (unsigned k = 0; k < scale; ++k) {
					for (unsigned l = 0; l < scale; ++l) {
						data_[x + i * scale + k][y + j * scale + l] = scale;
					}
				}
			}
		}
	}
}

unsigned char field::get_datum(unsigned x, unsigned y) const
{
	return data_[x + 1][y + 5 * 3];
}

unsigned field::erase_lines()
{
	unsigned score = 0;

	for (unsigned y = 5 * 3; y < 5 * 3 + 20 * 3; ++y) {
		bool line_exists = true;
		unsigned acc = 0;

		for (unsigned x = 1; x < 3 * 10 + 1; ++x) {
			if (data_[x][y] == 0) {
				line_exists = false;
				break;
			} else {
				acc += size_to_score[data_[x][y] - 1];
			}
		}

		if (line_exists) {
			score += acc;
			for (unsigned i = y; i > 0; --i) {
				for (unsigned j = 1; j < 3 * 10 + 1; ++j) {
					data_[j][i] = data_[j][i - 1];
				}
			}
		}
	}

	return score;
}
