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
#include <gasha/win_console.h>//Windowsコマンドプロンプト
#include <gasha/vs_console.h>//Windowsコマンドプロンプト
#include <gasha/tty_console.h>//Windowsコマンドプロンプト

#include <gasha/log_level.h>//ログレベル

#include <gasha/iterator.h>//イテレータ操作
#include <gasha/type_traits.h>//型特性ユーティリティ：toStr()

#ifdef GASHA_IS_GCC
//※別端末へのログ出力テスト用
//#include <sys/types.h>//open()
//#include <sys/stat.h>//open()
#include <fcntl.h>//open(), O_WRONLY, O_NDELAY, O_NOCTTY
#include <unistd.h>//close()
#include <cstdio>//fdopen()
#endif//GASHA_IS_GCC

GASHA_USING_NAMESPACE;//ネームスペース使用

#ifdef GASHA_HAS_DEBUG_LOG//デバッグログ無効時はまるごと無効化

//----------------------------------------
//レベル列挙
void printLevelAll()
{
	forEach(logLevelContainer(),//正順表示
	//reverseForEach(logLevelContainer(),//逆順表示
		[](logLevel& obj)
		{
			IConsole* print_console = obj.console();
			if (!print_console)
				print_console = &stdConsole::instance();
			char message[1024];
			char str_color[64];
			print_console->changeColor(obj.color());
			if(obj.isSpecial())
				print_console->printf(message, "(SP-%2d)\"%s\"", obj.value(), obj.name());
			else
				print_console->printf(message, "(%2d:%2d)\"%s\"", obj.value(), obj.outputLevel(), obj.name());
			print_console->outputCr();
			auto* console = obj.console();
			auto& color = obj.color();
			auto* console_for_notice = obj.consoleForNotice();
			auto& color_for_notice = obj.colorForNotice();
			auto prev = obj.prev();
			auto next = obj.next();
			if (console)
			{
				print_console->changeColor(obj.color());
				print_console->printf(message, "\tconsole: \"%s\"(%s)", console->name(), color.toStr(str_color));
				print_console->outputCr();
			}
			if (console_for_notice)
			{
				print_console->changeColor(obj.colorForNotice());
				print_console->printf(message, "\tnotice: \"%s\"(%s)", console_for_notice->name(), color_for_notice.toStr(str_color));
				print_console->outputCr();
			}
			if (prev)
			{
				print_console->changeColor(prev.color());
				print_console->printf(message, "\tprev: (%2d:%2d)\"%s\"", prev.value(), prev.outputLevel(), prev.name());
				print_console->outputCr();
			}
			if (next)
			{
				print_console->changeColor(next.color());
				print_console->printf(message, "\tnext: (%2d:%2d)\"%s", next.value(), next.outputLevel(), next.name());
				print_console->outputCr();
			}
		}
	);
}

#endif//GASHA_HAS_DEBUG_LOG//デバッグログ無効時はまるごと無効化


//----------------------------------------
//デバッグメッセージテスト
void example_debug_message()
{
#ifdef GASHA_HAS_DEBUG_LOG//デバッグログ無効時はまるごと無効化

	//コンソールカラー
	consoleColor color(consoleColor::iYELLOW, consoleColor::RED, consoleColor::UNDERLINE);

	//標準コンソールに文字列出力テスト
	//※標準コンソールは対象プラットフォームに応じて適切なコンソールが選択される
	{
		stdConsole console;
		console.changeColor(color);
		console.output("message for Standard");
		console.outputCr();
	}

	//TTY端末に文字列出力テスト
	//※エスケープシーケンスで着色
	//※Windowsの場合はコマンドプロンプトに出力
	{
		ttyConsole console(stdout);
		console.changeColor(color);
		console.output("message for TTY (stdout)");
		console.outputCr();
	}

	//TTY端末の標準エラー出力
	{
		ttyConsole console(stderr);
		console.changeColor(color);
		console.output("message for TTY (stderr)");
		console.outputCr();
	}

	//Windowsコマンドプロンプトに文字列出力テスト
	//※Windowsのコンソールテキスト属性で着色
	//※Windows以外の場合は標準出力に出力
	{
		winConsole console(stdout);
		console.changeColor(color);
		console.output("message for Win (stdout)");
		console.outputCr();
	}
	
	//Windowsコマンドプロンプトの標準エラー出力
	{
		winConsole console(stderr);
		console.changeColor(color);
		console.output("message for Win (stderr)");
		console.outputCr();
	}

	//Visual Studio 出力ウインドウに文字列出力テスト
	//※着色不可
	//※Windows以外／開発ツール無効時の場合は標準出力に出力
	{
		vsConsole console;
		console.changeColor(color);
		console.output("message for VS");
		console.outputCr();
	}

	//明示的なログレベルコンテナの初期化
	//※GASHA_SECURE_CONTAINER_INITIALIZE を設定していない場合、最初のログレベル使用前に、
	//　一度だけ（ローカル変数でいいので）コンテナのインスタンスを作り、初期化を済ませなければならない
	//※GASHA_SECURE_CONTAINER_INITIALIZE を設定している場合は、初回のコンテナアクセス時に自動的に初期化する。
	//※いずれにしても、CallOnceにより、スレッドセーフな初期化が保証される。
	{
		//logLevelContainer con;//ログレベルコンテナを生成（初回に限り、ログレベルをプールする静的データの初期化が行われる）
	}

	//ログレベルのコンソールを変更
	{
		static winConsole console(stdout, "win-console *CHANGED*");
		logLevel level(asNormal);
		level.console() = &console;
		//level->consoleForNotice() = &console;
	}

	//全ログレベルのコンソールをまとめて変更
	{
		static vsConsole console("vs-console *CHANGED*");
		//logLevel::setAllConsole(&console);
		logLevelContainer con;
		con.setAllConsoleForNotice(&console);
	}

	//全ログレベルの列挙
	printLevelAll();

	//【Unix用】別の端末にログを出力
#ifdef GASHA_IS_GCC
	{
		//端末をオープン
		//※dev/pty3/はCygwinの端末
		int fd = open("/dev/pty3", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		FILE* fp = fdopen(fd, "w");
		printf("fd=%d, fp=%p\n", fd, fp);
		static ttyConsole console(fp, "/dev/pty3");
		
		//ログレベルにコンソールを割り当て
		logLevelContainer con;
		con.setAllConsoleForNotice(&console);
		con.setAllConsoleForNotice(&console);

		//出力テスト
		printLevelAll();

		//標準コンソールに戻す
		con.setAllConsoleForNotice(&stdConsole::instance());
		con.setAllConsoleForNotice(&stdConsoleForNotice::instance());

		//端末をクローズ
		fclose(fp);
		close(fd);
	}
#endif//GASHA_IS_GCC

#endif//GASHA_HAS_DEBUG_LOG//デバッグログ無効時はまるごと無効化
}

// End of file
