//--------------------------------------------------------------------------------
// real_test.cpp
// 実ゲーム処理テスト【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/real_test.h"//実ゲーム処理テスト【インライン関数／テンプレート関数定義部】

#include "real_test/test_data/test_data.h"//テストデータ
#include "real_test/save_data/save_data.h"//セーブデータ
#include "real_test/save_data/load_target.h"//ロード対象（部分ロード設定）

#include "work_buff.h"//ワークバッファ
#include "file.h"//ファイル操作

#include <gasha/singleton.h>//シングルトン

GASHA_USING_NAMESPACE;//ネームスペース使用
using namespace for_real_test;//ネームスペース使用

//--------------------
//ファイルパス
static const char* BINARY_SAVE_DATA_FILE_PATH = "data/example_serialization/real_test_data.bin";//バイナリ形式セーブデータファイルパス
static const char* TEXT_SAVE_DATA_FILE_PATH = "data/example_serialization/real_test_data.json";//テキスト（json）形式セーブデータファイルパス

//--------------------
//バイナリ形式セーブデータをセーブ
static void saveBinary()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);

	//セーブデータ作成（シリアライズ）
	const std::size_t save_data_size = makeBinarySaveDataImage(save_data_buff, save_data_buff_size, work_buff, work_buff_size);
	if (save_data_size == 0)//失敗判定
		return;

	//ファイル保存
	writeFile(BINARY_SAVE_DATA_FILE_PATH, save_data_buff, save_data_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

//--------------------
//テキスト形式セーブデータをセーブ
static void saveText()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);

	//セーブデータ作成（シリアライズ）
	const std::size_t save_data_size = makeTextSaveDataImage(save_data_buff, save_data_buff_size, work_buff, work_buff_size);
	if (save_data_size == 0)//失敗判定
		return;

	//ファイル保存
	writeFile(TEXT_SAVE_DATA_FILE_PATH, save_data_buff, save_data_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

//--------------------
//バイナリ形式セーブデータをロード
static void loadBinary()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);

	//ファイル読み込み
	const std::size_t save_data_size = readFile(BINARY_SAVE_DATA_FILE_PATH, save_data_buff, save_data_buff_size);

	//セーブデータ読み込み（デシリアライズ）
	loadBinarySaveDataImage(save_data_buff, save_data_size, work_buff, work_buff_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}

#if 0//未実装
//--------------------
//テキスト形式セーブデータをロード
static void loadText()
{
	//セーブデータ用バッファ取得
	std::size_t save_data_buff_size = 0;
	void* save_data_buff = getWrokBuff(save_data_buff_size);

	//ワークバッファ取得
	std::size_t work_buff_size = 0;
	void* work_buff = getWrokBuff(work_buff_size);

	//ファイル読み込み
	const std::size_t save_data_size = readFile(TEXT_SAVE_DATA_FILE_PATH, save_data_buff, save_data_buff_size);

	//セーブデータ読み込み（デシリアライズ）
	loadTextSaveDataImage(save_data_buff, save_data_size, work_buff, work_buff_size);

	//バッファ解放
	releaseWorkBuff(save_data_buff);
	releaseWorkBuff(work_buff);
}
#endif

//--------------------
//実ゲーム処理テスト
void real_test()
{
	//-----
	//データを作成してセーブ
	
	//全データリセット
	resetAll();
	
	//テストデータ作成(pattern=0)
	makeTestData(0);
	
	//現在のデータを表示
	printDataAll();
	
	//セーブデータ用バージョンを表示
	printSaveDataVersion();
	
	//セーブ（シリアライズ＆ファイル書き込み）
	saveBinary();
	saveText();

	//セーブデータ用バージョンを表示
	printSaveDataVersion();

	//-----
	//データをリセットしロード

	//全データリセット
	resetAll();
	
	//現在のデータを表示
	printDataAll();
	
	//ロード
	loadBinary();

	//セーブデータ用バージョンを表示
	printSaveDataVersion();
	
	//現在のデータを表示
	printDataAll();

	//-----
	//部分ロードのテストのために、セーブデータと異なるデータを作成

	//全データリセット
	resetAll();
	
	//テストデータ作成(pattern=1)
	makeTestData(1);
	
	//現在のデータを表示
	printDataAll();

	//-----
	//部分ロード処理
	simpleSingleton<loadTarget> load_target("real_test");

	//部分ロード指定
	//load_target->setLoadTarget("gameStatus");
	load_target->setLoadTarget("chara");

	//ロード（ファイル読み込み＆デシリアライズ）
	loadBinary();
	
	//現在のデータを表示
	printDataAll();
	
	//部分ロード指定解除
	load_target->resetLoadTarget();
}

// End of file
