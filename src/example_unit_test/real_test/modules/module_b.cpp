//--------------------------------------------------------------------------------
// module_b.cpp
// ユニットテスト：モジュールB
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

class moduleB
{
public:
	int func1(int p1, int p2, int p3)
	{
		return p1 * p2 * p3;
	}
	void func2(int p1, int p2, int& ret)
	{
		ret = p1 * p2;
	}
};

class moduleC
{
public:
	float func(float p1, float p2, float& ret1, float& ret2)
	{
		ret1 = p1 + p2;
		ret2 = p1 - p2;
		return p1 * p2;
	}
};

//ユニットテスト
#include <gasha/unit_test.h>
#include "real_test/unit_test_id.h"
#pragma warning(push)
#pragma warning(disable: 4530)
GASHA_UT_BEGIN(moduleB, ut::GROUP_USER_B, ut::ATTR_AUTO)//モジュール名とグループID、属性を指定 ※特定のモジュール名やグループIDに絞ったテストを実行可能。
                                                        //                                     ※mainから自動実行するには ATTR_AUTO 属性を付与する必要がある。
                                                        //                                       属性は第0ビット目が自動実行用に予約されている以外は自由に使って良い。
                                                        //                                       テスト実行時に対象属性の指定があり、ビットマスク（&演算の結果が0以外かどうか）で実行の可否が判定される仕組み。
{
	moduleB module_b;
	int oparam = 0;
	GASHA_UT_EXPECT_EQ(module_b.func1(1, 2, 3), 1+2+3);
	GASHA_UT_EXPECT_NE(module_b.func1(3, 4, 5), 60);
	GASHA_UT_EXPECT_GT(module_b.func1(5, 6, 7), 10);
	GASHA_UT_EXPECT_GE(module_b.func1(7, 8, 9), 20);
	GASHA_UT_EXPECT_LT(module_b.func1(9, 10, 11), 30);
	GASHA_UT_EXPECT_LE(module_b.func1(11, 12, 13), 40);
	GASHA_UT_EXPR(module_b.func2(1, 2, oparam)); GASHA_UT_EXPECT_EQ_CHILD(oparam, 2);
	GASHA_UT_EXPR(module_b.func2(3, 4, oparam)); GASHA_UT_EXPECT_NE_CHILD(oparam, 12);
}
GASHA_UT_END()
GASHA_UT_BEGIN(moduleC, ut::GROUP_USER_B, ut::ATTR_MANUAL)//モジュール名とグループID、属性を指定
{
	moduleC module_c;
	float out_par1 = 0.f;
	float out_par2 = 0.f;
	GASHA_UT_EXPR_WITH_RET(module_c.func(1.1f, 2.2f, out_par1, out_par2)); GASHA_UT_EXPECT_EQ_CHILD(out_par1, 2.f); GASHA_UT_EXPECT_LT_CHILD(out_par2, 0.f);
	GASHA_UT_EXPR_WITH_RET(module_c.func(3.3f, 4.4f, out_par1, out_par2)); GASHA_UT_EXPECT_NE_CHILD(out_par1, 7.f); GASHA_UT_EXPECT_GT_CHILD(out_par2, 0.f);
	GASHA_UT_EXPR_WITH_RET(module_c.func(5.5f, 6.6f, out_par1, out_par2)); GASHA_UT_EXPECT_LE_CHILD(out_par1, 20.f); GASHA_UT_EXPECT_GE_CHILD(out_par2, 10.f);
	GASHA_UT_EXPR_WITH_RET(module_c.func(7.7f, 8.8f, out_par1, out_par2)); GASHA_UT_EXPECT_LT_CHILD(out_par1, 1.f); GASHA_UT_EXPECT_LE_CHILD(out_par2, 2.f);
}
GASHA_UT_END()
#pragma warning(pop)

// End of file
