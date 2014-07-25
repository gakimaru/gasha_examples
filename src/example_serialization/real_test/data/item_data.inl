#pragma once
#ifndef GASHA_INCLUDED_ITEM_DATA_INL
#define GASHA_INCLUDED_ITEM_DATA_INL

//--------------------------------------------------------------------------------
// item_data.inl
// アイテムデータ【インライン関数／テンプレート関数定義部】
//
// ※基本的に明示的なインクルードの必要はなし。（.h ファイルの末尾でインクルード）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/item_data.h"//アイテムデータ【宣言部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アイテム構造体

//キー
inline crc32_t itemData::key() const
{
	return m_id.crc();
}

//名前
inline const char* itemData::name() const
{
	return m_name.c_str();
}

//称号の最大サイズ（終端分含む）
inline std::size_t itemData::titleMaxSize() const
{
	return m_title ? sizeof(m_titleBuff) : 0;
}

//コピーコンストラクタ
inline itemData::itemData(const itemData& src) :
	m_id(src.m_id),
	m_name(src.m_name),
	m_basic(src.m_basic),
	m_recover(src.m_recover),
	m_num(src.m_num)
{
	setTitle(src.m_title);
}

//コンストラクタ
inline itemData::itemData(const char* id, const char* name, const int atk, const int def, const int recover, const int num) :
	m_id(id),
	m_name(name),
	m_basic(atk, def),
	m_recover(recover),
	m_num(num)
{
	m_title = m_titleBuff;
	m_title[0] = '\0';
}

//デフォルトコンストラクタ
inline itemData::itemData()
{
	m_title = m_titleBuff;
	m_titleBuff[0] = '\0';
}

//デストラクタ
inline itemData::~itemData()
{}

#endif//GASHA_INCLUDED_ITEM_DATA_INL

// End of file
