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

**関連リポジトリ**
* [`gasha` ライブラリ本体用リポジトリ][gasha]  
* [`gasha_settings` ライブラリ挙動設定用リポジトリ][gasha_settings]  
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
> マルチスレッドの対応が多い点も特徴の一つです。  

**基本的なアルゴリズム、コンテナ、メモリ管理、デバッグ機能などを用意しています。**  
> 他、メタプログラミング向けのテンプレートも用意しています。  
> 個人的な学習・調査・研究に応じて、気まぐれに追加します。  

**名前の由来**  
> 名前の由来は「[がしゃどくろ][GASHADOKURO]」から。  
> その「骨格」的な意味合いに、自身のハンドル名「Gakimaru」の頭文字「GA」を重ねて命名しました。
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
> > `sub/gasha` ... ライブラリ本体  
> > `sub/gasha_settings` ... ライブラリ挙動設定  
> > `sub/gasha_src` ... ライブラリソース

3. **ライブラリのビルド**  
> **【Visual C++の場合】**  
> > `proj/gasha_examples.sln` を開き、`[ビルド]→[バッチビルド]`メニューを実行し、すべての（もしくは任意の）プロジェクト／構成／プラットフォームを選んでビルドを実行して下さい。  
> 
> **【Unix系環境+GCCの場合】**  
> > proj/mk.sh を実行して下さい。  
> > > $ cd proj  
> > > $ ./mk.sh all  
> >
> > ルビルドする場合は、一度 ./mk.sh clean を実行してから ./mk.sh all を実行して下さい。

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
開発には下記のコンパイラを用いています。  
* Visual Studio 2013  
* GCC 4.7  

