//--------------------------------------------------------------------------------
// main.cpp
// アロケータテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char* argv[])
{
	//アロケータテスト
	example_allocator();
	
	return EXIT_SUCCESS;
}

// End of file
