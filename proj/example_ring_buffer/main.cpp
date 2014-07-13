//--------------------------------------------------------------------------------
// main.cpp
// リングバッファコンテナテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_ring_buffer.h"//リングバッファコンテナテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//リングバッファコンテナテスト
	example_ring_buffer();
	
	//シンプルリングバッファコンテナテスト
	example_simple_ring_buffer();

	return EXIT_SUCCESS;
}

// End of file