> 「可変長引数テンプレート」使用箇所を削除するなど、わずかな改変でC++11以前のコンパイラにも適用できるものが多数あります。  
> 逆に言えば、C++11仕様にさえ対応していれば、上記以外のコンパイラでも、ほとんど改変することなく利用可能です。  

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
        |-[gasha_settings] ... ライブラリ挙動設定用リポジトリ(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース用リポジトリ(https://github.com/gakimaru/gasha_src)

####ライブラ単体リビルド用リポジトリ####
    [gasha_proj]           ... ライブラリビルド用リポジトリ
     |
     |-[proj]              ... ライブラリビルドプロジェクト用
     `-[sub]               ... サブモジュール用
        |
        |-[gasha]          ... ライブラリ本体用リポジトリ(https://github.com/gakimaru/gasha)
        |-[gasha_settings] ... ライブラリ挙動設定用リポジトリ(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース用リポジトリ(https://github.com/gakimaru/gasha_src)

ライブラリ概要
--------
####【環境系】####
* `<gasha/build_settings/build_settings.h>` ... **プラットフォーム／言語設定**  
> 
> コンパイラの種類やバージョンを判別し、必要に応じて `nullptr`, `override`, `alignas`, `thread_local` などのC++11仕様に合わせた処理の独自実装版を有効化し、コード互換性の向上に寄与します。  
> 同様に、`__FUNCTION__`, `__PRETTY_FUNCTION__` なども各コンパイラで共通利用可能にします。  
> 通常このファイルは、強制インクルード ファイルに設定して利用します。  
> **資料：[本当にちょっとしたプログラミングTips.pdf][本当にちょっとしたプログラミングTips.pdf]**  
> **　　　［コーディングに関するTips］-［#defineマクロの活用］**  

####【算術系】####
* `<gasha/crc32.h>` ... **CRC32計算**  
> 
> `constexpr`, `ユーザー定義リテラル`によるメタプログラミング版と、SSE版にも対応しています。  
> **資料：[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]**  
> **　　　［メタプログラミング］-［constexprの活用：CRC値の算出］**  

####【アルゴリズム系】####
* **【分類：交換ソート】**  
* `<gasha/bubble_sort.h>` 【テンプレートライブラリ】**バブルソート**  
* `<gasha/shaker_sort.h>` 【テンプレートライブラリ】**シェーカーソート**  
* `<gasha/odd_even_sort.h>` 【テンプレートライブラリ】**奇遇転置ソート**  
* `<gasha/shear_sort.h>` 【テンプレートライブラリ】**シェアソート**  
* `<gasha/comb_sort.h>` 【テンプレートライブラリ】**コムソート**  
* `<gasha/gnome_sort.h>` 【テンプレートライブラリ】**ノームソート**  
* **【分類：分割交換ソート】**  
* `<gasha/quick_sort.h>` 【テンプレートライブラリ】**クイックソート**  
* **【分類：選択ソート】**  
* `<gasha/selection_sort.h>` 【テンプレートライブラリ】**選択ソート**  
* `<gasha/heap_sort.h>` 【テンプレートライブラリ】**ヒープソート**  
* **【分類：挿入ソート】**  
* `<gasha/insertion_sort.h>` 【テンプレートライブラリ】**挿入ソート**  
※配列以外のイテレータにも対応  
* `<gasha/shell_sort.h>` 【テンプレートライブラリ】**シェルソート**  
※配列以外のイテレータにも対応  
* **【分類：マージソート】**  
* `<gasha/inplace_merge_sort.h>` 【テンプレートライブラリ】**インプレースマージソート**  
* **【分類：混成ソート】**  
* `<gasha/intro_sort.h>` 【テンプレートライブラリ】**イントロソート**  
※配列以外のイテレータにも対応  
* **【分類：分布ソート】**※非比較ソート  
* `<gasha/radix_sort.h>` 【テンプレートライブラリ】**基数ソート**  
※外部ソート注意（内部で一時的にメモリ確保する）  
> 
> 配列もしくはコンテナのデータをソートします。
> **資料：[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> **　　　［データ構造とアルゴリズムによる最適化］-［ソートアルゴリズム］**  

* **【分類：探索】**  
* `<gasha/linear_search.h>` 【テンプレートライブラリ】**線形探索**  
※配列以外のイテレータにも対応  
* `<gasha/binary_search.h>` 【テンプレートライブラリ】**二分探索**  
※配列以外のイテレータにも対応  
> 
> 配列もしくはコンテナのデータを探索します。
> **資料：[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> **　　　［データ構造とアルゴリズムによる最適化］-［探索アルゴリズム］**  

####【スレッド系】####
* **【分類：ロック】**  
* `<gasha/spin_lock.h>` **スピンロック**  
* `<gasha/lw_spin_lock.h>` **サイズ軽量スピンロック**  
※通常版が4バイト、サイズ軽量版が1バイト。通常版の方が高速  
* `<gasha/dummy_lock.h>` **ダミーロック**  
> 
> スレッド間のロック制御を行います。  
> 一定回数のスピンでコンテキストスイッチを行います。  
> lock_guard（スコープロックパターン）に対応しています。  
> ダミーロックはロッククラスと同じインターフェースを備えますが、実際にロック制御しません。コンテナのロック制御を無効化する時に使用します。  
> **資料：[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> **　　　［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］**  

* **【分類：共有ロック】**  
* `<gasha/shared_spin_lock.h>` **共有スピンロック**  
* `<gasha/simple_shared_spin_lock.h>` **単純共有スピンロック**  
※通常版は排他ロックを優先して後続の共有ロックを待たせるのに対して、単純版は全て平等で軽量（共有ロックが込み合うと排他ロックが取得できなくなってしまう）  
* `<gasha/unshared_spin_lock.h>` **非共有スピンロック**  
* `<gasha/dummy_shared_lock.h>` **ダミー共有ロック**  
> 
> 共有ロック（リードロック）と排他ロック（ライトロック）を使い分けたロック制御を行います。  
> 共有ロックは複数のスレッドが同時に取得できますが、排他ロックは一つのスレッドしか取得できません。  
> 一定回数のスピンでコンテキストスイッチを行います。  
> shared_lock_guard（スコープロックパターン）に対応しています。  
> ダミー共有ロックは共有ロッククラスと同じインターフェースを備えますが、実際にロック制御しません。コンテナのロック制御を無効化する時に使用します。  
> 同様に、非共有ロックは共有ロッククラスと同じインターフェースを備えますが、実際には全て排他ロックで処理します。  
> **資料：[プログラム最適化Tips.pdf][プログラム最適化Tips.pdf]**  
> **　　　［データ構造とアルゴリズムによる最適化］-［マルチスレッドを最適化するアルゴリズム］**  

[本当にちょっとしたプログラミングTips.pdf]: https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E6%9C%AC%E5%BD%93%E3%81%AB%E3%81%A1%E3%82%87%E3%81%A3%E3%81%A8%E3%81%97%E3%81%9F%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips.pdf
[効果的なテンプレートテクニック.pdf]:       https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E5%8A%B9%E6%9E%9C%E7%9A%84%E3%81%AA%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF.pdf
[プログラム最適化Tips.pdf]:                 https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%A0%E6%9C%80%E9%81%A9%E5%8C%96Tips.pdf

ライブラリ特記事項
--------
ソースファイルは、Visual C++とGCCの互換性のために、下記の仕様で統一しています。
* 文字コード ... UTF8（BOM付き）
* 改行コード ... LF

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
           |     |                   ※ライブラリファイル名の競合を避けるため
           |     |- *.h/*.inl                   ... 各種インクルードファイル
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
           |                                        ※x86版Cygwinで開発したため、x64版は現状なし
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
        `-[gasha_settings]       ... サブモジュール：ライブラリ挙動設定用リポジトリ
           |
           `-[include]           ... ライブラリインクルードファイル用
              |
              `-[gasha]          ... ネームスペースディレクトリ
                 |                   ※ライブラリファイル名の競合を避けるため
                 `-[build_settings] ... ビルド設定用
                    |
                    `-[project]     ... プロジェクト固有設定用
                       |
                       |- first_settings.h      ... プロジェクト固有のライブラリ設定（先行設定）
                       |                            ※ネームスペース設定など
                       `- last_setteings.h      ... プロジェクト固有のライブラリ設定
                                                    ※SSE有効化設定など

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
        |-[vc]                   ... Visual C++でビルドした実行ファイルの置き場
        |  |
        |  |- run.bat                           ... x86＆x64全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |
        |  |-[x86]               ... x86(32bit)向け
        |  |  |
        |  |  |-（サンプル名）.exe              ... サンプルプログラム実行ファイル
        |  |  |- run.bat                        ... リリースビルド＆デバッグビルド全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |  |
        |  |  |-[bat]            ... サンプルプログラム実行と実行ログ記録用
        |  |  |  |
        |  |  |  |-（サンプル名）.bat           ... サンプルプログラム個別実行＆実行ログ記録用バッチファイル
        |  |  |  |- run.bat                     ... 全サンプルプログラム一括実行＆実行ログ記録用バッチファイル
        |  |  |  |
        |  |  |  `-[log]
        |  |  |     |
        |  |  |     `-（サンプル名）.log        ... サンプルプログラム個別実行ログファイル
        |  |  |
        |  |  `-[Debug]          ... デバッグビルド用
        |  |     |-...               ※親フォルダ（リリースビルド用）と同様の構成
        |  |
        |  `-[x64]               ... x64(64bit)向け
        |     |- ...                 ※x86版と同様の構成
        |
        `-[gcc]                  ... GCCでビルドした実行ファイルの置き場
           |
           `-[x86]               ... x86(32bit)向け
              |- ...                 ※Visual C++用と同様の構成
                                     ※シェルスクリプトの代わりにバッチファイル使用
                                     ※x86版Cygwinで開発したため、x64版は現状なし

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
        `- mk.sh                                ... GCC用ライブラリ＆全サンプルプログラムビルドシェルスクリプト

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

1. **ライブラリとライブラリ挙動設定リポジトリをプロジェクトに配置**  
> サブモジュールとして、ライブラリ用のリポジトリを配置して下さい。  
> > `project/sub/gasha` ... ライブラリ本体用リポジトリ(https://github.com/gakimaru/gasha)  
> > `project/sub/gasha_settings` ... ライブラリ挙動設定用リポジトリ(https://github.com/gakimaru/gasha_settings)  
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
> > ※x86版Cygwinで開発したため、x64版は現状なし  

4. **【推奨】強制インクルードとプリコンパイル済みヘッダーを設定**  
> プラットフォーム／言語設定を暗黙的に全ソースファイルに反映させるために、「強制インクルード」を使用することを推奨します。  
> 更に、強制インクルードファイルを「プリコンパイル済みヘッダー」にすることで、コンパイル速度を高速化することを、合わせて推奨します。  
> 
> **【設定例：standard.h】**  
> > プロジェクトファイル（*.vcxproj, Makefile）と同じディレクトリに standard.h を配置して下さい。  
> > `stadard.h`の内容（この一行のみ）  
> > > `#include <gasha/build_settings/build_settings.h>`  
> 
> **【設定例：standard.cpp】** ※Visual C++のみ必要  
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
        |
        |-[example_specify_environment] ... 環境判別処理テスト
        |-[example_crc32]               ... CRC32計算処理テスト
        |-[example_sort_and_search]     ... ソート＆探索処理テスト

========
■■以上
