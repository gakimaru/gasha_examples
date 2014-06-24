//--------------------------------------------------------------------------------
// main.cpp
// ビルド設定処理テスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_build_settings.h"//ビルド設定処理テスト

#include <stdio.h>
#include <stdlib.h>

USING_NAMESPACE_GASHA;//ネームスペース使用

int main(const int argc, const char* argv[])
{
	//ビルド設定が正しく実行環境に適合するかチェック
	checkBuildSettings();
	
	//ビルド設定処理テスト
	example_build_settings();
	
	return EXIT_SUCCESS;
}

// End of file
