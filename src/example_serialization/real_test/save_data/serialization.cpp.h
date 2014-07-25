#pragma once
#ifndef GASHA_INCLUDED_SERIALIZATION_CPP_H
#define GASHA_INCLUDED_SERIALIZATION_CPP_H

//--------------------------------------------------------------------------------
// serialization.cpp.h
// シリアライズ【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/inventory.h"//所持アイテム
#include "real_test/data/ability_collection.h"//アビリティコレクション
#include "real_test/data/chara_collection.h"//キャラコレクション
#include "real_test/data/game_status.h"//ゲーム状態
#include "real_test/save_data/save_data_version.h"//セーブデータバージョン
#include "real_test/save_data/load_target.h"//ロード対象

#include <gasha/serialization.h>//シリアライズ
#include <gasha/singleton.h>//シングルトン
#include <gasha/iterator.h>//イテレータ操作

namespace for_real_test
{
	//--------------------
	//セーブデータ作成用クラス
	//※空のクラス
	class saveData{};
}//namespace for_real_test

GASHA_USING_NAMESPACE;//ネームスペース使用
using namespace for_real_test;//ネームスペース使用

//--------------------
//シリアライズ対象データ構造にバージョンを設定
GASHA_SERIALIZATION_VERSION_DEF(saveData, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(saveDataVersion, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(itemData, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(abilityData, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(charaData, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(charaAbilityData, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(gameStatus, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(id_type, 1, 0);
GASHA_SERIALIZATION_VERSION_DEF(name_type, 1, 0);

//--------------------
//テスト用クラスのシリアライズ処理定義
GASHA_NAMESPACE_BEGIN;//ネームスペース：開始
namespace serialization
{
	//--------------------
	//シリアライズ処理：std::bitset<>
	template<class ARCHIVE, std::size_t N>
	struct serialize<ARCHIVE, std::bitset<N> > {
		void operator()(ARCHIVE& arc, const std::bitset<N>& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pairBin("bitset", obj);//オブジェクト全体をバイトデータとして扱う
		}
	};
	//--------------------
	//シリアライズ処理：fixedLenStr<>
	template<class ARCHIVE, std::size_t _MAX_SIZE>
	struct serialize<ARCHIVE, fixedLenStr<_MAX_SIZE> > {
		void operator()(ARCHIVE& arc, const fixedLenStr<_MAX_SIZE>& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("len", obj.m_len);
			arc & pairStr("str", obj.m_str);//文字列データとして扱う
		}
	};
	//--------------------
	//シリアライズ処理：fixedLenStrExt<>
	template<class ARCHIVE, std::size_t _MAX_SIZE>
	struct serialize<ARCHIVE, fixedLenStrExt<_MAX_SIZE> > {
		void operator()(ARCHIVE& arc, const fixedLenStrExt<_MAX_SIZE>& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair<typename fixedLenStrExt<_MAX_SIZE>::parent_type>("fixedLenStr", obj);//親クラスのシリアライズ
			arc & pair("crc", obj.m_crc);
		}
	};
	//--------------------
	//シリアライズ処理：basicData
	template<class ARCHIVE>
	struct serialize<ARCHIVE, basicData> {
		void operator()(ARCHIVE& arc, const basicData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("atk", obj.m_atk);
			arc & pair("def", obj.m_def);
		}
	};
	//--------------------
	//シリアライズ処理：itemData
	template<class ARCHIVE>
	struct serialize<ARCHIVE, itemData> {
		void operator()(ARCHIVE& arc, const itemData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("id", obj.m_id);
			arc & pair("name", obj.m_name);
			arc & pairStr("title", obj.m_title, obj.titleMaxSize());//可変長文字列（ポインタ） ※バッファサイズを渡す
			//arc & pairStr("title", obj.m_title);//可変長文字列（ポインタ） ※バッファサイズを渡さなければ、セーブデータのサイズ分読み込む（バッファサイズをオーバーする可能性がある）
			//arc & pairArray("title", obj.m_title, obj.titleMaxSize());//配列として扱うことも可
			arc & pair("basic", obj.m_basic);
			arc & pair("recover", obj.m_recover);
			arc & pair("num", obj.m_num);
		}
	};
	//--------------------
	//ロード後処理：itemData
	template<class ARCHIVE>
	struct afterLoad<ARCHIVE, itemData> {
		void operator()(ARCHIVE& arc, itemData& obj, const version& ver, const version& now_ver)
		{
			if (obj.m_title && obj.m_title[0] == '\0')//空文字列の称号はnullptrにする
				obj.m_title = nullptr;
		}
	};
	//--------------------
	//シリアライズ処理：abilityData
	template<class ARCHIVE>
	struct serialize<ARCHIVE, abilityData> {
		void operator()(ARCHIVE& arc, const abilityData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("id", obj.m_id);
			arc & pair("name", obj.m_name);
			arc & pair("basic", obj.m_basic);
		}
	};
	//--------------------
	//シリアライズ処理：charaData
	template<class ARCHIVE>
	struct serialize<ARCHIVE, charaData> {
		void operator()(ARCHIVE& arc, const charaData& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("id", obj.m_id);
			arc & pair("name", obj.m_name);
			arc & pair("level", obj.m_level);
			arc & pair("basic", obj.m_basic);
			arc & pair("weapon", obj.m_weaponId);
			arc & pair("shield", obj.m_shieldId);
			//下記の項目はセーブデータに記録しない
			//obj.m_weapon//itemData*
			//obj.m_shield//itemData*
			//obj.m_abilities//charaAbilityData*
		}
	};
	//--------------------
	//ロード後処理：charaData
	template<class ARCHIVE>
	struct afterLoad<ARCHIVE, charaData> {
		void operator()(ARCHIVE& arc, charaData& obj, const version& ver, const version& now_ver)
		{
			static const char* procedure_name = "afterLoad<charaData>";
			simpleSingleton<inventory> inventory(procedure_name);
			obj.m_weapon = inventory->find(obj.m_weaponId);//武器の参照（ポインタ）を割り当て
			obj.m_shield = inventory->find(obj.m_shieldId);//盾の参照（ポインタ）を割り当て
			obj.m_abilities.clear();//キャラ習得アビリティは一旦クリア
		}
	};
	//--------------------
	//収集処理：charaData
	template<class ARCHIVE>
	struct collector<ARCHIVE, charaData> {
		void operator()(ARCHIVE& arc, const charaData& obj, const version& ver)
		{
			forEach(obj.m_abilities, [&arc](const charaAbilityData& chara_ability)
				{
					//アビリティ情報取得
					abilityData* ability_data = chara_ability.m_ability;
					//セーブ用アビリティ情報作成
					//※一時的にローカル変数に記録した情報をセーブする
					charaAbilityData_temp chara_ability_data_for_save;
					//chara_ability_data_for_save.m_charaId = obj.m_id.key();//キャラID
					chara_ability_data_for_save.m_abilityId = ability_data->key();//アビリティID
					//シリアライズ
					arc << pair("charaAbility", chara_ability_data_for_save);
				}
			);
		}
	};
	//--------------------
	//分配処理：charaData
	template<class ARCHIVE>
	struct distributor<ARCHIVE, charaData> {
		void operator()(ARCHIVE& arc, charaData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			if (target_item == "charaAbility")//キャラ習得アビリティのデータか判定
			{
				//セーブ用アビリティ情報復元
				//※一時的にローカル変数に展開して実際のデータに反映させる
				charaAbilityData_temp chara_ability_data_for_save;
				//デシリアライズ
				arc >> pair("charaAbility", chara_ability_data_for_save);
				//アビリティ追加
				obj.addAbility(chara_ability_data_for_save.m_abilityId);
			}
		}
	};
	//--------------------
	//シリアライズ処理：charaAbilityData_temp
	template<class ARCHIVE>
	struct serialize<ARCHIVE, charaAbilityData_temp> {
		void operator()(ARCHIVE& arc, const charaAbilityData_temp& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("id", obj.m_abilityId);
		}
	};
	//--------------------
	//シリアライズ処理：gameStatus
	template<class ARCHIVE>
	struct serialize<ARCHIVE, gameStatus> {
		void operator()(ARCHIVE& arc, const gameStatus& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("phase", obj.m_phase);
			arc & pairBin("flagSet", obj.m_flagSet);
		}
	};
	//--------------------
	//シリアライズ処理：saveDataVersion
	template<class ARCHIVE>
	struct serialize<ARCHIVE, saveDataVersion> {
		void operator()(ARCHIVE& arc, const saveDataVersion& obj, const version& ver, const version& now_ver, const itemInfoBase* deserialize_obj)
		{
			arc & pair("major", obj.m_loadedMajorVer);
			arc & pair("minor", obj.m_loadedMinorVer);
			arc & pair("saveTime", obj.m_saveTime);
		}
	};
	//--------------------
	//収集処理：saveData
	template<class ARCHIVE>
	struct collector<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, const saveData& obj, const version& ver)
		{
			static const char* procedure_name = "collector<saveData>";
			//セーブデータバージョンのバージョンを保存用に更新
			simpleSingleton<saveDataVersion> save_data_version(procedure_name);
			save_data_version->updateLoadedVer();
			arc << pair("version", *save_data_version);//シリアライズ
			save_data_version->finishToSave();

			//インベントリのデータを収集
			simpleSingleton<inventory> inventory(procedure_name);
			for (auto item_data : *inventory)
			{
				//アイテムデータを一つずつシリアライズ
				arc << pair("item", *item_data);
			}
			//アビリティのデータを収集
			simpleSingleton<abilityCollection> ability_collection(procedure_name);
			for (auto ability_data : *ability_collection)
			{
				//アビリティデータを一つずつシリアライズ
				arc << pair("ability", *ability_data);
			}
			//キャラのデータを収集
			simpleSingleton<charaCollection> chara_collection(procedure_name);
			for (auto chara_data : *chara_collection)
			{
				//キャラデータを一つずつシリアライズ
				arc << pair("chara", *chara_data);
			}
			//ゲーム状態データをシリアライズ
			simpleSingleton<gameStatus> game_status(procedure_name);
			arc << pair("gameStatus", *game_status);
		}
	};
	//--------------------
	//致命的なエラー発生時処理処理：saveData
	template<class ARCHIVE>
	struct fatalSerializeErrorOccurred<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, const saveData& obj, const version& ver)
		{
			std::printf("シリアライズで致命的なエラー発生！\n");
		}
	};
	//--------------------
	//分配前処理：saveData
	template<class ARCHIVE>
	struct beforeDistribute<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver)
		{
			static const char* procedure_name = "beforeDistribute<saveData>";
			simpleSingleton<loadTarget> load_target(procedure_name);//ロード対象
			if (load_target->isLoadTarget("item"))//全体ロードもしくは部分ロードの対象なら実行
			{
				//インベントリデータクリア
				simpleSingleton<inventory> inventory(procedure_name);
				inventory.destroy(procedure_name);
			}
			if (load_target->isLoadTarget("ability"))//全体ロードもしくは部分ロードの対象なら実行
			{
				//アビリティデータクリア
				simpleSingleton<abilityCollection> ability_collection(procedure_name);
				ability_collection.destroy(procedure_name);
			}
			if (load_target->isLoadTarget("chara"))//全体ロードもしくは部分ロードの対象なら実行
			{
				//キャラデータクリア
				simpleSingleton<charaCollection> chara_collection(procedure_name);
				chara_collection.destroy(procedure_name);
			}
			if (load_target->isLoadTarget("ability") || load_target->isLoadTarget("chara"))//全体ロードもしくは部分ロードの対象なら実行
			{
				//キャラ習得アビリティプールクリア
				simpleSingleton<charaAbilityPool> chara_ability_pool(procedure_name);
				chara_ability_pool.destroy(procedure_name);
			}
			if (load_target->isLoadTarget("gameStatus"))//全体ロードもしくは部分ロードの対象なら実行
			{
				//ゲーム状態データクリア
				simpleSingleton<gameStatus> game_status(procedure_name);
				game_status.destroy(procedure_name);
			}
		}
	};
	//--------------------
	//分配処理：saveData
	template<class ARCHIVE>
	struct distributor<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver, const itemInfoBase& target_item)
		{
			static const char* procedure_name = "distributor<saveData>";
			simpleSingleton<loadTarget> load_target(procedure_name);//ロード対象
			if (target_item == "version")//対象項目なら実行
			{
				//セーブデータバージョン
				simpleSingleton<saveDataVersion> save_data_version(procedure_name);
				arc >> pair("version", *save_data_version);//デシリアライズ
				save_data_version->finishToLoad();
			}
			else if (target_item == "item" && load_target->isLoadTarget("item"))//対象項目かつロード対象なら実行
			{
				//インベントリデータ復元
				simpleSingleton<inventory> inventory(procedure_name);
				itemData item_data;
				arc >> pair("item", item_data);//デシリアライズ
				inventory->regist(item_data);//インベントリに登録
			}
			else if (target_item == "ability" && load_target->isLoadTarget("ability"))//対象項目かつロード対象なら実行
			{
				//アビリティデータ復元
				simpleSingleton<abilityCollection> ability_collection(procedure_name);
				abilityData ability_data;
				arc >> pair("ability", ability_data);//デシリアライズ
				ability_collection->regist(ability_data);//アビリティリストに登録
			}
			else if (target_item == "chara" && load_target->isLoadTarget("chara"))//対象項目かつロード対象なら実行
			{
				//キャラデータ復元
				simpleSingleton<charaCollection> chara_collection(procedure_name);
				charaData chara_data;
				arc >> pair("chara", chara_data);//デシリアライズ
				chara_collection->regist(chara_data);//キャラリストに登録
			}
			else if (target_item == "gameStatus" && load_target->isLoadTarget("gameStatus"))//対象項目かつロード対象なら実行
			{
				//ゲーム状態復元
				simpleSingleton<gameStatus> game_status(procedure_name);
				arc >> pair("gameStatus", *game_status);//デシリアライズ
			}
		}
	};
	//--------------------
	//分配後処理：saveData
	template<class ARCHIVE>
	struct afterDistribute<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const std::size_t array_num_on_save_data, const std::size_t array_num_loaded, const version& ver, const version& now_ver)
		{
			static const char* procedure_name = "afterDistribute<saveData>";
			simpleSingleton<loadTarget> load_target(procedure_name);//ロード対象
			
			//インベントリデータだけ復元してキャラデータがそのままなら武器と盾を参照し直す
			if (load_target->isPartLoad("item") && !load_target->isPartLoad("chara"))
			{
				simpleSingleton<charaCollection> chara_collection(procedure_name);
				for (auto& chara_data : *chara_collection)
				{
					chara_data->attachItems();//アイテムを参照し直す
				}
			}
		}
	};
	//--------------------
	//致命的なエラー発生時処理処理：saveData
	template<class ARCHIVE>
	struct fatalDeserializeErrorOccurred<ARCHIVE, saveData> {
		void operator()(ARCHIVE& arc, saveData& obj, const version& ver, const version& now_ver)
		{
			static const char* procedure_name = "fatalDeserializeErrorOccurred<saveData>";
			std::printf("デシリアライズで致命的なエラー発生！\n");
			
			//インベントリデータクリア
			simpleSingleton<inventory> inventory(procedure_name);
			inventory.destroy(procedure_name);
			
			//アビリティデータクリア
			simpleSingleton<abilityCollection> ability_collection(procedure_name);
			ability_collection.destroy(procedure_name);
			
			//キャラデータクリア
			simpleSingleton<charaCollection> chara_collection(procedure_name);
			chara_collection.destroy(procedure_name);
			
			//キャラ習得アビリティプールクリア
			simpleSingleton<charaAbilityPool> chara_ability_pool(procedure_name);
			chara_ability_pool.destroy(procedure_name);
			
			//ゲーム状態データクリア
			simpleSingleton<gameStatus> game_status(procedure_name);
			game_status.destroy(procedure_name);
		}
	};
}//namepsace serialization
GASHA_NAMESPACE_END;//ネームスペース：終了

#endif//GASHA_INCLUDED_SERIALIZATION_CPP_H

// End of file
