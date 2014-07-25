#pragma once
#ifndef GASHA_INCLUDED_CHARA_DATA_INL
#define GASHA_INCLUDED_CHARA_DATA_INL

//--------------------------------------------------------------------------------
// chara_data.inl
// キャラデータ【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/chara_data.h"//キャラデータ【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//キャラデータ構造体

//キー
inline crc32_t charaData::key() const
{
	return m_id.crc();
}

//名前
inline const char* charaData::name() const
{
	return m_name.c_str();
}

//武器をセット
inline void charaData::setWeapon(const char* weapon_id)
{
	setWeapon(calcCRC32(weapon_id));
}

//盾をセット
inline void charaData::setShield(const char* shield_id)
{
	setShield(calcCRC32(shield_id));
}

//アビリティを追加
inline void charaData::addAbility(const char* ability_id)
{
	addAbility(calcCRC32(ability_id));
}

//デフォルトコンストラクタ
inline charaData::charaData() :
	m_abilities()
{}

//デストラクタ
inline charaData::~charaData()
{}

#endif//GASHA_INCLUDED_CHARA_DATA_INL

// End of file
