//--------------------------------------------------------------------------------
// exmaple_poly_allocator.cpp
// 多態アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//【注意】多態アロケータを使用するには、ビルド設定（project_last_settings.h）で
//        GASHA_ENABLE_POLY_ALLOCATOR を有効化し、多態アロケータを有効化する必要あり。
//        これにより、標準new/deleteが多態アロケータに置き換わることに注意。

#include "example_allocator.h"//アロケータテスト

#include <gasha/new.h>//多態アロケータ対応new/delete：GASHA_NEW/GASHA_DELETE

#include <gasha/mono_allocator.h>//単一アロケータ
#include <gasha/lf_mono_allocator.h>//ロックフリー単一アロケータ
#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/dual_stack_allocator.h>//双方向スタックアロケータ
#include <gasha/lf_dual_stack_allocator.h>//ロックフリー双方向スタックアロケータ
#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ
#include <gasha/std_allocator.h>//標準アロケータ
#include <gasha/scoped_stack_allocator.h>//スコープスタックアロケータ
#include <gasha/scoped_dual_stack_allocator.h>//双方向スコープスタックアロケータ

#include <gasha/call_point.h>//コールポイント
#include <gasha/log_category.h>//ログカテゴリ

#include <cstdio>//std::printf()

//【VC++】例外を無効化した状態で <vector> をインクルードすると、もしくは、new演算子を使用すると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <vector>//std::vector

//【VC++】アライメントが指定されたクラス／構造体を new すると、warning C4316 が発生する
//  warning C4316 : '(class_name)' : ヒープで割り当てられたオブジェクトが (16) にアラインメントされていない可能性があります
#pragma warning(disable: 4316)//C4316を抑える

GASHA_USING_NAMESPACE;//ネームスペース使用

#define EXPR_PLAIN(...) std::printf("> %s\n", #__VA_ARGS__); __VA_ARGS__

//----------------------------------------
//様々なアロケータ
static stackAllocator_withBuff<1024> s_stackAllocator;//スタックアロケータ
static poolAllocator_withBuff<48, 8, 16> s_poolAllocator;//プールアロケータ

//----------------------------------------
//多態アロケータ使用基本形
static void testBasic()
{
	std::printf("\n");
	std::printf("--------------------------------------------------\n");
	std::printf("[ Test for polyAllocator with new/delete operator ]\n");
	std::printf("\n");

	EXPR_PLAIN(auto adapter = s_stackAllocator.adapter());//スタックアロケータのアダプターを取得（変数を直接操作することはない）
	EXPR_PLAIN(polyAllocator poly_allocator(adapter););//多態アロケータにアダプターをセット（変数を直接操作することはない）
	std::printf("poly_allocator: name=\"%s\", mode=\"%s\"\n", poly_allocator.name(), poly_allocator.mode());
	std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
	
	EXPR_PLAIN(data_t* p101 = new data_t;);//new演算子で領域確保
	EXPR_PLAIN(data_t* p102 = new data_t[2];);//new[]演算子で領域確保
	EXPR_PLAIN(data_t* p103 = new data_t[3];);
	std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
	
	{
		//ネストした処理ブロックで別のアロケータに切り替え
		std::printf("***** BEGIN BLOCK *****\n");
		EXPR_PLAIN(auto nested_adapter = s_poolAllocator.adapter());//プールアロケータのアダプターを取得
		EXPR_PLAIN(polyAllocator nested_poly_allocator(nested_adapter););//多態アロケータにアダプターをセット
		std::printf("nested_poly_allocator: name=\"%s\", mode=\"%s\"\n", nested_poly_allocator.name(), nested_poly_allocator.mode());
		std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
		
		EXPR_PLAIN(data_t* p201 = new data_t;);//new演算子で領域確保
		EXPR_PLAIN(data_t* p202 = new data_t[2];);//new[]演算子で領域確保
		EXPR_PLAIN(data_t* p203 = new(std::nothrow) data_t[3];);//ブロックサイズオーバーでメモリ確保失敗
		std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
		
		EXPR_PLAIN(delete p201;);//delete演算子で領域解放
		EXPR_PLAIN(delete[] p202;);//delete[]演算子で領域解放
		EXPR_PLAIN(delete[] p203;);
		std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
		std::printf("***** END BLOCK *****\n");
	}
	//処理ブロックを抜けると元のアロケータに戻る
	
	EXPR_PLAIN(data_t* p104 = new data_t;);//new演算子で領域確保
	std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
	
	EXPR_PLAIN(delete p101;);//delete演算子で領域解放
	EXPR_PLAIN(delete[] p102;);//delete[]演算子で領域解放
	EXPR_PLAIN(delete[] p103;);
	EXPR_PLAIN(delete p104;);
	{
		//スコープスタックもアダプター化可能
		auto scoped = s_stackAllocator.scopedAllocator();
		auto adapter = scoped.adapter();
		polyAllocator poly_allocator(adapter);
		char message[1024];
		scoped.debugInfo(message, sizeof(message)); std::printf("%s\n", message);
		data_t* p101 = new data_t;
		scoped.debugInfo(message, sizeof(message)); std::printf("%s\n", message);
		delete p101;
		scoped.debugInfo(message, sizeof(message)); std::printf("%s\n", message);
	}
	std::printf("stack:size=%d,count=%d, pool:size=%d,pool=%d\n", s_stackAllocator.size(), s_stackAllocator.count(), s_poolAllocator.size(), s_poolAllocator.usingPoolSize());
}

