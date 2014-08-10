//--------------------------------------------------------------------------------
// module_a.cpp
// ユニットテスト：モジュールA
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable: 4530)
#include <stdexcept>//std::runtime_error()
#pragma warning(pop)

class moduleA
{
public:
	int func1(int p1, int p2)
	{
		return p1 + p2;
	}
	bool func2()
	{
		return true;
	}
	bool func3()
	{
		throw std::runtime_error("Faile to custom unit-test");
		return true;
	}
};

//ユニットテスト
#include <gasha/unit_test.h>
#include "real_test/unit_test_id.h"
#pragma warning(push)
#pragma warning(disable: 4530)
GASHA_UT_BEGIN(moduleA, ut::GROUP_USER_A, ut::ATTR_AUTO)//モジュール名とグループID、属性を指定 ※特定のモジュール名やグループIDに絞ったテストを実行可能。
                                                        //                                     ※mainから自動実行するには ATTR_AUTO 属性を付与する必要がある。
                                                        //                                       属性は第0ビット目が自動実行用に予約されている以外は自由に使って良い。
                                                        //                                       テスト実行時に対象属性の指定があり、ビットマスク（&演算の結果が0以外かどうか）で実行の可否が判定される仕組み。
{
	moduleA module_a;
	GASHA_UT_EXPECT_EQ(module_a.func1(1, 2), 3);
	GASHA_UT_EXPECT_EQ(module_a.func1(3, 4), 7);
	GASHA_UT_EXPECT_NE(module_a.func1(4, 5), 9);
	GASHA_UT_EXPECT_EQ(module_a.func2(), true);
	GASHA_UT_EXPECT_NE(module_a.func2(), true);
	GASHA_UT_EXPECT_EQ(module_a.func3(), true);
}
GASHA_UT_END()
#pragma warning(pop)

// End of file
