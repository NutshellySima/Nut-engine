// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "stdafx.h"
#include "evaluation.h"


void evaluation::pop_state(chessboard & board)
{
	const auto& tmp = state.back();
	int row = tmp[0];
	int col = tmp[1];
	size_t curpos = 2;
	for (int ii = 0; ii < 15; ++ii)
	{
		board.layer_3[0][row][ii] = tmp[curpos++];
		board.layer_3[1][ii][col] = tmp[curpos++];
	}
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
		board.layer_3[2][x][y] = tmp[curpos++];
	}
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
		board.layer_3[3][x][y] = tmp[curpos++];
	}
	state.pop_back();
}

void evaluation::save_state(chessboard & board, int row, int col)
{
	std::array<uint8_t, 62>tmp;
	int curpos = 0;
	tmp[curpos++] = row;
	tmp[curpos++] = col;
	for (int ii = 0; ii < 15; ++ii)
	{
		tmp[curpos++] = board.layer_3[0][row][ii];
		tmp[curpos++] = board.layer_3[1][ii][col];
	}
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
		tmp[curpos++] = board.layer_3[2][x][y];
	}
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
		tmp[curpos++] = board.layer_3[3][x][y];
	}
	state.emplace_back(tmp);
}

void evaluation::reset_point(chessboard &board, int row, int col) noexcept
{
	if (row < 0 || col < 0 || row >= 15 || col >= 15)
	{
		for (int i = 0; i < 15; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				board.layer_3[0][i][ii] = 0;
				board.layer_3[1][i][ii] = 0;
				board.layer_3[2][i][ii] = 0;
				board.layer_3[3][i][ii] = 0;
			}
		}
	}
	else
	{
		save_state(board, row, col);
		for (int ii = 0; ii < 15; ++ii)
		{
			board.layer_3[0][row][ii] = 0;
			board.layer_3[1][ii][col] = 0;
		}
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
			board.layer_3[2][x][y] = 0;
		}
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
			board.layer_3[3][x][y] = 0;
		}
	}
}

