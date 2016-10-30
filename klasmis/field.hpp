#pragma once

#include <cstdlib>
#include <utility>

#include "klasmimino.hpp"

class field {
	unsigned char data_[10 * 3 + 2][20 * 3 + 5 * 3 + 1] = {};

public:
	// xÀ•W‚ÆyÀ•W‚Ì‘g
	using coord_type = std::pair<int, int>;

	field()
	{
		for (unsigned i = 0; i < _countof(data_[0]); ++i) {
			data_[0][i] = 1;
			data_[10 * 3 + 1][i] = 1;
		}

		for (unsigned i = 0; i < _countof(data_); ++i) {
			data_[i][20 * 3 + 5 * 3] = 1;
		}
	}

	void clear();
	bool can_place_klasmimino(const klasmimino &, const coord_type &) const;
	void place_klasmimino(const klasmimino & k, const coord_type &);
	unsigned char get_datum(unsigned x, unsigned y) const;
	unsigned erase_lines();
};
