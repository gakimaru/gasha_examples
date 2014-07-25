//--------------------------------------------------------------------------------
// test_data.cpp
// テストデータ【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/test_data/test_data.h"//テストデータ【宣言部】

#include "real_test/data/inventory.h"//所持アイテム
#include "real_test/data/ability_collection.h"//アビリティコレクション
#include "real_test/data/chara_collection.h"//キャラコレクション
#include "real_test/data/game_status.h"//ゲーム状態

#include <gasha/singleton.h>//シングルトン
#include <gasha/string.h>//文字列処理
#include <gasha/type_traits.h>//型特性ユーティリティ

#include <cstdio>//std::printf()

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//テストデータ用データ確認処理

//--------------------
//全データリセット
void resetAll()
{
	std::printf("------------------------------------------------------------\n");
	std::printf("【全データリセット】\n");

	static const char* procedure_name = "resetAll";

	//インベントリデータ破棄
	{
		simpleSingleton<inventory> inventory(procedure_name);
		inventory.destroy(procedure_name);
	}
	//アビリティデータ破棄
	{
		simpleSingleton<abilityCollection> ability_collection(procedure_name);
		ability_collection.destroy(procedure_name);
	}
	//キャラデータ破棄
	{
		simpleSingleton<charaCollection> chara_collection(procedure_name);
		chara_collection.destroy(procedure_name);
	}
	//キャラアビリティデータプール破棄
	{
		simpleSingleton<charaAbilityPool> chara_ability_pool(procedure_name);
		chara_ability_pool.destroy(procedure_name);
	}
	//フェーズ＆フラグデータ破棄
	{
		simpleSingleton<gameStatus> game_status(procedure_name);
		game_status.destroy(procedure_name);
	}
}

//--------------------
//テストデータ作成
void makeTestData(const int pattern)
{
	std::printf("------------------------------------------------------------\n");
	std::printf("【テストデータ作成】(pattern=%d)\n", pattern);

	static const char* procedure_name = "makeTestData";

	//インベントリデータ登録
	{
		simpleSingleton<inventory> inventory(procedure_name);
		//武器登録
		for (int i = 0; i < 5 + pattern * 10; i += (1 + pattern))
		{
			char id[8];
			char name[32];
			spnprintf(id, sizeof(id), "w%05d", (i + 1) * 10);
			spnprintf(name, sizeof(name), "武器%03d", i + 1);
			itemData item(id, name, 10 + i, i / 2, 0, 1);
			if (item.m_title && i > 0)
			{
				char buf[16];
				spnprintf(buf, sizeof(buf), "性能:+%d", i);
				item.setTitle(buf);
			}
			else
				item.setTitle(nullptr);
			inventory->regist(item);
		}
		//盾登録
		for (int i = 0; i < 5 + pattern * 10; i += (1 + pattern))
		{
			char id[8];
			char name[32];
			spnprintf(id, sizeof(id), "s%05d", (i + 1) * 10);
			spnprintf(name, sizeof(name), "盾%03d", i + 1);
			itemData item(id, name, 0, 5 + i, 0, 1);
			if (item.m_title && i > 0)
			{
				char buf[16];
				spnprintf(buf, sizeof(buf), "グレード:%c", 'A' + i);
				item.setTitle(buf);
			}
			else
				item.setTitle(nullptr);
			inventory->regist(item);
		}
		//回復薬登録
		for (int i = pattern; i < 3 + pattern * 3; ++i)
		{
			char id[8];
			char name[32];
			spnprintf(id, sizeof(id), "r%05d", (i + 1) * 10);
			spnprintf(name, sizeof(name), "回復薬%03d", i + 1);
			itemData item(id, name, 0, 0, 5 + i * 2, 10);
			item.setTitle(nullptr);
			inventory->regist(item);
		}
		inventory->sort();
	}
	//アビリティデータ登録
	{
		simpleSingleton<abilityCollection> ability_collection(procedure_name);
		for (int i = pattern; i < 10 + pattern * 10; ++i)
		{
			char id[8];
			char name[32];
			spnprintf(id, sizeof(id), "a%05d", (i + 1) * 10);
			spnprintf(name, sizeof(name), "特技%03d", i + 1);
			abilityData ability(id, name, 5 + i * 3, 4 + i * 2);
			ability_collection->regist(ability);
		}
		ability_collection->sort();
	}
	//キャラデータ登録
	{
		simpleSingleton<charaCollection> chara_collection(procedure_name);
		if (pattern & 1)
		{
			charaData chara1("c00010", "たろう", 11, 15, 20, "w00030", "s00030");
			charaData chara2("c00020", "じろう", 21, 25, 40, "w00050", "s00010");
			charaData chara3("c00030", "さぶろう", 31, 55, 3, "w00070", "s00050");
			charaData chara4("c00040", "しろう", 41, 55, 3, "w00090", "s00090");
			chara1.addAbility("a00030");
			chara1.addAbility("a00010");
			chara3.addAbility("a00050");
			chara3.addAbility("a00010");
			chara3.addAbility("a00040");
			chara4.addAbility("a00020");
			chara_collection->regist(chara1);
			chara_collection->regist(chara2);
			chara_collection->regist(chara3);
			chara_collection->regist(chara4);
			chara_collection->sort(true);
		}
		else
		{
			charaData chara1("c00010", "たろう", 10, 15, 20, "w00020", "s00020");
			charaData chara2("c00020", "じろう", 20, 25, 40, "w00050", "s00010");
			charaData chara3("c00030", "さぶろう", 30, 55, 3, "w00010", "s00030");
			chara1.addAbility("a00030");
			chara1.addAbility("a00020");
			chara1.addAbility("a00050");
			chara1.addAbility("a00010");
			chara3.addAbility("a00050");
			chara_collection->regist(chara1);
			chara_collection->regist(chara2);
			chara_collection->regist(chara3);
			chara_collection->sort();
		}
	}
	//進行データ更新
	{
		simpleSingleton<gameStatus> game_status(procedure_name);
		if (pattern & 1)
		{
			game_status->setPhase(3 + pattern);
			game_status->setFlag("flag A", true);
			game_status->setFlag("flag B", true);
			game_status->setFlag("flag Z", true);
		}
		else
		{
			game_status->setPhase(7 + pattern);
			game_status->setFlag("flag C", true);
			game_status->setFlag("flag D", true);
			game_status->setFlag("flag X", true);
			game_status->setFlag("flag Y", true);
		}
	}
}

