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

#include <gasha/build_settings/build_settings_diag.h>//ビルド設定診断

#include <cstdio>
#include <cstdlib>

GASHA_USING_NAMESPACE;//ネームスペース使用

int main(const int argc, const char* argv[])
{
	//ビルド設定処理テスト
	example_build_settings();

	//ビルド設定が正しく実行環境に適合するかチェック
	{
		char message[8192];
		std::size_t size = 0;

		//ライブラリビルド時のビルド設定を診断
		std::printf("\n");
		buildSettingsDiagnosticTest(message, size, diagForLibrary);
		std::printf(message);
		std::printf("message size = %d bytes.\n", size);

		//現在のビルド設定を診断
		std::printf("\n");
		buildSettingsDiagnosticTest(message, size, diagForProject);
		std::printf(message);
		std::printf("message size = %d bytes.\n", size);
	}

	std::printf("\n");
	std::printf("- End -\n");

	return EXIT_SUCCESS;
}

// End of file
