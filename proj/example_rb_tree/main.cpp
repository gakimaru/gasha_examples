//--------------------------------------------------------------------------------
// main.cpp
// 赤黒木コンテナテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_rb_tree.h"//赤黒木コンテナテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//赤黒木コンテナテスト
	example_rb_tree();
	
	//シンプル赤黒木コンテナテスト
	example_simple_rb_tree();

	return EXIT_SUCCESS;
}

// End of file