//----------------------------------------
//多態アロケータ活用例：ローカルメモリでSTLを使用
static void testSTL()
{
	std::printf("\n");
	std::printf("--------------------------------------------------\n");
	std::printf("[ Test for polyAllocator with STL ]\n");
	std::printf("\n");

	char message[1024];

	EXPR_PLAIN(smartStackAllocator_withBuff<2048> stack;);//スタックアロケータ
	{
		std::printf("***** BEGIN BLOCK *****\n");
		EXPR_PLAIN(auto adapter = stack.adapter(););//スタックアロケータのアダプターを取得
		EXPR_PLAIN(polyAllocator poly(adapter););//多態アロケータにアダプターをセット（処理ブロックを抜ける時に元に戻る）

		EXPR_PLAIN(std::vector<data_t> array;);//STLのstd::vectorを使用
		EXPR_PLAIN(data_t data;);
		EXPR_PLAIN(array.push_back(data););
		EXPR_PLAIN(array.push_back(data););
		EXPR_PLAIN(array.push_back(data););

		EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf("%s\n", message););//STL内のnewがローカルのスタックアロケータを使用していることを確認
		std::printf("***** END BLOCK *****\n");
	}
	EXPR_PLAIN(stack.debugInfo(message, sizeof(message)); std::printf("%s\n", message););//スマートスタックアロケータにより、アロケータのメモリが空いたことを確認
}

