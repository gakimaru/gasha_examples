//--------------------------------------------------------------------------------
// exmaple_debug_log_basic_test.cpp
// デバッグログ基本テスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_log.h"//デバッグログテスト

#include <gasha/print.h>//ログ出力操作 ※通常のログ出力は、このインクルード一つでOK

#include <gasha/debug_log.h>//ログ操作 ※様々なログ操作を行う場合にインクルード
#include <gasha/log_mask.h>//ログレベルマスク ※ログレベルのマスクを変更する場合にインクルード
#include <gasha/log_attr.h>//ログ属性 ※ログ表示の属性を変更する場合にインクルード

#include <gasha/vs_console.h>//Visual Studio出力ウインドウ ※ログの出力先を変更する場合にインクルード
//※ログ出力先のコンソールクラスは多数存在。他には、TTY端末用、Windowsコマンドプロンプト用、ダミーコンソール（出力を捨てる）、メモリコンソール（リングバッファに記録）。
//※コンソールクラスは独自に追加することも可能。ゲーム画面に表示するための独自クラスなどが可能。iConsole インターフェースを実装して作成する。

#include <gasha/std_console.h>//標準コンソール ※標準出力コンソール、標準エラーコンソール、標準通知コンソール、標準ダミーコンソール、標準メモリコンソールを利用する場合にインクルード

#include <gasha/assert.h>//アサーション／ブレークポイント／ウォッチポイント ※使用したい時にインクルード
#include <gasha/stdin_debug_pause.h>//標準入力デバッグポーズ ※アサーション違反／ブレークポイント時／ウォッチポイント適合時にポーズ処理を行いたい場合にインクルード
#include <gasha/dummy_debug_pause.h>//ダミーデバッグポーズ
//※デバッグポーズクラスは多数存在。標準（デバッガブレークポイントのみ）、標準入力（画面でエンターキー入力待ち）、ダミー（待たない）
//※デバッグポーズクラスは独自に追加することも可能。ゲーム画面でボタン入力やタップを待つ独自クラスなどが可能。iDebugPause インターフェースを実装して作成する。

#include <gasha/call_point.h>//コールポイント ※コールポイントを使用する場合にインクルード
#include <gasha/strconv.h>//文字列変換 ※ログ出力時に文字コード変換を行う場合にインクルード
#include <gasha/profiler.h>//プロファイラ ※プロファイルの集計および集計結果の取得を行う場合にインクルード

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//コールポイントテスト用共通処理
static void commonFunc();

