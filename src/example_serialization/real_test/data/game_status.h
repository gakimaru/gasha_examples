#pragma once
#ifndef GASHA_INCLUDED_GAME_STATUS_H
#define GASHA_INCLUDED_GAME_STATUS_H

//--------------------------------------------------------------------------------
// game_status.h
// ゲーム状態【宣言部】
//
// Gakimaru's researched and standard library for C++ - GASHA
//   Copyright (c) 2014 Itagaki Mamoru
//   Released under the MIT license.
//     https://github.com/gakimaru/gasha_examples/blob/master/LICENSE
//--------------------------------------------------------------------------------

#include <gasha/hash_table.h>//ハッシュテーブル
#include <gasha/rb_tree.h>//赤黒木
#include <gasha/stack_allocator.h>//スタックアロケータ
#include <gasha/crc32.h>//CRC32計算
#include <gasha/serialization/friends.h>//シリアライズ/フレンド宣言

#include <cstddef>//std::size_t
#include <cstdint>//C++11 std::uint32_t

#pragma warning(push)//【VC++】ワーニング設定を退避
#pragma warning(disable: 4530)//【VC++】C4530を抑える
#include <bitset>//std::bitset
#pragma warning(pop)//【VC++】ワーニング設定を復元

GASHA_USING_NAMESPACE;//ネームスペース使用

//--------------------
//ゲーム状態
class gameStatus
{
	//シリアライズ用のフレンド設定
	GASHA_SERIALIZATION_FRIENDS();
public:
	//定数
	static const std::size_t FLAG_SET_SIZE = 64;//フラグ数
	static const std::size_t FLAG_NAME_POOL_SIZE = FLAG_SET_SIZE * 15 / 10;//フラグ名のプールサイズ
	static const std::size_t FLAG_NAME_MAX_SIZE = 32;//フラグ名（一つあたりの）最大サイズ
	static const std::size_t FLAG_NAME_MAX_LEN = FLAG_NAME_MAX_SIZE - 1;//フラグ名（一つあたりの）最大長
	static const std::size_t FLAG_NAME_BUFF_SIZE = FLAG_SET_SIZE * FLAG_NAME_MAX_SIZE;//フラグ名（全部の）バッファサイズ
public:
	//型
	typedef int index_type;//インデックス型
	static const index_type INVALID_INDEX = -1;//無効なインデックス
	struct flagNameInfo
	{
		const char* m_name;//フラグ名
		crc32_t m_nameCrc;//フラグ名のCRC
		index_type m_index;//インデックス
		mutable const flagNameInfo* m_childS;//赤黒木用小（左）側の子ノード
		mutable const flagNameInfo* m_childL;//赤黒木用大（右）側の子ノード
		mutable bool m_isBlack;//赤黒木用色情報
		//コンストラクタ
		inline flagNameInfo(const char* name, const crc32_t name_crc, const index_type index);
	};
	typedef std::bitset<FLAG_SET_SIZE> flagSet_type;//フラグセット型
	struct flagNameTableOpe : public hash_table::baseOpe<flagNameTableOpe, flagNameInfo, crc32_t>//フラグ名テーブル操作型
	{};
	struct flagIndexTableOpe : public rb_tree::baseOpe<flagIndexTableOpe, flagNameInfo, index_type>//フラグインデックステーブル操作型
	{
		//子ノードを取得
		inline static const node_type* getChildL(const node_type& node){ return node.m_childL; }//大（右）側
		inline static const node_type* getChildS(const node_type& node){ return node.m_childS; }//小（左）側
		//子ノードを変更
		inline static void setChildL(node_type& node, const node_type* child){ node.m_childL = child; }//大（右）側
		inline static void setChildS(node_type& node, const node_type* child){ node.m_childS = child; }//小（左）側

		//ノードの色を取得
		inline static color_t getColor(const node_type& node){ return node.m_isBlack ? rb_tree::BLACK : rb_tree::RED; }
		//ノードの色を変更
		inline static void setColor(node_type& node, const color_t color){ node.m_isBlack = (color == rb_tree::BLACK); }

		//キーを取得
		inline static key_type getKey(const node_type& node){ return node.m_index; }
	};
	typedef hash_table::container<flagNameTableOpe, FLAG_NAME_POOL_SIZE> flagNameTable_type;//フラグ名テーブル型
	typedef rb_tree::container<flagIndexTableOpe> flagIndexTable_type;//フラグインデックステーブル型
	typedef stackAllocator_withBuff<FLAG_NAME_BUFF_SIZE> m_flagNameBuff_type;//フラグ名バッファ型
	typedef flagIndexTable_type::iterator iterator;//イテレータ型
	typedef flagIndexTable_type::const_iterator const_iterator;//イテレータ型
	typedef flagIndexTable_type::reverse_iterator reverse_iterator;//リバースイテレータ型
	typedef flagIndexTable_type::const_reverse_iterator const_reverse_iterator;//リバースイテレータ型
public:
	//フラグ用のイテレータ取得
	inline const iterator begin() const;//開始イテレータ
	inline const iterator end() const;//終了イテレータ
	inline iterator begin();//開始イテレータ
	inline iterator end();//終了イテレータ
	inline const_iterator cbegin() const;//開始イテレータ
	inline const_iterator cend() const;//終了イテレータ
	inline const reverse_iterator rbegin() const;//開始リバースイテレータ
	inline const reverse_iterator rend() const;//終了リバースイテレータ
	inline reverse_iterator rbegin();//開始リバースイテレータ
	inline reverse_iterator rend();//終了リバースイテレータ
	inline const_reverse_iterator crbegin() const;//開始リバースイテレータ
	inline const_reverse_iterator crend() const;//終了リバースイテレータ
public:
	//アクセッサ
	inline int phase() const;//フェーズ取得
	inline void setPhase(const int phase);//フェーズ更新
	inline index_type flagIndex(const crc32_t flag_name_crc) const;//フラグインデックス取得
	inline index_type flagIndex(const char* flag_name) const;//フラグインデックス取得
	inline bool flag(const crc32_t flag_name_crc) const;//フラグ取得
	inline bool flag(const char* flag_name) const;//フラグ取得
	inline void setFlag(const crc32_t flag_name_crc, const bool flag);//フラグ更新
	inline void setFlag(const char* flag_name, const bool flag);//フラグ更新
	inline const flagSet_type& flagSet() const;//フラグセットを取得
	//メソッド
	void registFlagIndex(const char* flag_name, const index_type index);//フラグインデックスを登録
	void registFlagSettings();//フラグ設定を登録
	void clear();//全ての登録情報をクリア
public:
	//デフォルトコンストラクタ
	inline gameStatus();
	//デストラクタ
	inline ~gameStatus();
private:
	//フィールド
	int m_phase;//進行フェーズ
	flagSet_type m_flagSet;//フラグセット
	flagNameTable_type m_flagNameTable;//フラグ名テーブル
	flagIndexTable_type m_flagIndexTable;//フラグインデックステーブル
	m_flagNameBuff_type m_flagNameBuff;//フラグ名バッファ
};

//.hファイルのインクルードに伴い、常に.inlファイルを自動インクルード
#include "real_test/data/game_status.inl"

#endif//GASHA_INCLUDED_GAME_STATUS_H

// End of file
