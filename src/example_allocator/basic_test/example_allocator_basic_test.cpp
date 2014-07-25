//--------------------------------------------------------------------------------
// exmaple_allocator_basic_test.cpp
// アロケータ系基本テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/lf_stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/new.h>//多態アロケータ対応new/delete

#include <cstdio>//std::printf()

//【VC++】アラインメントが設定された構造体のメモリを確保しようとすると、warning C4316 が発生する
//  warning C4316 : 'data_t' : ヒープで割り当てられたオブジェクトが 16 にアラインメントされていない可能性があります
#pragma warning(disable: 4316)//C4316を抑える

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//アロケータ系基本テスト
void example_allocator_basic_test()
{
	std::printf("\n");
	std::printf("================================================================================\n");
	
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Basic test for allocators ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	//スタックアロケータ
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Stack allocator\n");
		
		char stack_buff[1024];//スタックアロケータ用のバッファ
		stackAllocator<> stack(stack_buff);//通常のスタックアロケータ
		//stackAllocator_withBuff<1024> stack;//バッファを外部渡しせずに内包する場合
		//stackAllocator<spinLock> stack(stack_buff);//スピンロックで排他制御する場合
		//stackAllocator<std::mutex> stack(stack_buff);//C++11のミューテックスで排他制御する場合
		//lfStackAllocator<> stack(stack_buff);//ロックフリースタックアロケータを使用する場合（排他制御なしでマルチスレッドセーフ）

		void* p = stack.alloc(20);//スタックアロケータからメモリをアロケート
		data_t* data = stack.newObj<data_t>(123);//スタックアロケータを使用して new（メモリ確保とコンストラクタを呼び出し）※デフォルトコンストラクタ以外を呼び出せる
		data_t* array = stack.newArray<data_t>(3, 456);//スタックアロケータを使用して new[]（メモリ確保とコンストラクタを呼び出し）※new[] と異なり、デフォルトコンストラクタ以外を呼び出せる
		stack.free(p);//メモリ解放 ※実際には解放されない
		stack.deleteObj(data);//スタックアロケータに対する delete （デストラクタ呼び出しとメモリ解放） ※実際には解放されない
		stack.deleteArray(array, 3);//スタックアロケータに対する delete[] （デストラクタ呼び出しとメモリ解放） ※実際には解放されない ※配列数を渡す必要あり
		//stack.rewind(p);//指定ポインタの位置に空きメモリを戻す
		//stack.rewind(10);//バッファサイズ（使用中のサイズ）を指定して、空きメモリを戻す（使用サイズを増やすことはできない）
		//stack.clear();//全メモリ空き状態（初期状態）に戻す
		
		std::printf("data=%p, data->m_val=%d, array=%p, array[0].m_val=%d, stack.maxSize()=%d, stack.size()=%d, stack.remain()=%d\n", data, data->m_val, array, array[0].m_val, stack.maxSize(), stack.size(), stack.remain());
	}

	//スマートスタックアロケータ
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Smart stack allocator\n");

		char stack_buff[1024];//スタックアロケータ用のバッファ
		smartStackAllocator<> stack(stack_buff);//通常スマートスタックアロケータ

		void* p1 = stack.alloc(20);//スタックアロケータからメモリをアロケート
		void* p2 = stack.alloc(20);//（同上）
		void* p3 = stack.alloc(20);//（同上）
		stack.free(p1);//メモリ解放 ※実際には解放されない
		stack.free(p2);//メモリ解放 ※実際には解放されない
		stack.free(p3);//メモリ解放 ※アロケート数と同数の解放により、自動的にメモリクリア
		
		std::printf("p1=%p, p2=%p, p3=%p, stack.maxSize()=%d, stack.size()=%d, stack.remain()=%d\n", p1, p2, p3, stack.maxSize(), stack.size(), stack.remain());
	}

	//多態アロケータ
	//【注意】多態アロケータを使用するには、ビルド設定（project_last_settings.h）で
	//        GASHA_ENABLE_POLY_ALLOCATOR を有効化し、多態アロケータを有効化する必要あり。
	//        これにより、標準new/deleteが多態アロケータに置き換わることに注意。
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Poly allocator\n");

		stackAllocator_withBuff<1024> stack;//スタックアロケータを用意
		auto adapter = stack.adapter();//アロケータアダプターを取得　⇒　この auto 型は allocatorAdapter<stackAllocator<>>
		polyAllocator poly(adapter);//多態アロケータにアダプターを渡す
		//以上により、処理ブロック（スコープ）内での new/new[]/delte/delete[] が、暗黙的にスタックアロケータを使用するようになる

		data_t* data = new data_t(123);
		data_t* array = new data_t[3];
		delete data;
		delete[] array;

		data_t* data2 = GASHA_NEW(data_t, 123);//マクロを使用すると、アラインメントを保証する
		data_t* array2 = GASHA_NEW_ARRAY(data_t, 3);//配列にも対応 ※【注意】VC++では、アライメントサイズが16バイトより大きく、デストラクタのある配列は正しいメモリ確保ができないので注意
		GASHA_DELETE(data2);
		GASHA_DELETE_ARRAY(array2);

		std::printf("data=%p, data->m_val=%d, array=%p, array[0].m_val=%d, data2=%p, data2->m_val=%d, array2=%p, array2[0].m_val=%d, stack.maxSize()=%d, stack.size()=%d, stack.remain()=%d\n", data, data->m_val, array, array[0].m_val, data2, data2->m_val, array2, array2[0].m_val, stack.maxSize(), stack.size(), stack.remain());
	}

	//※双方向スタックアロケータ、単一アロケータ、プールアロケータ、標準アロケータも同様に操作可能
	
	std::printf("\n");
	std::printf("================================================================================\n");
}

// End of file
