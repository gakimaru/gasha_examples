#pragma once
#ifndef GASHA_INCLUDED_ABILITY_DATA_H
#define GASHA_INCLUDED_ABILITY_DATA_H

//--------------------------------------------------------------------------------
// ability_data.h
// アビリティデータ【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/basic_data.h"//基本データ型

#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言
#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アビリティデータ構造体
struct abilityData
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();

	//アクセッサ
	inline crc32_t key() const;//キー
	inline const char* name() const;//名前
	
	//コンストラクタ
	inline abilityData(const char* id, const char* name, const int atk, const int def);
	//デフォルトコンストラクタ
	inline abilityData();
	//デストラクタ
	inline ~abilityData();

	//フィールド
	id_type m_id;//ID
	name_type m_name;//名前
	basicData m_basic;//基本データ
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/ability_data.inl"

#endif//GASHA_INCLUDED_ABILITY_DATA_H

// End of file
