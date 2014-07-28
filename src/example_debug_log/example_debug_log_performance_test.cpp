//--------------------------------------------------------------------------------
// exmaple_debug_log_perfomance_test.cpp
// デバッグログパフォーマンステスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_debug_log.h"//デバッグログテスト

#include <gasha/print.h>//ログ出力操作
#include <gasha/debug_log.h>//ログ操作
#include <gasha/log_attr.h>//ログ属性
#include <gasha/log_mask.h>//ログレベルマスク
#include <gasha/call_point.h>//コールポイント
#include <gasha/profiler.h>//プロファイラー
#include <gasha/thread_id.h>//スレッドID
#include <gasha/lock_common.h>//ロック共通設定

#include <gasha/string.h>//文字列処理

#include <atomic>//C++11 std::atomic
#include <condition_variable>//C++11 std::condition_variable
#include <random>//C++11 std::random

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <thread>//C++11 std::thread
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//デバッグログパフォーマンステスト
void example_debugLog_perfomanceTest()
{
	std::printf("\n");
	std::printf("================================================================================\n");

	std::printf("\n");
	std::printf("--------------------------------------------------------------------------------\n");
	std::printf("[ Prformance test for debug-log ]\n");
	std::printf("--------------------------------------------------------------------------------\n");
	
	//--------------------
	//ログ情報初期化
	{
		debugLog log;
		log.initialize();
	}

	//ログキューモニターを起動
	beginLogQueueMonitor();

	//多数のスレッドで一斉ログ出力
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Many thread printing\n");
		std::printf("\n");

		//ログ出力属性をリセット（グローバル）
		logAttr attr;
		attr.reset();
		attr.add(logWithID|logHeaderOnlyColored);//IDを表示,ヘッダーのみ着色

		//ログレベルマスクをリセット（グローバル）
		logMask mask;
		mask.reset();

		//ログ出力スレッド
		std::atomic<bool> start;//一斉スタート用
		std::atomic<int> start_count(0);//一斉スタート用
		std::mutex mutex;//一斉スタート用
		std::condition_variable cond;//一斉スタート用
		auto pred = [&start]() -> bool { return start.load(); };//一斉スタート用
		auto logging_thread = [&start_count, &mutex, &cond, &pred](const int thread_no)
		{
			char thread_name[32];
			spprintf(thread_name, "THREAD[%02d]", thread_no);
			threadId thread_id(thread_name);

			//スタート待ち
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, pred);
			}
			start_count.fetch_add(1);

			//ログ出力
			debugLog log;
			for (int i = 0; i < 10; ++i)
			{
				log.reserve(asImportant, forAny, 1);//ログ予約
				defaultContextSwitch();
				print(asImportant, forAny, "Normal-message(%02d) on %s", i, thread_name);//通常ログ
				defaultContextSwitch();
				log.reservedPrint("Reserved-message(%02d) on %s", i, thread_name);//予約ログ
				defaultContextSwitch();
			}
		};
		//複数のスレッドで同時実行
		static const int threads = 10;
		std::thread* th[threads];
		for (int thread_no = 0; thread_no < threads; ++thread_no)
			th[thread_no] = new std::thread(logging_thread, thread_no);
		//処理時間計測開始
		elapsedTime elapsed_time;
		//一斉スタート
		start.store(true);
		while (start_count.load() < threads)
			cond.notify_all();
		//スレッド終了待ち
		for (int thread_no = 0; thread_no < threads; ++thread_no)
			th[thread_no]->join();
		//出力を終えた時点の処理時間
		const sec_t output_time = elapsed_time.now();
		//出力完了待ち
		debugLog log;
		log.flush();
		//ログモニターのフラッシュを終えた時点の処理時間
		const sec_t flush_time = elapsed_time.now();
		//スレッド破棄
		for (int thread_no = 0; thread_no < threads; ++thread_no)
			delete th[thread_no];
		log.flush();//ログ出力完了待ち
		//処理時間表示
		std::printf("*elapsed time: output=%.9lf sec, flush=%.9lf sec\n", output_time, flush_time);
		std::printf("--------------------------------------------------------------------------------\n");
	}

	//多数のスレッドでプロファイリング①（集計結果の確認用）
	{
		//プロファイルのクリア
		//※【注意】非スレッドセーフ処理
		profiler prof;
		prof.clear();
		//メインスレッドでダミー計測
		prof.sumup(profiler::withUpdatePeriod);
	}
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Many thread profiling(1) : 1+%d threads, %d frames, %d processes, %d repeats, sum on %d frames\n", PROFILE_TEST1_THREADS, PROFILE_TEST1_FRAMES, PROFILE_TEST1_PROCESSES, PROFILE_TEST1_REPEATS, PROFILE_TEST1_SUM_FRAMES);
		std::printf("\n");
		std::atomic<bool> start(false);//一斉スタート用
		std::atomic<int> start_count(0);//一斉スタート用
		std::mutex mutex;//一斉スタート用
		std::condition_variable cond;//一斉スタート用
		auto pred = [&start]() -> bool { return start.load(); };//一斉スタート用
		auto logging_thread = [&start_count, &mutex, &cond, &pred](const int thread_no, const int seed)
		{
			char thread_name[32];
			GASHA_ spprintf(thread_name, "THREAD[%02d]", thread_no);
			threadId thread_id(thread_name);

			//乱数初期化
			std::mt19937 rnd_engine(seed);
			std::uniform_int_distribution<int> rnd_dist(PROFILE_TEST1_TIME_MIN, PROFILE_TEST1_TIME_MAX);

			//スタート待ち
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, pred);
			}
			start_count.fetch_add(1);

			//PROFILE_TEST1_FRAMES フレームの計測
			for (int frame = 0; frame < PROFILE_TEST1_FRAMES; ++frame)
			{
				//1フレーム当たりPROFILE_TEST1_PROCESSES個×PROFILE_TEST1_REPEATSのパフォーマンスを計測
				for (int repeat = 0; repeat < PROFILE_TEST1_REPEATS; ++repeat)
				{
					for (int proc = 0; proc < PROFILE_TEST1_PROCESSES; ++proc)
					{
						char cp_name[32];
						GASHA_ spprintf(cp_name, "PROC[%02d]", proc);
						callPoint cp(forAny, cp_name, callPoint::useAutoProfiling, GASHA_CP_ARGS);
						//乱数で処理時間分のスリープ
						std::this_thread::sleep_for(std::chrono::microseconds(rnd_dist(rnd_engine)));
					}
				}
				//集計
				//※PROFILE_TEST1_SUM_FRAMESフレームに一回「期間集計」の反映
				profiler prof;
				profiler::profileSumup_type sumup_type = ((frame % PROFILE_TEST1_SUM_FRAMES) == (PROFILE_TEST1_SUM_FRAMES - 1)) ? profiler::withUpdatePeriod : profiler::isNormal;
				prof.sumup(sumup_type);
			}
		};
		//乱数初期化
		std::random_device rd;
		std::mt19937 rnd_engine(rd());
		std::uniform_int_distribution<int> rnd_dist(PROFILE_TEST1_TIME_MIN, PROFILE_TEST1_TIME_MAX);
		//複数のスレッドで同時実行
		std::thread* th[PROFILE_TEST1_THREADS];
		for (int thread_no = 0; thread_no <PROFILE_TEST1_THREADS; ++thread_no)
			th[thread_no] = new std::thread(logging_thread, thread_no, rd());
		//処理時間計測開始
		elapsedTime elapsed_time;
		//一斉スタート
		start.store(true);
		while (start_count.load() < PROFILE_TEST1_THREADS)
			cond.notify_all();
		//メインスレッドの処理計測：PROFILE_TEST1_FRAMESフレームの計測
		for (int frame = 0; frame < PROFILE_TEST1_FRAMES; ++frame)
		{
			//1フレーム当たりPROFILE_TEST1_PROCESSES個×PROFILE_TEST1_REPEATSのパフォーマンスを計測
			for (int repeat = 0; repeat < PROFILE_TEST1_REPEATS; ++repeat)
			{
				for (int proc = 0; proc < PROFILE_TEST1_PROCESSES; ++proc)
				{
					char cp_name[32];
					GASHA_ spprintf(cp_name, "MAIN-PROC[%02d]", proc);
					callPoint cp(forAny, cp_name, callPoint::useAutoProfiling, GASHA_CP_ARGS);
					//乱数で処理時間分のスリープ
					std::this_thread::sleep_for(std::chrono::microseconds(rnd_dist(rnd_engine)));
				}
			}
			//集計
			//※30フレームに一回「期間集計」の反映
			profiler prof;
			profiler::profileSumup_type sumup_type = ((frame % PROFILE_TEST1_SUM_FRAMES) == (PROFILE_TEST1_SUM_FRAMES - 1)) ? profiler::withUpdatePeriod : profiler::isNormal;
			prof.sumup(sumup_type);
		}
		//スレッド終了待ち
		for (int thread_no = 0; thread_no < PROFILE_TEST1_THREADS; ++thread_no)
			th[thread_no]->join();
		//処理を終えた時点の処理時間
		const sec_t finish_time = elapsed_time.now();
		//スレッド破棄
		for (int thread_no = 0; thread_no < PROFILE_TEST1_THREADS; ++thread_no)
			delete th[thread_no];
		debugLog log;
		log.flush();//ログ出力完了待ち
		std::printf("*elapsed time: finish=%.9lf sec\n", finish_time);
		std::printf("--------------------------------------------------------------------------------\n");
		//プロファイル確認
		{
			logAttr attr;
			attr.reset();

			//プロファイルを取得
			profiler prof;

			//全スレッド情報取得
			profiler::threadInfo thread_info[10];//スレッド情報の格納領域
			const std::size_t thread_info_num = prof.getThreadInfo(thread_info);

			//スレッドごとのプロファイル情報を取得
			for (std::size_t i = 0; i < thread_info_num; ++i)
			{
				const profiler::threadInfo&  thread_info_tmp = thread_info[i];
				std::printf("----------------------------------------\n");
				std::printf("THREAD:\"%s\"\n", thread_info_tmp.threadName());

				profiler::profileInfo profile_info[10];//プロファイル情報の格納領域
				const std::size_t profile_info_num = prof.getProfileInfo(thread_info_tmp, profile_info);
				for (std::size_t j = 0; j < profile_info_num; ++j)
				{
					const profiler::profileInfo&  profile_info_tmp = profile_info[j];
					const profiler::summarizedTimeInfo& period = profile_info_tmp.periodicTime();
					const profiler::summarizedTimeInfo& total = profile_info_tmp.totalTime();
					std::printf("  PROFILE:\"%s\"\n", profile_info_tmp.name());
					std::printf("    PERIOD:sums=%d,count=%d(max=%d,min=%d,avg=%.2f),time=%.6lf(max=%.6lf,min=%.6lf,avg=%.6lf)\n", period.summarizedCount(), period.count(), period.maxCount(), period.minCount(), period.avgCount(), period.time(), period.maxTime(), period.minTime(), period.avgTime());
					std::printf("    TOTAL: sums=%d,count=%d(max=%d,min=%d,avg=%.2f),time=%.6lf(max=%.6lf,min=%.6lf,avg=%.6lf)\n", total.summarizedCount(), total.count(), total.maxCount(), total.minCount(), total.avgCount(), total.time(), total.maxTime(), total.minTime(), total.avgTime());
				}
			}
		}
	}

	//多数のスレッドでプロファイリング②（処理時間の確認用）
	{
		//プロファイルのクリア
		//※【注意】非スレッドセーフ処理
		profiler prof;
		prof.clear();
		//メインスレッドでダミー計測
		prof.sumup(profiler::withUpdatePeriod);
	}
	{
		std::printf("\n");
		std::printf("--------------------------------------------------------------------------------\n");
		std::printf("Many thread profiling(2) : 1+%d threads, %d frames, %d processes, %d repeats, sum on %d frames\n", PROFILE_TEST2_THREADS, PROFILE_TEST2_FRAMES, PROFILE_TEST2_PROCESSES, PROFILE_TEST2_REPEATS, PROFILE_TEST2_SUM_FRAMES);
		std::printf("\n");
		std::atomic<bool> start(false);//一斉スタート用
		std::atomic<int> start_count(0);//一斉スタート用
		std::mutex mutex;//一斉スタート用
		std::condition_variable cond;//一斉スタート用
		auto pred = [&start]() -> bool { return start.load(); };//一斉スタート用
		auto logging_thread = [&start_count, &mutex, &cond, &pred](const int thread_no)
		{
			char thread_name[32];
			GASHA_ spprintf(thread_name, "THREAD[%02d]", thread_no);//※名前の重複したスレッドの集計をテストする場合、 thread_no / 2 などの値を用いる
			threadId thread_id(thread_name);

			//スタート待ち
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, pred);
			}
			start_count.fetch_add(1);

			//PROFILE_TEST2_FRAMES フレームの計測
			for (int frame = 0; frame < PROFILE_TEST2_FRAMES; ++frame)
			{
				//1フレーム当たりPROFILE_TEST2_PROCESSES個×PROFILE_TEST2_REPEATSのパフォーマンスを計測
				for (int repeat = 0; repeat < PROFILE_TEST2_REPEATS; ++repeat)
				{
					for (int proc = 0; proc < PROFILE_TEST2_PROCESSES; ++proc)
					{
						char cp_name[32];
						GASHA_ spprintf(cp_name, "PROC[%02d]", proc);
						callPoint cp(forAny, cp_name, callPoint::useAutoProfiling, GASHA_CP_ARGS);
					}
				}
				//集計
				//※PROFILE_TEST2_SUM_FRAMESフレームに一回「期間集計」の反映
				profiler prof;
				profiler::profileSumup_type sumup_type = ((frame % PROFILE_TEST2_SUM_FRAMES) == (PROFILE_TEST2_SUM_FRAMES - 1)) ? profiler::withUpdatePeriod : profiler::isNormal;
				prof.sumup(sumup_type);
			}
		};
		//複数のスレッドで同時実行
		std::thread* th[PROFILE_TEST2_THREADS];
		for (int thread_no = 0; thread_no <PROFILE_TEST2_THREADS; ++thread_no)
			th[thread_no] = new std::thread(logging_thread, thread_no);
		//処理時間計測開始
		elapsedTime elapsed_time;
		//一斉スタート
		start.store(true);
		while (start_count.load() < PROFILE_TEST2_THREADS)
			cond.notify_all();
		//メインスレッドの処理計測：PROFILE_TEST2_FRAMESフレームの計測
		for (int frame = 0; frame < PROFILE_TEST2_FRAMES; ++frame)
		{
			//1フレーム当たりPROFILE_TEST2_PROCESSES個×PROFILE_TEST2_REPEATSのパフォーマンスを計測
			for (int repeat = 0; repeat < PROFILE_TEST2_REPEATS; ++repeat)
			{
				for (int proc = 0; proc < PROFILE_TEST2_PROCESSES; ++proc)
				{
					char cp_name[32];
					GASHA_ spprintf(cp_name, "MAIN-PROC[%02d]", proc);
					callPoint cp(forAny, cp_name, callPoint::useAutoProfiling, GASHA_CP_ARGS);
				}
			}
			//集計
			//※30フレームに一回「期間集計」の反映
			profiler prof;
			profiler::profileSumup_type sumup_type = ((frame % PROFILE_TEST2_SUM_FRAMES) == (PROFILE_TEST2_SUM_FRAMES - 1)) ? profiler::withUpdatePeriod : profiler::isNormal;
			prof.sumup(sumup_type);
		}
		//スレッド終了待ち
		for (int thread_no = 0; thread_no < PROFILE_TEST2_THREADS; ++thread_no)
			th[thread_no]->join();
		//処理を終えた時点の処理時間
		const sec_t finish_time = elapsed_time.now();
		//スレッド破棄
		for (int thread_no = 0; thread_no < PROFILE_TEST2_THREADS; ++thread_no)
			delete th[thread_no];
		debugLog log;
		log.flush();//ログ出力完了待ち
		std::printf("*elapsed time: finish=%.9lf sec\n", finish_time);
		std::printf("--------------------------------------------------------------------------------\n");
		//プロファイル確認
		{
			logAttr attr;
			attr.reset();

			//プロファイルを取得
			profiler prof;

			//全スレッド情報取得
			profiler::threadInfo thread_info[20];//スレッド情報の格納領域
			const std::size_t thread_info_num = prof.getThreadInfo(thread_info);
			std::size_t profile_info_num_total = 0;
			std::size_t summarized_count_total = 0;
			std::size_t count_total = 0;
			sec_t time_total = 0;
			std::size_t summarized_count_period = 0;
			std::size_t count_period = 0;
			sec_t time_period = 0;

			//スレッドごとのプロファイル情報を取得
			for (std::size_t i = 0; i < thread_info_num; ++i)
			{
				const profiler::threadInfo&  thread_info_tmp = thread_info[i];
				
				profiler::profileInfo profile_info[100];//プロファイル情報の格納領域
				const std::size_t profile_info_num = prof.getProfileInfo(thread_info_tmp, profile_info);
				profile_info_num_total += profile_info_num;
				for (std::size_t j = 0; j < profile_info_num; ++j)
				{
					const profiler::profileInfo&  profile_info_tmp = profile_info[j];
					const profiler::summarizedTimeInfo& total = profile_info_tmp.totalTime();
					summarized_count_total += total.summarizedCount();
					count_total += total.count();
					time_total += total.time();
					const profiler::summarizedTimeInfo& period = profile_info_tmp.periodicTime();
					summarized_count_period += period.summarizedCount();
					count_period += period.count();
					time_period += period.time();
				}
			}

			std::printf("thread-infos=%d,processe-infos=%d,total:(sums=%d,counts=%d,time=%.9lf),period:(sums=%d,counts=%d,time=%.9lf)\n", static_cast<int>(thread_info_num), static_cast<int>(profile_info_num_total), static_cast<int>(summarized_count_total), static_cast<int>(count_total), static_cast<double>(time_total), static_cast<int>(summarized_count_period), static_cast<int>(count_period), static_cast<double>(time_period));
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

// End of file
