//--------------------------------------------------------------------------------
// main.cpp
// 開番地法ハッシュテーブルコンテナテスト - メイン
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_hash_table.h"//開番地法ハッシュテーブルコンテナテスト

#include <cstdio>
#include <cstdlib>

int main(const int argc, const char* argv[])
{
	//開番地法ハッシュテーブルコンテナテスト
	example_hash_table();
	
	//シンプル開番地法ハッシュテーブルコンテナテスト
	example_simple_hash_table();

	return EXIT_SUCCESS;
}

// End of file
