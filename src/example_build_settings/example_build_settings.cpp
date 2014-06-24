//--------------------------------------------------------------------------------
// exmaple_build_settings.cpp
// ビルド設定処理テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_build_settings.h"//ビルド設定処理テスト

#include <stdio.h>//printf()
#include <cstddef>//std::size_t

USING_NAMESPACE_GASHA;//ネームスペース使用

//----------------------------------------
//noinline/always_inlineテスト
void func_normal()
{
	printf("This is normal func.\n");
}
inline void func_inline()
{
	printf("This is inline func.\n");
}
no_inline void func_noinline()
{
	printf("This is noinline func.\n");
}
always_inline void func_always_inline()
{
	printf("This is always inline func.\n");
}

//----------------------------------------
//【C++11仕様】nullptrテスト
void* nullptr_var = nullptr;

//----------------------------------------
//【C++11仕様】overrideテスト
struct base
{
	virtual void func()
	{
		printf("This is virtual func.\n");
	}
};
struct derived : public base
{
	void func() override
	{
		printf("This is override func.\n");
	}
};
derived override_func_var;

//----------------------------------------
//【C++11仕様】constexprテスト
constexpr int constexpr_calc(int a, int b){ return a + b; }
constexpr int constexpr_var = constexpr_calc(1, 2) + 3;

//----------------------------------------
//【C++11仕様】ユーザー定義リテラルテスト
#ifdef HAS_USER_DEFINED_LITERAL
constexpr int operator "" _len(const char* str, const std::size_t len)
{
	return len;
}
const std::size_t user_defined_literal_var = "1234567890"_len;
#endif//HAS_USER_DEFINED_LITERAL

//----------------------------------------
//【C++11仕様】TLS変数宣言テスト
thread_local int m_var_tls = 123;

//----------------------------------------
//【C++11仕様】アライメント指定テスト
struct alignas(16) data_t
{
	int var[8];
};

//----------------------------------------
//ビルド設定処理テスト
void example_build_settings()
{
	//コンパイラ確認テスト
	printf("Compiler: name=\"%s\", Ver=[%d(0x%08x).%d(0x%08x)]\n", COMPILER_NAME, COMPILER_VER, COMPILER_VER, COMPILER_MINOR, COMPILER_MINOR);
#ifdef IS_VC
	printf("    This compiler is \"Visual C++\"\n");
#endif//IS_VC
#ifdef IS_GCC
	printf("    This compiler is \"GCC\"\n");
#endif//IS_GCC

	//C++言語確認テスト
	printf("\n");
	printf("Compiler-language: %s (C++ Ver.=%d)\n", COMPILER_LANGUAGE, CPP_VER);
#ifdef IS_CPP
	printf("    C++ is available.\n");
#endif//IS_CPP
#ifdef HAS_CPP98
	printf("        C++ is implemented C++98.\n");
#endif//HAS_CPP98
#ifdef HAS_CPP03
	printf("        C++ is implemented C++03.\n");
#endif//HAS_CPP03
#ifdef HAS_CPP11
	printf("        C++ is implemented C++11.\n");
#endif//HAS_CPP11

	//プラットフォーム確認テスト
	printf("\n");
	printf("Plataform: \"%s\"(%s %dbits, %s-endian), Ver=[%d(0x%08x).%d(0x%08x)]\n", PLATFORM_NAME, PLATFORM_ARCHITECTURE, PLATFORM_ARCHITECTURE_BITS, ENDIAN, PLATFORM_VER, PLATFORM_VER, PLATFORM_MINOR, PLATFORM_MINOR);
#ifdef IS_WIN
	printf("    Target plarform is \"Windows\"\n");
#endif//IS_WIN
#ifdef IS_LINUX
	printf("    Target plarform is \"Linux\"\n");
#endif//IS_LINUX
#ifdef IS_CYGWIN
	printf("    Target plarform is \"Cygwin\"\n");
#endif//IS_CYGWIN

	//定義済みマクロ表示テスト
	struct test
	{
		static void func()
		{
			printf("\n");
			printf("__FILE__=\"%s\"\n", __FILE__);
			printf("__LINE__=%d\n", __LINE__);
			printf("__FUNCTION__=\"%s\"\n", __FUNCTION__);
			printf("__PRETTY_FUNCTION__=\"%s\"\n", __PRETTY_FUNCTION__);
			printf("__FUNCSIG__=\"%s\"\n", __FUNCSIG__);
			printf("__func__=\"%s\"\n", __func__);
			printf("__FUNCDNAME__=\"%s\"\n", __FUNCDNAME__);
			printf("__DATE__=\"%s\"\n", __DATE__);
			printf("__TIME__=\"%s\"\n", __TIME__);
			printf("__TIMESTAMP__=\"%s\"\n", __TIMESTAMP__);
			printf("\n");
			printf("GET_FUNC_NAME()=\"%s\"\n", GET_FUNC_NAME());
			printf("GET_FILE_LINE()=\"%s\"\n", GET_FILE_LINE());
			printf("GET_FILE_LINE_TIME()=\"%s\"\n", GET_FILE_LINE_TIME());
		}
	};
	test::func();

	//noinline/always_inlineテスト
	func_normal();
	func_inline();
	func_noinline();
	func_always_inline();

	//【C++11仕様】nullptrテスト
	printf("\n");
	printf("nullptr_var=%p\n", nullptr_var);
#ifdef HAS_NULLPTR
	printf("    'nullptr' is featured.\n");
#endif//HAS_NULLPTR

	//【C++11仕様】overrideテスト
	printf("\n");
	override_func_var.func();
#ifdef HAS_OVERRIDE
	printf("    'override' is featured.\n");
#endif//HAS_OVERRIDE

	//【C++11仕様】constexprテスト
	printf("\n");
	printf("constexpr_var=%d\n", constexpr_var);
	printf("constexpr_calc(1, 2)=%d\n", constexpr_calc(1, 2));
#ifdef HAS_CONSTEXPR
	printf("    'constexpr' is featured.\n");
#endif//HAS_CONSTEXPR

	//【C++11仕様】ユーザー定義リテラルテスト
#ifdef HAS_USER_DEFINED_LITERAL
	printf("\n");
	printf("user_defined_literal_var=%d\n", user_defined_literal_var);
#endif//HAS_USER_DEFINED_LITERAL
#ifdef HAS_USER_DEFINED_LITERAL
	printf("    'operator \"\"'(user defined literal) is featured.\n");
#endif//HAS_USER_DEFINED_LITERAL

	//【C++11仕様】TLSテスト
	printf("\n");
	printf("TLS Variable=%d\n", m_var_tls);
#ifdef HAS_THREAD_LOCAL
	printf("    'thread_local' is featured.\n");
#endif//HAS_THREAD_LOCAL

	//【C++11仕様】アラインメント指定／取得／指定付メモリ確保と解放テスト
	printf("\n");
	printf("sizeof(data_t)=%d\n", sizeof(data_t));
	printf("alignof(data_t)=%d\n", alignof(data_t));//アラインメント取得テスト
	data_t* p = reinterpret_cast<data_t*>(_aligned_malloc(sizeof(data_t), alignof(data_t)));
	printf("_aligned_malloc(sizeof(data_t), alignof(data_t))=%p\n", p);
	_aligned_free(p);
	printf("_aligned_free(p)\n");
#ifdef HAS_ALIGNAS
	printf("    'alignas' is featured.\n");
#endif//HAS_ALIGNAS
#ifdef HAS_ALIGNOF
	printf("    'alignof' is featured.\n");
#endif//HAS_ALIGNOF
}

// End of file
