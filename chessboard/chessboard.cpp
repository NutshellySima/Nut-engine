// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "stdafx.h"
#include "chessboard.h"

chessboard::chessboard() noexcept
{
	reset();
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			pos[i][ii] = 7 - max(abs(i - 7), abs(ii - 7));
		}
	}
	score_cache.reserve(15 * 15);
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			score_cache.emplace_back(std::make_pair(i, ii));
		}
	}
	std::stable_sort(score_cache.rbegin(), score_cache.rend(), [&](const std::pair<int8_t, int8_t> &i1, const std::pair<int8_t, int8_t> &i2) {
		return pos[i1.first][i1.second] < pos[i2.first][i2.second];
	});
}

//reset the chess board

void chessboard::reset() noexcept
{
	number = 0;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			remote_cell[i][ii] = 0;
			board[i][ii] = 0;
		}
	}
	for (int x = 0; x < 4; ++x)
	{
		for (int i = 0; i < 29; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				layer_2[x][i][ii] = 0;
			}
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			for (int iii = 0; iii < 15; ++iii)
			{
				layer_3[i][ii][iii] = 0;
			}
		}
	}
}

int8_t chessboard::get(const int8_t row, const int8_t col) const noexcept
{
	if (row >= 0 && row < 15 && col >= 0 && col < 15)
		return board[row][col];
	else
		return -1;
}

bool chessboard::put(const int8_t row, const int8_t col, const int8_t x) noexcept
{
	if (row >= 0 && row < 15 && col >= 0 && col < 15 && board[row][col] == 0)
	{
		board[row][col] = x;
		update_remotecell(row, col, true);
		update_layer_2(row, col, true);
		layer_5[number] = std::make_pair(row, col);
		++number;
		return true;
	}
	else
	{
		return false;
	}
}

int8_t chessboard::undo(const int8_t row, const int8_t col) noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15 || board[row][col] == 0)
		return 2;
	board[row][col] = 0;
	--number;
	update_remotecell(row, col, false);
	update_layer_2(row, col, false);
	return 0;
}

int chessboard::check() const noexcept
{
	int counto = 0, countx = 0;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[i][ii] == 0)
			{
				countx = counto = 0;
			}
			if (board[i][ii] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[i][ii] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[ii][i] == 0)
			{
				countx = counto = 0;
			}
			if (board[ii][i] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[ii][i] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = -10; i <= 10; ++i)
	{
		int x = i, y = 0;
		while (x < 0)
			++x, ++y;
		for (; x < 15 && y < 15; ++x, ++y)
		{
			if (board[x][y] == 0)
			{
				countx = counto = 0;
			}
			if (board[x][y] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[x][y] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 4; i <= 24; ++i)
	{
		int x = 0;
		int y = i;
		while (y >= 15)
		{
			++x;
			--y;
		}
		for (; x < 15 && y >= 0; ++x, --y)
		{
			if (board[x][y] == 0)
			{
				countx = counto = 0;
			}
			if (board[x][y] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[x][y] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	return 0;
}

int8_t chessboard::checkpoint(const int8_t row, const int8_t col) const noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15)
		return 0;
	int counto = 0, countx = 0;
	for (int ii = 0; ii < 15; ++ii)
	{
		if ((countx != 0 || counto != 0) && board[row][ii] == 0)
		{
			countx = counto = 0;
		}
		if (board[row][ii] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[row][ii] == 2)
		{
			++countx;
			counto = 0;
		}
		if (counto == 5)
		{
			return 1;
		}
		if (countx == 5)
		{
			return 2;
		}
	}
	counto = countx = 0;
	for (int ii = 0; ii < 15; ++ii)
	{
		if ((countx != 0 || counto != 0) && board[ii][col] == 0)
		{
			countx = counto = 0;
		}
		if (board[ii][col] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[ii][col] == 2)
		{
			++countx;
			counto = 0;
		}
		if (counto == 5)
		{
			return 1;
		}
		if (countx == 5)
		{
			return 2;
		}
	}
	counto = countx = 0;
	int x, y;
	if (row < col)
	{
		y = col - row;
		x = 0;
	}
	else
	{
		y = 0;
		x = row - col;
	}
	for (; x < 15 && y < 15; ++x, ++y)
	{
		if ((countx != 0 || counto != 0) && board[x][y] == 0)
		{
			countx = counto = 0;
		}
		if (board[x][y] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[x][y] == 2)
		{
			++countx;
			counto = 0;
		}
		if (counto == 5)
		{
			return 1;
		}
		if (countx == 5)
		{
			return 2;
		}
	}
	counto = countx = 0;
	if (14 - row < col)
	{
		y = col - 14 + row;
		x = 14;
	}
	else
	{
		y = 0;
		x = row + col;
	}
	for (; x >= 0 && y < 15; --x, ++y)
	{
		if ((countx != 0 || counto != 0) && board[x][y] == 0)
		{
			countx = counto = 0;
		}
		if (board[x][y] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[x][y] == 2)
		{
			++countx;
			counto = 0;
		}
		if (counto == 5)
		{
			return 1;
		}
		if (countx == 5)
		{
			return 2;
		}
	}
	return 0;
}

void chessboard::update_remotecell(const int8_t row, const int8_t col, const bool add) noexcept
{
	for (int i = row - 2; i <= row + 2; ++i)
	{
		if (i < 0 || i >= 15)
			continue;
		for (int j = col - 2; j <= col + 2; ++j)
		{
			if (j < 0 || j >= 15)
				continue;
			if (add == true)
				++remote_cell[i][j];
			else
				--remote_cell[i][j];
		}
	}
}

void chessboard::update_layer_2(const int8_t row, const int8_t col, const bool add) noexcept
{
	// 0 - horizontal
	// 1 - vertical
	int xx;
	if (add == false)
		xx = 0;
	else
		xx = board[row][col];
	layer_2[0][row][col] = xx;
	layer_2[1][col][row] = xx;
	int x, y;
	if (row < col)
	{
		x = col - row;
		y = 0;
	}
	else
	{
		x = 0;
		y = row - col;
	}
	layer_2[2][row - col + 14][col - x] = xx;
	if (14 - row < col)
	{
		x = col - 14 + row;
		y = 14;
	}
	else
	{
		x = 0;
		y = row + col;
	}
	layer_2[3][row + col][col - x] = xx;
}

std::vector<std::tuple<int, int8_t, int8_t>> chessboard::genmove() const noexcept
{
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves.reserve(64);
	int score;
	for (int i = 0; i < 15 * 15; ++i)
	{
		auto& val = score_cache[i];
		if (remote_cell[val.first][val.second] != 0 && board[val.first][val.second] == 0)
		{
			score = pos[val.first][val.second];
			moves.emplace_back(std::make_tuple(score, val.first, val.second));
		}
	}
	if (0 == number)
		moves.emplace_back(std::make_tuple(3, 7, 7));
	return moves;
}

bool chessboard::Fullboard() const noexcept
{
	return number == 15 * 15;
}
