#pragma once
#ifndef GASHA_INCLUDED_BASIC_DATA_H
#define GASHA_INCLUDED_BASIC_DATA_H

//--------------------------------------------------------------------------------
// basic_data.h
// 基本データ型【宣言部】
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/fixed_len_str.h"//固定長文字列型
#include "real_test/data/fixed_len_str_ext.h"//CRC付き固定長文字列型

#include <cstddef>//std::size_t
#include <cstdint>//C++11 std::uint16_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------------------------------------------------------------------
//基本データ型

//--------------------
//ID型
static const std::size_t ID_TYPE_STR_SIZE = 8;
using id_type = fixedLenStrExt<ID_TYPE_STR_SIZE>;

//--------------------
//名前型
static const std::size_t NAME_TYPE_STR_SIZE = 32;
using name_type = fixedLenStr<NAME_TYPE_STR_SIZE>;

//--------------------
//基本データ構造体
struct basicData
{
	//型
	typedef std::uint16_t basic_param_type;
	
	//コピーコンストラクタ
	inline basicData(const basicData& src);
	//コンストラクタ
	inline basicData(const int atk, const int def);
	//デフォルトコンストラクタ
	inline basicData();
	//デストラクタ
	inline ~basicData();

	//フィールド
	basic_param_type m_atk;//攻撃力
	basic_param_type m_def;//守備力
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/basic_data.inl"

#endif//GASHA_INCLUDED_BASIC_DATA_H

// End of file
