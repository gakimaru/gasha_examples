#pragma once
#ifndef GASHA_INCLUDED_ABILITY_DATA_INL
#define GASHA_INCLUDED_ABILITY_DATA_INL

//--------------------------------------------------------------------------------
// ability_data.inl
// アビリティデータ【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/ability_data.h"//アビリティデータ【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アビリティデータ構造体

//キー
inline crc32_t abilityData::key() const
{
	return m_id.crc();
}

//名前
inline const char* abilityData::name() const
{
	return m_name.c_str();
}

//コンストラクタ
inline abilityData::abilityData(const char* id, const char* name, const int atk, const int def) :
	m_id(id),
	m_name(name),
	m_basic(atk, def)
{}

//デフォルトコンストラクタ
inline abilityData::abilityData()
{}

//デストラクタ
inline abilityData::~abilityData()
{}

#endif//GASHA_INCLUDED_ABILITY_DATA_INL

// End of file
