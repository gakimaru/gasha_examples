﻿//--------------------------------------------------------------------------------
// main.cpp
// デバッグメッセージテスト - メイン
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_message.h"//デバッグメッセージテスト

#include <cstdio>
#include <cstdlib>

#include <cstring>

#pragma warning(disable:4996)

int main(const int argc, const char* argv[])
{
	//デバッグメッセージテスト
	example_debug_message();
	
	return EXIT_SUCCESS;
}

// End of file
