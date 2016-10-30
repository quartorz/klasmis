#pragma once

#include "tetrimino.hpp"

class klasmimino : public tetrimino {
public:
	enum class size { s, m, l };

private:
	size size_;

public:
	klasmimino(shape sh, size si)
		: tetrimino(sh)
		, size_(si)
	{
	}

	void set_size(size s)
	{
		size_ = s;
	}

	size get_size() const
	{
		return size_;
	}
};
