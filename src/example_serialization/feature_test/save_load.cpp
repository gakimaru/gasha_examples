//--------------------------------------------------------------------------------
// save_load.cpp
// シリアライズテスト：機能テスト：セーブ／ロード処理
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト
#include "feature_test/save_load.h"//セーブ／ロード処理
#include "feature_test/save_load_def.cpp.h"//セーブ／ロード用処理定義

#include <gasha/archive/output_binary_archive.h>//バイナリ形式アーカイブ書き込み
#include <gasha/archive/input_binary_archive.h>//バイナリ形式アーカイブ読み込み
#include <gasha/archive/output_text_archive.h>//テキスト形式アーカイブ書き込み
//#include <gasha/archive/input_text_archive.h>//テキスト形式アーカイブ読み込み（未実装）
                                              //※シリアライズデータの保存形式に合わせて適切なファイルをインクルードする

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//処理結果表示
void printResult(const bool has_fatal_error, const std::size_t save_data_size, const std::size_t peak_work_size)
{
	std::printf("Result:\n");
	std::printf("  Fatal-error:    %s\n", has_fatal_error ? "Occured!" : "nothing");
	std::printf("  Save data size: %d\n", save_data_size);
	std::printf("  Used work-buffer size(peak): %d\n", peak_work_size);
}
void printResult(const archive::results& result)
{
	std::printf("  numSmallerSizeItem()=%d\n", result.numSmallerSizeItem());//サイズが縮小されたデータ項目の数を取得
	std::printf("  numLargerSizeItem()=%d\n", result.numLargerSizeItem());//サイズが拡大されたデータ項目の数を取得
	std::printf("  numSmallerArrItem()=%d\n", result.numSmallerArrItem());//配列要素数が縮小されたデータ項目の数を取得
	std::printf("  numLargerArrItem()=%d\n", result.numLargerArrItem());//配列要素数が拡大されたデータ項目の数を取得
	std::printf("  numOnlyOnSaveData()=%d\n", result.numOnlyOnSaveData());//セーブデータ上にのみ存在するデータ項目の数を取得
	std::printf("  numOnlyOnMem()=%d\n", result.numOnlyOnMem());//セーブデータ上にないデータ項目の数を取得
	std::printf("  numObjOnSaveDataOnly()=%d\n", result.numObjOnSaveDataOnly());//現在オブジェクト型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  numObjOnMemOnly()=%d\n", result.numObjOnMemOnly());//現在オブジェクト型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  numArrOnSaveDataOnly()=%d\n", result.numArrOnSaveDataOnly());//現在配列型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  numArrOnMemOnly()=%d\n", result.numArrOnMemOnly());//現在配列型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  numPtrOnSaveDataOnly()=%d\n", result.numPtrOnSaveDataOnly());//現在ポインタ型ではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  numPtrOnMemOnly()=%d\n", result.numPtrOnMemOnly());//現在ポインタ型だが、セーブデータ上ではそうではなかったデータ項目の数を取得
	std::printf("  numNulOnSaveDataOnly()=%d\n", result.numNulOnSaveDataOnly());//現在ヌルではないが、セーブデータ上ではそうだったデータ項目の数を取得
	std::printf("  numNulOnMemOnly()=%d\n", result.numNulOnMemOnly());//現在ヌルだが、セーブデータ上ではそうではなかったデータ項目の数を取得
}

//--------------------
//バイナリ形式セーブデータイメージ作成（シリアライズ）
std::size_t makeBinarySaveDataImage(void* save_data, const std::size_t save_data_max_size, void* work_buff, const std::size_t work_buff_max_size)
{
	std::printf("-------------------- [ makeBinarySaveDataImage ]\n");

	//シリアライズ
	GASHA_ archive::outputBinaryArchive arc(save_data, save_data_max_size, work_buff, work_buff_max_size);
	arc << GASHA_ serialization::pair<saveData>("SaveData");//シリアライズ
	printResult(arc.hasFatalError(), arc.saveDataSize(), arc.peakWorkSize());
	return arc.saveDataSize();
}

//--------------------
//テキスト形式セーブデータイメージ作成（シリアライズ）
std::size_t makeTextSaveDataImage(void* save_data, const std::size_t save_data_max_size, void* work_buff, const std::size_t work_buff_max_size)
{
	std::printf("-------------------- [ makeTextSaveDataImage ]\n");

	//シリアライズ
	GASHA_ archive::outputTextArchive arc(save_data, save_data_max_size, work_buff, work_buff_max_size);
	arc << GASHA_ serialization::pair<saveData>("SaveData");//シリアライズ
	printResult(arc.hasFatalError(), arc.saveDataSize(), arc.peakWorkSize());
	return arc.saveDataSize();
}

//--------------------
//バイナリ形式セーブデータイメージ読み込み（デシリアライズ）
bool loadBinarySaveDataImage(void* save_data, const std::size_t save_data_size, void* work_buff, const std::size_t work_buff_max_size)
{
	std::printf("-------------------- [ loadBinarySaveDataImage ]\n");

	//デシリアライズ
	GASHA_ archive::inputBinaryArchive arc(save_data, save_data_size, work_buff, work_buff_max_size);
	arc >> GASHA_ serialization::pair<saveData>("SaveData");//デシリアライズ
	printResult(arc.hasFatalError(), arc.saveDataSize(), arc.peakWorkSize());
	printResult(arc.result());
	return !arc.hasFatalError();
}

#if 0//未実装
//--------------------
//テキスト形式セーブデータイメージ読み込み（デシリアライズ）
bool loadTextSaveDataImage(void* save_data, const std::size_t save_data_size, void* work_buff, const std::size_t work_buff_max_size)
{
	std::printf("-------------------- [ loadTextSaveDataImage ]\n");

	//デシリアライズ
	GASHA_ archive::inputTextArchive arc(save_data, save_data_size, work_buff, work_buff_max_size);
	arc >> GASHA_ serialization::pair<saveData>("SaveData");//デシリアライズ
	printResult(arc.hasFatalError(), arc.saveDataSize(), arc.peakWorkSize());
	printResult(arc.result());
	return !arc.hasFatalError();
}
#endif//未実装

#include <cstdio>//std::printf()

// End of file
