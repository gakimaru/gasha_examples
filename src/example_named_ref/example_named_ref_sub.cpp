//--------------------------------------------------------------------------------
// exmaple_named_ref_sub.cpp
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

#include <cstdio>//std::printf()
#include <utility>//std::declval()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付きデータ参照：データ定義用
namespace//匿名ネームスペース
{
	//共有データ
	static int s_valA = 0;//読み書き可能データ
	static const int s_valB = 123;//読み取り専用データ
	static data_t s_dataC = { 0, { 0, 0 }, 0.f };//構造体のデータ
	static float s_valD = 0.f;//float型データ
	static double s_valE = 0.;//double型データ

	//名前付きデータ参照の自動登録
	static void registNamedRef() GASHA_CONSTRUCTOR_ATTRIBUTE;
	static void registNamedRef()
	{
		refTableWithLock refTbl;
		refTbl.initialize();
		refTbl.regist("valA", s_valA);
		refTbl.regist("valB", s_valB);
		refTbl.regist("dataC", s_dataC);
		refTbl.regist("valD", s_valD);
		refTbl.regist("valE", s_valE);
	}
	//名前付きデータ参照の登録解除
	//※デストラクタ処理のテスト用（本来プログラム終了に合わせて登録を解除する必要はない）
	static void unregistNamedRef() GASHA_DESTRUCTOR_ATTRIBUTE;
	static void unregistNamedRef()
	{
		refTableWithLock refTbl;
		refTbl.unregist("valA");
		refTbl.unregist("valB");
		refTbl.unregist("dataC");
		refTbl.unregist("valD");
		refTbl.unregist("valE");
	}
#if !defined(GASHA_HAS_CONSTRUCTOR_ATTRIBUTE) || !defined(GASHA_HAS_DESTRUCTOR_ATTRIBUTE)
	//コンストラクタ／デストラクタ属性非対応のコンパイラ用呼び出し処理
	static struct callToRegistNamedRef{
	#ifndef GASHA_HAS_CONSTRUCTOR_ATTRIBUTE
		//コンストラクタ属性非対応のコンパイラ用呼び出し処理
		callToRegistNamedRef(){
			registNamedRef();
		}
	#endif//GASHA_HAS_CONSTRUCTOR_ATTRIBUTE
	#ifndef GASHA_HAS_DESTRUCTOR_ATTRIBUTE
		//デストラクタ属性非対応のコンパイラ用呼び出し処理
		~callToRegistNamedRef(){
			unregistNamedRef();
		}
	#endif//GASHA_HAS_DESTRUCTOR_ATTRIBUTE
	} s_callToRegistNamedRef;
#endif//GASHA_HAS_CONSTRUCTOR_ATTRIBUTE, GASHA_HAS_DESTRUCTOR_ATTRIBUTE
}//anonymous namespace

//明示的なインスタンス化
#include <gasha/named_ref.cpp.h>//名前付きデータ参照【関数／実体定義部】
GASHA_INSTANCING_namedRef_withLock(refTableWithLock_type, REF_TABLE_WITH_LOCK_SIZE, sharedSpinLock);
GASHA_INSTANCING_namedRef(refTable_type, REF_TABLE_SIZE);

// End of file