//----------------------------------------
//デバッグログ基本テスト
void example_debugLog_basicTest()
{
	std::printf("\n");
	std::printf("================================================================================\n");

	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Basic test for debug-log ]\n");
	std::printf("--------------------------------------------------------------------------------\n");

	//--------------------
	//ログ情報初期化
	{
		debugLog log;
		log.initialize();
	}

	//--------------------
	//ログ出力
	//※ログレベル、ログカテゴリの指定がある以外は、printfと同じ
	print(asNormal, forAny, "【標準】メッセージ①");//※改行は自動的に行われる
	print(asCritical, forAny, "【重大】メッセージ①");//※画面通知も同時に行われる
	print(asDetail, forAny, "【詳細】メッセージ①");//※デフォルトでは詳細ログレベルは表示されない
	
	//※ビルド設定 GASHA_LOG_WORK_BUFF_BLOCK_SIZE よりも大きなサイズのメッセージは出力できないので注意。
	//　（最大サイズで切られたメッセージが出力される）

	//--------------------
	//ログキューモニタースレッドを起動
	//※ビルド設定で GASHA_LOG_PRINT_USE_QUEUE が指定されている時は、
	//　キューを通してログを出力する。未指定時はキューを通さず直接出力する。
	//※キューイングされたログを出力するために、ログキューモニタースレッドを起動しておく。
	//※キューを使用すると、マルチスレッドでログ出力が衝突しても表示が崩れることがなく、
	//　順序性も保証される。
	//※キューの使用／不使用に関わらず、専用のワークバッファを使用してログメッセージを
	//　作成するため、スタックサイズの小さなスレッドからのログ出力も可能。
	beginLogQueueMonitor();

	//--------------------
	//ログレベルマスクを変更
	//※指定のログレベルより低いレベルの出力が無視されるようになる。
	//※ログ出力と画面通知のそれぞれに対して設定可能
	//※デフォルトは、ログ出力は通常レベル（通常レベル以上のメッセージは表示される）
	//　画面通知は重大レベル（重大レベル以上のメッセージは表示される）
	logMask mask;
	mask.changeLevel(ofLog, asDetail, forAny);//ログ出力レベルを詳細レベルに下げる
	mask.changeLevel(ofNotice, asSilent, forAny);//画面通知レベルを静寂レベルに上げる

	//ログ出力
	print(asNormal, forAny, "【標準】メッセージ②");
	print(asCritical, forAny, "【重大】メッセージ②");
	print(asDetail, forAny, "【詳細】メッセージ②");

	//--------------------
	//ログ属性を変更
	logAttr attr;
	attr.add(logWithID | logWithTime | logWithLevel | logWithCategory);//メッセージにログID,時間,ログレベル名,ログカテゴリ名を表示

	//ログ出力
	print(asNormal, forAny, "【標準】メッセージ③");
	print(asCritical, forAny, "【重要】メッセージ③");
	print(asDetail, forAny, "【詳細】メッセージ③");

	//--------------------
	//独自のログカテゴリを追加
	//※開発者個人用ログカテゴリは、出力先を Visual Studio 出力ウインドウに設定
	{
		//開発者個人用のログ出力先設定
		static vsConsole vs_console("vs-console *custom*");//Visual Studio出力ウインドウオブジェクト
		                                                   //※GCCコンパイル時や開発ツール使用モードじゃない時は、
		                                                   //　TTY端末かWindowsコマンドプロンプトに自動的に変更される
		iConsole* consoles[] = { &vs_console, nullptr };//ログ出力先、画面通知先のセット（nullptrはログレベルの設定に合わせる）
		//iConsole* consoles[] = { &vs_console, &stdDummyConsole::instance() };//ダミーコンソールで画面通知を抑えたい場合（#include <gasha/std_console.h>）
		//iConsole* consoles[] = { &stdMemConsole::instance(), &stdDummyConsole::instance() };//メモリコンソールに出力したい場合（#include <gasha/std_console.h>）

		//カテゴリ追加
		//※カテゴリに名前と出力先を設定する
		regLogCategory<forMiniGame>()("forMiniGame");//ミニゲーム用
		regLogCategory<forTARO>()("forTARO", consoles);//開発者個人用
		regLogCategory<forJIRO>()("forJIRO", consoles);//開発者個人用

		//※ログ出力先は、ログレベルおよびログカテゴリに対して設定可能。
		//　デフォルトでは、ログレベルに対してのみ設定されており、ログカテゴリには未設定。
		//　デフォルトでは、ログレベルの警告（asWarning）と重大（asCritical）に対して、画面通知先も設定。
		//　ログレベルよりもログカテゴリに設定された出力先が優先的に使用される。
		//　既存のログレベル／ログカテゴリの設定も変更可能。（ログレベルごとのカラー設定も変更可能）
	}

	//ログ出力
	print(asNormal, forMiniGame,  "【通常】【ミニゲーム用】メッセージ④");
	print(asNormal, forTARO, "【通常】【開発者個人（太郎）用】メッセージ④");
	print(asNormal, forJIRO, "【通常】【開発者個人（次郎）用】メッセージ④");

	//ログレベルマスクはカテゴリごとに変更可能
	mask.changeLevel(ofLog, asCritical, forMiniGame);
	mask.changeLevel(ofLog, asDetail, forTARO);
	mask.changeLevel(ofLog, asDetail, forJIRO);

	//ログ出力
	print(asDetail, forMiniGame, "【詳細】【ミニゲーム用】メッセージ⑤");
	print(asDetail, forTARO, "【詳細】【開発者個人（太郎）用】メッセージ⑤");
	print(asDetail, forJIRO, "【詳細】【開発者個人（次郎）用】メッセージ⑤");
	print(asCritical, forMiniGame, "【重大】【ミニゲーム用】メッセージ⑥");
	print(asCritical, forTARO, "【重大】【開発者個人（太郎）用】メッセージ⑥");
	print(asCritical, forJIRO, "【重大】【開発者個人（次郎）用】メッセージ⑥");

