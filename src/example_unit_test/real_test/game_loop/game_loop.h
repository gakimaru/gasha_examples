#pragma once
#ifndef GASHA_INCLUDED_GAME_LOOP_H
#define GASHA_INCLUDED_GAME_LOOP_H

//--------------------------------------------------------------------------------
// game_loop.h
// ユニットテスト：ゲームループ
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//ゲームループクラス
class gameLoop
{
public:
	//初期化処理
	int initialize();
	
	//終了処理
	int finalize();

	//メイン処理
	int main();

public:
	//ムーブコンストラクタ
	gameLoop(gameLoop&&) = delete;
	//コピーコンストラクタ
	gameLoop(const gameLoop&) = delete;
public:
	//コンストラクタ
	gameLoop(const int argc, const char* argv[]);
	//デフォルトコンストラクタ
	gameLoop() = delete;
	//デストラクタ
	~gameLoop();

public:
	//キャストオペレータ
	inline operator int() const { return resultOfMain(); }

public:
	//アクセッサ
	inline int argc() const { return m_argc; }
//	inline const char** argv() const { return m_argv; }
	inline const char* argv(const int index) const { return index >= 0 && index < m_argc ? m_argv[index] : nullptr; }
	inline int resultOfInitialize() const { return m_resultOfInitialize; }
	inline int resultOfFinalize() const { return m_resultOfFinalize; }
	inline int resultOfMain() const { return m_resultOfMain; }

private:
	//実行時パラメータ
	const int m_argc;
	const char** m_argv;
	
	//処理結果
	int m_resultOfInitialize;
	int m_resultOfFinalize;
	int m_resultOfMain;
};

#endif//GASHA_INCLUDED_GAME_LOOP_H

// End of file
