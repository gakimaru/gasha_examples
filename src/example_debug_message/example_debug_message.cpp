﻿//--------------------------------------------------------------------------------
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
#include <gasha/win_console.h>//Windowsコマンドプロンプト
#include <gasha/vs_console.h>//Windowsコマンドプロンプト
#include <gasha/tty_console.h>//Windowsコマンドプロンプト

#include <gasha/log_level.h>//ログレベル

#include <gasha/iterator.h>//イテレータ操作
#include <gasha/type_traits.h>//型特性ユーティリティ：toStr()

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//レベル列挙
void printLevelAll()
{
	forEach(logLevel::container(),//正順表示
	//reverseForEach(logLevel::container(),//逆順表示
		[](logLevel& obj)
		{
			IConsole* console = obj.console();
			if (console)
			{
				char message[1024];
				char str_color[64];
				console->changeColor(obj.color());
				console->printf(message, "name=\"%s\", logLevel=%d, outputLevel=%d, forLog=%s, forNotice=%s, forMask=%s", obj.name(), obj.value(), obj.outputLevel(), toStr(obj.forLog()), toStr(obj.forNotice()), toStr(obj.forMask()));
				console->outputCr();
				console->changeColor(obj.color());
				console->printf(message, "\tconsole=\"%s\"(%s)", obj.console()->name(), obj.color().toStr(str_color));
				console->outputCr();
				console->changeColor(obj.colorForNotice());
				console->printf(message, "\tconsoleForNotice=\"%s\"(%s)", obj.consoleForNotice()->name(), obj.colorForNotice().toStr(str_color));
				console->outputCr();
				auto* prev = obj.prev();
				auto* next = obj.next();
				if (prev)
				{
					console->changeColor(prev->color());
					console->printf(message, "\tprev=%s(%d)", prev->name(), prev->value());
					console->outputCr();
				}
				if (next)
				{
					console->changeColor(next->color());
					console->printf(message, "\tnext=%s(%d)", next->name(), next->value());
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
	//コンソールカラー
	consoleColor color(consoleColor::iYELLOW, consoleColor::RED, consoleColor::UNDERLINE);

	//標準コンソールに文字列出力テスト
	//※標準コンソールは対象プラットフォームに応じて適切なコンソールが選択される
	{
		stdConsole con;
		con.changeColor(color);
		con.output("message for Standard");
		con.outputCr();
	}

	//TTY端末に文字列出力テスト
	//※エスケープシーケンスで着色
	//※Windowsの場合はコマンドプロンプトに出力
	{
		ttyConsole con(stdout);
		con.changeColor(color);
		con.output("message for TTY (stdout)");
		con.outputCr();
	}

	//TTY端末の標準エラー出力
	{
		ttyConsole con(stderr);
		con.changeColor(color);
		con.output("message for TTY (stderr)");
		con.outputCr();
	}

	//Windowsコマンドプロンプトに文字列出力テスト
	//※Windowsのコンソールテキスト属性で着色
	//※Windows以外の場合は標準出力に出力
	{
		winConsole con(stdout);
		con.changeColor(color);
		con.output("message for Win (stdout)");
		con.outputCr();
	}
	
	//Windowsコマンドプロンプトの標準エラー出力
	{
		winConsole con(stderr);
		con.changeColor(color);
		con.output("message for Win (stderr)");
		con.outputCr();
	}

	//Visual Studio 出力ウインドウに文字列出力テスト
	//※着色不可
	//※Windows以外／開発ツール無効時の場合は標準出力に出力
	{
		vsConsole con;
		con.changeColor(color);
		con.output("message for VS");
		con.outputCr();
	}

	//ログレベルのコンソールを変更
	{
		static winConsole con(stdout, "win-console*");
		logLevel* level = logLevel::at(asNormal);
		level->console() = &con;
		//level->consoleForNotice() = &con;
	}

	//全ログレベルのコンソールをまとめて変更
	{
		static vsConsole con("vs-console*");
		//logLevel::setAllConsole(&con);
		logLevel::setAllConsoleForNotice(&con);
	}
	
	//全ログレベルの列挙
	printLevelAll();
}

// End of file