//----------------------------------------
//多態アロケータ応用例：GASHA_NEW / GASHA_DELETE マクロでアラインメント保証＆デバッグ情報取得
static void testAdvanced()
{
	std::printf("\n");
	std::printf("--------------------------------------------------\n");
	std::printf("[ Test for polyAllocator with GASHA_NEW/GASHA_DELETE ]\n");
	std::printf("\n");

	//デバッグ用 operator new 時コールバック
	auto at_new = [](const iAllocatorAdapter& adapter, const void* p, const std::size_t size, const std::size_t align, const newMethod_type method, const debugAllocationInfo* info)
	{
	#ifdef GASHA_DEBUG_FEATURE_IS_ENABLED
		std::printf("[CALLBACK] Operator new%s(%d,%d)p=%p, allocator=\"%s:%s\"\n", method == methodOfNewArrays ? "[]" : "", size, align, p, adapter.name(), adapter.mode());
		if (info)
			std::printf(" %s[%d], file=\"%s\", func=\"%s\", time=%lf, cp=\"%s\", criticalCp=\"%s\"\n", info->m_typeName, info->m_arrayNum, info->m_fileName, info->m_funcName, info->m_time, info->m_cpName, info->m_criticalCpName);
	#endif//GASHA_DEBUG_FEATURE_IS_ENABLED
	};
	
	//デバッグ用 operator delete 時コールバック
	auto at_delete = [](const iAllocatorAdapter& adapter, const void* p, const deleteMethod_type method, const debugAllocationInfo* info)
	{
	#ifdef GASHA_DEBUG_FEATURE_IS_ENABLED
		std::printf("[CALLBACK] Operator delete%s(%p), allocator=\"%s:%s\"\n", method == methodOfDeleteArrays ? "[]" : "", p, adapter.name(), adapter.mode());
		if (info)
			std::printf(" %s%s, file=\"%s\", func=\"%s\", time=%lf, cp=\"%s\", criticalCp=\"%s\"\n", info->m_typeName, method == methodOfDeleteArrays ? "[]" : "", info->m_fileName, info->m_funcName, info->m_time, info->m_cpName, info->m_criticalCpName);
	#endif//GASHA_DEBUG_FEATURE_IS_ENABLED
	};

	//デバッグ用 多態アロケータ変更時コールバック
	auto at_change_allocator = [](const iAllocatorAdapter& adapter, const iAllocatorAdapter& next_adapter)
	{
	#ifdef GASHA_DEBUG_FEATURE_IS_ENABLED
		std::printf("[CALLBACK] Change allocator: \"%s:%s\" -> \"%s:%s\"\n", adapter.name(), adapter.mode(), next_adapter.name(), next_adapter.mode());
	#endif//GASHA_DEBUG_FEATURE_IS_ENABLED
	};

	//デバッグ用 多態アロケータ復帰時コールバック
	auto at_return_allocator = [](const iAllocatorAdapter& adapter, const iAllocatorAdapter& prev_adapter)
	{
	#ifdef GASHA_DEBUG_FEATURE_IS_ENABLED
		std::printf("[CALLBACK] Return allocator: \"%s:%s\" <- \"%s:%s\"\n", adapter.name(), adapter.mode(), prev_adapter.name(), prev_adapter.mode());
	#endif//GASHA_DEBUG_FEATURE_IS_ENABLED
	};

	EXPR_PLAIN(smartStackAllocator_withBuff<2048> stack;);//スタックアロケータ
	EXPR_PLAIN(auto adapter = stack.adapter(););//スタックアロケータのアダプターを取得
	EXPR_PLAIN(polyAllocator poly(adapter););//多態アロケータにアダプターをセット

	//デバッグ用オブザーバー（観察者）を多態アロケータに登録
	EXPR_PLAIN(debugAllocationObserver observer;);
	observer.m_atNew = at_new;
	observer.m_atDelete = at_delete;
	observer.m_atChangeAllocator = at_change_allocator;
	observer.m_atReturnAllocator = at_return_allocator;
	EXPR_PLAIN(poly.setDebugObserver(observer););//多態アロケータにオブザーバー（コールバックのセット）を追加

	//GASHA_NEW でメモリ確保
	callPoint cp1(forAny, "Test for GASHA_NEW", GASHA_CP_ARGS);
	EXPR_PLAIN(data_t* p1 = GASHA_NEW(data_t););
	{
		//GASHA_NEW で配列メモリ確保
		criticalCallPoint cp2(forAny, "Test for GASHA_NEW[]", GASHA_CP_ARGS);
		EXPR_PLAIN(data_t* p2 = GASHA_NEW(data_t[2]););
		{
			//GASHA_NEW_ARRAY で配列メモリ確保
			callPoint cp3(forAny, "Test for GASHA_NEW_ARRAY", GASHA_CP_ARGS);
			EXPR_PLAIN(data_t* p3 = GASHA_NEW_ARRAY(data_t, 3););
			
			//GASHA_DELETE_ARRAY でメモリ破棄
			EXPR_PLAIN(GASHA_DELETE_ARRAY(p3););
		}
		//GASHA_DELETE_ARRAY でメモリ破棄
		EXPR_PLAIN(GASHA_DELETE_ARRAY(p2););
	}
	//GASHA_DELETE でメモリ破棄
	EXPR_PLAIN(GASHA_DELETE(p1););

	//STLのstd::vectorを使用
	EXPR_PLAIN(std::vector<data_t> array;);
	EXPR_PLAIN(data_t data;);
	EXPR_PLAIN(array.push_back(data););
	EXPR_PLAIN(array.push_back(data););
	EXPR_PLAIN(array.push_back(data););

	{
		//ネストした処理ブロックで別のアロケータに切り替え
		std::printf("***** BEGIN BLOCK *****\n");
		EXPR_PLAIN(auto nested_adapter = s_poolAllocator.adapter());//プールアロケータのアダプターを取得
		EXPR_PLAIN(polyAllocator nested_poly_allocator(nested_adapter););//多態アロケータにアダプターをセット
		std::printf("***** END BLOCK *****\n");
	}
	
	//オブザーバーをリセット
	EXPR_PLAIN(poly.resetDebugObserver(););
}

