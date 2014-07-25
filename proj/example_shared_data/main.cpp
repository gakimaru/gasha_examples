//--------------------------------------------------------------------------------
// main.cpp
// マルチスレッド共有データテスト - メイン
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_shared_data.h"//マルチスレッド共有データテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//マルチスレッド共有データテスト
	example_shared_data();
	
	return EXIT_SUCCESS;
}

// End of file
