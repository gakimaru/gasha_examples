GASHA
========
**Gakimaru's researched and standard library for C++**  
Copyright (c) 2014 Itagaki Mamoru  
Released under the [MIT license][MIT].
<https://github.com/gakimaru/gasha_examples/blob/master/LICENSE>
[MIT]: http://www.opensource.org/licenses/mit-license.php

サンプルプログラム用リポジトリ
--------
**このリポジトリは、「GASHA」のサンプルプログラム用です。**
> 多数のサンプルプログラムのビルドと実行が可能です。  
> 
> また、サブモジュールにより、ライブラリ用の各リポジトリを適切な位置関係に配置しており、ライブラリファイルのビルド、および、ソースコードレベルでのライブラリのデバッグが可能です。  

**構成リポジトリ**
* [`gasha` ライブラリ本体用リポジトリ][gasha]  
* [`gasha_settings` プロジェクト固有のライブラリ挙動カスタマイズ用リポジトリ][gasha_settings]  
* [`gasha_src` ライブラリソース用リポジトリ][gasha_src]  
* [`gasha_examples` サンプルプログラム用リポジトリ][gasha_examples]  
* [`gasha_proj` ライブラリビルド用リポジトリ][gasha_proj]  
[gasha]: https://github.com/gakimaru/gasha
[gasha_settings]: https://github.com/gakimaru/gasha_settings
[gasha_src]: https://github.com/gakimaru/gasha_src
[gasha_examples]: https://github.com/gakimaru/gasha_examples
[gasha_proj]: https://github.com/gakimaru/gasha_proj

「GASHA」とは？
--------
**個人制作による、オープンソースのC++用基本ライブラリです。**  
> 主にゲーム開発での利用を想定し、暗黙的なメモリ確保／解放を行わない点が特徴の一つです。  
> 高速性もしくは生産性を追求し、品質と開発効率の向上を目的とするものです。  
> C++11を基盤に開発しているため、プラットフォーム／コンパイラ依存のコードが少なく、それによって標準化されたマルチスレッド処理対応が多い点も特徴です。  

**基本的なアルゴリズム、コンテナ、メモリ管理、デバッグ機能などを用意しています。**  
> STLライクなコンテナは、データ構造とアルゴリズムを分離した構成としており、ユーザー独自のデータを一時的にコンテナ化して操作するなど、柔軟に活用することができます。  
> 他、メタプログラミング向けのテンプレートなども用意しています。  
> 個人的な学習・調査・研究に応じて、気まぐれに追加します。  
> そのため、実用性がないアルゴリズム（例：幾つかの非効率なソートアルゴリズム）や、実用レベルとしては機能不十分な検証目的のクラス（例：SSEベクトル）、といったものも含んでいます。  

**より扱い易い開発環境を目指したリポジトリ設計を行っています。**  
> Gitのサブモジュール機能を活用し、多数のプロジェクトでライブラリを共通利用する環境を想定してリポジトリを構成しています。  
> プロジェクトごとに、ライブラリの動作要件をカスタマイズし易いように、複数のリポジトリを組み合わせて構成しています。  

**名前の由来は？**  
> 名前の由来は「[がしゃどくろ][GASHADOKURO]」から。読み方は「ガシャ」。  
> その「骨格」的な意味合い（？）がライブラリを象徴し、自身のハンドル名「Gakimaru」の頭文字「GA」を重ねて命名しました。  
[GASHADOKURO]: http://ja.wikipedia.org/wiki/%E3%81%8C%E3%81%97%E3%82%83%E3%81%A9%E3%81%8F%E3%82%8D

ライブラリサンプルプログラム実行手順
--------
###【クイックスタート】###
1. **`git`で`gasha_examples`リポジトリのクローンを作成**  
> $ git clone https://github.com/gakimaru/gasha_examples.git

2. **サブモジュールのアップデート**  
> $ cd gasha_examples  
> $ git submodule update --init  
> 
> これにより、サブモジュールとして配置された三つのリポジトリクローンが更新されます。  
> > `sub/gasha` ライブラリ本体  
> > `sub/gasha_settings` プロジェクト固有のライブラリ挙動カスタマイズ用  
> > `sub/gasha_src` ライブラリソース

3. **ライブラリのビルド**  
> **【Visual C++の場合】**  
> > `proj/gasha_examples.sln` を開き、`[ビルド]→[バッチビルド]`メニューを実行し、すべての（もしくは任意の）プロジェクト／構成／プラットフォームを選んでビルドを実行して下さい。  
> 
> **【Unix系環境+GCCの場合】**  
> > proj/mk_all.sh を実行して下さい。  
> > > $ cd proj  
> > > $ ./mk_all.sh all  
> >
> > ルビルドする場合は、一度 ./mk_all.sh clean を実行してから ./mk_all.sh all を実行して下さい。

4. **サンプルプログラムの実行**  
> **【Visual C++の場合】**  
> > `exe/vc/`以下のディレクトリから、`（サンプル名）.exe`を実行することで、サンプルプログラムを実行可能です。  
> > `exe/vc/`以下のディレクトリには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのバッチファイルも多数用意しています。  
> > また、Visual Studio 上から、「スタートアッププロジェクト」を設定すれば、デバッグ実行することも可能です。  
> 
> **【Unix系環境+GCCの場合】**  
> > `exe/gcc/`以下のディレクトリから、`（サンプル名）`の実行ファイルを実行することで、サンプルプログラムを実行可能です。  
> > `exe/gcc/`以下のディレクトリには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのシェルスクリプトも多数用意しています。  

対象コンパイラ
--------
ライブラリの利用、および、サンプルプログラムの実行には、**C++11仕様に対応したコンパイラ**が必要です。  

**【推奨コンパイラ】**  
* Visual C++ 12.0 (Visual Studio 2013) 以降  
* GCC 4.6 以降  

開発には下記のコンパイラを用いています。  
* Visual C++ 12.0 (Visual Studio 2013)  
* GCC 4.8.2 (32bit版Cygwin環境)  

> * 今後 Clang への対応を検討しています。  

