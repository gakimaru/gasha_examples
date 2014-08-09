//--------------------------------------------------------------------------------
// exmaple_named_ref_instance.cpp
// 名前付きデータ参照テスト（明示的なインスタンス化）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

GASHA_USING_NAMESPACE;//ネームスペース使用

//【VC++】example_named_ref_init.cpp よりも先に、確実に各種インスタンスの初期化を先に行わせるための設定
#ifdef GASHA_IS_VC
#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)
#endif//GASHA_IS_VC

//----------------------------------------
//明示的なインスタンス化
//※下記の初期化処理よりも先に行う必要あり
#include <gasha/named_ref.cpp.h>//名前付きデータ参照【関数／実体定義部】
GASHA_INSTANCING_namedRef(refTable_type);
GASHA_INSTANCING_namedRef(refTableEx_type);

#include <gasha/named_func.cpp.h>//名前付き関数【関数／実体定義部】
GASHA_INSTANCING_namedFunc(funcTable_type);

#include <gasha/named_value.cpp.h>//名前付き定数【関数／実体定義部】
GASHA_INSTANCING_namedValue(valueTable_type);

#include <gasha/str_pool.cpp.h>//文字列プール【関数／実体定義部】
GASHA_INSTANCING_strPool_withLock(VALUE_TABLE_STR_BUFF, VALUE_TABLE_SIZE, sharedSpinLock);

#include <gasha/singleton.cpp.h>//シングルトン【関数／実体定義部】
GASHA_INSTANCING_simpleSingleton(valueStrPool);

// End of file