#ifdef ENABLE_BASIC_ASSERT_TEST
	//--------------------
	//アサーション／ブレークポイント／ウォッチポイント
	//※アサーション／ブレークポイント／ウォッチポイントに対してもログレベルマスクが適用される
	//※ログレベルと無関係のシンプルアサーション／ブレークポイント／ウォッチポイントも別途用意（#include <gasha/simple_assert.h>）
	//※アサーションは、判定式が不一致の時にログ出力してブレークする
	GASHA_ASSERT(asNormal, forMiniGame, 1 + 1 == 3, "【通常】【ミニゲーム用】アサーション①");
	GASHA_ASSERT(asCritical, forMiniGame, 1 + 1 == 3, "【重大】【ミニゲーム用】アサーション②");
	//※ブレークポイントは、無条件にログ出力してブレークする
	GASHA_BREAKPOINT(asNormal, forMiniGame, "【通常】【ミニゲーム用】ブレークポイント①");
	GASHA_BREAKPOINT(asCritical, forMiniGame, "【重大】【ミニゲーム用】ブレークポイント②");
	//※ウォッチポイントは、判定式が一致の時にログ出力してブレークする
	GASHA_WATCHPOINT(asNormal, forMiniGame, 1 + 1 == 2, "【通常】【ミニゲーム用】ウォッチポイント①");
	GASHA_WATCHPOINT(asCritical, forMiniGame, 1 + 1 == 2, "【重大】【ミニゲーム用】ウォッチポイント②");

	//開発環境以外でアサーション違反／ブレークポイント／ウォッチポイント適合時に画面を停止させる場合は、
	//ポーズ処理を登録する
	static stdinDebugPause stdin_dbg_pause;//標準入力デバッグポーズ（エンターキー入力待ち）
	//デバッグポーズを登録
	breakPoint bp;
	bp.changeDebugPause(stdin_dbg_pause);

	//再度アサーション
	//※TTY端末／Windowsコマンドプロンプト上に「Hit enter key ...」と表示され、エンターキーの入力待ちになる
	GASHA_ASSERT(asCritical, forMiniGame, 1 + 1 == 3, "【重大】【ミニゲーム用】アサーション③");
	GASHA_BREAKPOINT(asCritical, forMiniGame, "【重大】【ミニゲーム用】ブレークポイント③");
	GASHA_WATCHPOINT(asCritical, forMiniGame, 1 + 1 == 2, "【重大】【ミニゲーム用】ウォッチポイント③");

	//デバッグポーズをリセット
	bp.resetDebugPause();