基本ディレクトリ構成とサブモジュール構成
--------
####サンプルプログラム＆ライブラリビルド用リポジトリ####
    [gasha_examples]       ... サンプルプログラム用リポジトリ
     |
     |-[exe]               ... 実行ファイル用
     |-[proj]              ... 各サンプルプログラムのビルドプロジェクト用
     |-[src]               ... 各サンプルプログラムのソースファイル用
     `-[sub]               ... サブモジュール用
        |
        |-[gasha]          ... ライブラリ本体用リポジトリ(https://github.com/gakimaru/gasha)
        |-[gasha_settings] ... プロジェクト固有のライブラリ挙動カスタマイズ用(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース用リポジトリ(https://github.com/gakimaru/gasha_src)

####ライブラ単体リビルド用リポジトリ####
    [gasha_proj]           ... ライブラリビルド用リポジトリ
     |
     |-[proj]              ... ライブラリビルドプロジェクト用
     `-[sub]               ... サブモジュール用
        |
        |-[gasha]          ... ライブラリ本体用リポジトリ(https://github.com/gakimaru/gasha)
        |-[gasha_settings] ... プロジェクト固有のライブラリ挙動カスタマイズ用(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース用リポジトリ(https://github.com/gakimaru/gasha_src)

ライブラリ概要
--------
> ＜注釈＞  
> * 以降の説明で「テンプレート関数」もしくは「テンプレートクラス」と表記しているものは、ライブラリファイルをリンクせずとも使用可能です。  
> * 以降の説明で「**※T**」の注釈記号が付いているものは、特殊なテンプレートクラスです。  
> コンパイル・リンク時間の短縮、および、クラス修正時の影響範囲の抑制（再コンパイル対象の抑制）を目的として、
ソースファイルを
**「宣言部」（.h）**
、
**「インライン関数／テンプレート関数定義部」（.inl）**
、
**「関数定義部」（.cpp.h）**
の三つに分けています。  
> それぞれ適切な場所でインクルードし、テンプレートクラスの明示的なインスタンス化を行う事で、ビルドを効率化できます。  
> なお、面倒なら全部まとめてインクルードすれば、明示的なインスタンス化をせずに使用可能です。  

####環境系####
* `gasha/build_settings/build_settings.h` **ビルド設定（統括）** ※1  
* `gasha/build_settings/project/first_settings.h` **プロジェクト固有カスタマイズ設定（先行設定）** ※2 ※3  
* `gasha/build_settings/project/last_settings.h` **プロジェクト固有カスタマイズ設定（最終設定）** ※2 ※4  

> ＜説明＞  
> * コンパイラの種類やバージョンを判別し、必要に応じて `nullptr`, `override`, `alignas`, `thread_local` などのC++11仕様に合わせた処理の独自実装版を有効化し、コード互換性の向上に寄与します。  
> * 同様に、`__FUNCTION__`, `__PRETTY_FUNCTION__` などの標準的なマクロも各コンパイラで共通利用可能にします。  
> * プロジェクト固有のカスタマイズが、対象プラットフォーム上で動作するかチェックする関数 `checkBuildSettings()` も用意しています。  
> * 現状、x86/x64系CPU ／ Windows + VC++12.0(VS2013) ／ Cygwin + GCC4.8.2 の環境でしか動作確認しておらず、他の環境に適合する状態ではありませんが、対象環境を拡張し易いように構成しています。  

> ＜注釈＞  
> * **※1**：build_settings.h が幾つかの設定ファイルをインクルードしてまとめています。
> 通常、このファイルを強制インクルードファイルに含めて利用します。
> プリコンパイル済みヘッダーに含めることも推奨します。  
> * **※2**(1)：ユーザー側（ライブラリの利用者側）で、ライブラリに対するプロジェクト固有のカスタマイズを行います。  
> 例えば「SSE命令をどこまで使用するか？」、「CRC-32の生成多項式はCRC-32Cを採用するか？」、「コンテナクラスの明示的インスタンス化でビルド効率を最適化するか？」といった設定を行います。  
> * **※2**(2)：この設定ファイルはライブラリ本体とは別のリポジトリで管理しており、ユーザー側に更新権限があるものとして位置付けます。  
> * **※2**(3)：対象マシン上で、カスタマイズ要件通りの動作が可能かチェックする関数 `diagnoseBuildSettings()` を用意しています。  
> 例えば、SSE4.2命令の使用が設定されていれば、そのマシン上でSSE4.2命令が使用可能か判定します。  
> その代わり、そうしたCPU依存の命令の使用が設定されているなら、各処理系では（例えばCRC32計算など）、対象マシンの対応状態を確認せずに命令を使用し、処理を効率化します。  
> * **※2**(4)：`diagnoseBuildSettings()` は、対象プラットフォームやC++11対応状態なども合わせて表示します。  
> これは、ライブラリビルド時の環境に基づきます。  
> * **※2**(5)：ビルド設定関係のファイル一式と下記ユーティリティの `cpuid` のソースだけあすれば、`diagnoseBuildSettings()` を実行できます。  
> 本ライブラリが動作しない環境であっても、コンパイラの状態や実行環境の確認に使うことができます。  
> * **※3**：ライブラリのネームスペースを独自にカスタマイズできます。  
> * **※4**：ライブラリの挙動を設定するためのコンパイラスイッチとしてのマクロを定義しています。
> ユーザー側で任意に有効化／無効化することでカスタマイズできます。  

> ＜資料＞  
> * **[本当にちょっとしたプログラミングTips.pdf][本当にちょっとしたプログラミングTips.pdf]**  
> ［コーディングに関するTips］-［#defineマクロの活用］  

####ユーティリティ系####
* `gasha/utility.h` ［テンプレート］**汎用ユーティリティ** ※1  
* `gasha/type_traits.h` ［テンプレート関数］**型特性ユーティリティ** ※2  
* `gasha/limits.h` ［テンプレートクラス］**限界値ユーティリティ** ※3  
* `gasha/cpuid.h` ［テンプレートクラス］**CPU情報ユーティリティ** ※4  

> ＜説明＞  
> * 汎用的なユーティリティ関数／クラスです。  
> メタプログラミングにも対応し、コンパイル時の情報取得に活用できます。  

> ＜注釈＞  
> * **※1**：任意の数の引数を指定可能な `min()`, `max()` 関数や、値を交換するための `swapValues()` 関数などを扱います。  
> * **※2**：配列の要素数や次元数を取得するための `extentof()`, `rankof()` などを扱います。  
> 標準ライブラリの `<type_traits>` と異なり、変数を引数にとって結果を返します。  
> * **※3**：各型の限界値を numeric_limits<type> で取得できます。  
> `std::numeric_limits` を継承し、指定の型の限界値を、静的メンバー `MIN`(最小値),`MAX`(最大値) で取得できます。  
> （`std::numeric_limits` は、`constexpr` に対応したコンパイラなら、`min()`,`max()` 関数を静的に扱えます。）  
> また、「値全域を扱える符号付きの型」`contained_signed_type`、「値の範囲型」`range_type`,`signed_rnage_type` なども扱えます。  
> 前者は、対象が符号なし型なら一段上の符号付き型を返し（例：`unsigned char` なら `short`、`char` なら `char`）、  
> 後者は、同精度の符号無し型および一段上の符号付き型を返します（例：`char` なら `unsigned char`, `short`）。  
> * **※4**：x86系CPUの `cpuid` 命令を実行します。  
> VC++には `__cpuid()` 関数が存在しており、それに合わせたGCC用の関数を用意しています。  

> ＜資料＞  
> * **[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]**  
> ［メタプログラミング］  

####算術系：基本####
* `gasha/basic_math.h` ［テンプレート関数含む］**基本算術** ※1  
* `gasha/fast_math.h` ［テンプレートクラス／関数含む］**高速算術** ※2  

> ＜説明＞  
> * 基本的な算術処理と、その高速化版を実装しています。  
> * メタプログラミングに対応し、コンパイル時に計算結果を算出することができるものも多数あります。  
> * 高速化は常に効果があるものではなく、ハードウェアやコンパイラおよびコンパイラオプションに依存します。  

> ＜注釈＞  
> * **※1**(1)：べき乗、素数判定、素数計算、ビット数／MSB／LSB計算といった基本的な算術系処理を扱います。  
> * **※1**(2)：高速演算と同じインターフェースを実装することを目的に、`std::sqrt`を透過的に呼び出す平方根を実装しています。  
> * **※1**(3)：基本的なベクトル演算関数も用意しています。テンプレートで n次元ベクトルのノルム、合成、差分、距離、正規化、スカラー倍、内積、外積を計算できます。  
> * **※1**(4)：基本的な行列演算関数も用意しています。テンプレートで n×m行列の加算、減算、乗算を計算できます。  
> * **※2**(1)：高速な四則演算、平方根、ベクトル演算を用意しています。  
> 演算子による演算や関数呼び出しの際に、値を`fastA`（fast arithmetic）クラスでラップすることで、高速演算が適用されるというインターフェースです。  
> * **※2**(2)：必ず効果があるわけではないので、使用の際は、プラットフォームやコンパイラに合わせた十分な検証が必要です。  
> * **※2**(3)：高速処理の内容は、例えば、SSE命令有効時にはSSE命令を使用し、無効時は通常計算を行うといったものです。  
> SSE命令に限らず、他のプラットフォームに合わせた高速演算を拡張できる構造です。  
> * **※2**(4)：プラットフォーム固有の高速演算命令（SSE命令など）を隠蔽しているので、ソースコードレベルで互換性のある高速演算として利用できます。  
> * **※2**(5)：例えば、SSE命令有効化状態で `float` 型に `fastA` を適用すると、内部で `__m128` 型で値を保持します。  
> 演算は高速になりますが、情報量が増えることには注意が必要です。  
> * **※2**(6)：**【注意】**高速演算は必ず効果のあるものではなく、コンパイラの最適化に任せた方が良い結果になることも多いので、コンパイル・実行環境に合わせて、よく検証した上で使用する必要があります。  
> コンパイル時にインライン展開を有効にしないと、適切な評価ができない点も注意が必要です。  
> * **※2**(7)：ベクトル演算と行列演算は、`fastA` を明示的に適用せずとも、SSE命令有効化時、かつ、適合する型（float[3],float[4]など）の場合は、SSE命令を使用します。  

> ＜資料＞  
> * **[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]**  
> ［メタプログラミング］  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［ハードウェアに基づく最適化］-［SIMD演算の活用］  
> ［ハードウェアに基づく最適化］-［CPU命令パイプラインに関する最適化］  

####算術系：CRC計算####
* `gasha/crc32.h` **CRC32計算**  

> ＜説明＞  
> * CRC計算を扱います。現状、32ビットのみに対応しています。  
> * C++11の `constexpr`, `ユーザー定義リテラル`による、メタプログラミング版（コンパイル時計算）に対応しています。  
> GCCは、constexprが4.6から、ユーザー定義リテラルが4.7から対応しています。  
> Visual C++ 12.0(VS2013)はどちらも未対応ですが、constexprは次期バージョンで対応予定です。  
> * ランタイム時のCRC計算は、設定に応じて、高速なSSE命令に対応します。  
> ただし、SSE命令使用時の多項式はCRC-32Cに限定します。  
> SSE命令を使用しないように指定することもできます。  
> 
> ＜資料＞  
> * **[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]**  
> ［メタプログラミング］-［constexprの活用：CRC値の算出］  

####文字列系####
* `gasha/fast_string.h` **高速文字列処理** ※1  

> ＜説明＞  
> * 基本的な文字列処理の高速化版を実装しています。  
> * 高速化は常に効果があるものではなく、ハードウェアやコンパイラおよびコンパイラオプションに依存します。  

> ＜注釈＞  
> * **※1**(1)：SSE命令を用いた `strlen`, `strcmp`, `strncmp`, `strchr`, `strrchr`, `strstr`, `srtccpy`, `strncpy` を実装しています。  
> * **※1**(1)：**【注意】**関数によっては、対象とする文字列の長さなども関係し、コンパイラの標準関数の方が効率的です。  
対象プラットフォーム、コンパイラおよびコンパイラオプションで検証の上、効果のある関数のみ有効化して使用する必要があります。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［ハードウェアに基づく最適化］-［SIMD演算の活用］  
> ［ハードウェアに基づく最適化］-［CPU命令パイプラインに関する最適化］  

####アルゴリズム系：ソート####
**【交換ソート】**  
* `gasha/bubble_sort.h` ［テンプレート関数］**バブルソート** ※1  
* `gasha/shaker_sort.h` ［テンプレート関数］**シェーカーソート** ※1  
* `gasha/odd_even_sort.h` ［テンプレート関数］**奇遇転置ソート** ※1  
* `gasha/shear_sort.h` ［テンプレート関数］**シェアソート**  
* `gasha/comb_sort.h` ［テンプレート関数］**コムソート** ※R  
* `gasha/gnome_sort.h` ［テンプレート関数］**ノームソート** ※1  

**【分割交換ソート】**  
* `gasha/quick_sort.h` ［テンプレート関数］**クイックソート**  

**【選択ソート】**  
* `gasha/selection_sort.h` ［テンプレート関数］**選択ソート** ※1  
* `gasha/heap_sort.h` ［テンプレート関数］**ヒープソート**  

**【挿入ソート】**  
* `gasha/insertion_sort.h` ［テンプレート関数］**挿入ソート** ※1 ※3 ※R  
* `gasha/shell_sort.h` ［テンプレート関数］**シェルソート** ※3 ※R  

**【マージソート】**  
* `gasha/inplace_merge_sort.h` ［テンプレート関数］**インプレースマージソート** ※1  

**【混成ソート】**  
* `gasha/intro_sort.h` ［テンプレート関数］**イントロソート** ※3 ※R  

**【分布ソート】** （非比較ソート）  
* `gasha/radix_sort.h` ［テンプレート関数］**基数ソート** ※1 ※2 ※4 ※R  

**【整列状態確認】**  
* `gasha/is_ordered.h` ［テンプレート関数］**整列状態確認** ※3 ※5  

> ＜説明＞  
> * 配列もしくはコンテナのデータをソートします。  
> * 通常は「イントロソート」（大抵は最速）、コードサイズを削減したければ「コムソート」または「シェルソート」（コムソートの方が小さくシェルソートの方が速い）、連結リストなら「挿入ソート」、メモリ度外視で大量データを高速ソートしたければ「基数ソート」（大量データでは最速）、といった使い分けができます。  

> ＜注釈＞  
> * **※1**：安定ソートです。  
> * **※2**：外部ソートです。内部で一時的にメモリ確保します。  
> * **※3**：配列以外のコンテナ（イテレータ）にも対応しています。  
> * **※4**：整数キー専用です。浮動小数点の大小比較などはできません。  
> * **※5**：順序が逆転している要素数を計上する関数も用意しています。  
> * **※R**：(Recommended) 推奨アルゴリズムです。特に処理効率／メモリ効率（小さいコードサイズ）が良いものです。  
> データ構造やデータ量、処理要件、プログラムサイズの状況等に基づいて、この中から最適なアルゴリズムを選択できます。  

> ＜補足＞  
> もう一つ重要なアルゴリズムの「マージソート」（外部ソート）に未対応です。  
> 分布ソートのように小さなワークメモリで済まず、全データをまるごコピー可能なバッファが必要になることや、環境に合わせて実装方法を変えるべきアルゴリズムであるといった理由から、対応しませんでした。  
> しかし、マージソートはかなり高速であり、分散・並列処理との相性も抜群です。超大量データのソートにも使えます。  
> 適度な粒度に分割したものを並列に（他のアルゴリズムを使って）ソートし、それをマージソートするのが特に効率的です。  
> STLの std::stable_sort がマージソートなので、メモリに余裕がある場合は選択肢の一つとして検討して下さい。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［ソートアルゴリズム］  

####アルゴリズム系：探索####
* `gasha/linear_search.h` ［テンプレート関数］**線形探索** ※1  
* `gasha/binary_search.h` ［テンプレート関数］**二分探索** ※1 ※2  

> ＜説明＞  
> * 配列もしくはコンテナのデータを探索します。  

> ＜注釈＞  
> * **※1**：配列以外のコンテナ（イテレータ）にも対応しています。  
> * **※2**：ソート済み状態を前提とした探索です。重複キーは必ず先頭にマッチします。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［探索アルゴリズム］  

####マルチスレッド系：ロック####
* `gasha/spin_lock.h` **スピンロック** ※1  
* `gasha/lw_spin_lock.h` **サイズ軽量スピンロック** ※1 ※2  
* `gasha/dummy_lock.h` **ダミーロック** ※1 ※3  
* `gasha/lock_guard.h` ［テンプレートクラス］**スコープロック** ※4  
* `gasha/unique_lock.h` ［テンプレートクラス］**単一ロック** ※5  

> ＜説明＞  
> * スレッド間のロック制御を行います。  
> * 通常、スピンロックはマルチコア環境に最適であり、シングルコア環境ではプリエンプション（タイムスライスを使い切る）までコンテキストスイッチが発生しないため、非効率です。  
> * その対策として、一定回数のスピンでコンテキストスイッチを行う仕組みを組み込んでいます。  

> ＜注釈＞  
> * **※1**：スコープロックパターン制御（lock_guard, unique_lock）に対応しています。  
> * **※2**：通常版が4バイトに対して、軽量版が1バイトです。通常版の方が若干高速です。  
> * **※3**：ロッククラスと同じインターフェースを備えますが、実際にはロック制御しません。  
> コンテナのロック制御を無効化する時などに使用します。  
> * **※4**：スコープロックによるロック制御を行います。コンストラクタでロックを自動的に取得します。  
> * **※5**：スコープロックによるロック制御を行います。コンストラクタでロックを取得するかどうかを選べます。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］  
> * **[マルチスレッドプログラミングの基礎.pdf][マルチスレッドプログラミングの基礎.pdf]**  
> ［様々な同期手法］-［排他制御：スピンロック］  

####マルチスレッド系：共有ロック####
* `gasha/shared_spin_lock.h` **共有スピンロック** ※1  
* `gasha/simple_shared_spin_lock.h` **単純共有スピンロック** ※1 ※2  
* `gasha/unshared_spin_lock.h` **非共有スピンロック** ※1 ※3  
* `gasha/dummy_shared_lock.h` **ダミー共有ロック** ※1 ※4  
* `gasha/shared_lock_guard.h` ［テンプレートクラス］**スコープ共有ロック** ※5  
* `gasha/unique_shared_lock.h` ［テンプレートクラス］**単一共有ロック** ※6  

> ＜説明＞  
> * 共有ロック（リードロック）と排他ロック（ライトロック）を使い分けたロック制御を行います。  
> * 共有ロックは複数のスレッドが同時に取得できますが、排他ロックは一つのスレッドしか取得できません。  
> * 一定回数のスピンでコンテキストスイッチを行います。  

> ＜注釈＞  
> * **※1**：スコープロックパターン制御（lock_guard, shared_lock_guard, unique_shared_lock）に対応しています。  
> * **※2**：排他ロックを優先しません。
> 通常版は、排他ロック取得待ちが発生すると、後続の共有ロックが取得待ち状態になります。
> 単純版は、共有ロックが混み合うと、排他ロックが取得できなくなります。  
> * **※3**：共有ロッククラスと同じインターフェースを備えますが、実際には全て排他ロックします。  
> * **※4**：共有ロッククラスと同じインターフェースを備えますが、実際にはロック制御しません。  
> コンテナのロック制御を無効化する時などに使用します。  
> * **※5**：スコープロックによる共有ロック制御を行います。コンストラクタでロックを自動的に取得します。  
> * **※6**：スコープロックによる共有ロック制御を行います。コンストラクタでロックを取得するかどうかを選べます。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］  
> * **[マルチスレッドプログラミングの基礎.pdf][マルチスレッドプログラミングの基礎.pdf]**  
> ［様々な同期手法］-［排他制御：リード・ライトロック］  

####マルチスレッド系：共有データ####
* `gasha/shared_pool_allocator.h` ［テンプレートクラス］**共有プールアロケータ** ※1 ※T  
* `gasha/shared_stack.h` ［テンプレートクラス］**共有スタック** ※1 ※T  
* `gasha/shared_queue.h` ［テンプレートクラス］**共有キュー** ※1 ※T  

> ＜説明＞  
> * マルチスレッドでデータを共有するためのテンプレートクラスです。  
> * コンテナとしては機能しません。至ってシンプルなアロケート／フリー／プッシュ／ポップ／エンキュー／デキューのみに対応します。  

> ＜注釈＞  
> * **※1**：共有にはロック制御を伴いますが、使用する同期オブジェクトをテンプレート引数で指定します。  
> デフォルトは spinLock ですが、std::mutex（C++11標準ライブラリ）などに変更することができます。  
> dummyLock を指定すれば、ロック制御を無効化し、シングルスレッドで高速に動作させることができます。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］  

####マルチスレッド系：ロックフリー共有データ####
* `gasha/lf_pool_allocator.h` ［テンプレート］**ロックフリープールアロケータ** ※T  
* `gasha/lf_stack.h` ［テンプレート］**ロックフリースタック** ※1 ※T  
* `gasha/lf_queue.h` ［テンプレート］**ロックフリーキュー** ※1 ※T  
* `gasha/tagged_ptr.h` ［テンプレート］**タグ付きポインター**  

> ＜説明＞  
> * ロックフリーアルゴリズムの実践です。  
> * マルチスレッドでデータを共有するためのテンプレートクラスですが、ロック制御をしません。  
> * コンテナとしては機能しません。至ってシンプルなアロケート／フリー／プッシュ／ポップ／エンキュー／デキューのみに対応します。  
> * 「キャッシュコヒーレンシ問題」、「ABA問題」に対処済みです。  
> * スレッドが混み合うような状況下では、通常のロック制御よりも効率的に動作します。  
> * 逆に競合が少ない場面では、スピンロックによるロック制御の方が高速です。  
> * なお、ロックフリーアルゴリズムは、シングルコア環境でも効果を発揮します。  

> ＜注釈＞  
> * **※1**：ロックフリーアルゴリズム特有の「ABA問題」に対処するために、「タグ付きポインター」を使用します。  
> タグ付きポインターは、ポインターの一部にタグとなる情報を仕込んで、ポインターの誤認を防ぐ手法です。  
> 32ビット環境では、ポインターを64ビットに拡張して32ビットのタグを扱うことでABA問題をほぼ完全に防ぎます。  
> 64ビット環境では、アラインメントの隙間（2～4びっとほど）か、実質的な有効アドレス範囲外の上位8ビットなどを利用します。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］  
> > ABA問題やキャッシュコヒーレンシ問題についての解説も記述しています。  

####コンテナ系：擬似コンテナ（外部データコンテナ）####
**【配列】**  
* `gasha/dynamic_array.h` ［テンプレートクラス］**動的配列** ※1 ※3 ※T  
* `gasha/ring_buffer.h` ［テンプレートクラス］**リングバッファ** ※1 ※4 ※T  

**【連結リスト】**  
* `gasha/linked_list.h` ［テンプレートクラス］**双方向連結リスト** ※2 ※5 ※T  
* `gasha/singly_linked_list.h` ［テンプレートクラス］**片方向連結リスト** ※2 ※6 ※T  

**【二分探索木】**  
* `gasha/rb_tree.h` ［テンプレートクラス］**赤黒木** ※2 ※7 ※T  

> ＜説明＞  
> * メモリ管理を行わず、外部のデータを操作する疑似コンテナです。  
> * STLライクなインターフェースで操作可能ですが、データ構造とアルゴリズムを分離して扱えるため、より自由度の高い活用が可能です。  
> * 使用時の設定により、マルチスレッドセーフ（ロック制御）の仕組みを有効化できます。  

> ＜注釈＞  
> * **※1** 配列系コンテナは、外部から既存の配列を受け取って扱います。  
> * **※2** 連結リスト／木系コンテナは、外部からノードを受け取り、連結・解除・ソート・回転（平衡化）などのアルゴリズムを適用します。  
> * **※3** 動的配列 dynamic_array は、STL の std::vector （動的配列）をモデルにしたインターフェースを実装しています。  
> * **※4** リングバッファ ring_buffer は、STL の std::deque （双方向キュー）をモデルにしたインターフェースを実装しています。  
> * **※5** 双方連結リスト linked_list は、STL の std::list （双方向連結リスト）をモデルにしたインターフェースを実装しています。  
> * **※6** 片方向連結リスト singly_linked_list は、STL の std::forward_list （C++11：片方向連結リスト）をモデルにしたインターフェースを実装しています。  
> * **※7** 赤黒木 rb_tree は、STL の std::multimap/std::multiset をモデルにしたインターフェースを実装しています。（キー重複を許容します。）  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［データ構造］  
> > 赤黒木の平衡化アルゴリズムの解説を記述しています。  

####コンテナ系：コンテナ（内部データコンテナ）####
* `gasha/binary_heap.h` ［テンプレートクラス］**二分ヒープ** ※1 ※T  
* `gasha/hash_table.h` ［テンプレートクラス］**開番地法ハッシュテーブル** ※2 ※T  

> ＜説明＞  
> * コンテナが保持する固定バッファのみで処理を行い、動的なメモリ確保／解放を行わないコンテナです。  
> * STLライクなインターフェースで操作可能です。  
> * 使用時の設定により、マルチスレッドセーフ（ロック制御）の仕組みを有効化できます。  

> ＜注釈＞  
> * **※1** 二分ヒープ binary_heap は、STL の std::priority_queue （優先度付きキュー）をモデルにしたインターフェースを実装しています。  
> * **※2** 開番地法ハッシュテーブル hash_table は、STL の std::unordered_map （C++11：ハッシュテーブル）をモデルにしたインターフェースを実装しています。  
> キー重複を許容しません。重複キーの登録は失敗するか、置き換えにするかを指定できます。  
> なお、STLは（おそらく）連鎖法ハッシュテーブルです。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［データ構造］  

####コンテナ系：コンテナアダプタ####
* `gasha/priority_queue.h` ［テンプレートクラス］**優先度付きキュー** ※1 ※T  

> ＜説明＞  
> * 内部でコンテナを使用し、アルゴリズムを適用するコンテナアダプタです。  
> * 使用時の設定により、マルチスレッドセーフ（ロック制御）の仕組みを有効化できます。  

> ＜注釈＞  
> * **※1** 優先度付きキュー priority_queue は、STL を特にモデルとしていません。  
> STLの優先度付きキューと異なり、エンキューの順序性をデキュー時に保証します。  
> 内部で使用するデフォルトのコンテナは二分ヒープ（binary_heap）です。  
> push/pop/upHeap/downHeap インターフェースを条件としてコンテナを使用します。  

> ＜資料＞  
> * **[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> ［データ構造とアルゴリズムによる最適化］-［データ構造］  

####【検討事項】対応を検討しているライブラリ####
以下、検討事項を箇条書きします。メモ書きです。  
まだ最適な構造・手法を確立していないものです。  

* 【検討事項：コンテナ】`gasha/***linked_list.h` ［テンプレートクラス］**片方向／双方向連結リスト**  
> * 高速外部ソート対応。（基数ソートを片方向連結リストに対応する）  

* 【検討事項：コンテナ】`gasha/aggregate.h` ［テンプレートクラス］**集合**  
> * STLの集合(std::set)とは全く異なり、他のコンテナの部分集合を扱うことを目的としたクラスです。  
> * 例えば、ゲームのインベントリデータでありがちな、部分的なカテゴリの抜き出しや一時的なソートなどを扱い易くします。  
> * 簡易トランザクション機能を検討します。通常の部分集合は元の集合を直接参照しますが、値のコピーを同時作成し、元のデータに影響せずに値を変更することに対応します。  
> * 簡易トランザクションは、最終的なコミット（元の集合に反映）・ロールバック（変更を破棄）の機能を持ちます。  
> * トランザクションを開始した時点で要素ごとにコピーを作成する「コピーオンライト」の動作を行い、更に、要素ごとに書き戻し権限を扱うようなマルチスレッド処理サポートも検討します。  
> * 【課題】SQLiteやRDBなどとの連携も考慮するか？SQL、トランザクション、カーソルなどの対応？  

* 【検討事項：コンテナ】`gasha/abstract_syntax_tree.h` ［テンプレートクラス］**抽象構文木**  
> * 抽象構文木の対応。
> * 計算式を解析・演算するための独自パーサーを、ユーザー側で開発するための基本処理とする。  
> * 想定する用途：ツール開発（パーサー部分）に利用。ランタイムパーサーに利用。計算式の演算に利用。  
> ゲームデータ用Excelに記述したゲーム制御用の計算式など利用可能。  
> * 下記処理との組み合わせ対応。計算式を含んだJSONパーサーの独自実装が一つの目標。  
> > * 構文解析に対応。（パーサー用ステートマシンを汎用化？）  
> > * パーサーの構文エラー通知に対応。  
> > * 抽象構文木の中置記法から前置記法（ポーランド記法）、後置記法（逆ポーランド記法）への変換に対応。  
> > * 中置記法、前置記法、後置記法の演算に対応。  
> > * シリアライズ／デシリアライズに対応。  
> > * 四則演算、ビット演算、論理演算、三項演算子に対応。  
> > * 文字列に対応。  
> > * 組み込み関数（算術系）に対応。  
> > * ユーザー定義関数に対応。  
> > * （できれば）変数、制御文、ループ分に対応。  
> > * 演算時のオブジェクトバインディングに対応。  
> > * 基本的にコルーチンには非対応。（拡張は考慮）  

* 【検討事項：コンテナ】`gasha/graph.h` ［テンプレートクラス］**グラフ**  
> * 重み付き有向グラフの対応。（一方通行のある経路探索に適用可能）  
> * マトリックス形式データ、エッジ（2ノード連結情報）リスト形式データのどちらか、もしくは、それぞれの対応。  
> * 「扉の開閉」のような、一方通行のON/OFFも考慮。  
> * ダイクストラ法に対応。主に経路探索の事前計算用。データのシリアライズ／デシリアライズも考慮。  
> * A*法に対応。主にランタイムの経路探索用。  
> * 所属ノード探索に対応。自分の座標や目標点の座標からノードを素早く算出することを想定。  
> * 高速ノード探索のためのインデックステーブルもしくはハッシュテーブルの対応。  
> * 高速最短経路探索のためのヒープアルゴリズム検証。（二項ヒープ、フィボナッチヒープ、ペアリングヒープ、Skewヒープなど）  

* 【検討事項：コンテナ】`gasha/hierarchy.h` ［テンプレートクラス］**階層**  
> * 階層構造データ。  
> * 親ノードへの連結、両隣の兄弟ノードへの連結、最初の子ノードへの連結情報を持つ。もしくは、そこから親と兄への連結を廃止。  
> * デザインパターンのCompositeパターンや、Visitorパターン、ChainOfResponsibilityパターンをサポート。  
> * シーングラフのようなデータへの応用を想定。  

* 【検討事項：コンテナ】`gasha/spatial_partitioning.h` ［テンプレートクラス］**空間分割**  
> * バイナリ空間分割木（BSP木）／四分木／八分木／kd木など検討。  
> * 衝突判定の絞り込み、描画対象の絞り込み、処理対象の間引きなど、処理効率化のためのデータ構造とアルゴリズムに対応。  
> * std::set のような集合を管理するコンテナとして扱い、指定範囲に該当する部分集合を返すなどの構造を検討。  
> 部分集合取得時は、情報格納用の配列を渡すインターフェースや、近い順にソートして返すなどの機能を検討。  
> 更には、階層構造と組み合わせ、シーングラフ上のオブジェクトのアニメーション順序・省略決定、移動・衝突判定の順序・省略決定、不透明／半透明描画の順序・省略決定など、処理要件に柔軟に対応できる構造を目指す。  

ライブラリの命名規則
--------
ライブラリに用いている命名規則は、基本的には下記のとおりです。  
* **【ネームスペース名】**：すべて小文字でアンダースコアを区切り文字としたスネークケース。
> * （例）word_and_word  

* **【クラス名／関数名／メソッド名】**：小文字から始まるローワーキャメルケース。  
> * （例）wordAndWord  

* **【例外】**：標準ライブラリのクラスやメソッドど同質・同類・類似の場合、それがスネークケースであっても、同じ名前か同様の名前を用いる。  
> * （例）push_backメソッド, lock_guardクラス, max_sizeRealメソッド（類似メソッド）  

* **【混在の許容】**：一つのソースファイルやクラス内に、キャメルケースと例外（スネークケース）が混在することを問題とせず、むしろ独自拡張を識別可能なものとして扱う。  
> * （例）dynamic_array::conrtainerクラスは、std::vectorに準拠し、assign, popo_back などのメソッドを実装しており、独自メソッドは assignArray のようなキャメルケースの名前で実装。  

ライブラリのソースファイル
--------
ソースファイルは、Visual C++とGCCの互換性のために、下記の仕様で統一しています。

* 文字コード ... UTF-8（BOM付き）
    
    > * Visual Studio が扱う標準的なユニコードは UTF-8（BOM付き）です。  
    > * GCCが扱う標準的なユニコードは UTF-8（BOMなし）ですが、4.3.x 以降、BOM付きにも対応しています。  

* 改行コード ... LF
    
    > * Windows系OSの標準的な改行コードは CR+LF ですが、Visual StudioはLFのみのソースファイルも扱えます。  
    > * Unix系OSの標準的な改行コードは LF です。  

ビルド構成
--------
####ビルド構成####
以下、本ライブラリが想定するビルド構成です。  
任意に追加しても問題ありません。  
* `DEBUG` **フルデバッグ設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_FULL_DEBUG`  
> デバッグ機能：あり（冗長）  
> アサーション：有効  
> 最適化：なし  
> ファイルシステム：ローカル／ROM切り替え（データで設定）  
> ユニットテスト：可  
> シンボル情報：あり  

* `DEBUG_MODERATE` **プログラム開発向け設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_DEBUG_MODERATE`  
> デバッグ機能：あり（冗長）  
> アサーション：有効  
> 最適化：小  
> ファイルシステム：ローカル／ROM切り替え（データで設定）  
> ユニットテスト：可  
> シンボル情報：あり  

* `DEBUG_OPT` **コンテンツ制作・QA向け設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_DEBUG_OPT`  
> デバッグ機能：あり  
> アサーション：有効  
> 最適化：最大  
> ファイルシステム：ローカル／ROM切り替え（データで設定）  
> ユニットテスト：可  
> シンボル情報：あり  

* `UNIT_TEST` **自動回帰テスト向け設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_REGRESSION_TEST`  
> デバッグ機能：あり  
> アサーション：有効  
> 最適化：最大  
> ファイルシステム：ローカル／ROM切り替え（データで設定）  
> ユニットテスト：専用  
> シンボル情報：あり  

* `LOCAL_RELEASE` **製品テスト向け設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_LOCAL_RELEASE`  
> デバッグ機能：なし  
> アサーション：無効  
> 最適化：最大  
> ファイルシステム：ローカル／ROM切り替え（データで設定）  
> ユニットテスト：不可  
> シンボル情報：あり  

* `RELEASE` **製品向け設定**  
> マクロ：`GASHA_BUILD_CONFIG_IS_RELEASE`  
> デバッグ機能：なし  
> アサーション：無効  
> 最適化：最大  
> ファイルシステム：ROMのみ  
> ユニットテスト：不可  
> シンボル情報：なし  

> 注釈  
> * 現状のライブラリは、`DEBUG` と `RELEASE` 以外のビルド構成を用意していません。  
> また、ファイルシステムには対応しておらず、マクロの定義のみを行っています。  
> これらは、整然とした開発環境の構築を目的に、扱うべきビルド構成とその内訳を整理したものです。  

####ビルド構成に関係するマクロ####
以下、ビルド構成に応じて指定されるマクロです。  
このマクロに基づいて、処理が有効化／無効化されてライブラリがビルドされます。  
ライブラリを使用するプロジェクト（ユーザー側）でも使用可能です。  
* `GASHA_HAS_DEBUG_FEATURE`       ... デバッグ機能有効化  
* `GASHA_HAS_VERBOSE_DEBUG`       ... 冗長デバッグ機能有効化  
* `GASHA_ASSERTION_IS_ENABLED`    ... アサーション有効化  
* `GASHA_NO_OPTIMIZED`            ... 最適化なし ※1  
* `GASHA_OPTIMIZED_MODERATELY`    ... 適度に最適化 ※1  
* `GASHA_OPTIMIZED`               ... 最大限の最適化 ※1  
* `GASHA_FILE_SYSTEM_IS_ROM`      ... ROM専用ファイルシステム  
* `GASHA_FILE_SYSTEM_IS_FLEXIBLE` ... ローカル／ROM切り替えファイルシステム（ローカルデータで設定）  
* `GASHA_UNITE_TEST_ENABLED`      ... ユニットテスト（の仕組みが）有効  
* `GASHA_IS_REGRESSION_TEST`      ... 回帰テストモード有効 ※2  
* `GASHA_HAS_SYMBOLS`             ... シンボル情報あり ※3  
* `GASHA_IS_STRIPPED_SYMBOLS`     ... シンボル情報なし ※3  

> 注釈
> * **※1**：これらは、ビルドの最適化状態を示すことを目的としたマクロであり、実際にこのマクロに基づいて最適化するわけではありません。  
> ビルド構成ごとに、別途最適化オプションが設定されます。  
> なお、このマクロに基づいて、最適化コードの切り替えを行う処理を組んでも構いません。  
> * **※3**：「回帰テスト」はユニットテストの自動実行です。  
> * **※2**：これもビルドの状態を示すことを目的としたマクロであり、実際にこのマクロに基づいてシンボル情報の組み込み／削除を行うわけではありません。  

####ターゲットプラットフォーム####
現状、x86系Windows、x64系Windows、Cygwinに対応しています。  
Visual C++ では、`Win32` と`x64` という名称のプラットフォームです。  

ディレクトリ構成
--------
####サブモジュール／ライブラリ本体####
    [gasha_examples/gasha_proj]  ... ライブラリサンプルプログラム／ビルドプロジェクト用リポジトリ
     |
     |-[sub]                     ... サブモジュール用
        |
        `-[gasha]                ... サブモジュール：ライブラリ本体用リポジトリ
           |
           |-[include]           ... インクルードファイル用
           |  |
           |  `-[gasha]          ... ネームスペースディレクトリ
           |     |                   （ライブラリファイルパスの競合を避けるためサブディレクトリ）
           |     |- *.h/*.inl/*.cpp.h           ... 各種インクルードファイル
           |     |
           |     `-[build_settings] ... ビルド設定用
           |        |
           |        `- build_settings.h         ... ライブラリビルド設定ファイル
           |
           |-[lib]               ... ライブラリファイル用
           |  |
           |  |-[vc]             ... Visual C++用
           |  |  |
           |  |  |- gasha_x86.lib               ... x86リリースビルド用
           |  |  |- gasha_x86_debug.lib         ... x86デバッグビルド用
           |  |  |- gasha_x64.lib               ... x64リリースビルド用
           |  |  `- gasha_x64_debug.lib         ... x64デバッグビルド用
           |  |
           |  `-[gcc]            ... GCC用
           |     |
           |     |- gasha_x86.a                 ... x86リリースビルド用
           |     `- gasha_x86_debug.a           ... x86デバッグビルド用
           |                                        （x86版Cygwinで開発したため、x64版は現状なし）
           |
           `-[proj]
              |
              `- Makefile_common ... GCC用メイクファイルの共通部分
                                     （ライブラリ／サンプルプログラム共用）

####サブモジュール／ライブラリ挙動設定####
    [gasha_examples/gasha_proj]  ... ライブラリサンプルプログラム／ビルドプロジェクト用リポジトリ
     |
     |-[sub]                     ... サブモジュール用
        |
        `-[gasha_settings]       ... サブモジュール：プロジェクト固有のライブラリ挙動カスタマイズ用
           |
           `-[include]           ... ライブラリインクルードファイル用
              |
              `-[gasha]          ... ネームスペースディレクトリ
                 |                   （ライブラリファイルパスの競合を避けるためのサブディレクトリ）
                 `-[build_settings] ... ビルド設定用
                    |
                    `-[project]     ... プロジェクト固有設定用
                       |
                       |- first_settings.h      ... プロジェクト固有のライブラリ設定（先行設定）
                       |
                       `- last_setteings.h      ... プロジェクト固有のライブラリ設定（最終設定）

####サブモジュール／ライブラリソース####
    [gasha_examples/gasha_proj]  ... ライブラリサンプルプログラム／ビルドプロジェクト用リポジトリ
     |
     |-[sub]                     ... サブモジュール用
        |
        `-[gasha_src]            ... サブモジュール：ライブラリソース用リポジトリ
           |
           |-[proj]              ... ライブラリビルドプロジェクト用
           |  |
           |  |- standard.h/.cpp                ... 強制インクルード／プリコンパイル済みヘッダー
           |  |
           |  |- gasha.vcxproj                  ... Visual C++用ライブラリビルドプロジェクト
           |  |- Makefile                       ... GCC用ライブラリビルドメイクファイル
           |  `- mk.sh                          ... GCC用ライブラリビルドシェルスクリプト
           |
           `-[src]               ... ライブラリソースファイル用
              |-...

####実行ファイル####
    [gasha_examples]             ... ライブラリサンプルプログラム用リポジトリ
     |
     |-[exe]                     ... 実行ファイル用
        |
        |- run_all.bat                          ... Visual C++ 版全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |- run_all.sh                           ... GCC版全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |
        |-[vc]                   ... Visual C++でビルドした実行ファイルの置き場
        |  |
        |  |- run_all.bat                       ... x86＆x64全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |
        |  |-[x86]               ... x86(32bit)向け
        |  |  |
        |  |  |-（サンプル名）.exe              ... サンプルプログラム実行ファイル
        |  |  |- run_all.bat                    ... リリースビルド＆デバッグビルド全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |  |
        |  |  |-[bat]            ... サンプルプログラム実行と実行ログ記録用
        |  |  |  |
        |  |  |  |-（サンプル名）.bat           ... サンプルプログラム個別実行＆実行ログ記録用バッチファイル
        |  |  |  |- run_all.bat                 ... 全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |  |  |
        |  |  |  `-[log]
        |  |  |     |
        |  |  |     `-（サンプル名）.log        ... サンプルプログラム個別実行ログファイル
        |  |  |
        |  |  `-[Debug]          ... デバッグビルド用
        |  |     |-...               （親フォルダ（リリースビルド用）と同様の構成）
        |  |
        |  `-[x64]               ... x64(64bit)向け
        |     |- ...                 （x86版と同様の構成）
        |
        `-[gcc]                  ... GCCでビルドした実行ファイルの置き場
           |
           `-[x86]               ... x86(32bit)向け
              |- ...                 （Visual C++用と同様の構成）
                                     （シェルスクリプトの代わりにバッチファイル使用）
                                     （x86版Cygwinで開発したため、x64版は現状なし）

####各サンプルプログラムのソース####
    [gasha_examples]             ... ライブラリサンプルプログラム用リポジトリ
     |
     |-[src]                     ... サンプルプログラムソースファイル用
        |
        |-[（サンプル名）]       ... 各サンプルプログラムソースファイル用
        |  |
        |  |- *.cpp/.h

####ライブラリおよび各サンプルプログラムのビルドプロジェクト####
    [gasha_examples]             ... ライブラリサンプルプログラム用リポジトリ
     |
     |-[proj]                    ... ライブラリ／サンプルプログラムビルドプロジェクト用
        |
        |-[（サンプル名）]       ... 各サンプルプログラムビルドプロジェクト用
        |  |
        |  |- main.cpp                          ... サンプルプログラム用メイン関数
        |  |- standard.h/.cpp                   ... サンプルプログラム用強制インクルード／プリコンパイル済みヘッダー
        |  |
        |  |- （サンプル名）.vcxproj            ... Visual C++用サンプルプログラムビルドプロジェクト
        |  |- Makefile                          ... GCC用サンプルプログラムビルドメイクファイル
        |  `- mk.sh                             ... GCC用サンプルプログラムビルドシェルスクリプト
        |
        |- Makefile_example_common              ... GCC用サンプルプログラムメイクファイルの共通部分
        |
        |- gasha_examples.sln                   ... Visual C++用ライブラリ＆全サンプルプログラムビルドソリューション
        `- mk_all.sh                            ... GCC用ライブラリ＆全サンプルプログラムビルドシェルスクリプト

####ライブラリおよび各サンプルプログラムのビルドプロジェクト####
    [gasha_proj]                 ... ライブラリビルドプロジェクト用リポジトリ
     |
     |-[proj]                    ... ライブラリビルドプロジェクト用
        |
        |- gasha.sln                            ... Visual C++用ライブラリビルドソリューション
        `- mk.sh                                ... GCC用ライブラリビルドシェルスクリプト

ライブラリの利用手順
--------
* **前提１：GASHAもしくはその派生ライブラリをgitリポジトリで管理するものとします。**  
* **前提２：ライブラリを複数プロジェクトで共通利用するものとします。**  

1. **ライブラリ用リポジトリとプロジェクト固有のライブラリ挙動カスタマイズ用リポジトリをプロジェクトに配置**  
> サブモジュールとして、ライブラリ用のリポジトリを配置して下さい。  
> > `project/sub/gasha` ... ライブラリ本体用リポジトリ(https://github.com/gakimaru/gasha)  
> > `project/sub/gasha_settings` ... プロジェクト固有のライブラリ挙動カスタマイズ用(https://github.com/gakimaru/gasha_settings)  
> 
> ライブラリ自体をプロジェクト向けにビルドする場合は、ライブラリソース用リポジトリも配置して下さい。  
> > `project/sub/gasha_src` ... ライブラリソース用リポジトリ(https://github.com/gakimaru/gasha_src)   

2. **インクルードパスを設定**  
> 下記のパスをプロジェクトのインクルードパスに追加して下さい。  
> > `project/sub/gasha/include`  ... ライブラリ用  
> > `project/sub/gasha_setting/include`  ... ライブラリ挙動設定用  

3. **ライブラリパスとライブラリファイルを設定**  
> 下記のいずれか一つのファイルをプロジェクトのライブラリファイルに追加して下さい。  
> 
> **【Visual C++用】**  
> > `project/sub/gasha/lib/vc/gasha_x86.lib` ... x86リリースビルド用  
> > `project/sub/gasha/lib/vc/gasha_x86_debug.lib` ... x86デバッグビルド用  
> > `project/sub/gasha/lib/vc/gasha_x64.lib` ... x64リリースビルド用  
> > `project/sub/gasha/lib/vc/gasha_x64_debug.lib` ... x64デバッグビルド用  
> 
> **【GCC用】**  
> > `project/sub/gasha/lib/gcc/gasha_x86.a` ... x86リリースビルド用  
> > `project/sub/gasha/lib/gcc/gasha_x86_debug.a` ... x86デバッグビルド用  
> > （x86版Cygwinで開発したため、x64版は現状なし）  

4. **【推奨】強制インクルードとプリコンパイル済みヘッダーを設定**  
> プラットフォーム／言語設定を暗黙的に全ソースファイルに反映させるために、「強制インクルード」を使用することを推奨します。  
> 更に、強制インクルードファイルを「プリコンパイル済みヘッダー」にすることで、コンパイル速度を高速化することを、合わせて推奨します。  
> 
> **【設定例：standard.h】**  
> > プロジェクトファイル（*.vcxproj, Makefile）と同じディレクトリに standard.h を配置して下さい。  
> > `stadard.h`の内容（この一行のみ）  
> > > `#include <gasha/build_settings/build_settings.h>`  
> 
> **【設定例：standard.cpp】** （Visual C++のみ必要なファイル）  
> > プロジェクトファイル（*.vcxproj, Makefile）と同じディレクトリに standard.cpp を配置して下さい。  
> > `stadard.cpp`の内容（この一行のみ）  
> > > `#include <standard.h>`  
> 
> **【Visual C++の場合】**  
> > プロジェクトのプロパティから、［C/C++］→［詳細設定］ページの設定を下記のように変更して下さい。  
> > > ・［必ず使用されるインクルードファイル］に standard.h を指定  
> > 
> > プロジェクトのプロパティから、［C/C++］→［プリコンパイル済みヘッダー］ページの設定を下記のように変更して下さい。  
> > > ・［プリコンパイル済みヘッダー］に「使用(/Yu)」を指定  
> > > ・［プリコンパイル済みヘッダーファイル］に standard.h を指定  
> > 
> > 更に、standard.cpp のプロパティから、［C/C++］→［プリコンパイル済みヘッダー］ページの設定を下記のように変更して下さい。  
> > > ・［プリコンパイル済みヘッダー］に「作成(/Yc)」を指定  
> > > ・［プリコンパイル済みヘッダーファイル］に standard.h を指定  
> 
> **【GCCの場合】**  
> > g++コマンドでプリコンパイル済みヘッダーファイル standard.h.gch を作成して下さい。  
> > 【例】  
> > > $ g++ （-std=c++11 や -g などのコンパイルオプション） -x c++-header standard.h  
> > 
> > standard.h.gch はインクルードパスが通った場所に配置して下さい。  
> > 
> > コンパイル時には、g++コマンドに -inlude オプションを指定して下さい。  
> > .h.gch ではなく、.h ファイルを指定します。  
> > 【例】  
> > > $ g++ （-std=c++11 や -g などのコンパイルオプション） -include standard.h -c xxx.cpp -o xxx.o

5. **サブモジュールのブランチ（もしくはタグ／バージョン）を確定してコミット**  
> サブモジュールの挙動設定ファイルの変更やライブラリのビルドなどを行った後、プロジェクトをコミットして下さい。  
> 
> サブモジュール用のフォルダがコミット対象になります。  
> 
> これにより、プロジェクトで使用するサブモジュール（ライブラリのリポジトリ）のバージョンが設定されます。  

サンプルプログラム用プロジェクト
--------
####サンプルプログラム用プロジェクト####
    [gasha_examples]  ... ライブラリサンプルプログラム用リポジトリ
     |
     `-[proj]
        |                                   【ビルド設定】
        |-[example_build_settings]      ... ビルド設定テスト
        |
        |                                   【ユーティリティ】
        |-[example_utility]             ... 汎用ユーティリティテスト
        |-[example_type_traits]         ... 型特性ユーティリティテスト
        |
        |                                   【算術】
        |-[example_basic_math]          ... 基本算術処理テスト
        |-[example_fast_math]           ... 高速算術処理テスト
        |-[example_crc32]               ... CRC32計算処理テスト
        |
        |                                   【文字列】
        |-[example_fast_string]         ... 高速文字列処理テスト
        |
        |                                   【アルゴリズム】
        |-[example_sort_and_search]     ... ソート＆探索処理テスト
        |
        |                                   【スレッド】
        |-[example_shared_data]         ... マルチスレッド共有データ／ロックフリー共有データテスト
        |
        |                                   【擬似コンテナ（外部データコンテナ）】
        |-[example_dynamic_array]       ... 動的配列コンテナテスト
        |-[example_reing_buffer]        ... リングバッファコンテナテスト
        |-[example_linked_list]         ... 双方向連結リストコンテナテスト
        |-[example_singly_linked_list]  ... 片方向連結リストコンテナテスト
        |-[example_rb_tree]             ... 赤黒木コンテナテスト
        |
        |                                   【コンテナ（内部データコンテナ）】
        |-[example_hahs_table]          ... 開番地法ハッシュテーブルコンテナテスト
        |
        |                                   【コンテナアダプタ】
        |-[example_priority_queue]      ... 二分ヒープコンテナ＆優先度付きキューコンテナアダプタテスト

> 各サンプルプログラムは、チュートリアル的な内容ではなく、開発過程で機能やパフォーマンスの評価に使用したものそのままです。  
> 条件を変えながら多彩なテストをできるように構成しているものも多く、難解に見えるかもしれません。  
> ライブラリを習得いただく際は、関連資料とソースコード（主にヘッダーファイル）のコメントを合わせてご確認頂ければ幸いです。  

免責事項
--------
本ライブラリの実績は不十分であり、品質を保証できる状態ではありません。  
ライブラリの不具合には基本的に対応できませんので、ご利用の際は、自己責任でお願いします。  
なお、MITライセンスに基づき、自由な改変や商用利用も可能ですので、独自に改変・修正の上でご利用頂けます。  
不具合を見つけた場合には、教えて頂けると幸いです。  

関連資料
--------
[ゲームシステムのアーキテクチャと開発環境（GitHub）][ゲームシステムのアーキテクチャと開発環境]  
[ゲームシステムのアーキテクチャと開発環境（DropBox）][ゲームシステムのアーキテクチャと開発環境DropBox]  

> 資料にはサンプルプログラムも多数掲載しており、ライブラリのプログラムと同じものも含んでいます。  
> サンプルプログラム作成とそれに基づく資料作成が先行し、その後ライブラリにまとめ直しているので、両者のソースコードには多少の相違があります。  

[ゲームシステムのアーキテクチャと開発環境DropBox]: https://www.dropbox.com/sh/xh1tauzgqhkk7hy/AACyWgV6uaEnjOJNT5FVBG4Ia/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8
[ゲームシステムのアーキテクチャと開発環境]: https://github.com/gakimaru/public/tree/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8
[本当にちょっとしたプログラミングTips.pdf]: https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E6%9C%AC%E5%BD%93%E3%81%AB%E3%81%A1%E3%82%87%E3%81%A3%E3%81%A8%E3%81%97%E3%81%9F%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips.pdf
[効果的なテンプレートテクニック.pdf]:       https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E5%8A%B9%E6%9E%9C%E7%9A%84%E3%81%AA%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF.pdf
[プログラム最適化Tips.pdf]:                 https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E6%9C%80%E9%81%A9%E5%8C%96Tips.pdf
[マルチスレッドプログラミングの基礎.pdf]:   https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%9E%E3%83%AB%E3%83%81%E3%82%B9%E3%83%AC%E3%83%83%E3%83%89%E5%88%B6%E5%BE%A1/%E3%83%9E%E3%83%AB%E3%83%81%E3%82%B9%E3%83%AC%E3%83%83%E3%83%89%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%9F%BA%E7%A4%8E.pdf
[効率化と安全性のためのロック制御.pdf]:     https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%9E%E3%83%AB%E3%83%81%E3%82%B9%E3%83%AC%E3%83%83%E3%83%89%E5%88%B6%E5%BE%A1/%E5%8A%B9%E7%8E%87%E5%8C%96%E3%81%A8%E5%AE%89%E5%85%A8%E6%80%A7%E3%81%AE%E3%81%9F%E3%82%81%E3%81%AE%E3%83%AD%E3%83%83%E3%82%AF%E5%88%B6%E5%BE%A1.pdf

========
■■以上
