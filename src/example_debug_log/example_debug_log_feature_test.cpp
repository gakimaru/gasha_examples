//--------------------------------------------------------------------------------
// exmaple_debug_log_feqture_test.cpp
// デバッグログ個別機能テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_log.h"//デバッグログテスト

#include <gasha/console_color.h>//コンソールカラー
#include <gasha/tty_console.h>//コンソール：TTY端末
#include <gasha/win_console.h>//コンソール：Windowsコマンドプロンプト
#include <gasha/vs_console.h>//コンソール：Visual Studio出力ウインドウ
#include <gasha/mem_console.h>//コンソール：メモリコンソール
#include <gasha/dummy_console.h>//コンソール：ダミーコンソール
#include <gasha/std_console.h>//標準コンソール

#include <gasha/log_purpose.h>//ログ用途
#include <gasha/log_level.h>//ログレベル
#include <gasha/log_category.h>//ログカテゴリ
#include <gasha/log_mask.h>//ログreberu マスク
#include <gasha/log_work_buff.h>//ログワークバッファ
#include <gasha/log_attr.h>//ログ属性
#include <gasha/log_print_info.h>//ログ出力情報
#include <gasha/log_queue.h>//ログキュー
#include <gasha/std_log_print.h>//標準ログ出力
#include <gasha/log_queue_monitor.h>//ログキューモニター
#include <gasha/debug_log.h>//ログ操作
#include <gasha/print.h>//ログ出力操作 ※これだけインクルードしていれば print() が使用できる
#include <gasha/call_point.h>//コールポイント
#include <gasha/assert.h>//アサーション
#include <gasha/i_debug_pause.h>//デバッグポーズインターフェス
#include <gasha/std_debug_pause.h>//標準デバッグポーズ
#include <gasha/stdin_debug_pause.h>//標準入力デバッグポーズ
#include <gasha/dummy_debug_pause.h>//ダミーデバッグポーズ
#include <gasha/simple_assert.h>//シンプルアサーション
#include <gasha/profiler.h>//プロファイラ

#include <gasha/iterator.h>//イテレータ操作
#include <gasha/type_traits.h>//型特性ユーティリティ：toStr()
#include <gasha/shared_spin_lock.h>//共有スピンロック
#include <gasha/strconv.h>//文字列変換
#include <gasha/chrono.h>//時間処理ユーティリティ：nowElapsedTime()

#include <cstdio>//FILE, std::printf()

#ifdef GASHA_IS_GCC
//※別端末へのログ出力テスト用
//#include <sys/types.h>//open()
//#include <sys/stat.h>//open()
#include <fcntl.h>//open(), O_WRONLY, O_NDELAY, O_NOCTTY
#include <unistd.h>//close()
#include <cstdio>//fdopen()
#endif//GASHA_IS_GCC

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//ログレベル列挙
void printAllLogLevel()
{
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Enumeration of all log-levels ]\n");
	std::printf("\n");

	forEach(logLevelContainer(),//正順表示
	//reverseForEach(logLevelContainer(),//逆順表示
		[](logLevel& level_obj)
		{
			iConsole* print_console = level_obj.console(ofLog);
			iConsole* print_notice_console = level_obj.console(ofNotice);
			if (!print_console || *print_console == dummyConsole())
				print_console = &stdOutConsole::instance();
			if (!print_notice_console || *print_notice_console == dummyConsole())
				print_notice_console = &stdOutConsole::instance();
			char message[1024];
			char str_color[64];
			print_console->changeColor(level_obj.color(ofLog));
			if (level_obj.isSpecial())
				print_console->printf(message, "[%2d](SP%2d)\"%s\"", level_obj.outputLevel(), level_obj.value(), level_obj.name());
			else
				print_console->printf(message, "[%2d](%2d)\"%s\"", level_obj.outputLevel(), level_obj.value(), level_obj.name());
			print_console->putCr();
			iConsole* console = level_obj.console(ofLog);
			consoleColor& color = level_obj.color(ofLog);
			iConsole* notice_console = level_obj.console(ofNotice);
			consoleColor& color_of_notice = level_obj.color(ofNotice);
			logLevel prev = level_obj.prev();
			logLevel next = level_obj.next();
			if (console)
			{
				print_console->changeColor(level_obj.color(ofLog));
				print_console->printf(message, "\tconsole: \"%s\"(%s)", console->name(), color.toStr(str_color));
				print_console->putCr();
			}
			if (notice_console)
			{
				print_notice_console->changeColor(level_obj.color(ofNotice));
				print_notice_console->printf(message, "\tnotice: \"%s\"(%s)", notice_console->name(), color_of_notice.toStr(str_color));
				print_notice_console->putCr();
				print_notice_console->end();//画面通知は出力終了時に改行する
			}
			if (prev)
			{
				print_console->changeColor(prev.color(ofLog));
				print_console->printf(message, "\tprev: [%2d](%2d)\"%s\"", prev.outputLevel(), prev.value(), prev.name());
				print_console->putCr();
			}
			if (next)
			{
				print_console->changeColor(next.color(ofLog));
				print_console->printf(message, "\tnext: [%2d](%2d)\"%s", next.outputLevel(), next.value(), next.name());
				print_console->putCr();
			}
		}
	);
}

