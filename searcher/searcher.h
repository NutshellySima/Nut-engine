// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once
#include"stdafx.h"
#include"evaluation.h"
#include"chessboard.h"

#ifndef DllExport
#define DllExport   __declspec( dllexport )  
#endif // !DllExport

class searcher
{
public:
	DllExport std::tuple<int, int8_t, int8_t> alpha_beta_search(int8_t turn, chessboard board, int8_t depth, int timeout);
private:
	std::vector<std::tuple<int, int8_t, int8_t>> smart_genmove(const int8_t turn, chessboard& board, const int8_t current, evaluation& evaluator);
	std::tuple<int, int8_t, int8_t> max_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator, int score);
	std::tuple<int, int8_t, int8_t> min_value(int8_t turn, chessboard& board, int alpha, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator, int score);
	std::tuple<int, int8_t, int8_t> max_value_first(int8_t turn, chessboard& board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation& evaluator, int score);
	void min_value_first(int8_t turn, chessboard board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator, int score);
	void min_value_second(int8_t turn, chessboard board, int beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator, int score);
	std::tuple<int, int8_t, int8_t> max_value_second(int8_t turn, chessboard board, int alpha, int& beta, int8_t depth, int8_t i, int8_t ii, int8_t ply, evaluation evaluator, int score);
	void write_val(const std::tuple<int, int8_t, int8_t>&);
	void getTrueVal(std::tuple<int, int8_t, int8_t>& temp);
	int getAlphaVal();
	int timeoutnum = 30000;
	std::chrono::time_point<std::chrono::steady_clock> start;
	int8_t search_depth;
	std::tuple<int, int8_t, int8_t> trueval;
	int alphaval;
	uint8_t maxnum;
};
