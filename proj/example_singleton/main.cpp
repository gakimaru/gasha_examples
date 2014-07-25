//--------------------------------------------------------------------------------
// main.cpp
// シングルトンテスト - メイン
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_singleton.h"//シングルトンテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//シングルトンテスト
	example_singleton();
	
	return EXIT_SUCCESS;
}

// End of file