void evaluation::analyse_line(const std::array<uint8_t, 15> &line, int num, const int pos) noexcept
{
	flag = 0;
	//std::fill_n(result.begin(), num, TODO);
	if (num < 5)
	{
		for (int i = 0; i < num; ++i)
		{
			result[i] = ANALYSED;
			copy_place[flag] = i;
			++flag;
		}
		//std::fill_n(result.begin(), num, ANALYSED);
		return;
	}
	const int8_t stone = line[pos];
	const int8_t inverse = nturn[stone];
	--num;
	int xl = pos, xr = pos;
	while (xl > 0)
	{
		if (line[xl - 1] != stone)
			break;
		--xl;
	}
	while (xr < num)
	{
		if (line[xr + 1] != stone)
			break;
		++xr;
	}
	int left_range = xl, right_range = xr;
	while (left_range > 0)
	{
		if (right_range - left_range >= 4 || line[left_range - 1] == inverse)
			break;
		--left_range;
	}
	while (right_range < num)
	{
		if (right_range - left_range >= 4 || line[right_range + 1] == inverse)
			break;
		++right_range;
	}
	if (right_range - left_range < 4)
	{
		for (int k = left_range; k <= right_range; ++k)
		{
			result[k] = ANALYSED;
			copy_place[flag] = k;
			++flag;
		}
		return;
	}
	for (int k = xl; k <= xr; ++k)
	{
		result[k] = ANALYSED;
		copy_place[flag] = k;
		++flag;
	}
	const int srange = xr - xl;
	bool left2 = false;
	bool leftfour = false;
	bool left3 = false;
	switch (srange)
	{
	case 3:
		if (xl > 0)
		{
			// Maybe only left or double
			if (line[xl - 1] == 0)
			{
				leftfour = true;
				result[pos] = SFOUR;
				copy_place[flag] = pos;
				++flag;
			}
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				// double
				if (leftfour)
				{
					result[pos] = FOUR;
					copy_place[flag] = pos;
					++flag;
				}
				// only right has blank
				else
				{
					result[pos] = SFOUR;
					copy_place[flag] = pos;
					++flag;
				}
			}
		}
		return;
		break;
	case 2:

		if (xl > 0)
		{
			if (line[xl - 1] == 0)
			{
				// -|--- SFOUR
				if (xl > 1 && line[xl - 2] == stone)
				{
					result[xl] = SFOUR;
					copy_place[flag] = xl;
					++flag;
					result[xl - 2] = ANALYSED;
					copy_place[flag] = xl - 2;
					++flag;
				}
				// x|---
				else
				{
					left3 = true;
					result[pos] = STHREE;
					copy_place[flag] = pos;
					++flag;
				}
			}
			// ---
			else if (xr == num || line[xr + 1] != 0)
				return;
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				// ---|- SFOUR
				if (xr < num - 1 && line[xr + 2] == stone)
				{
					result[xr] = SFOUR;
					copy_place[flag] = xr;
					++flag;
					result[xr + 2] = ANALYSED;
					copy_place[flag] = xr + 2;
					++flag;
				}
				// x|---|x THREE
				else if (left3)
				{
					result[xr] = THREE;
					copy_place[flag] = xr;
					++flag;
				}
				// x--- STHREE
				else
				{
					result[xr] = STHREE;
					copy_place[flag] = xr;
					++flag;
				}
			}
		}
		return;
		break;
	case 1:
		if (xl > 0)
		{
			if (line[xl - 1] == 0)
			{
				if (xl > 2 && line[xl - 2] == stone)
				{
					// --|--
					if (line[xl - 3] == stone)
					{
						result[xl - 3] = ANALYSED;
						copy_place[flag] = xl - 3;
						++flag;
						result[xl - 2] = ANALYSED;
						copy_place[flag] = xl - 2;
						++flag;
						result[xl] = SFOUR;
						copy_place[flag] = xl;
						++flag;
					}
					// |-|--
					else if (line[xl - 3] == 0)
					{
						result[xl - 2] = ANALYSED;
						copy_place[flag] = xl - 2;
						++flag;
						result[xl] = STHREE;
						copy_place[flag] = xl;
						++flag;
					}
				}
				else
				{
					left2 = true;
					result[pos] = STWO;
					copy_place[flag] = pos;
					++flag;
				}
			}
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (xr < num - 2 && line[xr + 2] == stone)
				{
					if (line[xr + 3] == stone)
					{
						result[xr + 3] = ANALYSED;
						copy_place[flag] = xr + 3;
						++flag;
						result[xr + 2] = ANALYSED;
						copy_place[flag] = xr + 2;
						++flag;
						result[xr] = SFOUR;
						copy_place[flag] = xr;
						++flag;
					}
					else if (line[xr + 3] == 0)
					{
						result[xr + 2] = ANALYSED;
						copy_place[flag] = xr + 2;
						++flag;
						if (left2)
						{
							result[xr] = THREE;
							copy_place[flag] = xr;
							++flag;
						}
						else
						{
							result[xr] = STHREE;
							copy_place[flag] = xr;
							++flag;
						}
					}
				}
				else
				{
					if (result[xl] == SFOUR)
						return;
					if (result[xl] == STHREE)
					{
						result[xl] = THREE;
						copy_place[flag] = xl;
						++flag;
						return;
					}
					if (left2)
					{
						result[pos] = TWO;
						copy_place[flag] = pos;
						++flag;
					}
					else
					{
						result[pos] = STWO;
						copy_place[flag] = pos;
						++flag;
					}
				}
			}
		}
		return;
		break;
	case 0:
		break;
	default:
		result[pos] = FIVE;
		copy_place[flag] = pos;
		++flag;
		return;
		break;
	}
}

void evaluation::analysis_horizon(chessboard &board, int i, int j) noexcept
{
	analyse_line(std::ref(board.layer_2[0][i]), 15, j);
	size_t x;
	for (int copy = 0; copy < flag; ++copy)
	{
		x = copy_place[copy];
		board.layer_3[0][i][x] = result[x];
	}
}

