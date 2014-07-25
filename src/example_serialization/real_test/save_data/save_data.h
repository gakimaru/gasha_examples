#pragma once
#ifndef GASHA_INCLUDED_SAVE_DATA_H
#define GASHA_INCLUDED_SAVE_DATA_H

//--------------------------------------------------------------------------------
// save_data.h
// セーブデータ【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_serialization.h"//シリアライズテスト

#include <cstddef>//std::size_t

namespace for_real_test
{
	//--------------------
	//セーブデータバージョンを表示
	void printSaveDataVersion();

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

}//namespace for_real_test

#endif//GASHA_INCLUDED_SAVE_DATA_H

// End of file
