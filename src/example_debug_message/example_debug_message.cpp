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
#include <gasha/tty_console.h>//コンソール：TTY端末
#include <gasha/win_console.h>//コンソール：Windowsコマンドプロンプト
#include <gasha/vs_console.h>//コンソール：Visual Studio出力ウインドウ

#include <gasha/log_purpose.h>//ログ用途
#include <gasha/log_level.h>//ログレベル
#include <gasha/log_category.h>//ログカテゴリ
#include <gasha/log_mask.h>//ログマスク
#include <gasha/log_work_buff.h>//ログワークバッファ
#include <gasha/log_attr.h>//ログ属性
#include <gasha/log_print_info.h>//ログ出力情報
#include <gasha/log_queue.h>//ログキュー
#include <gasha/std_log_print.h>//標準ログ出力
#include <gasha/log_queue_monitor.h>//ログキューモニター

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
//ログレベル列挙
void printAllLogLevel()
{
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Enumeration of all log-levels ]\n");
	printf("\n");

	forEach(logLevelContainer(),//正順表示
	//reverseForEach(logLevelContainer(),//逆順表示
		[](logLevel& obj)
		{
			IConsole* print_console = obj.console(ofLog);
			IConsole* notice_console = obj.console(ofNotice);
			if (!print_console)
				print_console = &stdOutConsole::instance();
			if (!notice_console)
				notice_console = &stdOutConsole::instance();
			char message[1024];
			char str_color[64];
			print_console->changeColor(obj.color(ofLog));
			if(obj.isSpecial())
				print_console->printf(message, "(SP-%2d)\"%s\"", obj.value(), obj.name());
			else
				print_console->printf(message, "(%2d:%2d)\"%s\"", obj.value(), obj.outputLevel(), obj.name());
			print_console->outputCr();
			auto* console = obj.console(ofLog);
			auto& color = obj.color(ofLog);
			auto* console_of_notice = obj.console(ofNotice);
			auto& color_of_notice = obj.color(ofNotice);
			auto prev = obj.prev();
			auto next = obj.next();
			if (console)
			{
				print_console->changeColor(obj.color(ofLog));
				print_console->printf(message, "\tconsole: \"%s\"(%s)", console->name(), color.toStr(str_color));
				print_console->outputCr();
			}
			if (console_of_notice)
			{
				notice_console->changeColor(obj.color(ofNotice));
				notice_console->printf(message, "\tnotice: \"%s\"(%s)", console_of_notice->name(), color_of_notice.toStr(str_color));
				notice_console->outputCr();
			}
			if (prev)
			{
				print_console->changeColor(prev.color(ofLog));
				print_console->printf(message, "\tprev: (%2d:%2d)\"%s\"", prev.value(), prev.outputLevel(), prev.name());
				print_console->outputCr();
			}
			if (next)
			{
				print_console->changeColor(next.color(ofLog));
				print_console->printf(message, "\tnext: (%2d:%2d)\"%s", next.value(), next.outputLevel(), next.name());
				print_console->outputCr();
			}
		}
	);
}

//----------------------------------------
//ログカテゴリ列挙
void printAllLogCategory()
{
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Enumeration of all log-categoris ]\n");
	printf("\n");

	forEach(logCategoryContainer(),//正順表示
	//reverseForEach(logCategoryContainer(),//逆順表示
		[](logCategory& obj)
		{
			IConsole* print_console = obj.console(ofLog);
			if (!print_console)
				print_console = &stdOutConsole::instance();
			char message[1024];
			if (obj.isSpecial())
				print_console->printf(message, "(SP-%2d)\"%s\"", obj.value(), obj.name());
			else
				print_console->printf(message, "(%2d)\"%s\"", obj.value(), obj.name());
			print_console->outputCr();
			auto* console = obj.console(ofLog);
			auto* console_of_notice = obj.console(ofNotice);
			if (console)
			{
				print_console->printf(message, "\tconsole: \"%s\"", console->name());
				print_console->outputCr();
			}
			if (console_of_notice)
			{
				print_console->printf(message, "\tnotice: \"%s\"", console_of_notice->name());
				print_console->outputCr();
			}
		}
	);
}

