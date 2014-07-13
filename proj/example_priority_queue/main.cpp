//--------------------------------------------------------------------------------
// main.cpp
// 優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_priority_queue.h"//優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//優先度付きキューコンテナアダプタテスト
	example_priority_queue();

	//二分ヒープコンテナテスト
	example_binary_heap();
	
	//シンプル優先度付きキューコンテナアダプタテスト
	example_simple_priority_queue();

	//シンプル二分ヒープコンテナテスト
	example_simple_binary_heap();

	return EXIT_SUCCESS;
}

// End of file
