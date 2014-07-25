#pragma once
#ifndef GASHA_INCLUDED_ITEM_DATA_H
#define GASHA_INCLUDED_ITEM_DATA_H

//--------------------------------------------------------------------------------
// item_data.h
// アイテムデータ【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/basic_data.h"//基本データ型

#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言
#include <gasha/crc32.h>//CRC32計算

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//アイテム構造体
struct itemData
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();

	//アクセッサ
	inline crc32_t key() const;//キー
	inline const char* name() const;//名前
	
	inline std::size_t titleMaxSize() const;//称号の最大サイズ（終端分含む） ※m_title が nullptr の時は 0 を返す

	//メソッド
	void setTitle(const char* title);//称号をセット
	
	//コピーコンストラクタ
	inline itemData(const itemData& src);
	//コンストラクタ
	inline itemData(const char* id, const char* name, const int atk, const int def, const int recover, const int num);
	//デフォルトコンストラクタ
	inline itemData();
	//デストラクタ
	inline ~itemData();

	//フィールド
	id_type m_id;//ID
	name_type m_name;//名前
	char* m_title;//称号　※可変長文字列（ポインタ）のテスト用
	basicData m_basic;//基本データ
	short m_recover;//回復力
	short m_num;//アイテム個数
private:
	//フィールド（プライベート）
	char m_titleBuff[16];//称号用バッファ
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/item_data.inl"

#endif//GASHA_INCLUDED_ITEM_DATA_H

// End of file
