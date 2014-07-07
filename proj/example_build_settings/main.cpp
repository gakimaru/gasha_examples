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

GASHA_USING_NAMESPACE;//ネームスペース使用

int main(const int argc, const char* argv[])
{
	//ビルド設定処理テスト
	example_build_settings();

	//ビルド設定が正しく実行環境に適合するかチェック
	{
		char message[4096];
		std::size_t size = 0;

		//ライブラリビルド時のビルド設定を診断
		printf("\n");
		diagnoseBuildSettings(message, size, diagnosisModeForLibrary);
		printf(message);
		printf("message size = %d butes.\n", size);

		//現在のビルド設定を診断
		printf("\n");
		diagnoseBuildSettings(message, size, diagnosisModeForCurrent);
		printf(message);
		printf("message size = %d butes.\n", size);
	}

	printf("\n");
	printf("- End -\n");

	return EXIT_SUCCESS;
}

// End of file