//----------------------------------------
//ログカテゴリ列挙
void printAllLogCategory()
{
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Enumeration of all log-categories ]\n");
	std::printf("\n");

	forEach(logCategoryContainer(),//正順表示
	//reverseForEach(logCategoryContainer(),//逆順表示
		[](logCategory& category_obj)
		{
			iConsole* print_console = category_obj.console(ofLog);
			if (!print_console)
				print_console = &stdOutConsole::instance();
			char message[1024];
			if (category_obj.isSpecial())
				print_console->printf(message, "(SP%2d)\"%s\"", category_obj.value(), category_obj.name());
			else
				print_console->printf(message, "(%2d)\"%s\"", category_obj.value(), category_obj.name());
			print_console->putCr();
			iConsole* console = category_obj.console(ofLog);
			iConsole* notice_console = category_obj.console(ofNotice);
			if (console)
			{
				print_console->printf(message, "\tconsole: \"%s\"", console->name());
				print_console->putCr();
			}
			if (notice_console)
			{
				print_console->printf(message, "\tnotice: \"%s\"", notice_console->name());
				print_console->putCr();
			}
		}
	);
}

//----------------------------------------
//ログレベルマスク列挙
void printAllLogMask(const logLevel::level_type level)
{
	logLevel level_obj(level);
	
	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Enumeration of all log-level-masks : Test for put message %s ]\n", level_obj.name());
	std::printf("\n");

	forEach(logMask(),//正順表示
		[&level_obj](logMask::iterator& mask_obj)
	//reverseForEach(logMask(),//逆順表示
	//	[&level_obj](logMask::reverse_iterator& mask_obj)
		{
			const logCategory& category = mask_obj.category();
			logLevel level_mask(mask_obj.level(ofLog));
			logLevel notice_level_mask(mask_obj.level(ofNotice));
			iConsole* print_console = level_obj.console(ofLog);
			iConsole* print_notice_console = level_obj.console(ofNotice);
			if (!print_console || *print_console == dummyConsole())
				print_console = &stdOutConsole::instance();
			if (!print_notice_console || *print_notice_console == dummyConsole())
				print_notice_console = &stdOutConsole::instance();
			char message[1024];
			if (category.isSpecial())
				print_console->printf(message, "(SP%2d)\"%s\"", category.value(), category.name());
			else
				print_console->printf(message, "(%2d)\"%s\"", category.value(), category.name());
			print_console->putCr();
			iConsole* console = mask_obj.console(ofLog, level_obj);
			const consoleColor* color = mask_obj.color(ofLog, level_obj);
			iConsole* notice_console = mask_obj.console(ofNotice, level_obj);
			const consoleColor* color_of_notice = mask_obj.color(ofNotice, level_obj);

			print_console->changeColor(level_mask.color(ofLog));
			print_console->printf(message, "\tmask(of log): (%2d)\"%s\"", level_mask.value(), level_mask.name());
			print_console->putCr();
			if (console)
			{
				print_console->changeColor(level_mask.color(ofLog));
				print_console->printf(message, "\t\tOK ... ");
				print_console->changeColor(*color);
				print_console->printf(message, "log-message: (%2d)\"%s\", \"%s\"", level_obj.value(), level_obj.name(), console->name());
				print_console->putCr();
			}
			else
			{
				print_console->changeColor(level_mask.color(ofLog));
				print_console->printf(message, "\t\tMASKED! ... ");
				print_console->changeColor(level_obj.color(ofLog));
				print_console->printf(message, "notice-message: (%2d)\"%s\"", level_obj.value(), level_obj.name());
				print_console->putCr();
			}
		
			print_console->changeColor(notice_level_mask.color(ofNotice));
			print_console->printf(message, "\tmask(of notice): (%2d)\"%s\"", notice_level_mask.value(), notice_level_mask.name());
			print_console->putCr();
			if (notice_console)
			{
				print_console->changeColor(notice_level_mask.color(ofNotice));
				print_console->printf(message, "\t\tOK ... ");
				print_notice_console->changeColor(*color_of_notice);
				print_notice_console->printf(message, "notice-message: (%2d)\"%s\", \"%s\"", level_obj.value(), level_obj.name(), notice_console->name());
				print_notice_console->putCr();
				print_notice_console->end();//画面通知は出力終了時に改行する
				if (*print_console != *print_notice_console)
					print_console->putCr();
			}
			else
			{
				print_console->changeColor(notice_level_mask.color(ofNotice));
				print_console->printf(message, "\t\tMASKED! ... ");
				print_console->changeColor(level_obj.color(ofNotice));
				print_console->printf(message, "notice-message: (%2d)\"%s\"", level_obj.value(), level_obj.name());
				print_console->putCr();
			}
		}
	);
}


