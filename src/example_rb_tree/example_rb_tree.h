#pragma once
#ifndef __EXAMPLE_RB_TREE_H_
#define __EXAMPLE_RB_TREE_H_

//--------------------------------------------------------------------------------
// exmaple_rb_tree.h
// 赤黒木テスト
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//【謝辞】
//このプログラムのアルゴリズムは、下記の個人サイトにて発表されております、
//サイト主様が開発されたアルゴリズムを参考にいたしました。
//    URL： http ://www.moon.sannet.ne.jp/okahisa/rb-tree/rb-tree.html
//    記事：「Red - Black Tree by Java -- これで分かった赤黒木」
//          トップページURL : http ://www.moon.sannet.ne.jp/okahisa/
//                           「OKおじさんのホームページ」
//とても分かり易く優れたアルゴリズムであり、問題なくプログラムを作成でき
//ましたことを感謝いたします。
//なお、ソースコードは引用しておりません。
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//赤黒木テスト用設定とコンパイラスイッチ
static const int TEST_DATA_KEY_MIN = 1;//テストデータの最小キー
static const int TEST_DATA_KEY_MAX = 10;//テストデータの最大キー
static const int TEST_DATA_REG_NUM = 20;//テストデータの登録数
static const int TEST_DATA_STACK_DEPTH_MAX = 32;//テストデータの赤黒木操作用スタックの最大の深さ（デフォルトは32で、平衡木なら10万件は扱える）
//#define REGIST_TEST_DATA_SEQUENTIALLY//データをシーケンシャルに登録する場合は、このマクロを有効化する（無効化時はランダム）
#define PRINT_TEST_DATA_TREE//テストデータの木を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_SEARCH//テストデーたの検索結果を表示する場合は、このマクロを有効化する（表示しなくても検索は実行する）
#define PRINT_TEST_DATA_COLOR_COUNT//テストデータの赤黒カウント数計測を表示する場合は、このマクロを有効化する
#define PRINT_TEST_DATA_DETAIL//テストデータの詳細を表示する場合は、このマクロを有効化する
//#define ENABLE_CALC_COUNT_PERFORMANCE//データ件数カウントの処理時間を計測する場合は、このマクロを有効化する

//テストデータを固定順に登録する場合は、このマクロを有効化する（無効時はランダム、ただし、REGIST_TEST_DATA_SEQUENTIALLYが優先）
//#define TEST_DATA_REGISTRATION_LIST { 54, 59, 71, 84, 60, 85, 54, 84, 42, 62, 64, 38, 43, 29, 89, 5, 96, 27, 38, 47, 79, 81, 52, 47, 56, 39, 92, 83, 7, 33, 8, 64, 2, 36, 83, 95, 77, 14, 87, 87, 97, 47, 79, 80, 46, 52, 78, 67, 11, 72, 63, 58, 14, 53, 94, 75, 52, 10, 41, 47, 26, 18, 77, 73, 45, 21, 56, 13, 1, 32, 61, 14, 61, 22, 61, 38, 94, 90, 68, 44, 35, 61, 43, 90, 69, 9, 6, 96, 66, 65, 67, 17, 21, 35, 12, 75, 31, 60, 36, 32}

//テストデータを固定順に削除する場合は、このマクロを有効化する（無効時はランダム）
//#define TEST_DATA_REMOVING_LIST { 41, 72, 12, 14, 9, 39, 18, 38, 66, 53, 84, 31, 68, 52, 44, 87 }

//--------------------------------------------------------------------------------
//赤黒木アルゴリズム用コンパイラスイッチ
//#define DISABLE_COLOR_FOR_ADD//ノード追加時の色操作と回転処理を無効化する場合は、このマクロを有効化する
//#define DISABLE_COLOR_FOR_REMOVE//ノード削除時の色操作と回転処理を無効化する場合は、このマクロを有効化する
//#define DEBUG_PRINT_FOR_ADD//ノード追加時のデバッグ情報表示を有効化する場合は、このマクロを有効化する
//#define DEBUG_PRINT_FOR_REMOVE//ノード削除時のデバッグ情報表示を有効化する場合は、このマクロを有効化する


//赤黒木テスト
void example_rb_tree();

#endif//__EXAMPLE_RB_TREE_H_

// End of file
