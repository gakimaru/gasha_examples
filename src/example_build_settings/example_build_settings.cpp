//--------------------------------------------------------------------------------
// exmaple_build_settings.cpp
// ビルド設定処理テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_build_settings.h"//ビルド設定処理テスト

#include <cstdio>//std::printf()
#include <cstddef>//std::size_t

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//noinline/always_inlineテスト
void func_normal()
{
	std::printf("This is normal func.\n");
}
inline void func_inline()
{
	std::printf("This is inline func.\n");
}
GASHA_NOINLINE void func_noinline()
{
	std::printf("This is noinline func.\n");
}
GASHA_ALWAYS_INLINE void func_always_inline()
{
	std::printf("This is always inline func.\n");
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
		std::printf("This is virtual func.\n");
	}
};
struct derived : public base
{
	void func() override
	{
		std::printf("This is override func.\n");
	}
};
derived override_func_var;

//----------------------------------------
//【C++11仕様】constexprテスト
constexpr int constexpr_calc(int a, int b){ return a + b; }
constexpr int constexpr_var = constexpr_calc(1, 2) + 3;

//----------------------------------------
//【C++11仕様】ユーザー定義リテラルテスト
#ifdef GASHA_HAS_USER_DEFINED_LITERAL
constexpr int operator "" _len(const char* str, const std::size_t len)
{
	return len;
}
const std::size_t user_defined_literal_var = "1234567890"_len;
#endif//GASHA_HAS_USER_DEFINED_LITERAL

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
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("\n");
	
	//コンパイラ確認テスト
	std::printf("Compiler: name=\"%s\", Ver=[%d(0x%08x).%d(0x%08x)]\n", GASHA_COMPILER_NAME, GASHA_COMPILER_VER, GASHA_COMPILER_VER, GASHA_COMPILER_MINOR, GASHA_COMPILER_MINOR);
#ifdef GASHA_IS_VC
	std::printf("    This compiler is \"Visual C++\"\n");
#endif//GASHA_IS_VC
#ifdef GASHA_IS_GCC
	std::printf("    This compiler is \"GCC\"\n");
#endif//GASHA_IS_GCC

	//C++言語確認テスト
	std::printf("\n");
	std::printf("Compiler-language: %s (C++ Ver.=%d)\n", GASHA_PROGRAM_LANGUAGE_NAME, GASHA_CPP_VER);
#ifdef GASHA_IS_CPP
	std::printf("    C++ is available.\n");
#endif//GASHA_IS_CPP
#ifdef GASHA_HAS_CPP98
	std::printf("        C++ is implemented C++98.\n");
#endif//GASHA_HAS_CPP98
#ifdef GASHA_HAS_CPP03
	std::printf("        C++ is implemented C++03.\n");
#endif//GASHA_HAS_CPP03
#ifdef GASHA_HAS_CPP11
	std::printf("        C++ is implemented C++11.\n");
#endif//GASHA_HAS_CPP11

	//プラットフォーム確認テスト
	std::printf("\n");
	std::printf("Plataform: \"%s\"(%s %dbits, %s-endian), Ver=[%d(0x%08x).%d(0x%08x)]\n", GASHA_PLATFORM_NAME, GASHA_PLATFORM_ARCHITECTURE_NAME, GASHA_PLATFORM_ARCHITECTURE_BITS, GASHA_ENDIAN_NAME, GASHA_PLATFORM_VER, GASHA_PLATFORM_VER, GASHA_PLATFORM_MINOR, GASHA_PLATFORM_MINOR);
#ifdef GASHA_IS_WIN
	std::printf("    Target plarform is \"Windows\"\n");
#endif//GASHA_IS_WIN
#ifdef GASHA_IS_LINUX
	std::printf("    Target plarform is \"Linux\"\n");
#endif//GASHA_IS_LINUX
#ifdef GASHA_IS_CYGWIN
	std::printf("    Target plarform is \"Cygwin\"\n");
#endif//GASHA_IS_CYGWIN

	//定義済みマクロ表示テスト
	struct test
	{
		static void func()
		{
			std::printf("\n");
			std::printf("__FILE__=\"%s\"\n", __FILE__);
			std::printf("__LINE__=%d\n", __LINE__);
			std::printf("__FUNCTION__=\"%s\"\n", __FUNCTION__);
			std::printf("__PRETTY_FUNCTION__=\"%s\"\n", __PRETTY_FUNCTION__);
			std::printf("__FUNCSIG__=\"%s\"\n", __FUNCSIG__);
			std::printf("__func__=\"%s\"\n", __func__);
			std::printf("__FUNCDNAME__=\"%s\"\n", __FUNCDNAME__);
			std::printf("__DATE__=\"%s\"\n", __DATE__);
			std::printf("__TIME__=\"%s\"\n", __TIME__);
			std::printf("__TIMESTAMP__=\"%s\"\n", __TIMESTAMP__);
			std::printf("\n");
			std::printf("GASHA_FUNC_NAME()=\"%s\"\n", GASHA_FUNC_NAME());
			std::printf("GASHA_SRC_FILE_LINE()=\"%s\"\n", GASHA_SRC_FILE_LINE());
			std::printf("GASHA_SRC_FILE_LINE_TIME()=\"%s\"\n", GASHA_SRC_FILE_LINE_TIME());
		}
	};
	test::func();

	//noinline/always_inlineテスト
	func_normal();
	func_inline();
	func_noinline();
	func_always_inline();

	//【C++11仕様】nullptrテスト
	std::printf("\n");
	std::printf("nullptr_var=%p\n", nullptr_var);
#ifdef GASHA_HAS_NULLPTR
	std::printf("    'nullptr' is featured.\n");
#endif//GASHA_HAS_NULLPTR

	//【C++11仕様】overrideテスト
	std::printf("\n");
	override_func_var.func();
#ifdef GASHA_HAS_OVERRIDE
	std::printf("    'override' is featured.\n");
#endif//GASHA_HAS_OVERRIDE

	//【C++11仕様】constexprテスト
	std::printf("\n");
	std::printf("constexpr_var=%d\n", constexpr_var);
	std::printf("constexpr_calc(1, 2)=%d\n", constexpr_calc(1, 2));
#ifdef GASHA_HAS_CONSTEXPR
	std::printf("    'constexpr' is featured.\n");
#endif//GASHA_HAS_CONSTEXPR

	//【C++11仕様】ユーザー定義リテラルテスト
#ifdef GASHA_HAS_USER_DEFINED_LITERAL
	std::printf("\n");
	std::printf("user_defined_literal_var=%d\n", static_cast<int>(user_defined_literal_var));
#endif//GASHA_HAS_USER_DEFINED_LITERAL
#ifdef GASHA_HAS_USER_DEFINED_LITERAL
	std::printf("    'operator \"\"'(user defined literal) is featured.\n");
#endif//GASHA_HAS_USER_DEFINED_LITERAL

	//【C++11仕様】TLSテスト
	std::printf("\n");
	std::printf("TLS Variable=%d\n", m_var_tls);
#ifdef GASHA_HAS_THREAD_LOCAL
	std::printf("    'thread_local' is featured.\n");
#endif//GASHA_HAS_THREAD_LOCAL
}

// End of file
