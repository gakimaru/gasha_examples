#pragma once
#ifndef GASHA_INCLUDED_ASVE_LOAD_H
#define GASHA_INCLUDED_ASVE_LOAD_H

//--------------------------------------------------------------------------------
// save_load.h
// シリアライズテスト：機能テスト：セーブ／ロード処理【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト
#include "feature_test/feature_test.h"//シリアライズテスト：機能テスト

namespace for_feature_test
{
	//--------------------
	//バイナリ形式セーブデータイメージ作成（シリアライズ）
	//※セーブデータのバッファとワークバッファを受け渡し、セーブデータのバッファにセーブデータイメージを格納して返す。
	//※セーブデータサイズを返す（作成に失敗したら 0 を返す）。
	std::size_t makeBinarySaveDataImage(void* save_data, const std::size_t save_data_max_size, void* work_buff, const std::size_t work_buff_max_size);

	//--------------------
	//テキスト形式セーブデータイメージ作成（シリアライズ）
	//※セーブデータのバッファとワークバッファを受け渡し、セーブデータのバッファにセーブデータイメージを格納して返す。
	//※セーブデータサイズを返す（作成に失敗したら 0 を返す）。
	std::size_t makeTextSaveDataImage(void* save_data, const std::size_t save_data_max_size, void* work_buff, const std::size_t work_buff_max_size);

	//--------------------
	//バイナリ形式セーブデータイメージ読み込み（デシリアライズ）
	//※セーブデータイメージとバッファとワークバッファを受け渡し、セーブデータをロードする。
	//※成功したらtrueを返す。
	bool loadBinarySaveDataImage(void* save_data, const std::size_t save_data_size, void* work_buff, const std::size_t work_buff_max_size);

#if 0//未実装
	//--------------------
	//テキスト形式セーブデータイメージ読み込み（デシリアライズ）
	//※セーブデータイメージとバッファとワークバッファを受け渡し、セーブデータをロードする。
	//※成功したらtrueを返す。
	bool loadTextSaveDataImage(void* save_data, const std::size_t save_data_size, void* work_buff, const std::size_t work_buff_max_size);
#endif//未実装

}//namespace for_feature_test

#endif//GASHA_INCLUDED_ASVE_LOAD_H

// End of file
