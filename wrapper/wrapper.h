// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifdef WRAPPER_EXPORTS
#define WRAPPER_API __declspec(dllexport)
#else
#define WRAPPER_API __declspec(dllimport)
#endif
#include"stdafx.h"
#include"chessboard.h"
#include"searcher.h"
#include<memory>
#include<iostream>
#include<string>


class Cwrapper {
public:
	WRAPPER_API Cwrapper(void);
	WRAPPER_API std::string About()
	{
		return "Nutengine v2.5.0";
	}
	WRAPPER_API void Resetboard()
	{
		board.reset();
	}
	WRAPPER_API bool Fullboard()
	{
		return board.Fullboard();
	}
	WRAPPER_API auto Getboard()
	{
		return board;
	}
	WRAPPER_API bool Undoboard(int row, int col)
	{
		return board.undo(row, col);
	}
	WRAPPER_API int Checkwin(const int row, const int col)
	{
		return board.checkpoint(row, col);
	}
	WRAPPER_API auto Putcomputer(int turn, int timeout)
	{
		std::tuple<int, int8_t, int8_t> y;
		if (timeout >= 4000)
			y = searchman.alpha_beta_search(turn, board, 7, timeout);
		else
			y = searchman.alpha_beta_search(turn, board, 6, timeout);
		return y;
	}
	WRAPPER_API bool Put(int row, int col, int turn)
	{
		return board.put(row, col, turn);
	}
	WRAPPER_API bool IsFree(int8_t i, int8_t ii)
	{
		return board.get(i, ii) == 0;
	}
private:
	chessboard board;
	searcher searchman;
};