﻿//--------------------------------------------------------------------------------
// exmaple_fast_string.cpp
// 高速文字列操作テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_fast_string.h"//高速文字列操作テスト

#include <gasha/fast_string.h>//高速文字列処理
#include <gasha/chrono.h>//時間処理ユーティリティ：nowTime(), calcElapsedTime()
#include <gasha/crc32.h>//CRC32計算
#include <gasha/type_traits.h>//型特性ユーティリティ：extentof()

#include <cstddef>//std::size_t
#include <cstring>//memset()
#include <cstdio>//std::printf()

//【VC++】str*** を使用すると、error C4996 が発生する
//  error C4996: 'str***': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable: 4996)//C4996を抑える

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//高速文字列操作テスト
void example_fast_string()
{
	char src_buff[128];
	char buff[81];
	auto reset_buff = [&buff]()
	{
		std::memset(buff, 'X', sizeof(buff));
		buff[sizeof(buff) - 1] = '\0';
	};

	//テスト用データ初期化
	initStrSet();

	//strlenテスト
	auto test_strlen = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strlen_fast ]\n");
		}
		int num = 0;
		for (std::size_t i = 0; i < getStrSetNum(); ++i)
		{
			++num;
			std::size_t ret1 = 0;
			std::size_t ret2 = 0;
			std::size_t ret3 = 0;
			if (test_mode == 0 || test_mode == 1)
			{
				ret1 = strlen_fast(getStr(src_buff, i, loop));
				if (test_mode == 0)
					std::printf("strlen_fast(str[%d]): ret=%d\n", static_cast<int>(i), static_cast<int>(ret1));
			}
			if (test_mode == 0 || test_mode == 2)
			{
				ret2 = strlen_sse(getStr(src_buff, i, loop));
				if (test_mode == 0)
					std::printf("strlen_sse( str[%d]): ret=%d\n", static_cast<int>(i), static_cast<int>(ret2));
			}
			if (test_mode == 0 || test_mode == 3)
			{
				ret3 = GASHA_ strlen(getStr(src_buff, i, loop));
				if (test_mode == 0)
					std::printf("strlen(     str[%d]): ret=%d\n", static_cast<int>(i), static_cast<int>(ret3));
			}
			if (test_mode == 0)
			{
				if (ret1 != ret2 || ret1 != ret3)
					std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			}
		}
		return num;
	};

	//strnlenテスト
	auto test_strnlen = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strnlen_fast ]\n");
		}
		static const std::size_t max_len_types[] = { 0, 1, 15, 16, 17, 32, 33, sizeof(buff) };
		int num = 0;
		for (std::size_t type = 0; type < extentof(max_len_types); ++type)
		{
			const std::size_t max_len = max_len_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				std::size_t ret1 = 0;
				std::size_t ret2 = 0;
				std::size_t ret3 = 0;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strnlen_fast(getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
						std::printf("strnlen_fast(str[%d], %d): ret=%d (strlen=%d)\n", static_cast<int>(i), static_cast<int>(max_len), static_cast<int>(ret1), static_cast<int>(getStrLen(i, loop)));
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strnlen_sse(getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
						std::printf("strnlen_sse( str[%d], %d): ret=%d (strlen=%d)\n", static_cast<int>(i), static_cast<int>(max_len), static_cast<int>(ret2), static_cast<int>(getStrLen(i, loop)));
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = GASHA_ strnlen(getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
						std::printf("strnlen(     str[%d], %d): ret=%d (strlen=%d)\n", static_cast<int>(i), static_cast<int>(max_len), static_cast<int>(ret3), static_cast<int>(getStrLen(i, loop)));
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strcmpテスト
	auto test_strcmp = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strcmp_fast ]\n");
		}
		int num = 0;
		static const char* pattern_types[] = { "1", "12", "123456789012345", "1234567890123456", "12345678901234567", "1234567890123456789012345678901", "12345678901234567890123456789012", "123456789012345678901234567890123", "Z" };
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char* pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				int ret1 = 0;
				int ret2 = 0;
				int ret3 = 0;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strcmp_fast(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strcmp_fast(str[%d], \"%s\"): ret=%d\n", static_cast<int>(i), pattern, ret1);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strcmp_sse(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strcmp_sse( str[%d], \"%s\"): ret=%d\n", static_cast<int>(i), pattern, ret2);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = GASHA_ strcmp(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strcmp(     str[%d], \"%s\"): ret=%d\n", static_cast<int>(i), pattern, ret3);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};
	
	//strncmpテスト
	auto test_strncmp = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strncmp_fast ]\n");
		}
		static const char* pattern_types[] = { "1", "12", "123456789012345", "1234567890123456", "12345678901234567", "1234567890123456789012345678901", "12345678901234567890123456789012", "123456789012345678901234567890123", "Z" };
		static const std::size_t pattern_len_types[] = { 1, 2, 15, 16, 17, 31, 32, 33, 1};
		static_assert(GASHA_EXTENT1OF(pattern_types) == GASHA_EXTENT1OF(pattern_len_types), "pattern_types[] != pattern_len_types[]");
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			for (int max_len_diff = -1; max_len_diff <= 1; ++max_len_diff)
			{
				const char* pattern = pattern_types[type];
				for (std::size_t i = 0; i < getStrSetNum(); ++i)
				{
					++num;
					std::size_t max_len = static_cast<std::size_t>(static_cast<int>(pattern_len_types[type]) + max_len_diff);
					int ret1 = 0;
					int ret2 = 0;
					int ret3 = 0;
					if (test_mode == 0 || test_mode == 1)
					{
						ret1 = strncmp_fast(getStr(src_buff, i, loop), pattern, max_len);
						if (test_mode == 0)
							std::printf("strncmp_fast(str[%d], \"%s\", %d): ret=%d\n", static_cast<int>(i), pattern, static_cast<int>(max_len), ret1);
					}
					if (test_mode == 0 || test_mode == 2)
					{
						ret2 = strncmp_sse(getStr(src_buff, i, loop), pattern, max_len);
						if (test_mode == 0)
							std::printf("strncmp_sse( str[%d], \"%s\", %d): ret=%d\n", static_cast<int>(i), pattern, static_cast<int>(max_len), ret2);
					}
					if (test_mode == 0 || test_mode == 3)
					{
						ret3 = GASHA_ strncmp(getStr(src_buff, i, loop), pattern, max_len);
						if (test_mode == 0)
							std::printf("strncmp(     str[%d], \"%s\", %d): ret=%d\n", static_cast<int>(i), pattern, static_cast<int>(max_len), ret3);
					}
					if (test_mode == 0)
					{
						if (ret1 != ret2 || ret1 != ret3)
							std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
					}
				}
			}
		}
		return num;
	};

	//strchrテスト
	auto test_strchr = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strstr_fast ]\n");
		}
		static const char pattern_types[] = { '0', '1', '2', 'e', 'f', 't', 'u' };
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strchr_fast(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strchr_fast(str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret1, ret1);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strchr_sse(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strchr_sse( str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret2, ret2);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = strchr(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strchr(     str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret3, ret3);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strrchrテスト
	auto test_strrchr = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strstr_fast ]\n");
		}
		static const char pattern_types[] = { '0', '1', '2', 'e', 'f', 't', 'u' };
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strrchr_fast(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strrchr_fast(str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret1, ret1);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strrchr_sse(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strrchr_sse( str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret2, ret2);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = strrchr(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strrchr(     str[%d], \'%c\'): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret3, ret3);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strstrテスト
	auto test_strstr = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strstr_fast ]\n");
		}
		static const char* pattern_types[] = { "1", "12", "123456789012345", "1234567890123456", "12345678901234567", "8901234567890123", "89012345678901234" };
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char* pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strstr_fast(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr_fast(str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret1, ret1);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strstr_sse(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr_sse( str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret2, ret2);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = GASHA_ strstr(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr(     str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret3, ret3);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strstrbmテスト
	auto test_strstrbm = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strstrbm_fast ]\n");
			std::printf("*callback-funcion \'found_it\' will be return \'true\' when found it 2 times.\n");
		}
		static const char* pattern_types[] = { "1", "12", "123456789012345", "1234567890123456", "12345678901234567", "8901234567890123", "89012345678901234" };
		int found_count = 0;
		auto found_it = [&found_count](const char* found, const char* str) ->bool//コールバック関数
		{
			++found_count;
			if (found_count == 2)//二つ見つかったらOK
				return true;
			return false;
		};
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char* pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				int found_count1 = 0;
				int found_count2 = 0;
				int found_count3 = 0;
				if (test_mode == 0 || test_mode == 1)
				{
					found_count = 0;
					ret1 = strstrbm_fast(getStr(src_buff, i, loop), pattern, found_it);
					found_count1 = found_count;
					if (test_mode == 0)
						std::printf("strstrbm_fast(str[%d], \"%s\", found_it): ret=%p(\"%s\") found_count=%d\n", static_cast<int>(i), pattern, ret1, ret1, found_count);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					found_count = 0;
					ret2 = strstrbm_sse(getStr(src_buff, i, loop), pattern, found_it);
					found_count2 = found_count;
					if (test_mode == 0)
						std::printf("strstrbm_sse( str[%d], \"%s\", found_it): ret=%p(\"%s\") found_count=%d\n", static_cast<int>(i), pattern, ret2, ret2, found_count);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					found_count = 0;
					ret3 = strstrbm(getStr(src_buff, i, loop), pattern, found_it);
					found_count3 = found_count;
					if (test_mode == 0)
						std::printf("strstrbm(     str[%d], \"%s\", found_it): ret=%p(\"%s\") found_count=%d\n", static_cast<int>(i), pattern, ret3, ret3, found_count);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3 || found_count1 != found_count2 || found_count1 != found_count3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strstr0テスト
	auto test_strstr0 = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strstr0_fast ]\n");
		}
		static const char* pattern_types[] = { "1", "12", "123456789012345", "1234567890123456", "12345678901234567", "8901234567890123", "89012345678901234" };
		int num = 0;
		for (std::size_t type = 0; type < extentof(pattern_types); ++type)
		{
			const char* pattern = pattern_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				if (test_mode == 0 || test_mode == 1)
				{
					ret1 = strstr0_fast(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr0_fast(str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret1, ret1);
				}
				if (test_mode == 0 || test_mode == 2)
				{
					ret2 = strstr0_sse(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr0_sse( str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret2, ret2);
				}
				if (test_mode == 0 || test_mode == 3)
				{
					ret3 = strstr0(getStr(src_buff, i, loop), pattern);
					if (test_mode == 0)
						std::printf("strstr0(     str[%d], \"%s\"): ret=%p(\"%s\")\n", static_cast<int>(i), pattern, ret3, ret3);
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};

	//strcpyテスト
	auto test_strcpy = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strcpy_fast ]\n");
		}
		int num = 0;
		for (std::size_t i = 0; i < getStrSetNum(); ++i)
		{
			++num;
			const char* ret1 = nullptr;
			const char* ret2 = nullptr;
			const char* ret3 = nullptr;
			crc32_t crc1 = 0;
			crc32_t crc2 = 0;
			crc32_t crc3 = 0;
			if (test_mode == 0 || test_mode == 1)
			{
				reset_buff();
				ret1 = strcpy_fast(buff, getStr(src_buff, i, loop));
				if (test_mode == 0)
				{
					crc1 = calcCRC32(ret1);
					std::printf("strcpy_fast(buff, str[%d]): ret=%p, buff=\"%s\"\n", static_cast<int>(i), ret1, buff);
				}
			}
			if (test_mode == 0 || test_mode == 2)
			{
				reset_buff();
				ret2 = strcpy_sse(buff, getStr(src_buff, i, loop));
				if (test_mode == 0)
				{
					crc2 = calcCRC32(ret2);
					std::printf("strcpy_sse( buff, str[%d]): ret=%p, buff=\"%s\"\n", static_cast<int>(i), ret2, buff);
				}
			}
			if (test_mode == 0 || test_mode == 3)
			{
				reset_buff();
				ret3 = GASHA_ strcpy(buff, getStr(src_buff, i, loop));
				if (test_mode == 0)
				{
					crc3 = calcCRC32(ret3);
					std::printf("strcpy(     buff, str[%d]): ret=%p, buff=\"%s\"\n", static_cast<int>(i), ret3, buff);
				}
			}
			if (test_mode == 0)
			{
				if (ret1 != ret2 || ret1 != ret3 || crc1 != crc2 || crc1 != crc3)
					std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			}
		}
		return num;
	};

	//strncpyテスト
	auto test_strncpy = [&src_buff, &buff, &reset_buff](const int test_mode, const int loop) -> int
	{
		if (test_mode == 0)
		{
			std::printf("--------------------------------------------------------------------------------\n");
			std::printf("[ Test for strncpy_fast ]\n");
		}
		static const std::size_t max_len_types[] = { 0, 1, 15, 16, 17, 32, 33, sizeof(buff) };
		int num = 0;
		for (std::size_t type = 0; type < extentof(max_len_types); ++type)
		{
			const std::size_t max_len = max_len_types[type];
			for (std::size_t i = 0; i < getStrSetNum(); ++i)
			{
				++num;
				const char* ret1 = nullptr;
				const char* ret2 = nullptr;
				const char* ret3 = nullptr;
				crc32_t crc1 = 0;
				crc32_t crc2 = 0;
				crc32_t crc3 = 0;
				if (test_mode == 0 || test_mode == 1)
				{
					reset_buff();
					ret1 = strncpy_fast(buff, getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
					{
						crc1 = calcCRC32(ret1);
						std::printf("strncpy_fast(buff, str[%d], %d): ret=%p, buff=\"%s\"\n", static_cast<int>(i), static_cast<int>(max_len), ret1, buff);
					}
				}
				if (test_mode == 0 || test_mode == 2)
				{
					reset_buff();
					ret2 = strncpy_sse(buff, getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
					{
						crc2 = calcCRC32(ret2);
						std::printf("strncpy_sse( buff, str[%d], %d): ret=%p, buff=\"%s\"\n", static_cast<int>(i), static_cast<int>(max_len), ret2, buff);
					}
				}
				if (test_mode == 0 || test_mode == 3)
				{
					reset_buff();
					ret3 = GASHA_ strncpy(buff, getStr(src_buff, i, loop), max_len);
					if (test_mode == 0)
					{
						crc3 = calcCRC32(ret3);
						std::printf("strncpy(     buff, str[%d], %d): ret=%p, buff=\"%s\"\n", static_cast<int>(i), static_cast<int>(max_len), ret3, buff);
					}
				}
				if (test_mode == 0)
				{
					if (ret1 != ret2 || ret1 != ret3 || crc1 != crc2 || crc1 != crc3)
						std::printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NG! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}
			}
		}
		return num;
	};
	
	//パフォーマンステスト
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("[ Test for performance ]\n");
	std::printf("\n");
	auto type_name = [](const int type)
	{
		return
			type == 1 ? "_fast" :
			type == 2 ? "_sse " :
			            "     ";
	};
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strlen%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRLEN_REPEAT_NUM; repeat++)
			num += test_strlen(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strnlen%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRNLEN_REPEAT_NUM; repeat++)
			num += test_strnlen(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strcmp%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRCMP_REPEAT_NUM; repeat++)
			num += test_strcmp(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strncmp%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRNCMP_REPEAT_NUM; repeat++)
			num += test_strncmp(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strchr%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRCHR_REPEAT_NUM; repeat++)
			num += test_strchr(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strrchr%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRRCHR_REPEAT_NUM; repeat++)
			num += test_strrchr(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strstr%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRSTR_REPEAT_NUM; repeat++)
			num += test_strstr(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strstrbm%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRSTRBM_REPEAT_NUM; repeat++)
			num += test_strstrbm(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strstr0%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRSTR0_REPEAT_NUM; repeat++)
			num += test_strstr0(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strcpy%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRCPY_REPEAT_NUM; repeat++)
			num += test_strcpy(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}
	for (int type = 1; type <= 3; ++type)
	{
		std::printf("strncpy%s", type_name(type));
		const auto begin = nowTime();
		int num = 0;
		for (int repeat = 0; repeat < TEST_STRNCPY_REPEAT_NUM; repeat++)
			num += test_strncpy(type, repeat);
		const double elapsed_time = calcElapsedTime(begin);
		std::printf(" (*%d times repeat) ... %.9lf sec\n", num, elapsed_time);
	}

	//ロジックテスト
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("[ Test for logic ]\n");
	std::printf("\n");
	for (std::size_t i = 0; i < getStrSetNum(); ++i)
		std::printf("str[%d]=\"%s\"\n", static_cast<int>(i), getStr(src_buff, i, 0));
	std::printf("\n");
	test_strlen(0, 0);
	test_strnlen(0, 0);
	test_strcmp(0, 0);
	test_strncmp(0, 0);
	test_strchr(0, 0);
	test_strrchr(0, 0);
	test_strstr(0, 0);
	test_strstrbm(0, 0);
	test_strstr0(0, 0);
	test_strcpy(0, 0);
	test_strncpy(0, 0);

	//spprintfテスト
	//※高速文字列処理ではないが、いっしょにテスト
	std::printf("\n");
	std::printf("================================================================================\n");
	std::printf("[ Test for spptinf ]\n");
	{
		char buff[32];
		memset(buff, 0xcd, sizeof(buff));
		std::size_t pos = 0;
		int ret;
	#define EXPR(expr) expr; std::printf("%s buff=\"%s\", pos=%d, ret=%d\n", #expr, buff, static_cast<int>(pos), ret);

		std::printf("char buff[10];\n");
		EXPR(ret = spprintf(buff, 10, pos, "1"););
		EXPR(ret = spprintf(buff, 10, pos, "%d", 23););
		EXPR(ret = spprintf(buff, 10, pos, "%d%d%d", 4, 5, 6););
		EXPR(ret = spprintf(buff, 10, pos, "%d", 7890););
		EXPR(ret = spprintf(buff, 10, pos, "ABC"););
		
		EXPR(pos = 0;);
		EXPR(ret = spprintf(buff, pos, "1"););
		EXPR(ret = spprintf(buff, pos, "%d", 23););
		EXPR(ret = spprintf(buff, pos, "%d%d%d", 4, 5, 6););
		EXPR(ret = spprintf(buff, pos, "%d", 7890););
		EXPR(ret = spprintf(buff, pos, "ABC"););
		#undef EXPR
	}

	std::printf("\n");
	std::printf("- end -\n");
}

// End of file
