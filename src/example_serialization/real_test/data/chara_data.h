#pragma once
#ifndef GASHA_INCLUDED_CHARA_DATA_H
#define GASHA_INCLUDED_CHARA_DATA_H

//--------------------------------------------------------------------------------
// chara_data.h
// キャラデータ【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/basic_data.h"//基本データ型
#include "real_test/data/chara_ability_data.h"//キャラ習得アビリティデータ型

#include <gasha/singly_linked_list.h>//片方向連結リスト
#include <gasha/crc32.h>//CRC32計算
#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//クラス宣言
struct itemData;
struct abilityData;

//--------------------
//キャラデータ構造体
struct charaData
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();
	
	//型
	struct charaAbilityListOpe : public singly_linked_list::baseOpe<charaAbilityListOpe, charaAbilityData>
	{
		inline static const node_type* getNext(const node_type& node){ return node.m_next; }//次ノードを取得
		inline static void setNext(node_type& node, const node_type* next){ node.m_next = next; }//次ノードを変更
	};
	typedef singly_linked_list::container<charaAbilityListOpe> charaAbilityList;//キャラ習得アビリティリスト型

	//アクセッサ
	inline crc32_t key() const;//キー
	inline const char* name() const;//名前
	
	//メソッド
	void setWeapon(const crc32_t weapon_id);//武器をセット
	inline void setWeapon(const char* weapon_id);//武器をセット
	void setShield(const crc32_t shield_id);//盾をセット
	inline void setShield(const char* shield_id);//盾をセット
	void attachItems();//アイテムを参照し直す
	void addAbility(const crc32_t ability_id);//アビリティを追加
	inline void addAbility(const char* ability_id);//アビリティを追加
	abilityData* getAbility(const int index);//アビリティを取得
	
	//コンストラクタ
	charaData(const char* id, const char* name, const int level, const int atk, const int def, const char* weapon_id, const char* shield_id);
	//デフォルトコンストラクタ
	inline charaData();
	//デストラクタ
	inline ~charaData();
	
	//フィールド
	id_type m_id;//ID
	name_type m_name;//名前
	char m_level;//レベル
	basicData m_basic;//基本データ
	const itemData* m_weapon;//武器
	const itemData* m_shield;//盾
private:
	//フィールド（プライベート）
	crc32_t m_weaponId;//武器ID
	crc32_t m_shieldId;//盾ID
	charaAbilityList m_abilities;//キャラ習得アビリティ
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/chara_data.inl"

#endif//GASHA_INCLUDED_CHARA_DATA_H

// End of file