void evaluation::analysis_vertical(chessboard &board, int i, int j) noexcept
{
	analyse_line(std::ref(board.layer_2[1][j]), 15, i);
	size_t x;
	for (int copy = 0; copy < flag; ++copy)
	{
		x = copy_place[copy];
		board.layer_3[1][x][j] = result[x];
	}
}

void evaluation::analysis_left(chessboard &board, int i, int j) noexcept
{
	int x, y, k = 0;
	if (i < j)
	{
		x = j - i;
		y = 0;
	}
	else
	{
		x = 0;
		y = i - j;
	}
	k = 15 - abs(i - j);
	analyse_line(board.layer_2[2][i - j + 14], k, j - x);
	size_t s;
	for (int copy = 0; copy < flag; ++copy)
	{
		s = copy_place[copy];
		board.layer_3[2][y + s][x + s] = result[s];
	}
}

void evaluation::analysis_right(chessboard &board, int i, int j) noexcept
{
	int x, y, k = 0;
	if (14 - i < j)
	{
		x = j - 14 + i;
		y = 14;
	}
	else
	{
		x = 0;
		y = i + j;
	}
	k = 15 - abs(i + j - 14);
	analyse_line(board.layer_2[3][i + j], k, j - x);
	size_t s;
	for (int copy = 0; copy < flag; ++copy)
	{
		s = copy_place[copy];
		board.layer_3[3][y - s][x + s] = result[s];
	}
}