//----------------------------------------
//全種オブジェクトのアダプター化を確認
void testAllAdapters()
{
	std::printf("\n");
	std::printf("--------------------------------------------------\n");
	std::printf("[ Test for polyAllocator adapters ]\n");
	std::printf("\n");

	char buff[1024];

	//スタックアロケータ
	{
		EXPR_PLAIN(stackAllocator<> stack_allocator(buff););
		EXPR_PLAIN(auto adapter = stack_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		{
			EXPR_PLAIN(auto scoped_allcator = stack_allocator.scopedAllocator(););
			EXPR_PLAIN(auto adapter = scoped_allcator.adapter(););
			EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		}
	}

	//ロックフリースタックアロケータ
	{
		EXPR_PLAIN(lfStackAllocator<> lf_stack_allocator(buff););
		EXPR_PLAIN(auto adapter = lf_stack_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		{
			EXPR_PLAIN(auto scoped_allcator = lf_stack_allocator.scopedAllocator(););
			EXPR_PLAIN(auto adapter = scoped_allcator.adapter(););
			EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		}
	}

	//双方向スタックアロケータ
	{
		EXPR_PLAIN(dualStackAllocator<> dual_stack_allocator(buff););
		EXPR_PLAIN(auto adapter = dual_stack_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		{
			EXPR_PLAIN(auto scoped_allcator = dual_stack_allocator.scopedAllocator(););
			EXPR_PLAIN(auto adapter = scoped_allcator.adapter(););
			EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		}
	}

	//ロックフリー双方向スタックアロケータ
	{
		EXPR_PLAIN(lfDualStackAllocator<> lf_dual_stack_allocator(buff););
		EXPR_PLAIN(auto adapter = lf_dual_stack_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		{
			EXPR_PLAIN(auto scoped_allcator = lf_dual_stack_allocator.scopedAllocator(););
			EXPR_PLAIN(auto adapter = scoped_allcator.adapter(););
			EXPR_PLAIN(polyAllocator poly_allocator(adapter););
		}
	}

	//単一アロケータ
	{
		EXPR_PLAIN(monoAllocator<> mono_allocator(buff););
		EXPR_PLAIN(auto adapter = mono_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
	}

	//ロックフリー単一アロケータ
	{
		EXPR_PLAIN(lfMonoAllocator lf_mono_allocator(buff););
		EXPR_PLAIN(auto adapter = lf_mono_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
	}

	//標準アロケータ
	{
		EXPR_PLAIN(stdAllocator<> std_allocator;);
		EXPR_PLAIN(auto adapter = std_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
	}

	//アラインメント対応標準アロケータ
	{
		EXPR_PLAIN(stdAlignAllocator<> std_align_allocator;);
		EXPR_PLAIN(auto adapter = std_align_allocator.adapter(););
		EXPR_PLAIN(polyAllocator poly_allocator(adapter););
	}
}

//----------------------------------------
//多態アロケータテスト
void example_poly_allocator()
{
	std::printf("\n");
	std::printf("================================================================================\n");
	
	//多態アロケータ使用基本形
	testBasic();

	//多態アロケータ活用例：ローカルメモリでSTLを利用
	testSTL();
	
	//多態アロケータ応用例：GASHA_NEW / GASHA_DELETE マクロでアラインメント保証＆デバッグ情報取得
	testAdvanced();

	//全種オブジェクトのアダプター化を確認
	testAllAdapters();
}

// End of file
