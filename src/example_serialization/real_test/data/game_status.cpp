//--------------------------------------------------------------------------------
// game_status.cpp
// ゲーム状態【関数／実体定義部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/game_status.inl"//ゲーム状態【インライン関数／テンプレート関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//ゲーム状態

//フラグインデックスを登録
void gameStatus::registFlagIndex(const char* flag_name, const gameStatus::index_type index)
{
	if (!flag_name)
		return;
	const crc32_t flag_name_crc = calcCRC32(flag_name);
	const std::size_t flag_name_len = strnlen_fast(flag_name, FLAG_NAME_MAX_LEN);
	const std::size_t flag_name_size = flag_name_len + 1;
	char* flag_name_buff = m_flagNameBuff.newArray<char>(flag_name_size);
	strncpy_fast(flag_name_buff, flag_name, flag_name_len);
	flag_name_buff[flag_name_len] = '\0';
	flagNameInfo* flag_info = m_flagNameTable.emplace(flag_name_crc, flag_name_buff, flag_name_crc, index);
	m_flagIndexTable.insert(*flag_info);
}

//フラグ設定を登録
void gameStatus::registFlagSettings()
{
	registFlagIndex("flag A", 1);
	registFlagIndex("flag B", 2);
	registFlagIndex("flag C", 3);
	registFlagIndex("flag D", 4);
	registFlagIndex("flag X", 61);
	registFlagIndex("flag Y", 62);
	registFlagIndex("flag Z", 63);
}

//全ての登録情報をクリア
void gameStatus::clear()
{
	m_flagSet.reset();//フラグセット
	m_flagIndexTable.clear();//フラグインデックステーブル
	m_flagNameTable.clear();//フラグ名テーブル
	m_flagNameBuff.clear();//フラグ名バッファ
	registFlagSettings();
}

//--------------------
//フラグ名テーブル／フラグ名バッファの明示的なインスタンス化

#include <gasha/hash_table.cpp.h>//ハッシュテーブル【関数／実体定義部】
#include <gasha/rb_tree.cpp.h>//赤黒木【関数／実体定義部】
#include <gasha/stack_allocator.cpp.h>//スタックアロケータ【関数／実体定義部】

GASHA_INSTANCING_hTable(gameStatus::flagNameTableOpe);
GASHA_INSTANCING_rbTree(gameStatus::flagIndexTableOpe);
GASHA_INSTANCING_stackAllocator();

//--------------------
//ゲーム情報シングルトンの明示的なインスタンス化

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】

GASHA_INSTANCING_simpleSingleton(gameStatus);//シングルトンのインスタンス化

// End of file
