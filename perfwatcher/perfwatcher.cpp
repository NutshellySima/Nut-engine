// Copyright 2017-2018 Chijun Sima
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "stdafx.h"

using namespace std;

int main()
{
	cin.tie(0);
	cout.tie(0);
	ios::sync_with_stdio(false);
	__int64 maxnum = 0, avgnum = 0;
	size_t time = 0;
	Cwrapper wrapper;
	cout << wrapper.About() << "\n";
	chrono::steady_clock clock;
	int turn = 2;
	auto start = clock.now();
	decltype(start) end = clock.now();
	while (wrapper.Fullboard() == false)
	{
		++time;
		if (turn == 1)
			turn = 2;
		else
			turn = 1;
		auto x = wrapper.Putcomputer(turn, 60000);
		wrapper.Put(get<1>(x), get<2>(x), turn);
		if (wrapper.Checkwin(get<1>(x), get<2>(x)))
			break;
		__int64 xx = chrono::duration_cast<chrono::milliseconds>(clock.now() - end).count();
		end = clock.now();
		cout << turn << ": " << static_cast<char>(get<1>(x) + 'A') << " " << static_cast<char>('A' + get<2>(x)) << ": " << get<0>(x) <<
			"   Duration: " << xx << " ms." << "\n";
		maxnum = max(maxnum, xx);
		avgnum += xx;
	}
	end = clock.now();
	cout << "Duration: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms." << "\n";
	cout << "Avg: " << static_cast<double>(avgnum) / time << " ms." << "\n";
	cout << "Max: " << maxnum << " ms." << "\n";
	return 0;
}