//----------------------------------------
//デバッグログ個別機能テスト
void example_debugLog_featureTest()
{
	std::printf("\n");
	std::printf("================================================================================\n");

	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Feature test for debug-log ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	//コンソールカラー
	consoleColor color(consoleColor::iYELLOW, consoleColor::RED, consoleColor::UNDERLINE);

	//標準コンソールに文字列出力テスト
	//※標準コンソールは対象プラットフォームに応じて適切なコンソールが選択される
	{
		stdOutConsole console;
		console.changeColor(color);
		console.put("message for Standard");
		console.putCr();
	}

	//TTY端末に文字列出力テスト
	//※エスケープシーケンスで着色
	//※Windowsの場合はコマンドプロンプトに出力
	{
		ttyConsole console(stdout);
		console.changeColor(color);
		console.put("message for TTY (stdout)");
		console.putCr();
	}

	//TTY端末の標準エラー出力
	{
		ttyConsole console(stderr);
		console.changeColor(color);
		console.put("message for TTY (stderr)");
		console.putCr();
	}

	//カラーなしTTY端末に文字列出力テスト
	//※着色を無視
	//※Windowsの場合はコマンドプロンプトに出力
	{
		monoTtyConsole console(stdout);
		console.changeColor(color);
		console.put("message for Mono-TTY (stdout)");
		console.putCr();
	}

	//Windowsコマンドプロンプトに文字列出力テスト
	//※Windowsのコンソールテキスト属性で着色
	//※Windows以外の場合は標準出力に出力
	{
		winConsole console(stdout);
		console.changeColor(color);
		console.put("message for Win (stdout)");
		console.putCr();
	}
	
	//Windowsコマンドプロンプトの標準エラー出力
	{
		winConsole console(stderr);
		console.changeColor(color);
		console.put("message for Win (stderr)");
		console.putCr();
	}

	//カラーなしWindowsコマンドプロンプトに文字列出力テスト
	//※着色を無視
	//※Windows以外の場合は標準出力に出力
	{
		monoWinConsole console(stdout);
		console.changeColor(color);
		console.put("message for Mono-Win (stdout)");
		console.putCr();
	}

	//Visual Studio 出力ウインドウに文字列出力テスト
	//※着色不可
	//※Windows以外／開発ツール無効時の場合は標準出力に出力
	{
		vsConsole console;
		console.changeColor(color);
		console.put("message for VS");
		console.putCr();
	}

	//明示的なログレベル／カテゴリコンテナの初期化
	//※GASHA_LOG_***_SECURE_INITIALIZE 設定時には不要
	{
	#ifndef GASHA_LOG_LEVEL_CONTAINER_SECURE_INITIALIZE
		logLevelContainer level_con(logLevelContainer::explicitInit);//ログレベルの明示的な初期化
	#endif//GASHA_LOG_LEVEL_CONTAINER_SECURE_INITIALIZE
	#ifndef GASHA_LOG_CATEGORY_CONTAINER_SECURE_INITIALIZE
		logCategoryContainer cate_con(logCategoryContainer::explicitInit);//ログカテゴリの明示的な初期化
	#endif//GASHA_LOG_CATEGORY_CONTAINER_SECURE_INITIALIZE
	#ifndef GASHA_LOG_MASK_SECURE_INITIALIZE
		logMask mask(logMask::explicitInit);//ログレベルマスクの明示的な初期化
	#endif//GASHA_LOG_MASK_SECURE_INITIALIZE
	#ifndef GASHA_LOG_ATTR_SECURE_INITIALIZE
		logAttr attr(logAttr::explicitInit);//ログ属性の明示的な初期化
	#endif//GASHA_LOG_ATTR_SECURE_INITIALIZE
#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	#ifndef GASHA_LOG_WORK_BUFF_SECURE_INITIALIZE
		logWorkBuff log_buff(logWorkBuff::explicitInit);//ログワークバッファの明示的な初期化
	#endif//GASHA_LOG_WORK_BUFF_SECURE_INITIALIZE
	#ifndef GASHA_LOG_QUEUE_SECURE_INITIALIZE
		logQueue log_queue(logQueue::explicitInit);//ログキューの明示的な初期化
	#endif//GASHA_LOG_QUEUE_SECURE_INITIALIZE
	#ifndef GASHA_LOG_QUEUE_MONITOR_SECURE_INITIALIZE
		logQueueMonitor log_queue_monitor(logQueueMonitor::explicitInit);//ログキューモニターの明示的な初期化
	#endif//GASHA_LOG_QUEUE_MONITOR_SECURE_INITIALIZE
#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	}

	//ログレベルのコンソールを変更
	{
		static winConsole console(stdout, "win-console *CHANGED*");
		logLevel level(asNormal);
		level.setConsole(ofLog, console);
		//level->setConsole(ofNotice, console);
	}

	//全ログレベルのコンソールをまとめて変更
	//※replaceEachConsole()メソッドは、nullptr が設定されているコンソールは変更しない
	{
		static vsConsole console("vs-console *CHANGED*");
		//logLevel::replaceEachConsole(ofLog, &console);
		logLevelContainer con;
		con.replaceEachConsole(ofNotice, &console);
	}

	//独自ログレベルを追加
	enum extLogLevel : logLevel::level_type
	{
		asMoreDetail = GASHA_MAKE_LOG_LEVEL_VALUE(0, 0),//更に詳細
		asAboveNormal = GASHA_MAKE_LOG_LEVEL_VALUE(4, 1),//通常より少し上のレベル
	};
	{
		static vsConsole vs_console("vs-console *CUSTOM*");
		iConsole* consoles[] = { &stdOutConsole::instance(), &vs_console };
		consoleColor colors[] = { consoleColor(consoleColor::WHITE, consoleColor::iBLACK), consoleColor(consoleColor::stdColor) };
		regLogLevel<asMoreDetail>()("asMoreDetail", consoles, colors);
	}
	{
		iConsole* consoles[] = { &stdOutConsole::instance(), nullptr };
		consoleColor colors[] = { consoleColor(consoleColor::CYAN), consoleColor(consoleColor::stdColor) };
		regLogLevel<asAboveNormal>()("asAboveNormal", consoles, colors);
	}

	//全ログレベルの列挙
	printAllLogLevel();

	//全カテゴリレベルの列挙
	printAllLogCategory();

	//ログレベルマスクを変更
	{
		logMask mask;
		{
			auto ite = mask.find(forTARO);//太郎用ログレベルマスク（イテレータ）を取得
			ite.changeLevel(ofLog, asDetail);//ログレベルマスクを詳細メッセージレベルに下げる
			ite.changeLevel(ofNotice, asNormal);//画面通知マスクを通常メッセージレベルに下げる
		}
		{
			mask.changeLevel(ofLog, asSilent, forJIRO);//次郎用ログレベルマスクを静寂レベルに上げる
			mask.changeLevel(ofNotice, asSilentAbsolutely, forJIRO);//次郎用画面通知マスクを絶対静寂レベルに上げる
		}
	}

	//ローカルログレベルマスク＆ローカルログ属性
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for change log-level-mask & log-attribute ]\n");
		std::printf("\n");
		logMask mask;
		logAttr attr;
		char mask_serialize_buff[sizeof(logMask::mask_type)];
		char attr_serialize_buff[sizeof(logAttr::attr_type)];
		std::printf("1:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("1:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.changeRef(logMask::isLocal);
		attr.changeRef(logAttr::isLocal);
		std::printf("2:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("2:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.changeRef(logMask::isGlobal);
		attr.changeRef(logAttr::isGlobal);
		std::printf("2a:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("2a:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.changeRef(logMask::isTls);
		attr.changeRef(logAttr::isTls);
		std::printf("2b:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("2b:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.changeRef(logMask::isLocal);
		attr.changeRef(logAttr::isLocal);
		std::printf("2c:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("2c:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.changeLevel(ofLog, asNormal, forEvery);
		attr.add(logWithCategory);
		attr.add(noticeWithLevel);
		std::printf("3:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("3:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		{
			logMask mask;
			logAttr attr;
			std::printf("4:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
			std::printf("4:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
			{
				logMask mask;
				logAttr attr;
				std::printf("5:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("5:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isLocal);
				attr.changeRef(logAttr::isLocal);
				std::printf("6:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("6:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeLevel(ofLog, asDetail, forEvery);
				attr.remove(logWithCategory);
				attr.remove(noticeWithLevel);
				attr.add(logWithLevel);
				attr.add(noticeWithCategory);
				std::printf("7:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isLocal);
				attr.changeRef(logAttr::isLocal);
				std::printf("7a:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7a:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isGlobal);
				attr.changeRef(logAttr::isGlobal);
				std::printf("7b:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7b:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				{
					logMask mask;
					logAttr attr;
					std::printf("7bx:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
					std::printf("7bx:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					mask.changeRef(logMask::isLocal);
					attr.changeRef(logAttr::isLocal);
					std::printf("7bx2:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
					std::printf("7bx2:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					mask.changeLevel(ofLog, asSilentAbsolutely, forEvery);
					attr.add(logWithLevel);
					attr.add(logWithCategory);
					std::printf("7bx3:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
					std::printf("7bx3:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					mask.serialize(mask_serialize_buff, sizeof(mask_serialize_buff));
					attr.serialize(attr_serialize_buff, sizeof(attr_serialize_buff));
					logMask mask2(std::move(mask));
					logAttr attr2(std::move(attr));
					std::printf("7bx4:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
					std::printf("7bx4:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					std::printf("7bx4:mask2: refType=%d, level=%d\n", mask2.refType(), mask2.level(ofLog, forAny));
					std::printf("7bx4:attr2: refType=%d, attr=0x%08x\n", attr2.refType(), attr2.attrValue());
					logMask mask3;
					logAttr attr3;
					mask3 = std::move(mask2);
					attr3 = std::move(attr2);
					std::printf("7bx5:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
					std::printf("7bx5:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					std::printf("7bx5:mask2: refType=%d, level=%d\n", mask2.refType(), mask2.level(ofLog, forAny));
					std::printf("7bx5:attr2: refType=%d, attr=0x%08x\n", attr2.refType(), attr2.attrValue());
					std::printf("7bx5:mask3: refType=%d, level=%d\n", mask3.refType(), mask3.level(ofLog, forAny));
					std::printf("7bx5:attr3: refType=%d, attr=0x%08x\n", attr3.refType(), attr3.attrValue());
					{
						logMask mask;
						logAttr attr;
						std::printf("7bxx:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
						std::printf("7bxx:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
					}
				}
				mask.changeRef(logMask::isLocal);
				attr.changeRef(logAttr::isLocal);
				std::printf("7c:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7c:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isTls);
				attr.changeRef(logAttr::isTls);
				std::printf("7d:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7d:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isGlobal);
				attr.changeRef(logAttr::isGlobal);
				std::printf("7e:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7e:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isLocal);
				attr.changeRef(logAttr::isLocal);
				std::printf("7f:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7f:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
				mask.changeRef(logMask::isGlobal);
				attr.changeRef(logAttr::isGlobal);
				std::printf("7g:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
				std::printf("7g:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
			}
			std::printf("8:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
			std::printf("8:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		}
		std::printf("9:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("9:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
		mask.deserialize(mask_serialize_buff, sizeof(mask_serialize_buff));
		attr.deserialize(attr_serialize_buff, sizeof(attr_serialize_buff));
		std::printf("9a:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("9a:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
	}
	{
		logMask mask;
		logAttr attr;
		std::printf("10:mask: refType=%d, level=%d\n", mask.refType(), mask.level(ofLog, forAny));
		std::printf("10:attr: refType=%d, attr=0x%08x\n", attr.refType(), attr.attrValue());
	}

	//ログレベルマスク列挙
	{
		printAllLogMask(asDetail);//詳細レベルの表示チェック
		printAllLogMask(asNormal);//通常レベルの表示チェック
		printAllLogMask(asCritical);//重大レベルの表示チェック
		printAllLogMask(asAbsolute);//絶対レベルの表示チェック
	}

#ifdef GASHA_IS_GCC
	//【Unix用】別の端末にログを出力
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for console that put other tty ]\n");
		std::printf("\n");
		
		//端末をオープン
		//※dev/pty*/はCygwinの端末
		int fd1 = open("/dev/pty1", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		int fd2 = open("/dev/pty2", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		int fd3 = open("/dev/pty3", O_WRONLY | O_NDELAY | O_NOCTTY);//書き込み専用＋（可能なら）非停止モード＋制御端末割り当て禁止（端末でCtrl+Cなどの制御が効かない）
		std::FILE* fp1 = stdout;
		std::FILE* fp2 = stderr;
		std::FILE* fp3 = stderr;
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
		con.replaceEachConsole(ofNotice, &stdNoticeConsole::instance());

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
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for log-queue-monitor ]\n");
		std::printf("\n");
		
	#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		//ログキューモニタースレッド実行
		//auto monitor_thread_func = []()
		//{
		//	logQueueMonitor mon;
		//	mon.monitor(stdLogPrint());
		//};
		//std::thread monitor_thread(monitor_thread_func);
		beginLogQueueMonitor();

		//メッセージのエンキュー
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
				logWorkBuff work_buff(logWorkBuff::explicitInit);
				logQueue queue(logQueue::explicitInit);
				logQueueMonitor mon;
				mon.reset();//ログキューモニターリセット
			}

			logQueue queue;
			logQueue::id_type reserved_id = queue.reserve(1);//エンキュー予約
			for (int i = 1; i < 1000000000; i *= 3)
			{
				//エンキュー
				{
					logWorkBuff work_buff;
					char* message = work_buff.alloc();
					if (message)
					{
						std::size_t message_len = 0;
						work_buff.spprintf(message, message_len, "message:%d", i);
						logMask mask;
						logAttr attr;
						logLevel::level_type level = asNormal;
						logCategory::category_type category = forTARO;
						logPrintInfo print_info;
						print_info.setId(0);//IDは自動発番
						print_info.setTime(nowElapsedTime());
						print_info.setMessage(message);
						print_info.setMessageSize(message_len + 1);
						print_info.setLevel(level);
						print_info.setCategory(category);
						print_info.setAttr(attr.attrValue());
						for (logPurpose::purpose_type purpose = 0; purpose < logPurpose::NUM; ++purpose)
						{
							print_info.setConsole(purpose, mask.console(purpose, level, category));
							print_info.setColor(purpose, mask.color(purpose, level, category));
						}
						//stdLogPrint()(print_info);//エンキューせずに直接表示する場合
						const bool result = queue.enqueue(print_info);//エンキュー
						if (result)
						{
							//モニターに通知
							logQueueMonitor mon;
							mon.notify();
						}
						else
							std::printf("enqueu failed.\n");
						work_buff.free(message);
					}
					else
						std::printf("message making failed.\n");
				}
			#if 0
				//デキュー
				{
					logQueue::node_type node;
					logQueue queue;
					const bool result = queue.dequeue(node);
					if (result)
					{
						std::printf("id=%lld, category=%d, level=%d, message=\"%s\"\n", node.m_id, node.m_category, node.m_level, node.m_message);
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
					std::size_t message_len = 0;
					work_buff.spprintf(message, message_len, "reserved message");
					logMask mask;
					logAttr attr;
					logPrintInfo print_info;
					logLevel::level_type level = asNormal;
					logCategory::category_type category = forTARO;
					print_info.setId(reserved_id);//予約したID
					print_info.setTime(nowElapsedTime());
					print_info.setMessage(message);
					print_info.setMessageSize(message_len + 1);
					print_info.setLevel(level);
					print_info.setCategory(category);
					print_info.setAttr(attr.attrValue());
					for (logPurpose::purpose_type purpose = 0; purpose < logPurpose::NUM; ++purpose)
					{
						print_info.setConsole(purpose, mask.console(purpose, level, category));
						print_info.setColor(purpose, mask.color(purpose, level, category));
					}
					//stdLogPrint()(print_info);//エンキューせずに直接表示する場合
					const bool result = queue.enqueue(print_info);//エンキュー
					if (result)
					{
						//モニターに通知
						logQueueMonitor mon;
						mon.notify();
					}
					else
						std::printf("enqueu of reservation failed.\n");
					work_buff.free(message);
				}
				else
					std::printf("message making of reservation failed.\n");
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
		//monitor_thread.join();
		waitLogQueueMonitor();
	#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	}
	
	//メモリコンソールのテスト
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for memory console ]\n");
		std::printf("\n");
		
		char buff[256];
		std::size_t message_len;
		memConsole<128, sharedSpinLock> console;//任意のバッファサイズのメモリコンソールを作成
		//auto& console = stdMemConsole::instance();//標準メモリコンソールを使用
		console.printScreen();
		std::printf("\n");
		console.printf(buff, "%d%d", 1234567890, 1234567890);
		console.printScreen();
		std::printf("\n");
		console.clear();
		console.printf(buff, 16, "%d%d", 1234567890, 1234567890);
		console.printScreen();
		std::printf("\n");
		console.clear();
		console.printScreen();
		std::printf("\n");
		message_len = console.copy(buff, sizeof(buff));
		std::printf("%s(len=%d)\n", buff, message_len);
		for (int i = 0; i <= 32; ++i)
		{
			console.clear();
			const char* str = "1234567890123456789012345678901234567890";
			char buff[32 + 1];
			strncpy_fast(buff, str, i);
			buff[i] = '\0';
			console.put(buff);
			std::memset(buff, 0, sizeof(buff));
			message_len = console.copy(buff, sizeof(buff));
			console.printScreen();
			std::printf("\n");
			std::printf("%s(len=%d)\n", buff, message_len);
			console.put("#");
			message_len = console.copy(buff, sizeof(buff));
			console.printScreen();
			std::printf("\n");
			std::printf("%s(len=%d)\n", buff, message_len);
		}
		console.clear();
		for (int i = 0; i < 10; ++i)
		{
			console.put(nullptr);
			console.put("1");
			console.put("23456");
			console.put("7");
			console.put("8");
			console.put("9");
			console.put("0");
			console.put("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			console.put("abcdefghijklmnopqrstuvwxyz");
			message_len = console.copy(buff, sizeof(buff));
			console.printScreen();
			std::printf("\n");
			std::printf("%s(len=%d)\n", buff, message_len);
		}
	}
	
	//ログ出力操作のテスト
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for print log ]\n");
		std::printf("\n");
		
		debugLog log;
		log.initialize();

	#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		//ログキューモニタースレッド実行
		//auto monitor_thread_func = []()
		//{
		//	logQueueMonitor mon;
		//	mon.monitor();
		//};
		//std::thread monitor_thread(monitor_thread_func);
		beginLogQueueMonitor();
	#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化

		logAttr attr;
		attr.add(logWithID);
		attr.add(logWithTime);
		attr.add(logWithLevel);
		attr.add(logWithCategory);
		print(asImportant, forAny, "(01)Test Message:%d", 10);

		attr.reset();
		attr.add(ofLog, logPurposeWithID);
		attr.add(ofLog, logPurposeWithTime);

		log.reserve(asImportant, forAny, 8);

		log.print(asImportant, forAny, "(02)Test Message:%d", 10);
		
		log.convPrint(dummyStrConv(), asImportant, forAny, "(03)Test Message:%d", 10);
		log.convPrint(upperCaseConv, asImportant, forAny, "(04)Test Message:%d", 10);
		log.convPrint(lowerCaseConv, asImportant, forAny, "(05)Test Message:%d", 10);

		log.put(asImportant, forAny, "(06)Test Message");
		
		log.convPut(dummyStrConv(), asImportant, forAny, "(07)Test Message");
		log.convPut(upperCaseConv, asImportant, forAny, "(08)Test Message");
		log.convPut(lowerCaseConv, asImportant, forAny, "(09)Test Message");

		log.reservedPrint("(10)Test Message:%d", 10);
		
		log.reservedConvPrint(dummyStrConv(), "(11)Test Message");
		log.reservedConvPrint(upperCaseConv, "(12)Test Message");
		log.reservedConvPrint(lowerCaseConv, "(13)Test Message");
		
		log.reservedPut("(14)Test Message");
		
		log.reservedConvPut(dummyStrConv(), "(15)Test Message");
		log.reservedConvPut(upperCaseConv, "(16)Test Message");
		log.reservedConvPut(lowerCaseConv, "(17)Test Message");
		
		log.flush();

		attr.remove(ofLog, logPurposeWithID);
		attr.add(ofLog, logPurposeHeaderOnlyColored);

		log.printDirect(asImportant, forAny, "(18)Test Message:%d", 10);
		log.printDirect(stdLogPrint(), asImportant, forAny, "(19)Test Message:%d", 10);
		
		log.convPrintDirect(dummyStrConv(), asImportant, forAny, "(20)Test Message:%d", 10);
		log.convPrintDirect(upperCaseConv, asImportant, forAny, "(21)Test Message:%d", 10);
		log.convPrintDirect(lowerCaseConv, asImportant, forAny, "(22)Test Message:%d", 10);
		log.convPrintDirect(stdLogPrint(), dummyStrConv(), asImportant, forAny, "(23)Test Message:%d", 10);
		log.convPrintDirect(stdLogPrint(), upperCaseConv, asImportant, forAny, "(24)Test Message:%d", 10);
		log.convPrintDirect(stdLogPrint(), lowerCaseConv, asImportant, forAny, "(25)Test Message:%d", 10);

		attr.add(ofLog, logPurposeWithoutColor);

		log.putDirect(asImportant, forAny, "(26)Test Message");
		log.putDirect(stdLogPrint(), asImportant, forAny, "(27)Test Message");
		
		log.convPutDirect(dummyStrConv(), asImportant, forAny, "(28)Test Message");
		log.convPutDirect(upperCaseConv, asImportant, forAny, "(29)Test Message");
		log.convPutDirect(lowerCaseConv, asImportant, forAny, "(30)Test Message");
		log.convPutDirect(stdLogPrint(), dummyStrConv(), asImportant, forAny, "(31)Test Message");
		log.convPutDirect(stdLogPrint(), upperCaseConv, asImportant, forAny, "(32)Test Message");
		log.convPutDirect(stdLogPrint(), lowerCaseConv, asImportant, forAny, "(33)Test Message");

		log.flush();
		log.pause();
		log.resume();
		log.abort();

	#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		//monitor_thread.join();
		waitLogQueueMonitor();
	#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	}

	//コールポイントのテスト
	char message[1024];
	auto print_cp = [](const callPoint* cp)
	{
		std::printf("> recent critical-cp info\n");
		if (cp)
		{
			logCategory category(cp->category());
			std::printf("  - name = \"%s\"\n", cp->name());
			std::printf("  - category = \"%s\"\n", category.isExist() ? category.name() : "");
			std::printf("  - type = \"%s\"\n", cp->type() == callPoint::isNormal ? "isNormal" : cp->type() == callPoint::isCritical ? "isCritical" : "isReference");
			std::printf("  - autoProfiling = \"%s\"\n", cp->autoProfiling() == callPoint::useAutoProfiling ? "useAutoProfiling" : "noProfiling");
			std::printf("  - source file = \"%s\"\n", cp->fileName());
			std::printf("  - function = \"%s\"\n", cp->funcName());
			std::printf("  - begin time = %.6lf\n", cp->beginTime());
		}
		else
		{
			std::printf("  (nothing)\n");
		}
	};
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for call-point ]\n");
		std::printf("\n");

		callPoint cp(forTARO, "test", callPoint::useAutoProfiling, GASHA_CP_ARGS);
		{
			callPoint cp;
			const callPoint* recent_cp = cp.find();
			print_cp(recent_cp);
			{
				criticalCallPoint cp(forTARO, "test2", callPoint::useAutoProfiling, GASHA_CP_ARGS);
				{
					callPoint cp(forTARO, "test3", GASHA_CP_ARGS);
					const callPoint* recent_cp = cp.find();
					print_cp(recent_cp);
					recent_cp = cp.findCritical();
					print_cp(recent_cp);
					callPoint cp2(std::move(cp));
					{
						callPoint cp;
						const callPoint* recent_cp = cp.find();
						print_cp(recent_cp);
						callPoint cp2(forTARO, "test4", GASHA_CP_ARGS);
						recent_cp = cp.find();
						print_cp(recent_cp);
						cp = std::move(cp2);
						recent_cp = cp.find();
						print_cp(recent_cp);
						const std::size_t message_len = cp.debugInfo(message, sizeof(message));
						std::printf("debugInfo(len=%d):\n%s\n", message_len, message);
					}
					recent_cp = cp.find();
					print_cp(recent_cp);
					recent_cp = cp.findCritical();
					print_cp(recent_cp);
				}
				const callPoint* recent_cp = cp.find();
				print_cp(recent_cp);
				recent_cp = cp.findCritical();
				print_cp(recent_cp);
			}
			recent_cp = cp.find();
			print_cp(recent_cp);
			recent_cp = cp.findCritical();
			print_cp(recent_cp);
		}
		const callPoint* recent_cp = cp.find();
		print_cp(recent_cp);
		recent_cp = cp.findCritical();
		print_cp(recent_cp);
	}
	{
		callPoint cp;
		const callPoint* recent_cp = cp.find();
		print_cp(recent_cp);
		recent_cp = cp.findCritical();
		print_cp(recent_cp);
		const std::size_t message_len = cp.debugInfo(message, sizeof(message));
		std::printf("debugInfo(len=%d):\n%s\n", message_len, message);
	}
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("[ Test for assert/break-point/watch-point ]\n");
		std::printf("\n");

		debugLog log;
		log.initialize();

	#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		//ログキューモニタースレッド実行
		//auto monitor_thread_func = []()
		//{
		//	threadId thread_id("monitor");
		//	{
		//		callPoint cp(forAny, "log monitor", callPoint::useAutoProfiling, GASHA_CP_ARGS);
		//		logQueueMonitor mon;
		//		mon.monitor();
		//	}
		//	profiler prof;
		//	prof.sumup(profiler::withUpdatePeriod);
		//};
		//std::thread monitor_thread(monitor_thread_func);
		beginLogQueueMonitor();
	#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		
		logMask mask;
		mask.changeLevel(ofLog, asNormal, forAny);
		logAttr attr;
		attr.reset();
		attr.add(ofLog, logPurposeWithLevel | logPurposeWithCategory);

		{
			//ダミーデバッグポーズ
			breakPoint bp;
			dummyDebugPause dummy_pause;
		#ifndef ENABLE_FEATURE_ASSERT_TEST
			bp.changeDebugPause(dummy_pause);
		#endif//ENABLE_FEATURE_ASSERT_TEST
			
		#ifdef ENABLE_FEATURE_ASSERT_TEST
			GASHA_SIMPLE_ASSERT(1 == 1, "SIMPLE ASSERT:%d", 123);
			GASHA_SIMPLE_ASSERT(1 != 1, "SIMPLE ASSERT:%d", 123);
			GASHA_SIMPLE_BREAKPOINT("SIMPLE BREAKPOINT:%d", 123);
			GASHA_SIMPLE_WATCHPOINT(1 != 1, "SIMPLE WATCHPOINT:%d", 123);
			GASHA_SIMPLE_WATCHPOINT(1 == 1, "SIMPLE WATCHPOINT:%d", 123);
		#endif//ENABLE_FEATURE_ASSERT_TEST

			criticalCallPoint cp(forTARO, "CP1", GASHA_CP_ARGS);
			{
				criticalCallPoint cp(forJIRO, "CP2", GASHA_CP_ARGS);
				{
					GASHA_ASSERT(asNormal, forAny, 1 == 1, "ASSERT:%d", 123);
					GASHA_ASSERT(asNormal, forAny, 1 != 1, "ASSERT:%d", 123);
					GASHA_BREAKPOINT(asNormal, forAny, "BREAKPOINT:%d", 123);
					GASHA_WATCHPOINT(asNormal, forAny, 1 != 1, "WATCHPOINT:%d", 123);
					GASHA_WATCHPOINT(asNormal, forAny, 1 == 1, "WATCHPOINT:%d", 123);
					
					GASHA_ASSERT(asNormal, forCallPoint, 1 != 1, "ASSERT:%d", 123);
					GASHA_BREAKPOINT(asNormal, forCallPoint, "BREAKPOINT:%d", 123);
					GASHA_WATCHPOINT(asNormal, forCallPoint, 1 == 1, "WATCHPOINT:%d", 123);

					GASHA_ASSERT(asDetail, forAny, 1 != 1, "ASSERT:%d", 123);
					GASHA_BREAKPOINT(asDetail, forAny, "BREAKPOINT:%d", 123);
					GASHA_WATCHPOINT(asDetail, forAny, 1 == 1, "WATCHPOINT:%d", 123);

					GASHA_ASSERT(asDetail, forCriticalCallPoint, 1 != 1, "ASSERT:%d", 123);
					GASHA_BREAKPOINT(asDetail, forCriticalCallPoint, "BREAKPOINT:%d", 123);
					GASHA_WATCHPOINT(asDetail, forCriticalCallPoint, 1 == 1, "WATCHPOINT:%d", 123);
				}
			}

		#ifdef ENABLE_FEATURE_ASSERT_TEST
			//ダミーデバッグポーズ
			bp.changeDebugPause(dummy_pause);
		#endif//ENABLE_FEATURE_ASSERT_TEST

			GASHA_ASSERT(asNormal, forAny, 1 != 1, "ASSERT:%d", 123);
			GASHA_BREAKPOINT(asNormal, forAny, "BREAKPOINT:%d", 123);
			GASHA_WATCHPOINT(asNormal, forAny, 1 == 1, "WATCHPOINT:%d", 123);

		#ifdef ENABLE_FEATURE_ASSERT_TEST
			//標準入力デバッグポーズ
			stdinDebugPause stdin_pause;
			bp.changeDebugPause(stdin_pause);
		#endif//ENABLE_FEATURE_ASSERT_TEST

			GASHA_ASSERT(asNormal, forAny, 1 != 1, "ASSERT:%d", 123);
			GASHA_BREAKPOINT(asNormal, forAny, "BREAKPOINT:%d", 123);
			GASHA_WATCHPOINT(asNormal, forAny, 1 == 1, "WATCHPOINT:%d", 123);

		#ifdef ENABLE_FEATURE_ASSERT_TEST
			//標準デバッグポーズに戻す
			bp.resetDebugPause();
		#endif//ENABLE_FEATURE_ASSERT_TEST
			
			GASHA_ASSERT(asNormal, forAny, 1 != 1, "ASSERT:%d", 123);
			GASHA_BREAKPOINT(asNormal, forAny, "BREAKPOINT:%d", 123);
			GASHA_WATCHPOINT(asNormal, forAny, 1 == 1, "WATCHPOINT:%d", 123);

		#ifdef ENABLE_FEATURE_ASSERT_TEST
			//標準デバッグポーズに戻す
			bp.resetDebugPause();
		#endif//ENABLE_FEATURE_ASSERT_TEST
		}

		log.abort();

	#ifdef GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
		//monitor_thread.join();
		waitLogQueueMonitor();
	#endif//GASHA_LOG_IS_ENABLED//デバッグログ無効時は無効化
	}

	{
		profiler prof;
		//集計	
		prof.sumup(profiler::isNormal);
		//カウント追加
		for (int i = 0; i < 3; ++i)
		{
			callPoint cp(forAny, "test", callPoint::useAutoProfiling, GASHA_CP_ARGS);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		//再度集計
		prof.sumup(profiler::withUpdatePeriod);
		//結果取得
		std::printf("\n");
		std::printf("--- Profile ---\n");
		profiler::threadInfo thread_info_array[10];
		const std::size_t num = prof.getThreadInfo(thread_info_array);
		for (std::size_t i = 0; i < num; ++i)
		{
			const auto& thread_info = thread_info_array[i];
			std::printf("----------\n");
			std::printf("thread: \"%s\"\n", thread_info.threadName());
			profiler::profileInfo prof_info_array[10];
			const std::size_t num = prof.getProfileInfo(thread_info, prof_info_array);
			for (std::size_t i = 0; i < num; ++i)
			{
				const auto& prof_info = prof_info_array[i];
				std::printf("  -----");
				std::printf("  Process:\"%s\"\n", prof_info.name());
				const auto& total_time = prof_info.totalTime();
				std::printf("    TOTAL: count=%d, time=%.9lf, max=%.9lf, min=%.9lf, avg=%.9lf, sum-cnt=%d, max-cnt=%d, min-cnt=%d, avg-cnt=%.2f\n", total_time.count(), total_time.time(), total_time.maxTime(), total_time.minTime(), total_time.avgTime(), total_time.summarizedCount(), total_time.maxCount(), total_time.minCount(), total_time.avgCount());
				const auto& period_time = prof_info.periodicTime();
				std::printf("    PERIOD:count=%d, time=%.9lf, max=%.9lf, min=%.9lf, avg=%.9lf, sum-cnt=%d, max-cnt=%d, min-cnt=%d, avg-cnt=%.2f\n", period_time.count(), period_time.time(), period_time.maxTime(), period_time.minTime(), period_time.avgTime(), period_time.summarizedCount(), period_time.maxCount(), period_time.minCount(), period_time.avgCount());
				//const auto& time = prof_info.time();
				//printf("    NOW:   count=%d, time=%.9lf, max=%.9lf, min=%.9lf, avg=%.9lf\n", time.count(), time.time(), time.maxTime(), time.minTime(), time.avgTime());
			}
		}
		std::printf("--------------\n");
		std::printf("\n");
	}
	
	std::printf("\n");
	std::printf("================================================================================\n");
}

//明示的なインスタンス化
#include <gasha/mem_console.cpp.h>
GASHA_INSTANCING_memConsole_withLock(128, sharedSpinLock);

// End of file