#endif//ENABLE_BASIC_ASSERT_TEST

	//ログ属性をリセット
	attr.reset();
	//ログレベルマスクをリセット
	mask.reset();

	//--------------------
	//ログ属性、ログレベルマスクは、一時的に変更することが可能
	//※ローカルで変更して処理ブロック（スコープ）を抜ける時に自動的に戻る。
	//　処理ブロックを抜けるまでは、以降の処理（呼び出した関数内の処理）にも適用される。
	//※他のスレッドには影響がない。
	print(asNormal, forMiniGame, "【通常】【ミニゲーム用】【ローカル処理ブロック前】メッセージ");
	{
		logAttr local_attr;
		logMask local_mask;
		local_attr.changeRef(logAttr::isLocal);//ローカルに変更（デフォルトはグローバルまたはTLS）
		local_mask.changeRef(logMask::isLocal);//ローカルに変更（デフォルトはグローバルまたはTLS）

		local_attr.add(logWithTime | logWithLevel | logWithCategory);//ローカルのログ属性を変更
		
		print(asNormal, forMiniGame, "【通常】【ミニゲーム用】【ローカル処理ブロック中①】メッセージ");
		
		local_mask.changeLevel(ofLog, asCritical, forMiniGame);//ローカルのログレベルマスクを変更
		
		print(asNormal, forMiniGame, "【通常】【ミニゲーム用】【ローカル処理ブロック中②】メッセージ");
	}
	print(asNormal, forMiniGame, "【通常】【ミニゲーム用】【ローカル処理ブロック後】メッセージ");

	//--------------------
	//コールポイント
	//※コールスタックのトレースや、共通処理のログ出力／アサーションを操作するために使用する。
	//※アサーション違反などのブレーク時にコールポイントスタックを表示し、処理の流れを追跡できる。
	//※自動的なプロファイル記録（パフォーマンス計測）にも対応。
	//※【注意】プロファイルを記録するには、事前に threadId クラスでスレッドに名前を付けておく必要があることに注意。
	//　　　　　スレッドに名前が付いていれば、スレッドの生成と破棄を繰り返した場合でも、同じ名前のスレッドの計測を集計する。
	{
		//テストのために一時的にログレベルマスクを変更
		logMask local_mask;
		local_mask.changeRef(logMask::isLocal);//ローカルに変更（デフォルトはグローバルまたはTLS）
		                                       //※同時にローカルがTLSに反映され、以降の処理に適用される
		local_mask.changeLevel(ofLog, asSilent, forAny);//通常カテゴリは非表示
		local_mask.changeLevel(ofLog, asDetail, forMiniGame);//ミニゲームカテゴリは詳細レベルで表示

		//テストのために一時的にデバッグポーズを無効化
		//※元に戻す際は手動で変更する必要あり
		breakPoint bp;
		dummyDebugPause dummy_debug_pause;
		bp.changeDebugPause(dummy_debug_pause);

		{
			//コールポイント設定
			callPoint cp(forAny, "CallPoint", GASHA_CP_ARGS);//カテゴリ、コールポイント名を付ける。GASHA_CP_ARGS はお決まりの指定（ソースファイル名と関数名が渡される）
			
			//共通処理呼び出し
			commonFunc();
			{
				//クリティカルコールポイント設定
				criticalCallPoint cp(forMiniGame, "CP level2(with perform)", callPoint::useAutoProfiling, GASHA_CP_ARGS);//自動プロファイリングを有効
				
				//共通処理呼び出し
				commonFunc();
				{
					//コールポイント設定
					callPoint cp(forFileSystem, "CP level3", GASHA_CP_ARGS);

					//共通処理呼び出し
					commonFunc();
				}
			}
		}
		
		//デバッグポーズを元に戻す
		bp.resetDebugPause();
	}

	//--------------------
	//ログを予約して遅延表示
	//※マルチスレッドで処理の到達順序を正確にログに反映したい時に利用する
	//※ログキューを使用している時にしか使えない
	{
		logAttr attr;
		attr.changeRef(logAttr::isLocal);
		attr.add(logWithID);//IDを表示
		
		logMask mask;
		mask.changeRef(logMask::isLocal);
		mask.changeLevel(ofLog, asNormal, forMiniGame);//ミニゲームのログレベルマスクを変更

		//ログ予約
		debugLog log;
		log.reserve(asNormal, forMiniGame, 2);//指定のログレベルとログカテゴリに対して、メッセージ二つ分を予約

		//ログ予約中の他の処理
		//※他のスレッドの処理が割り込んだものとして通常メッセージを出力
		//※ログIDが予約IDの次のIDになる（後続の print よりもIDが後になり、実際の表示順も後に回される）
		print(asCritical, forFileSystem, "【重大】【ファイルシステム用】予約ログ中の通常メッセージ\n（他のスレッドから出力されたメッセージを想定）\n予約後、予約ログメッセージの出力前に出力しているが、\n予約メッセージの後に出力される");

		//予約したログを遅延出力
		log.reservedPrint("【通常】【ミニゲーム用】予約メッセージ①");//予約出力では、予約時のログレベルとログカテゴリで出力
		log.reservedPrint("【通常】【ミニゲーム用】予約メッセージ②");
		log.reservedPrint("【通常】【ミニゲーム用】予約メッセージ③");//予約の範囲外なので出力されない

		//※予約ログの出力があまりに遅延すると、他のログが待機しきれず、先に出力されてしまう
	}

	//--------------------
	//メッセージ変換付き出力
	//※文字コード変換処理などを指定して出力することが可能
	print(asNormal, forAny, "メッセージ変換機能付き出力（指定なし）：Alphabet Message");
	convPrint(upperCaseConv, asNormal, forAny, "メッセージ変換機能付き出力（大文字化）：Alphabet Message");//単純大文字化 ※二本語が崩れるので注意
	convPrint(lowerCaseConv, asNormal, forAny, "メッセージ変換機能付き出力（小文字化）：Alphabet Message");//単純小文字化 ※（同上）
	//convPrint(utf8toSjis, asNormal, forAny, "メッセージ変換機能付き出力（UTF8 to ShiftJIS）：Alphabet Message");//シフトJIS変換（未実装）

	//※メッセージ変換指定時は、ビルド設定 GASHA_LOG_WORK_BUFF_BLOCK_SIZE の半分を使用して変換を行う。
	//　その為、出力可能なメッセージの長さが短くなることに注意。

	//--------------------
	//プロファイル集計
	//※プロファイルの計測、更新、情報取得は、全てマルチスレッドで競合しないように保護されている。
	//※【注意】プロファイルのクリア処理 profiler::clear() はスレッドセーフじゃないので注意。
	{
		//毎フレーム1回実行することで、そのフレーム内に発生した処理を集計する
		profiler prof;
		//prof.sumup(profiler::isNormal);//通常集計
		prof.sumup(profiler::withUpdatePeriod);//集計と同時に期間集計を反映
		
		//数フレームに1回は withUpdatePeriod を指定し、数フレーム分の集計結果を反映する
		//※例えば、1秒ごとに最長処理時間、平均処理時間、最短処理時間などをリアルタイム表示するような場合に利用できる。
		//※毎フレームのリアルタイムなパフォーマンスを表示したい場合は、withUpdatePeriod を毎フレーム実行すれば良い。
	}
	
	//--------------------
	//プロファイル確認
	{
		//プロファイルを取得
		profiler prof;

		//全スレッド情報取得
		profiler::threadInfo thread_info[10];//スレッド情報の格納領域
		const std::size_t thread_info_num = prof.getThreadInfo(thread_info);

		//スレッドごとのプロファイル情報を取得
		for (std::size_t i = 0; i < thread_info_num; ++i)
		{
			const profiler::threadInfo&  thread_info_tmp = thread_info[i];
			print(asNormal, forAny, "----------------------------------------\n");
			print(asNormal, forAny, "thread: \"%s\"\n", thread_info_tmp.threadName());

			profiler::profileInfo profile_info[10];//プロファイル情報の格納領域
			const std::size_t profile_info_num = prof.getProfileInfo(thread_info_tmp, profile_info);
			for (std::size_t j = 0; j < profile_info_num; ++j)
			{
				const profiler::profileInfo&  profile_info_tmp = profile_info[i];
				const profiler::summarizedTimeInfo& period = profile_info_tmp.periodicTime();
				print(asNormal, forAny, "  profile: \"%s\", max=%.6lf, min=%.6lf, avg=%.6lf\n", profile_info_tmp.name(), period.maxTime(), period.minTime(), period.avgTime());
			}
		}
	}

	//--------------------
	//ログキューモニタースレッド終了
	{
		debugLog log;
		log.flush();//ログ出力完了待ち
		log.abort();//ログキューモニター中断
	}
	
	//ログキューモニタースレッド終了待ち
	waitLogQueueMonitor();

	//--------------------
	//ログ情報初期化
	{
		debugLog log;
		log.initialize();
	}

	std::printf("\n");
	std::printf("================================================================================\n");
}

//--------------------
//コールポイントテスト用共通処理
static void commonFunc()
{
	logAttr attr;
	attr.changeRef(logAttr::isLocal);
	attr.add(logWithLevel | logWithCategory);

	//直近のコールポイントのカテゴリとして出力
	//※なければ forAny 扱い
	print(asNormal, forCallPoint, "【通常】【コールポイント用】メッセージ");

	//直近のクリティカルコールポイントのカテゴリとして出力
	//※なければ forAny 扱い
	print(asNormal, forCriticalCallPoint, "【通常】【クリティカルコールポイント用】メッセージ");

	//アサーション
	//※コールポイントスタックも表示される
	GASHA_ASSERT(asImportant, forCriticalCallPoint, 1 + 1 == 3, "【重要】【クリティカルコールポイント用】アサーション");
}

// End of file