int evaluation::evaluate(chessboard &board, int turn, const int row, const int col, bool pure) noexcept
{
	if (pure)
	{
		reset_point(board, row, col);
		evaluate_point(board, row, col);
		return 0;
	}
	std::array<std::array<uint8_t, 9>, 3> layer_4;
	for (int i = 0; i < 3; ++i)
	{
		for (int ii = 0; ii < 9; ++ii)
		{
			layer_4[i][ii] = 0;
		}
	}
	for (int i = 0; i < board.number; ++i)
	{
		const auto& temp = board.layer_5[i];
		const uint8_t ch1 = board.layer_3[0][temp.first][temp.second];
		const uint8_t ch2 = board.layer_3[1][temp.first][temp.second];
		const uint8_t ch3 = board.layer_3[2][temp.first][temp.second];
		const uint8_t ch4 = board.layer_3[3][temp.first][temp.second];
		++layer_4[board.board[temp.first][temp.second]][ch1];
		++layer_4[board.board[temp.first][temp.second]][ch2];
		++layer_4[board.board[temp.first][temp.second]][ch3];
		++layer_4[board.board[temp.first][temp.second]][ch4];
	}
	if (layer_4[BLACK][FIVE])
	{
		return 100000 * checkturn(BLACK, turn);
	}
	if (layer_4[WHITE][FIVE])
	{
		return 100000 * checkturn(WHITE, turn);
	}
	if (layer_4[WHITE][SFOUR] >= 2)
		++layer_4[WHITE][FOUR];
	if (layer_4[BLACK][SFOUR] >= 2)
		++layer_4[BLACK][FOUR];
	int score, wvalue = 0, bvalue = 0;
	if (turn == WHITE)
	{
		if (layer_4[WHITE][FOUR] > 0)
		{
			score = 9990; goto end;
		}
		if (layer_4[WHITE][SFOUR] > 0)
		{
			score = 9980; goto end;
		}
		if (layer_4[BLACK][FOUR] > 0)
		{
			score = -9970; goto end;
		}
		if (layer_4[BLACK][SFOUR] && layer_4[BLACK][THREE])
		{
			score = -9960; goto end;
		}
		if (layer_4[WHITE][THREE] && layer_4[BLACK][SFOUR] == 0)
		{
			score = 9950; goto end;
		}
		if (layer_4[BLACK][THREE] > 1 &&
			layer_4[WHITE][SFOUR] == 0 &&
			layer_4[WHITE][THREE] == 0 &&
			layer_4[WHITE][STHREE] == 0)
		{
			score = -9940; goto end;
		}
		if (layer_4[WHITE][THREE] > 1)
			wvalue += 2000;
		else if (layer_4[WHITE][THREE])
			wvalue += 200;
		if (layer_4[BLACK][THREE] > 1)
			bvalue += 500;
		else if (layer_4[BLACK][THREE])
			bvalue += 100;
		wvalue += layer_4[WHITE][STHREE] * 10;
		bvalue += layer_4[BLACK][STHREE] * 10;
		wvalue += layer_4[WHITE][TWO] * 4;
		bvalue += layer_4[BLACK][TWO] * 4;
		wvalue += layer_4[WHITE][STWO];
		bvalue += layer_4[BLACK][STWO];
		score = wvalue - bvalue;
	}
	else
	{
		if (layer_4[BLACK][FOUR] > 0)
		{
			score = 9990; goto end;
		}
		if (layer_4[BLACK][SFOUR] > 0)
		{
			score = 9980; goto end;
		}
		if (layer_4[WHITE][FOUR] > 0)
		{
			score = -9970; goto end;
		}
		if (layer_4[WHITE][SFOUR] && layer_4[WHITE][THREE])
		{
			score = -9960; goto end;
		}
		if (layer_4[BLACK][THREE] && layer_4[WHITE][SFOUR] == 0)
		{
			score = 9950; goto end;
		}
		if (layer_4[WHITE][THREE] > 1 &&
			layer_4[BLACK][SFOUR] == 0 &&
			layer_4[BLACK][THREE] == 0 &&
			layer_4[BLACK][STHREE] == 0)
		{
			score = -9940; goto end;
		}
		if (layer_4[BLACK][THREE] > 1)
			bvalue += 2000;
		else if (layer_4[BLACK][THREE])
			bvalue += 200;
		if (layer_4[WHITE][THREE] > 1)
			wvalue += 500;
		else if (layer_4[WHITE][THREE])
			wvalue += 100;
		bvalue += layer_4[BLACK][STHREE] * 10;
		wvalue += layer_4[WHITE][STHREE] * 10;
		bvalue += layer_4[BLACK][TWO] * 4;
		wvalue += layer_4[WHITE][TWO] * 4;
		bvalue += layer_4[BLACK][STWO];
		wvalue += layer_4[WHITE][STWO];
		score = bvalue - wvalue;
	}
end:
	const uint8_t stone = nturn[turn];
	if (abs(score) > 9000)
	{
		const int x = sign(score);
		if (x == 1)
		{
			for (int i = 1; i <= 7; ++i)
				if (layer_4[stone][i] > 0)
					score += i;
		}
		else
		{
			for (int i = 1; i <= 7; ++i)
				if (layer_4[stone][i] > 0)
					score -= i;
		}
	}
	return score;
}

void evaluation::evaluate_point(chessboard &board, int row, int col) noexcept
{
	if (row < 0 || col < 0 || row >= 15 || col >= 15)
	{
		for (int num = 0; num < board.number; ++num)
		{
			auto temp = board.layer_5[num];
			if (board.layer_3[0][temp.first][temp.second] == TODO)
				analysis_horizon(board, temp.first, temp.second);
			if (board.layer_3[1][temp.first][temp.second] == TODO)
				analysis_vertical(board, temp.first, temp.second);
			if (board.layer_3[2][temp.first][temp.second] == TODO)
				analysis_left(board, temp.first, temp.second);
			if (board.layer_3[3][temp.first][temp.second] == TODO)
				analysis_right(board, temp.first, temp.second);
		}
	}
	else
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board.board[row][ii] != 0 && board.layer_3[0][row][ii] == TODO)
				analysis_horizon(board, row, ii);
		}
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board.board[ii][col] != 0 && board.layer_3[1][ii][col] == TODO)
				analysis_vertical(board, ii, col);
		}
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
			if (board.board[x][y] != 0 && board.layer_3[2][x][y] == TODO)
				analysis_left(board, x, y);
		}
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
			if (board.board[x][y] != 0 && board.layer_3[3][x][y] == TODO)
				analysis_right(board, x, y);
		}
	}
}
