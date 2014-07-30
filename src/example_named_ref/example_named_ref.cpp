//--------------------------------------------------------------------------------
// exmaple_named_ref.cpp
// 名前付きデータ参照テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_named_ref.h"//名前付きデータ参照テスト

#include <gasha/named_ref.h>//名前付きデータ参照

#include <cstdio>//std::printf()
#include <utility>//std::declval()

GASHA_USING_NAMESPACE;//ネームスペース使用

static int a = 0;
//----------------------------------------
//名前付きデータ参照テスト
void example_named_ref()
{
	std::printf("----- Test for namedRef -----\n");
#if 0
	namedRef ref;
	ref.regist("valueA", a);
	auto x1 = ref.load<int>("valueA");
	auto& x2 = ref.ref<int>("valueA");
	x2 = 1;
	ref.store<int>("valueA", 2);
#endif

	std::printf("- end -\n");
}

// End of file
