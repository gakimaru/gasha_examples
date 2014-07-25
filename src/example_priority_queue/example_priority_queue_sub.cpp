//--------------------------------------------------------------------------------
// exmaple_priority_queue_sub.cpp
// 優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//
// Gakimaru's standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include "example_priority_queue.h"//優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト

#include <gasha/binary_heap.cpp.h>//二分ヒープコンテナ【関数定義部】
#include <gasha/priority_queue.cpp.h>//優先度付きキューコンテナアダプタ【関数定義部】

GASHA_USING_NAMESPACE;//ネームスペース使用

//----------------------------------------
//テンプレートの明示的なインスタンス化

//※コンパイル速度の劣化や、ソース改変時の広範囲な再コンパイルの影響を抑えるために、
//　コンテナのヘッダーファイルを三つに分割。
//　　・*.h     ... 宣言部                                （クラスの宣言が必要な場所でインクルード）
//　　・*.inl   ... インライン関数／テンプレート関数定義部（クラスの操作が必要な場所でインクルード）
//　　・*.cpp.h ... 関数定義部                            （クラスの実体化が必要な場所でインクルード）
//※わざわざ分けて書くのが面倒なら、クラスの操作が必要な場所で
//　このヘッダーファイルをインクルードすれば、明示的なインスタンス化は
//　必要ない。
//　（コンパイルへの影響を気にしないなら、ヘッダーファイルに
//　インクルードしてしまっても良い）

//明示的インスタンス化
//※専用マクロを使用
GASHA_INSTANCING_pQueue_withBHeap(ope, TEST_DATA_MAX);//template class priority_queue::container<ope, TEST_DATA_MAX>; と同じ
GASHA_INSTANCING_pQueue_withBHeap(ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER);//template class priority_queue::container<ptr_ope, TEST_DATA_TABLE_SIZE_FOR_POINTER>; と同じ
GASHA_INSTANCING_bHeap(heap_ope, TEST_DATA_MAX);//template class binary_heap::container<heap_ope, TEST_DATA_MAX>; と同じ

//----------------------------------------
//シンプル優先度付きキューコンテナアダプタ／二分ヒープコンテナテスト
//※.inl と .cpp.h をインクルードした後は、明示的なインスタンス化をせずにコンテナを使用可能。
//※シンプルコンテナを使用すると、コンテナ操作用構造体の定義も省略可能。
//　ただし、ソート用関数や探索用関数の定義、ロックオブジェクトの指定といった細かいカスタマイズはできない。
//　シンプルコンテナも明示的なインスタンス化は可能。

#include <cstdio>//std::printf()

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <algorithm>//std::find(), std::binary_search(), std::lower_bound()
#pragma warning(pop)//【VC++】ワーニング設定を復元

//シンプル優先度付きキューコンテナアダプタテスト
void example_simple_priority_queue()
{
	std::printf("\n");
	std::printf("--- example_simple_priority_queue ---\n");

	{
		simplePQueue<short, 100>::con con;//シンプル優先度付きキューコンテナアダプタ
		con.enqueue(5, 15);
		con.enqueue(1, 11);
		con.enqueue(3, 13);
		auto print = [&con]()
		{
			std::printf("data =");
			simplePQueue<short, 100>::node node;
			while (con.dequeueCopying(node))
			{
				std::printf(" %d", node.m_value);
			}
			std::printf("\n");
		};
		print();
	}
	{
		//ローカルクラス（関数内クラス）を使うと、明示的なインスタンス化ができない点に注意
		struct data_t
		{
			int m_val1;
			int m_val2;
			data_t(const int val) :
				m_val1(val / 10),
				m_val2(val % 10)
			{}
			data_t(const int val1, const int val2) :
				m_val1(val1),
				m_val2(val2)
			{}
			data_t() :
				m_val1(0),
				m_val2(0){}
		};
		typedef simplePQueue<data_t, 100> con_t;
		con_t::con con;//シンプル動的配列コンテナ
		con.enqueue(5, 56);
		con.enqueue(1, 1, 2);
		con.enqueue(3, 34);
		auto print = [&con]()
		{
			std::printf("data =");
			con_t::node node;
			while (con.dequeueCopying(node))
			{
				std::printf(" {%d, {%d,%d}}", node.m_priority, node->m_val1, node->m_val2);
			}
			std::printf("\n");
		};
		print();
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
GASHA_INSTANCING_simplePQueue(short, 100);
//GASHA_INSTANCING_simplePQueue(data_t, 100);//ローカルクラス（関数内クラス）を使ったものは、明示的なインスタンス化ができない

//二分ヒープコンテナテスト
void example_simple_binary_heap()
{
	std::printf("\n");
	std::printf("--- example_simple_binary_heap ---\n");

	{
		simpleBHeap<short, 100>::con con;//シンプル優先度付きキューコンテナアダプタ
		con.push(5);
		con.push(1);
		con.push(3);
		auto print = [&con]()
		{
			std::printf("data =");
			short node;
			while (con.popCopying(node))
			{
				std::printf(" %d", node);
			}
			std::printf("\n");
		};
		print();
	}
	{
		//ローカルクラス（関数内クラス）を使うと、明示的なインスタンス化ができない点に注意
		struct data_t
		{
			int m_val1;
			int m_val2;
			bool operator<(const data_t& rhs) const { return m_val1 < rhs.m_val1; }//upHeap(), downHeap()用
			data_t(const int val) :
				m_val1(val / 10),
				m_val2(val % 10)
			{}
			data_t(const int val1, const int val2) :
				m_val1(val1),
				m_val2(val2)
			{}
			data_t() :
				m_val1(0),
				m_val2(0){}
		};
		typedef simpleBHeap<data_t, 100> con_t;
		con_t::con con;//シンプル動的配列コンテナ
		con.push(56);
		con.push(1, 2);
		con.push(34);
		auto print = [&con]()
		{
			std::printf("data =");
			data_t node;
			while (con.popCopying(node))
			{
				std::printf(" {%d,%d}", node.m_val1, node.m_val2);
			}
			std::printf("\n");
		};
		print();
	}
}
//明示的インスタンス化する場合
//※専用マクロ使用
GASHA_INSTANCING_simpleBHeap(short, 100);
//GASHA_INSTANCING_simpleBHeap(data_t, 100);//ローカルクラス（関数内クラス）を使ったものは、明示的なインスタンス化ができない

// End of file
