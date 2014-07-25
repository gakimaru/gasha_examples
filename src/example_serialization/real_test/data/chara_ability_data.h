#pragma once
#ifndef GASHA_INCLUDED_CHARA_ABILITY_DATA_H
#define GASHA_INCLUDED_CHARA_ABILITY_DATA_H

//--------------------------------------------------------------------------------
// chara_ability_data.h
// キャラアビリティデータ【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//クラス宣言
struct abilityData;

//--------------------
//キャラ習得アビリティデータ構造体
struct charaAbilityData
{
	mutable const charaAbilityData* m_next;//連結リスト
	abilityData* m_ability;//アビリティ

	//singlyLinkedList 用の演算オペレータ（ダミー）
	inline bool operator<(const charaAbilityData& rhs) const { return true; }
};

//--------------------
//キャラ習得アビリティデータ構造体；シリアライズ用
struct charaAbilityData_temp
{
	//crc32_t m_charaId;//キャラID
	crc32_t m_abilityId;//アビリティID
};

//--------------------
//キャラ習得アビリティプールクラス
class charaAbilityPool
{
public:
	//定数
	static const std::size_t POOL_SIZE = 100;//最大プール数
public:
	//メソッド
	inline charaAbilityData* alloc();//アロケート
	inline void free(charaAbilityData* data);//解放
public:
	//コンストラクタ
	inline charaAbilityPool();
	//デストラクタ
	inline ~charaAbilityPool();
private:
	//フィールド
	poolAllocator_withType<charaAbilityData, POOL_SIZE> m_pool;//データ用のバッファ
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/chara_ability_data.inl"

#endif//GASHA_INCLUDED_CHARA_ABILITY_DATA_H

// End of file
