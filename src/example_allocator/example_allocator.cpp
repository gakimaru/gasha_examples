//--------------------------------------------------------------------------------
// exmaple_thrad_id.cpp
// アロケータテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_allocator.h"//アロケータテスト

//#include <gasha/stack_allocator.h>//スタックアロケータ
//#include <gasha/stack_allocator.h>//双方向スタックアロケータ
//#include <gasha/stack_allocator.h>//ロックフリースタックアロケータ
#include <gasha/pool_allocator.h>//プールアロケータ
#include <gasha/lf_pool_allocator.h>//ロックフリープールアロケータ
//アロケータアダプタ
//多態アロケータ

#include <gasha/pool_allocator.cpp.h>//プールアロケータ
#include <gasha/lf_pool_allocator.cpp.h>//ロックフリープールアロケータ

#include <stdio.h>//printf()

//【VC++】例外を無効化した状態で <thread> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <thread>//C++11 std::thread

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//アロケータテスト
void example_allocator()
{
	printf("----- Test for allocator -----\n");

	//仮
	{
		int x[10];
		poolAllocator<10> x1(x);
		lfPoolAllocator<10> x2(x);

		poolAllocator<10> x3(x, sizeof(x));
		lfPoolAllocator<10> x4(x, sizeof(x));
	}

	printf("- end -\n");
}

// End of file