//----------------------------------------
//ログマスク列挙
void printAllLogMask(const logLevel::level_type level)
{
	logLevel level_obj(level);
	
	printf("\n");
	printf("--------------------------------------------------------------------------------\n");
	printf("[ Enumeration of all log-level-masks : Test for output message %s ]\n", level_obj.name());
	printf("\n");

	forEach(logMask(),//正順表示
		[&level_obj](logMask::iterator& mask_obj)
	//reverseForEach(logMask(),//逆順表示
	//	[&level_obj](logMask::reverse_iterator& mask_obj)
	{
		const logCategory& category = mask_obj.category();
		logLevel level_mask(mask_obj.level(ofLog));
		logLevel level_mask_of_notice(mask_obj.level(ofNotice));
		IConsole* print_console = level_mask.console(ofLog);
		if (!print_console)
			print_console = &stdOutConsole::instance();
		char message[1024];
		if (category.isSpecial())
			print_console->printf(message, "(SP-%2d)\"%s\"", category.value(), category.name());
		else
			print_console->printf(message, "(%2d)\"%s\"", category.value(), category.name());
		print_console->outputCr();
		IConsole* console = mask_obj.console(ofLog, level_obj);
		const consoleColor* color = mask_obj.color(ofLog, level_obj);
		IConsole* console_of_notice = mask_obj.console(ofNotice, level_obj);
		const consoleColor* color_of_notice = mask_obj.color(ofNotice, level_obj);

		print_console->changeColor(level_mask.color(ofLog));
		print_console->printf(message, "\tmask(of log): (%2d)\"%s\"", level_mask.value(), level_mask.name());
		print_console->outputCr();
		if (console)
		{
			print_console->changeColor(level_mask.color(ofLog));
			print_console->printf(message, "\t\tOK ... ");
			console->changeColor(*color);
			console->printf(message, "log-message: (%2d)\"%s\", \"%s\"", level_obj.value(), level_obj.name(), console->name());
			console->outputCr();
			if (*print_console != *console)
				print_console->outputCr();
		}
		else
		{
			print_console->changeColor(level_mask.color(ofLog));
			print_console->printf(message, "\t\tMASKED! ... ");
			print_console->changeColor(level_obj.color(ofLog));
			print_console->printf(message, "notice-message: (%2d)\"%s\"", level_obj.value(), level_obj.name());
			print_console->outputCr();
		}
		
		print_console->changeColor(level_mask_of_notice.color(ofNotice));
		print_console->printf(message, "\tmask(of notice): (%2d)\"%s\"", level_mask_of_notice.value(), level_mask_of_notice.name());
		print_console->outputCr();
		if (console_of_notice)
		{
			print_console->changeColor(level_mask_of_notice.color(ofNotice));
			print_console->printf(message, "\t\tOK ... ");
			console_of_notice->changeColor(*color_of_notice);
			console_of_notice->printf(message, "notice-message: (%2d)\"%s\", \"%s\"", level_obj.value(), level_obj.name(), console_of_notice->name());
			console_of_notice->outputCr();
			if (*print_console != *console_of_notice)
				print_console->outputCr();
		}
		else
		{
			print_console->changeColor(level_mask_of_notice.color(ofNotice));
			print_console->printf(message, "\t\tMASKED! ... ");
			print_console->changeColor(level_obj.color(ofNotice));
			print_console->printf(message, "notice-message: (%2d)\"%s\"", level_obj.value(), level_obj.name());
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
		stdOutConsole console;
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

	//明示的なログレベル／カテゴリコンテナの初期化
	//※GASHA_LOG_LEVEL(CATEGORY)_CONTAINER_SECURE_INITIALIZE 設定時には不要
	{
	#ifndef GASHA_LOG_LEVEL_CONTAINER_SECURE_INITIALIZE
		logLevelContainer level_con(logLevelContainer::explicitInitialize);//ログレベルの明示的な初期化
	#endif//GASHA_LOG_LEVEL_CONTAINER_SECURE_INITIALIZE
	#ifndef GASHA_LOG_CATEGORY_CONTAINER_SECURE_INITIALIZE
		logCategoryContainer cate_con(logCategoryContainer::explicitInitialize);//ログカテゴリの明示的な初期化
	#endif//GASHA_LOG_CATEGORY_CONTAINER_SECURE_INITIALIZE
	#ifndef GASHA_LOG_MASK_SECURE_INITIALIZE
		logMask mask(logMask::explicitInitialize);//ログマスクの明示的な初期化
	#endif//GASHA_LOG_MASK_SECURE_INITIALIZE
	#ifndef GASHA_LOG_WORK_BUFF_SECURE_INITIALIZE
		logWorkBuff log_buff(logWorkBuff::explicitInitialize);//ログワークバッファの明示的な初期化
	#endif//GASHA_LOG_WORK_BUFF_SECURE_INITIALIZE
	#ifndef GASHA_LOG_QUEUE_SECURE_INITIALIZE
		logQueue log_queue(logQueue::explicitInitialize);//ログキューの明示的な初期化
	#endif//GASHA_LOG_QUEUE_SECURE_INITIALIZE
	#ifndef GASHA_LOG_QUEUE_MONITOR_SECURE_INITIALIZE
		logQueueMonitor log_queue_monitor(logQueueMonitor::explicitInitialize);//ログキューモニターの明示的な初期化
	#endif//GASHA_LOG_QUEUE_MONITOR_SECURE_INITIALIZE
	}

	//ログレベルのコンソールを変更
	{
		static winConsole console(stdout, "win-console *CHANGED*");
		logLevel level(asNormal);
		level.console(ofLog) = &console;
		//level->console(ofNotice) = &console;
	}

	//全ログレベルのコンソールをまとめて変更
	//※replaceEachConsole()メソッドは、nullptr が設定されているコンソールは変更しない
	{
		static vsConsole console("vs-console *CHANGED*");
		//logLevel::replaceEachConsole(ofLog, &console);
		logLevelContainer con;
		con.replaceEachConsole(ofNotice, &console);
	}

	//全ログレベルの列挙
	printAllLogLevel();

	//独自ログカテゴリの追加
	enum exCategory : logCategory::category_type
	{
		forMiniGame = MAKE_LOG_CATEGORY_VALUE(10),//ミニゲーム
		forTARO = MAKE_LOG_CATEGORY_VALUE(20),//太郎用（可発者個人用）
		forJIRO = MAKE_LOG_CATEGORY_VALUE(21),//次郎用（可発者個人用）
	};
	regLogCategory<forMiniGame>()("forMiniGame");
	static vsConsole vs_console;
	IConsole* console[] = { &vs_console, nullptr };
	regLogCategory<forTARO>()("forTARO", console);//開発者用のログはVSの出力ウインドウに
	regLogCategory<forJIRO>()("forJIRO", console);//開発者用のログはVSの出力ウインドウに

	//全カテゴリレベルの列挙
	printAllLogCategory();

	//ログマスクを変更
	{
		logMask mask;
		{
			auto ite = mask.find(forTARO);//太郎用ログマスク（イテレータ）を取得
			ite.changeLevel(ofLog, asDetail);//ログマスクを詳細メッセージレベルに下げる
			ite.changeLevel(ofNotice, asNormal);//画面通知マスクを通常メッセージレベルに下げる
		}
		{
			mask.changeLevel(ofLog, asSilent, forJIRO);//次郎用ログマスクを静寂レベルに上げる
			mask.changeLevel(ofNotice, asSilentAbsolutely, forJIRO);//次郎用画面通知マスクを絶対静寂レベルに上げる
		}
	}

	//ログマスク列挙
	printAllLogMask(asDetail);//詳細レベルの表示チェック
	printAllLogMask(asNormal);//通常レベルの表示チェック
	printAllLogMask(asCritical);//重大レベルの表示チェック
	printAllLogMask(asAbsolute);//絶対レベルの表示チェック

	//【Unix用】別の端末にログを出力
#ifdef GASHA_IS_GCC
	{
		//端末をオープン
		//※dev/pty*/はCygwinの端末
		int fd1 = open("/dev/pty1", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		int fd2 = open("/dev/pty2", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		int fd3 = open("/dev/pty3", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		FILE* fp1 = stdout;
		FILE* fp2 = stderr;
		FILE* fp3 = stderr;
		const char* consle_name_fp1 = "stdout(/dev/pty1 open failed)";
		const char* consle_name_fp2 = "stderr(/dev/pty2 open failed)";
		const char* consle_name_fp3 = "stderr(/dev/pty3 open failed)";
		if(fd1 >= 0){ fp1 = fdopen(fd1, "w"); if(fp1) consle_name_fp1 = "/dev/pty1"; else fp1 = stdout; }
		if(fd2 >= 0){ fp2 = fdopen(fd2, "w"); if(fp2) consle_name_fp2 = "/dev/pty2"; else fp2 = stderr; }
		if(fd3 >= 0){ fp3 = fdopen(fd3, "w"); if(fp3) consle_name_fp3 = "/dev/pty3"; else fp3 = stderr; }
		ttyConsole new_stdout_console(fp1, consle_name_fp1);
		ttyConsole new_stderr_console(fp2, consle_name_fp2);
		ttyConsole new_notice_console(fp3, consle_name_fp3);

		//ログレベルにコンソールを割り当て
		logLevelContainer con;
		con.replaceEachConsole(ofLog, &stdOutConsole::instance(), &new_stdout_console);
		con.replaceEachConsole(ofLog, &stdErrConsole::instance(), &new_stderr_console);
		con.replaceEachConsole(ofNotice, &new_notice_console);

		//出力テスト
		printAllLogLevel();

		//標準コンソールに戻す
		con.replaceEachConsole(ofLog, &new_stdout_console, &stdOutConsole::instance());
		con.replaceEachConsole(ofLog, &new_stderr_console, &stdErrConsole::instance());
		con.replaceEachConsole(ofNotice, &stdConsoleOfNotice::instance());

		//端末をクローズ
		if(fp1) fclose(fp1);
		if(fp2) fclose(fp2);
		if(fp3) fclose(fp3);
		if (fd1 >= 0) close(fd1);
		if (fd1 >= 1) close(fd2);
		if (fd1 >= 2) close(fd3);
	}
#endif//GASHA_IS_GCC

	{
		//ログキューモニタースレッド実行
		auto monitor_thread_func = []()
		{
			logQueueMonitor mon;
			mon.monitor(stdLogPrint());
		};
		std::thread monitor_thread(monitor_thread_func);

		//メッセージのキューイング
		for (int mode = 0; mode < 4; ++mode)
		{
			if (mode == 1)
			{
				//ログキュー中断
				logQueue queue;
				queue.abort();
			}
			else if (mode == 2)
			{
				//ログワークバッファ中断
				logWorkBuff work_buff;
				work_buff.abort();
			}
			else if (mode == 3)
			{
				//ログキューとログワークバッファを再開
				logWorkBuff work_buff(logWorkBuff::explicitInitialize);
				logQueue queue(logQueue::explicitInitialize);
				logQueueMonitor mon;
				mon.reset();//ログキューモニターリセット
			}

			logQueue queue;
			logQueue::id_type reserved_id = queue.reserve();
			for (int i = 1; i < 1000000000; i *= 3)
			{
				//エンキュー
				{
					logWorkBuff work_buff;
					char* message = work_buff.alloc();
					if (message)
					{
						std::size_t size = 0;
						work_buff.spprintf(message, size, "message:%d", i);
						logMask mask;
						logPrintInfo print_info;
						logLevel::level_type level = asNormal;
						logCategory::category_type category = forTARO;
						print_info.m_id = 0;//IDは自動発番
						print_info.m_message = message;
						print_info.m_messageSize = static_cast<logPrintInfo::message_size_type>(size + 1);
						print_info.m_level = level;
						print_info.m_category = category;
						print_info.m_attr = noLogAttr;
						for (logPurpose::purpose_type purpose = 0; purpose < logPurpose::NUM; ++purpose)
						{
							print_info.m_consoles[purpose] = mask.console(purpose, level, category);
							print_info.m_colors[purpose] = mask.color(purpose, level, category);
						}
						//stdLogPrint()(print_info);//キューイングせずに直接表示する場合
						const bool result = queue.enqueue(print_info);//キューイング
						if (result)
						{
							//モニターに通知
							logQueueMonitor mon;
							mon.notify();
						}
						else
							printf("enqueu failed.\n");
						work_buff.free(message);
					}
					else
						printf("message making failed.\n");
				}
			#if 0
				//デキュー
				{
					logQueue::node_type node;
					logQueue queue;
					const bool result = queue.dequeue(node);
					if (result)
					{
						printf("id=%lld, category=%d, level=%d, message=\"%s\"\n", node.m_id, node.m_category, node.m_level, node.m_message);
						queue.release(node);
					}
				}
			#endif
			}
			//予約したメッセージを出力
			{
				logWorkBuff work_buff;
				char* message = work_buff.alloc();
				if (message)
				{
					std::size_t size = 0;
					work_buff.spprintf(message, size, "reserved message");
					logMask mask;
					logPrintInfo print_info;
					logLevel::level_type level = asNormal;
					logCategory::category_type category = forTARO;
					print_info.m_id = reserved_id;//予約したID
					print_info.m_message = message;
					print_info.m_messageSize = static_cast<logPrintInfo::message_size_type>(size + 1);
					print_info.m_level = level;
					print_info.m_category = category;
					print_info.m_attr = noLogAttr;
					for (logPurpose::purpose_type purpose = 0; purpose < logPurpose::NUM; ++purpose)
					{
						print_info.m_consoles[purpose] = mask.console(purpose, level, category);
						print_info.m_colors[purpose] = mask.color(purpose, level, category);
					}
					//stdLogPrint()(print_info);//キューイングせずに直接表示する場合
					const bool result = queue.enqueue(print_info);//キューイング
					if (result)
					{
						//モニターに通知
						logQueueMonitor mon;
						mon.notify();
					}
					else
						printf("enqueu of reservation failed.\n");
					work_buff.free(message);
				}
				else
					printf("message making of reservation failed.\n");
			}

			//モニター終了待ち
			{
				logQueueMonitor mon;
				mon.flush();
			}
		}

		//モニター終了
		{
			logQueueMonitor mon;
			mon.flush();
			mon.abort();
		}
		monitor_thread.join();
	}

#endif//GASHA_HAS_DEBUG_LOG//デバッグログ無効時はまるごと無効化
}

// End of file
