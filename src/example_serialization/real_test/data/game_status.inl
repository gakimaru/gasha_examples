#pragma once
#ifndef GASHA_INCLUDED_GAME_STATUS_INL
#define GASHA_INCLUDED_GAME_STATUS_INL

//--------------------------------------------------------------------------------
// game_status.inl
// ゲーム状態【インライン関数／テンプレート関数定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "game_status.h"//ゲーム状態【宣言部】

#include <gasha/fast_string.h>//高速文字列処理

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//ゲーム状態

//コンストラクタ
inline gameStatus::flagNameInfo::flagNameInfo(const char* name, const crc32_t name_crc, const index_type index) :
	m_name(name),
	m_nameCrc(name_crc),
	m_index(index)
{}

//開始イテレータ
inline const gameStatus::iterator gameStatus::begin() const
{
	return m_flagIndexTable.begin();
}

//終了イテレータ
inline const gameStatus::iterator gameStatus::end() const
{
	return m_flagIndexTable.end();
}

//開始イテレータ
inline gameStatus::iterator gameStatus::begin()
{
	return m_flagIndexTable.begin();
}

//終了イテレータ
inline gameStatus::iterator gameStatus::end()
{
	return m_flagIndexTable.end();
}

//開始イテレータ
inline gameStatus::const_iterator gameStatus::cbegin() const
{
	return m_flagIndexTable.cbegin();
}

//終了イテレータ
inline gameStatus::const_iterator gameStatus::cend() const
{
	return m_flagIndexTable.cend();
}

//開始リバースイテレータ
inline const gameStatus::reverse_iterator gameStatus::rbegin() const
{
	return m_flagIndexTable.rbegin();
}

//終了リバースイテレータ
inline const gameStatus::reverse_iterator gameStatus::rend() const
{
	return m_flagIndexTable.rend();
}

//開始リバースイテレータ
inline gameStatus::reverse_iterator gameStatus::rbegin()
{
	return m_flagIndexTable.rbegin();
}

//終了リバースイテレータ
inline gameStatus::reverse_iterator gameStatus::rend()
{
	return m_flagIndexTable.rend();
}

//開始リバースイテレータ
inline gameStatus::const_reverse_iterator gameStatus::crbegin() const
{
	return m_flagIndexTable.crbegin();
}

//終了リバースイテレータ
inline gameStatus::const_reverse_iterator gameStatus::crend() const
{
	return m_flagIndexTable.crend();
}

//フェーズ取得
inline int gameStatus::phase() const
{
	return m_phase;
}

//フェーズ更新
inline void gameStatus::setPhase(const int phase)
{
	m_phase = phase;
}

//フラグインデックス取得
inline gameStatus::index_type gameStatus::flagIndex(const crc32_t flag_name_crc) const
{
	const flagNameInfo* flag_name_info = m_flagNameTable.at(flag_name_crc);
	if (!flag_name_info)
		return INVALID_INDEX;
	return flag_name_info->m_index;
}

//フラグインデックス取得
inline gameStatus::index_type gameStatus::flagIndex(const char* flag_name) const
{
	return flagIndex(calcCRC32(flag_name));
}

//フラグ取得
inline bool gameStatus::flag(const crc32_t flag_name_crc) const
{
	const index_type index = flagIndex(flag_name_crc);
	if (!index)
		return false;
	return m_flagSet[index];
}

//フラグ取得
inline bool gameStatus::flag(const char* flag_name) const
{
	return flag(calcCRC32(flag_name));
}

//フラグ更新
inline void gameStatus::setFlag(const crc32_t flag_name_crc, const bool flag)
{
	const index_type index = flagIndex(flag_name_crc);
	if (index == INVALID_INDEX)
		return;
	m_flagSet[index] = flag;
}

//フラグ更新
inline void gameStatus::setFlag(const char* flag_name, const bool flag)
{
	return setFlag(calcCRC32(flag_name), flag);
}

//フラグセットを取得
const gameStatus::flagSet_type& gameStatus::flagSet() const
{
	return m_flagSet;
}

//デフォルトコンストラクタ
inline gameStatus::gameStatus() :
	m_phase(0),
	m_flagSet(),
	m_flagNameTable(),
	m_flagIndexTable(),
	m_flagNameBuff()
{
	m_flagSet.reset();
	registFlagSettings();
}

//デストラクタ
inline gameStatus::~gameStatus()
{}

#endif//GASHA_INCLUDED_GAME_STATUS_INL

// End of file
