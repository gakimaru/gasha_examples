//--------------------------------------------------------------------------------
// chara_data.cpp
// キャラデータ【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/chara_data.inl"//キャラデータ【インライン関数／テンプレート関数定義部】

#include "real_test/data/inventory.h"//所持アイテムデータ
#include "real_test/data/ability_collection.h"//アビリティコレクション
#include "real_test/data/chara_ability_data.h"//キャラアビリティデータ

#include <gasha/singleton.h>//シングルトン

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//キャラデータ構造体

//武器をセット
void charaData::setWeapon(const crc32_t weapon_id)
{
	simpleSingleton<inventory> inventory("charaData::setWeapon");
	itemData* item = inventory->find(weapon_id);//イベントリから検索
	if (!item)
	{
		//対象アイテムがないので持っていないことにする
		m_weaponId = 0;
		m_weapon = nullptr;
		return;
	}
	m_weaponId = weapon_id;
	m_weapon = item;
}
//盾をセット
void charaData::setShield(const crc32_t shield_id)
{
	simpleSingleton<inventory> inventory("charaData::setShield");
	itemData* item = inventory->find(shield_id);//イベントリから検索
	if (!item)
	{
		//対象アイテムがないので持っていないことにする
		m_shieldId = 0;
		m_shield = nullptr;
		return;
	}
	m_shieldId = shield_id;
	m_shield = item;
}
//アイテムを参照し直す
void charaData::attachItems()
{
	setWeapon(m_weaponId);//武器
	setShield(m_shieldId);//盾
}
//キャラ習得アビリティを追加
void charaData::addAbility(const crc32_t ability_id)
{
	simpleSingleton<abilityCollection> ability_coll("charaData::addAbility");
	abilityData* ability = ability_coll->find(ability_id);//アビリティリストから検索
	if (!ability)//アビリティがなければ終了
		return;
	//キャラ習得アビリティのバッファを割り当て
	simpleSingleton<charaAbilityPool> chara_ability_pool("charaData::addAbility");
	charaAbilityData* new_chara_ability = chara_ability_pool->alloc();
	if (!new_chara_ability)//バッファが足りなければ失敗
		return;
	//バッファにアビリティをセットして連結リストの最後に連結
	new_chara_ability->m_ability = ability;
	m_abilities.push_back(*new_chara_ability);
}
//キャラ習得アビリティを取得
abilityData* charaData::getAbility(const int index)
{
	charaAbilityData* chara_ability = m_abilities.at(index);
	if (!chara_ability)
		return nullptr;
	return chara_ability->m_ability;
}

//コンストラクタ
charaData::charaData(const char* id, const char* name, const int level, const int atk, const int def, const char* weapon_id, const char* shield_id) :
	m_id(id),
	m_name(name),
	m_level(level),
	m_basic(atk, def),
	m_weapon(nullptr),
	m_shield(nullptr),
	m_abilities()
{
	setWeapon(weapon_id);
	setShield(shield_id);
}

//--------------------
//キャラデータの明示的なインスタンス化

#include <gasha/singly_linked_list.cpp.h>//片方向連結リスト【関数／実体定義部】

GASHA_INSTANCING_slList(charaData::charaAbilityListOpe);

// End of file
