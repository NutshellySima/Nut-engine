// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once
#include "stdafx.h"
#include "chessboard.h"

#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport

class evaluation
{
public:
	DllExport evaluation() noexcept {}
	DllExport int evaluate(chessboard &, const int turn, const int row, const int col, bool pure) noexcept;
	DllExport void pop_state(chessboard &board);

private:
	void save_state(chessboard &board, int row, int col);
	std::vector<std::array<uint8_t, 62>>state;
	void reset_point(chessboard &board, int row, int col) noexcept;
	void analyse_line(const std::array<uint8_t, 15> &line, int num, const int pos) noexcept;
	void analysis_horizon(chessboard &, int i, int j) noexcept;
	void analysis_vertical(chessboard &, int i, int j) noexcept;
	void analysis_left(chessboard &, int i, int j) noexcept;
	void analysis_right(chessboard &, int i, int j) noexcept;
	inline int checkturn(const int side, const int turn) const noexcept
	{
		if (side == turn)
			return 1;
		return -1;
	}
	inline int sign(int number) const noexcept
	{
		if (number < 0)
			return -1;
		return 1;
	}
	void evaluate_point(chessboard &board, int row, int col) noexcept;
	const uint8_t TODO = 0, STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7,
		ANALYSED = 8;
	const uint8_t BLACK = 1, WHITE = 2;
	const std::vector<uint8_t> nturn = { 0, 2, 1 };
	std::vector<uint8_t> result = std::vector<uint8_t>(15);
	uint8_t flag;
	std::vector<uint8_t>copy_place = std::vector<uint8_t>(100);
};

