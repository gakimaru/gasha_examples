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
	static int s_valA = 123;//読み書き可能データ
	static const int s_valB = 123;//読み取り専用データ
	static data_t s_dataC = { 123, { 123, 123 }, 123.f };//構造体のデータ
	static float s_valD = 123.f;//float型データ
	static double s_valE = 123.;//double型データ
	static int s_valF = 123;//読み書き可能データ（ラップアラウンド演算）
	static unsigned short s_valG = 123;//読み書き可能データ（飽和演算）
	static float s_valH = 123.f;//float型データ
	static float s_valI = 123.f;//float型データ
	static double s_valJ = 123.;//double型データ
	static double s_valK = 123.;//double型データ
	static bool s_valL = true;//bool型データ
	static uint128_t s_valM(0, 123);//uint128型

	//名前付きデータ参照の自動登録
	static void registNamedRef() GASHA_CONSTRUCTOR_ATTRIBUTE;
	static void registNamedRef()
	{
		refTableWithLock refTbl;
		refTbl.initialize();
		refTbl.regist("valA", s_valA);
		refTbl.regist("valA'", s_valA, refTableWithLock::readonly);
		refTbl.regist("valB", s_valB);
		refTbl.regist("dataC", s_dataC);
		refTbl.regist("valD", s_valD);
		refTbl.regist("valE", s_valE);
		refTbl.regist("valF", s_valF, refTableWithLock::wraparound, 5, -5);
		//refTbl.regist("valG", s_valG, refTableWithLock::saturation, 3, 1);//エラー（型があいまい）
		refTbl.regist("valG", s_valG, refTableWithLock::saturation, static_cast<unsigned short>(3), static_cast<unsigned short>(1));
		refTbl.regist("valH", s_valH, refTableWithLock::wraparound, 5.5f, -5.5f);
		refTbl.regist("valI", s_valI, refTableWithLock::saturation, 3.5f, 1.5f);
		//refTbl.regist("valJ", s_valJ, refTableWithLock::wraparound, 5.5, -5.5);//エラー（sizeof(T) > sizeof(void*) の時は、max/minは参照型でなければならない）
		//refTbl.regist("valK", s_valK, refTableWithLock::saturation, 3.5, 1.5);//エラー（sizeof(T) > sizeof(void*) の時は、max/minは参照型でなければならない）
		static const double s_valJ_max = 5.5f, s_valJ_min = -5.5f;
		refTbl.regist("valJ", s_valJ, refTableWithLock::wraparound, s_valJ_max, s_valJ_min);
		static const double s_valK_max = 3.5f, s_valK_min = 1.5f;
		refTbl.regist("valK", s_valK, refTableWithLock::saturation, s_valK_max, s_valK_min);
		refTbl.regist("valL", s_valL);
		static const uint128_t s_valM_max(0, 3), s_valM_min(0, 1);
		refTbl.regist("valM", s_valM, refTableWithLock::wraparound, s_valM_max, s_valM_min);
	}
	//名前付きデータ参照の登録解除
	//※デストラクタ処理のテスト用（本来プログラム終了に合わせて登録を解除する必要はない）
	static void unregistNamedRef() GASHA_DESTRUCTOR_ATTRIBUTE;
	static void unregistNamedRef()
	{
		refTableWithLock refTbl;
		refTbl.unregist("valA");
		refTbl.unregist("valA'");
		refTbl.unregist("valB");
		refTbl.unregist("dataC");
		refTbl.unregist("valD");
		refTbl.unregist("valE");
		refTbl.unregist("valF");
		refTbl.unregist("valG");
		refTbl.unregist("valH");
		refTbl.unregist("valI");
		refTbl.unregist("valJ");
		refTbl.unregist("valK");
		refTbl.unregist("valL");
		refTbl.unregist("valM");
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
GASHA_INSTANCING_namedRef(refTableWithLock_type);
GASHA_INSTANCING_namedRef(refTable_type);

// End of file
