#pragma once
#ifndef GASHA_INCLUDED_CHARA_ABILITY_DATA_INL
#define GASHA_INCLUDED_CHARA_ABILITY_DATA_INL

//--------------------------------------------------------------------------------
// chara_ability_data.inl
// キャラアビリティデータ【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "chara_ability_data.h"//キャラアビリティデータ【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//キャラ習得アビリティプールクラス

//アロケート
inline charaAbilityData* charaAbilityPool::alloc()
{
	return m_pool.newDefault();
}

//解放
inline void charaAbilityPool::free(charaAbilityData* data)
{
	m_pool.deleteDefault(data);
}

//コンストラクタ
inline charaAbilityPool::charaAbilityPool()
{}

//デストラクタ
inline charaAbilityPool::~charaAbilityPool()
{}

#endif//GASHA_INCLUDED_CHARA_ABILITY_DATA_INL

// End of file
