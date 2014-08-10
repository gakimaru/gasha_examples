#pragma once
#ifndef GASHA_INCLUDED_UNIT_TEST_ID_H
#define GASHA_INCLUDED_UNIT_TEST_ID_H

//--------------------------------------------------------------------------------
// unit_test_id.h
// ユニットテスト：ユニットテストID定義
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/unit_test.h>

#ifdef GASHA_UNIT_TEST_ENABLED

GASHA_NAMESPACE_BEGIN;//ネームスペース：開始

namespace ut
{
	//ユニットテストグループID
	enum groupEnum
	{
		GROUP_NONE = 0,
		GROUP_USER_A = 1,
		GROUP_USER_B = 2,
	};

}//namespace ut

GASHA_NAMESPACE_END;//ネームスペース：終了

#endif//GASHA_UNIT_TEST_ENABLED

#endif//GASHA_INCLUDED_UNIT_TEST_ID_H

// End of file
