//--------------------------------------------------------------------------------
// exmaple_named_ref_sub_init.cpp
// 名前付きデータ参照テスト（初期化／終了処理）
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

#include <gasha/singleton.h>//シングルトン

#include <cstdio>//std::printf()

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

	//名前付き関数テスト
	//※通常関数
	static bool funcA(const int a, const int b)
	{
		std::printf("funcA: a=%d, b=%d\n", a, b);
		return true;
	}
	static void funcB()
	{
		std::printf("funcB:\n");
	}
	
	//名前付き関数テスト
	//※ラムダ式
	auto lambdaA = [](const int a, const int b) -> bool
	{
		std::printf("lambdaA: a=%d, b=%d\n", a, b);
		return true;
	};
	auto lambdaB = []()
	{
		std::printf("lambdaB:\n");
	};
	
	//名前付き関数テスト
	//※関数オブジェクト
	struct functorA{
		bool operator()(const int a, const int b)
		{
			std::printf("functorA: a=%d, b=%d\n", a, b);
			return true;
		}
	};
	struct functorB{
		void operator()()
		{
			std::printf("functorB:\n");
		}
	};

	//名前付きデータ参照の登録
	//※main()関数実行前の初期化は、静的変数のコンストラクタ活用よりも、GASHA_CONSTRUCTOR_ATTRIBUTE ( = __attribute__((constructor))) を推奨。
	//　GASHA_CONSTRUCTOR_ATTRIBUTE は、静的変数のコンストラクタよりも後に実行されることを保証できる。
	//　なお、GASHA_CONSTRUCTOR_ATTRIBUTE_WITH_PRIORITY() の場合は、静的変数のコンストラクタよりも先に実行されるので注意。
	//※VC++は GASHA_CONSTRUCTOR_ATTRIBUTE が使えないので、先に初期化しなければならない静的変数の実体が配置された .cpp ファイルに #pragma init_seg(lib) を指定すると良い。
	static void registNamedRef() GASHA_CONSTRUCTOR_ATTRIBUTE;
	static void registNamedRef()
	{
		//名前付きデータ参照の登録
		refTable ref_tbl;
		ref_tbl.initialize();
		ref_tbl.regist("valA", s_valA);
		ref_tbl.regist("valA'", s_valA, refTable::readonly);
		ref_tbl.regist("valB", s_valB);
		ref_tbl.regist("dataC", s_dataC);
		ref_tbl.regist("valD", s_valD);
		ref_tbl.regist("valE", s_valE);
		ref_tbl.regist("valF", s_valF, refTable::wraparound, 5, -5);
		//ref_tbl.regist("valG", s_valG, refTable::saturation, 3, 1);//エラー（型があいまい）
		ref_tbl.regist("valG", s_valG, refTable::saturation, static_cast<unsigned short>(3), static_cast<unsigned short>(1));
		ref_tbl.regist("valH", s_valH, refTable::wraparound, 5.5f, -5.5f);
		ref_tbl.regist("valI", s_valI, refTable::saturation, 3.5f, 1.5f);
		//ref_tbl.regist("valJ", s_valJ, refTable::wraparound, 5.5, -5.5);//エラー（sizeof(T) > sizeof(void*) の時は、max/minは参照型でなければならない）
		//ref_tbl.regist("valK", s_valK, refTable::saturation, 3.5, 1.5);//エラー（sizeof(T) > sizeof(void*) の時は、max/minは参照型でなければならない）
		static const double s_valJ_max = 5.5f, s_valJ_min = -5.5f;
		ref_tbl.regist("valJ", s_valJ, refTable::wraparound, s_valJ_max, s_valJ_min);
		static const double s_valK_max = 3.5f, s_valK_min = 1.5f;
		ref_tbl.regist("valK", s_valK, refTable::saturation, s_valK_max, s_valK_min);
		ref_tbl.regist("valL", s_valL);
		static const uint128_t s_valM_max(0, 3), s_valM_min(0, 1);
		ref_tbl.regist("valM", s_valM, refTable::wraparound, s_valM_max, s_valM_min);

		//名前付き関数の登録
		//※登録時には、関数のプロトタイプをテンプレート引数に指定する必要あり
		funcTable func_tbl;
		func_tbl.initialize();

		//通常関数
		func_tbl.regist<bool(int, int)>("funcGroupA", "funcA", funcA);//引数、戻り値あり
		func_tbl.regist<bool(int, int)>("funcGroupA", "funcA const", funcA, funcTable::constFunc);//引数、戻り値あり、const関数
		func_tbl.regist<void()>("funcGroupA", "funcB", funcB);//引数、戻り値なし
		func_tbl.regist<void()>("funcGroupA", "funcB const", funcB, funcTable::constFunc);//引数、戻り値なし、const関数

		//ラムダ式
		func_tbl.regist<bool(int, int)>("funcGroupB", "lambdaA", lambdaA);//引数、戻り値あり
		func_tbl.regist<bool(int, int)>("funcGroupB", "lambdaA const", lambdaA, funcTable::constFunc);//引数、戻り値あり、const関数
		func_tbl.regist<void()>("funcGroupB", "lambdaB", lambdaB);//引数、戻り値なし
		func_tbl.regist<void()>("funcGroupB", "lambdaB const", lambdaB, funcTable::constFunc);//引数、戻り値なし、const関数

		//関数オブジェクト
		func_tbl.regist<bool(int, int)>("funcGroupC", "functorA", functorA());//引数、戻り値あり
		func_tbl.regist<bool(int, int)>("funcGroupC", "functorA const", functorA(), funcTable::constFunc);//引数、戻り値あり、const関数
		func_tbl.regist<void()>("funcGroupC", "functorB", functorB());//引数、戻り値なし
		func_tbl.regist<void()>("funcGroupC", "functorB const", functorB(), funcTable::constFunc);//引数、戻り値なし、const関数

		//クラスメンバー関数
		func_tbl.registObj<bool(objType&, int, int)>("funcGroupD", "memberA", &objType::memberA);//引数、戻り値あり
		func_tbl.registObj<bool(objType&, int, int)>("funcGroupD", "memberA const", &objType::memberA, funcTable::constFunc);//引数、戻り値あり、const関数
		func_tbl.registObj<void(objType&)>("funcGroupD", "memberB", &objType::memberB_noconst);//引数、戻り値なし
		func_tbl.registObj<void(objType&)>("funcGroupD", "memberB const", &objType::memberB, funcTable::constFunc);//引数、戻り値なし、const関数

		//グループなし
		//※グループなしではロック制御なし
		func_tbl.regist<bool(int, int)>(nullptr, "funcA", funcA);//通常関数
		func_tbl.regist<bool(int, int)>(nullptr, "lambdaA", lambdaA);//ラムダ式
		func_tbl.regist<bool(int, int)>(nullptr, "functorA", functorA());//関数オブジェクト
		func_tbl.registObj<bool(objType&, int, int)>(nullptr, "memberA", &objType::memberA);//クラスメンバー関数
		
		//名前付きデータ参照と組み合わせ
		func_tbl.registObj<int(data_t&, int)>("data_t", "methodA", &data_t::methodA);//クラスメンバー関数

		//名前付き定数の登録
		valueTable value_tbl;
		simpleSingleton<valueStrPool> value_str("regist");
		value_tbl.initialize();
		value_str->clear();
		value_tbl.regist("valueGroupA", "value1", 1); value_str->regist("value1");
		value_tbl.regist("valueGroupA", "value2", 2); value_str->regist("value2");
		value_tbl.regist("valueGroupA", "value9", 9); value_str->regist("value9");
		value_tbl.regist("valueGroupA", "value10", 10); value_str->regist("value10");
		value_tbl.regist("valueGroupB", "valueA", 1.1f); value_str->regist("valueA");
		value_tbl.regist("valueGroupB", "valueB", 2.2f); value_str->regist("valueB");
		value_tbl.regist("valueGroupB", "valueC", 9.9f); value_str->regist("valueC");
		value_tbl.regist("valueGroupB", "valueD", 10.2f); value_str->regist("valueD");
	#define REGIST_ENUM(TYPE, VALUE) \
		value_tbl.regist<TYPE>(#TYPE, #VALUE, VALUE); value_str->regist(#VALUE);
		REGIST_ENUM(testValues, testValueA);
		REGIST_ENUM(testValues, testValueB);
		REGIST_ENUM(testValues, testValueC);
		value_tbl.regist(nullptr, "g_value1", 102); value_str->regist("g_value1");
		value_tbl.regist(nullptr, "g_value2", 202); value_str->regist("g_value2");
		value_tbl.regist(nullptr, "g_value9", 209); value_str->regist("g_value9");
		value_tbl.regist(nullptr, "g_value10", 210); value_str->regist("g_value10");
		static const uint128_t s_val128(0, 123);//uint128型
		value_tbl.regist(nullptr, "g_value128", s_val128); value_str->regist("g_value128");
	}
	
	//名前付きデータ参照の登録解除
	//※デストラクタ処理のテスト用（本来プログラム終了に合わせて登録を解除する必要はない）
	static void unregistNamedRef() GASHA_DESTRUCTOR_ATTRIBUTE;
	static void unregistNamedRef()
	{
		//名前付きデータ参照の登録解除
		refTable ref_tbl;
		ref_tbl.unregist("valA");
		ref_tbl.unregist("valA'");
		ref_tbl.unregist("valB");
		ref_tbl.unregist("dataC");
		ref_tbl.unregist("valD");
		ref_tbl.unregist("valE");
		ref_tbl.unregist("valF");
		ref_tbl.unregist("valG");
		ref_tbl.unregist("valH");
		ref_tbl.unregist("valI");
		ref_tbl.unregist("valJ");
		ref_tbl.unregist("valK");
		ref_tbl.unregist("valL");
		ref_tbl.unregist("valM");
		
		//名前付き関数の登録解除
		funcTable func_tbl;
		func_tbl.unregist("funcGroupA", "funcA");
		func_tbl.unregist("funcGroupB", "lambdaA");
		func_tbl.unregist("funcGroupC", "functorA");
		func_tbl.unregist("funcGroupD", "memberA");
		func_tbl.unregist("funcGroupA", "funcA const");
		func_tbl.unregist("funcGroupB", "lambdaA const");
		func_tbl.unregist("funcGroupC", "functorA const");
		func_tbl.unregist("funcGroupD", "memberA const");
		func_tbl.unregist("funcGroupA", "funcB");
		func_tbl.unregist("funcGroupB", "lambdaB");
		func_tbl.unregist("funcGroupC", "functorB");
		func_tbl.unregist("funcGroupD", "memberB");
		func_tbl.unregist("funcGroupA", "funcB const");
		func_tbl.unregist("funcGroupB", "lambdaB const");
		func_tbl.unregist("funcGroupC", "functorB const");
		func_tbl.unregist("funcGroupD", "memberB const");
		func_tbl.unregist(nullptr, "funcA");
		func_tbl.unregist(nullptr, "lambdaA");
		func_tbl.unregist(nullptr, "functorA");
		func_tbl.unregist(nullptr, "memberA");
		func_tbl.unregist("data_t", "methodA");

		//名前付き定数の登録解除
		valueTable value_tbl;
		value_tbl.initialize();
		value_tbl.unregist("value1");
		value_tbl.unregist("value2");
		value_tbl.unregist("value9");
		value_tbl.unregist("value10");
		value_tbl.unregist("valueA");
		value_tbl.unregist("valueB");
		value_tbl.unregist("valueC");
		value_tbl.unregist("valueD");
		value_tbl.unregist("testValueA");
		value_tbl.unregist("testValueB");
		value_tbl.unregist("testValueC");
		value_tbl.unregist("g_value1");
		value_tbl.unregist("g_value2");
		value_tbl.unregist("g_value9");
		value_tbl.unregist("g_value10");
		value_tbl.unregist("g_value128");
		simpleSingleton<valueStrPool> value_str("unregist");
		value_str->clear();

		//名前付きデータ参照の列挙
		enumNamedRef();
		//名前付き関数の列挙
		enumNamedFunc();
		//名前付き定数の列挙
		enumNamedValue();
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

// End of file
