//--------------------------------------------------------------------------------
// exmaple_thrad_id.cpp
// ユニットテストテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_unit_test.h"//ユニットテストテスト

#include "real_test/game_loop/game_loop.h"

#include <cstdio>

//----------------------------------------
//ユニットテストテスト
int example_unit_test(const int argc, const char* argv[])
{
	std::printf("----- Test for UnitTest -----\n");

	gameLoop game_loop(argc, argv);

	//初期化処理
	game_loop.initialize();

	//メイン処理
	game_loop.main();

	//終了処理
	game_loop.finalize();

	std::printf("- end -\n");

	//終了
	//※ユニットテストのエラー数を返し、これを main() 関数の戻り値として返す
	return game_loop;
}

// End of file