//--------------------
//現在のデータを表示
void printDataAll()
{
	std::printf("------------------------------------------------------------\n");
	std::printf("【現在のデータを表示】\n");

	static const char* procedure_name = "printDataAll";

	//インベントリデータ表示
	std::printf("--- インベントリ ---\n");
	{
		simpleSingleton<inventory> inventory(procedure_name);
		for (auto& item_data : *inventory)
		{
			std::printf("ID=\"%s\"(0x%08x), name=\"%s\", atk=%d, def=%d, recover=%d, num=%d\n",
				item_data->m_id.c_str(), item_data->m_id.crc(), item_data->m_name.c_str(), item_data->m_basic.m_atk, item_data->m_basic.m_def, item_data->m_recover, item_data->m_num);
			if (item_data->m_title)
				std::printf("                              title=\"%s\"\n", item_data->m_title);
		}
	}
	//アビリティデータ表示
	std::printf("--- アビリティ ---\n");
	{
		simpleSingleton<abilityCollection> ability_collection(procedure_name);
		for (auto& ability_data : *ability_collection)
		{
			std::printf("ID=\"%s\"(0x%08x), name=\"%s\", atk=%d, def=%d\n",
				ability_data->m_id.c_str(), ability_data->m_id.crc(), ability_data->m_name.c_str(), ability_data->m_basic.m_atk, ability_data->m_basic.m_def);
		}
	}
	//キャラデータ表示
	std::printf("--- キャラ ---\n");
	{
		simpleSingleton<charaCollection> chara_collection(procedure_name);
		for (auto& chara_data : *chara_collection)
		{
			std::printf("ID=\"%s\"(0x%08x), name=\"%s\", level=%d, atk=%d, def=%d\n",
				chara_data->m_id.c_str(), chara_data->m_id.crc(), chara_data->m_name.c_str(), chara_data->m_level, chara_data->m_basic.m_atk, chara_data->m_basic.m_def);
			if (chara_data->m_weapon)
				std::printf("                              weapon=%s\n", chara_data->m_weapon->m_name.c_str());
			if (chara_data->m_shield)
				std::printf("                              shield=%s\n", chara_data->m_shield->m_name.c_str());
			for (int i = 0;; ++i)
			{
				const abilityData* ability = chara_data->getAbility(i);
				if (!ability)
					break;
				std::printf("                              [%s]\n", ability->m_name.c_str());
			}
		}
	}
	//ゲーム状態表示
	std::printf("--- ゲーム状態 ---\n");
	{
		simpleSingleton<gameStatus> game_status(procedure_name);
		std::printf("pahse=%d\n", game_status->phase());
		std::printf("flags:\n");
		for (auto& flag_info : *game_status)
		{
			std::printf("  [%2d]\"%s\"(0x%08x)=%s\n",
				flag_info.m_index, flag_info.m_name, flag_info.m_nameCrc, toStr(game_status->flag(flag_info.m_nameCrc)));
		}
	}
}

//--------------------
//データ操作テスト
void testDataControl()
{
	//全データリセット
	resetAll();
	//現在のデータを表示
	printDataAll();
	//テストデータ作成(pattern=0)
	makeTestData(0);
	//現在のデータを表示
	printDataAll();
	//全データリセット
	resetAll();
	//テストデータ作成(pattern=1)
	makeTestData(1);
	//現在のデータを表示
	printDataAll();
}

// End of file
