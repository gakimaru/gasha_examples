//--------------------------------------------------------------------------------
// basic_data.cpp
// 基本データ型【関数／実体定義部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "real_test/data/basic_data.inl"//基本データ【インライン関数／テンプレート関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//基本データ構造体

//（なし）

//--------------------
//ID型、名前型の明示的なインスタンス化

#include "real_test/data/fixed_len_str.cpp.h"//固定長文字列【関数／実体定義部】
#include "real_test/data/fixed_len_str_ext.cpp.h"//CRC付き固定長文字列【関数／実体定義部】

template class fixedLenStr<ID_TYPE_STR_SIZE>;//名前型
template class fixedLenStrExt<ID_TYPE_STR_SIZE>;//ID型
template class fixedLenStr<NAME_TYPE_STR_SIZE>;//名前型

// End of file
