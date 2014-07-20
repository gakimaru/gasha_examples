//--------------------------------------------------------------------------------
// exmaple_thrad_id.cpp
// スレッドIDテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_thread_id.h"//スレッドIDテスト

#include <gasha/thread_id.h>//スレッドID

#include <gasha/type_traits.h>//型特性ユーティリティ：toStr()

#include <cstdio>//std::printf()

//【VC++】例外を無効化した状態で <thread> をインクルードすると、warning C4530 が発生する
//  warning C4530: C++ 例外処理を使っていますが、アンワインド セマンティクスは有効にはなりません。/EHsc を指定してください。
#pragma warning(disable: 4530)//C4530を抑える

#include <thread>//C++11 std::thread

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//スレッドIDテスト
void example_thread_id()
{
	std::printf("----- Test for threadId -----\n");

	threadId main_thread_id("Main Thread");
	std::printf("MainThread: ID=0x%08x, Name=\"%s\"(CRC=0x%08x)\n", main_thread_id.id(), main_thread_id.name(), main_thread_id.nameCrc());

	auto sub_thread_func = [](const char* name, threadId src_thread_id)
	{
		threadId this_thread_id(name);
		threadId copied_thread_id;
		copied_thread_id = src_thread_id;
		std::printf("This thread: ID=0x%08x, name=\"%s\"(CRC=0x%08x)\n", this_thread_id.id(), this_thread_id.name(), this_thread_id.nameCrc());
		std::printf("Source thread: ID=0x%08x, name=\"%s\"(CRC=0x%08x)\n", src_thread_id.id(), src_thread_id.name(), src_thread_id.nameCrc());
		std::printf("Copied thread id: ID=0x%08x, name=\"%s\"(CRC=0x%08x)\n", copied_thread_id.id(), copied_thread_id.name(), copied_thread_id.nameCrc());
		std::printf("src_thread_id == this_thread_id ... %s\n", toStr(src_thread_id == this_thread_id));
		std::printf("src_thread_id != this_thread_id ... %s\n", toStr(src_thread_id != this_thread_id));
		std::printf("src_thread_id == copied_thread_id ... %s\n", toStr(src_thread_id == copied_thread_id));
		std::printf("src_thread_id != copied_thread_id ... %s\n", toStr(src_thread_id != copied_thread_id));
	};

	std::thread* sub_thread = new std::thread(sub_thread_func, "Sub thread", main_thread_id);
	sub_thread->join();
	delete sub_thread;
	sub_thread = nullptr;

	std::printf("- end -\n");
}

// End of file
