//--------------------------------------------------------------------------------
// main.cpp
// 片方向連結リストテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singly_linked_list.h"//片方向連結リストテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//片方向連結リストテスト
	example_singly_linked_list();

	//シンプル片方向連結リストコンテナテスト
	example_simple_singly_linked_list();

	return EXIT_SUCCESS;
}

// End of file
