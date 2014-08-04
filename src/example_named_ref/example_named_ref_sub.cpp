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

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//名前付き関数テスト
bool objType::memberA(const int a, const int b)
{
	std::printf("objType::memberA() const: a=%d, b=%d, m_mem=%d\n", a, b, m_mem);
	return true;
}
void objType::memberB() const
{
	std::printf("objType::memberB() const: m_mem=%d\n", m_mem);
}
void objType::memberB_noconst()
{
	std::printf("objType::memberB_noconst() const: m_mem=%d\n", m_mem);
}

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

	static bool funcA(const int a, const int b)
	{
		std::printf("funcA: a=%d, b=%d\n", a, b);
		return true;
	}
	static void funcB()
	{
		std::printf("funcB:\n");
	}
	auto lambdaA = [](const int a, const int b) -> bool
	{
		std::printf("lambdaA: a=%d, b=%d\n", a, b);
		return true;
	};
	auto lambdaB = []()
	{
		std::printf("lambdaB:\n");
	};
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

	template<class>
	struct funcSpec;
	template<typename R, typename... Tx>
	struct funcSpec<R(Tx...)>
	{
		std::function<R(Tx...)> f;
		template<class FUNCTION>
		void reg(FUNCTION func)
		{
			f = func;
		}
		template<typename... ARGS>
		R run(ARGS&&... args)
		{
			return f(std::forward<ARGS>(args)...);
		}
	};
	template<class>
	struct objFuncSpec;
	template<typename R, class OBJ, typename... Tx>
	struct objFuncSpec<R(OBJ&, Tx...)>
	{
		std::function<R(OBJ&, Tx...)> f;
		template<class FUNCTION>
		void reg(FUNCTION func)
		{
			f = std::mem_fn(func);
		}
		template<typename... ARGS>
		R run(OBJ& obj, ARGS&&... args)
		{
			return f(obj, std::forward<ARGS>(args)...);
		}
	};
	template<class FUNC_SPEC, class FUNCTION, typename... Tx>
	void setFunc(FUNCTION function, Tx&&... args)
	{
		funcSpec<FUNC_SPEC> spec;
		spec.reg(function);
		spec.run(std::forward<Tx>(args)...);
	}
	template<class FUNC_SPEC, class FUNCTION, class OBJ, typename... Tx>
	void setObjFunc(FUNCTION function, OBJ& obj, Tx&&... args)
	{
		objFuncSpec<FUNC_SPEC> spec;
		spec.reg(function);
		spec.run(obj, std::forward<Tx>(args)...);
	}

	//名前付きデータ参照の自動登録
	static void registNamedRef() GASHA_CONSTRUCTOR_ATTRIBUTE;
	static void registNamedRef()
	{
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

		funcTable func_tbl;
		func_tbl.initialize();

		func_tbl.regist<bool(int, int)>("funcGroupA", "funcA", funcA);
		func_tbl.regist<bool(int, int)>("funcGroupB", "lambdaA", lambdaA);
		func_tbl.regist<bool(int, int)>("funcGroupC", "functorA", functorA());
		func_tbl.registObj<bool(objType&, int, int)>("funcGroupD", "memberA", &objType::memberA);
		
		func_tbl.regist<bool(int, int)>("funcGroupA", "funcA const", funcA, funcTable::constFunc);
		func_tbl.regist<bool(int, int)>("funcGroupB", "lambdaA const", lambdaA, funcTable::constFunc);
		func_tbl.regist<bool(int, int)>("funcGroupC", "functorA const", functorA(), funcTable::constFunc);
		func_tbl.registObj<bool(objType&, int, int)>("funcGroupD", "memberA const", &objType::memberA, funcTable::constFunc);
		
		func_tbl.regist<void()>("funcGroupA", "funcB", funcB);
		func_tbl.regist<void()>("funcGroupB", "lambdaB", lambdaB);
		func_tbl.regist<void()>("funcGroupC", "functorB", functorB());
		func_tbl.registObj<void(objType&)>("funcGroupD", "memberB", &objType::memberB_noconst);

		func_tbl.regist<void()>("funcGroupA", "funcB const", funcB, funcTable::constFunc);
		func_tbl.regist<void()>("funcGroupB", "lambdaB const", lambdaB, funcTable::constFunc);
		func_tbl.regist<void()>("funcGroupC", "functorB const", functorB(), funcTable::constFunc);
		func_tbl.registObj<void(objType&)>("funcGroupD", "memberB const", &objType::memberB, funcTable::constFunc);
	}
	
	//名前付きデータ参照の登録解除
	//※デストラクタ処理のテスト用（本来プログラム終了に合わせて登録を解除する必要はない）
	static void unregistNamedRef() GASHA_DESTRUCTOR_ATTRIBUTE;
	static void unregistNamedRef()
	{
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
GASHA_INSTANCING_namedRef(refTable_type);
GASHA_INSTANCING_namedRef(refTableEx_type);

#include <gasha/named_func.cpp.h>//名前付き関数【関数／実体定義部】
GASHA_INSTANCING_namedFunc(funcTable_type);

// End of file
