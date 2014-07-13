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
#include <gasha/console_win.h>//Windowsコマンドプロンプト
#include <gasha/console_vs.h>//Windowsコマンドプロンプト
#include <gasha/console_tty.h>//Windowsコマンドプロンプト

#include <gasha/log_level.h>//ログレベル

#include <gasha/iterator.h>//イテレータ操作

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//レベルコンテナ列挙
void printLevelAll()
{
	auto con = logLevel::container();
	forEach(con,
	//reverseForEach(con,
		[](logLevel& obj)
		{
			//const logLevel& obj = ite;//イテレータを変換（イテレータのままでもアロー演算子で直接値操作可能）
			IConsole* console = obj.console();
			if (console)
			{
				console->changeColor(obj.color());
				char message[1024];
				console->printf(message, "logLevel=%d, name=\"%s\", outputLevel=%d, forLog=%d, forNotice=%d, forMask=%d", obj.value(), obj.name(), obj.outputLevel(), obj.forLog(), obj.forNotice(), obj.forMask());
				console->outputCr();
				auto* prev = obj.prev();
				auto* next = obj.next();
				if (prev)
				{
					console->changeColor(prev->color());
					console->printf(message, "             prev=%s(%d)", prev->name(), prev->value());
					console->outputCr();
				}
				if (next)
				{
					console->changeColor(next->color());
					console->printf(message, "             next=%s(%d)", next->name(), next->value());
					console->outputCr();
				}
			}
		}
	);
}

//----------------------------------------
//デバッグメッセージテスト
void example_debug_message()
{
	consoleColor color(consoleColor::iYELLOW, consoleColor::RED, consoleColor::UNDERLINE);
	//consoleTTY con_impl(stdout);
	consoleWin con_impl(stdout);
	//consoleVS con_impl;
	IConsole* con = &con_impl;
	con->changeColor(color);
	con->output("message");
	con->resetColor();
	con->output("\n");

	printLevelAll();
}

// End of file
