//--------------------------------------------------------------------------------
// main.cpp
// ユニットテストテスト - メイン
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_unit_test.h"//ユニットテストテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//ユニットテストテスト
	const int ret = example_unit_test(argc, argv);
	
	return ret;
}

// End of file
