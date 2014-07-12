//--------------------------------------------------------------------------------
// exmaple_debug_message.cpp
// デバッグメッセージテスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_message.h"//デバッグメッセージテスト

#include <gasha/console_color.h>//コンソールカラー

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//デバッグメッセージテスト
void example_debug_message()
{
	console con(console_WIN, stdout);
	consoleColor color(con, consoleColor::iRed, consoleColor::iYellow, true, true);
	color.changeColor();
	con.printf("message\n");
	color.resetColor();
}

// End of file
