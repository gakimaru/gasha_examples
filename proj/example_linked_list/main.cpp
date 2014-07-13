//--------------------------------------------------------------------------------
// main.cpp
// 双方向連結リストコンテナテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_linked_list.h"//双方向連結リストコンテナテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//双方向連結リストコンテナテスト
	example_linked_list();
	
	//シンプル双方向連結リストコンテナテスト
	example_simple_linked_list();
	
	return EXIT_SUCCESS;
}

// End of file
