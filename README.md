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
> 
> **関連リポジトリ**
> * [`gasha` ライブラリ本体用リポジトリ][gasha]  
> * [`gasha_settings` ライブラリ挙動設定用リポジトリ][gasha_settings]  
> * [`gasha_src` ライブラリソース用リポジトリ][gasha_src]  
> * [`gasha_examples` サンプルプログラム用リポジトリ][gasha_examples]  
> * [`gasha_proj` ライブラリビルド用リポジトリ][gasha_proj]  
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

**基本的なアルゴリズム、コンテナ、メモリ管理、デバッグ機能などを用意しています。**  
> 他、メタプログラミング向けのテンプレートも用意しています。  
> 個人的な学習・調査・研究に応じて、気まぐれに追加します。  

**名前の由来**  
> 名前の由来は「[がしゃどくろ][GASHADOKURO]」から。  
> その「骨格」的な意味合いに、自身のハンドル名「Gakimaru」の頭文字「GA」を重ねて命名しました。
[GASHADOKURO]: http://ja.wikipedia.org/wiki/%E3%81%8C%E3%81%97%E3%82%83%E3%81%A9%E3%81%8F%E3%82%8D

ライブラリ概要
--------
####【環境系】####
* `<build_settings.h>`  
    **ビルド環境／言語に応じた自動環境設定です。**  
    
    > コンパイラの種類やバージョンを判別し、必要に応じて `nullptr`, `override`, `alignas`, `thread_local` などのC++11仕様に合わせた処理の独自実装版を有効化し、コード互換性の向上に寄与します。  
    > 同様に、`__FUNCTION__`, `__PRETTY_FUNCTION__` なども各コンパイラで共通利用可能にします。  
    > 通常このファイルは、強制インクルード ファイルに設定して利用します。  
    > **資料：[本当にちょっとしたプログラミングTips.pdf][本当にちょっとしたプログラミングTips.pdf]**  
    > **　　　［コーディングに関するTips］-［#defineマクロの活用］**  
[本当にちょっとしたプログラミングTips.pdf]: https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E6%9C%AC%E5%BD%93%E3%81%AB%E3%81%A1%E3%82%87%E3%81%A3%E3%81%A8%E3%81%97%E3%81%9F%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips.pdf

####【算術系】####
* `<crc32>`  
    **CRC32計算用です。**  
    
    > `constexpr`, `ユーザー定義リテラル`によるメタプログラミング版と、SSE版にも対応しています。  
    > **資料：[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]**  
    > **　　　［メタプログラミング］-［constexprの活用：CRC値の算出］**  
[効果的なテンプレートテクニック.pdf]: https://github.com/gakimaru/public/blob/master/document/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E5%8A%B9%E6%9E%9C%E7%9A%84%E3%81%AA%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF.pdf

対象コンパイラ
--------
**C++11仕様に対応したコンパイラが必要です。**  
開発には下記のコンパイラを用いています。  
* Visual Studio 2013  
* GCC 4.7  

> 「可変長引数テンプレート」を削除するなど、わずかな改変でC++11以前のコンパイラにも適用できるものが多数あります。  
> 逆に言えば、C++11仕様にさえ対応していれば、上記以外のコンパイラでも、ほとんど改変することなく利用可能です。  

基本ディレクトリ構成とサブモジュール構成
--------
####サンプルリビルド用リポジトリ####
    [gasha_examples]       ... サンプルプログラム用リポジトリ
     |  
     |-[exe]               ... 実行ファイル用
     |-[proj]              ... 各サンプルプログラムのビルドプロジェクト用
     |-[src]               ... 各サンプルプログラムのソースファイル用
     `-[sub]               ... サブモジュール用
        |
        |-[gasha]          ... ライブラリ本体(https://github.com/gakimaru/gasha)
        |-[gasha_settings] ... ライブラリ挙動設定(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース(https://github.com/gakimaru/gasha_src)

####ライブラリビルド用リポジトリ####
    [gasha_proj]           ... ライブラリビルド用リポジトリ
     |  
     |-[proj]              ... ライブラリビルドプロジェクト用
     `-[sub]               ... サブモジュール用
        |
        |-[gasha]          ... ライブラリ本体(https://github.com/gakimaru/gasha)
        |-[gasha_settings] ... ライブラリ挙動設定(https://github.com/gakimaru/gasha_settings)
        `-[gasha_src]      ... ライブラリソース(https://github.com/gakimaru/gasha_src)

サンプルプログラム実行手順
--------
1. `git`で`gasha_examples`リポジトリのクローンを作成  
> $ git clone https://github.com/gakimaru/gasha_examples.git

2. サブモジュールのアップデート  
> $ cd gasha_examples  
> $ git submodule update --init  
> 
> これにより、サブモジュールとして配置された三つのリポジトリクローンが更新されます。  
> > `sub/gasha` ... ライブラリ本体  
> > `sub/gasha_settings` ... ライブラリ挙動設定  
> > `sub/gasha_src` ... ライブラリソース

3. ライブラリのビルド【Visual Studioの場合】  
> `proj/gasha_examples.sln` を開き、`[ビルド]→[バッチビルド]`メニューを実行し、すべての（もしくは任意の）プロジェクト／構成／プラットフォームを選んでビルドを実行してください。  

4. ライブラリのビルド【Unix系環境+GCCの場合】  
> proj/mk.sh を実行してください。  
> > $ cd proj  
> > $ ./mk.sh  

5. サンプルプログラムの実行【Visual Studioの場合】  
> `exe/vc/`以下のディレクトリから、`（サンプル名）.exe`を実行することで、サンプルプログラムを実行可能です。  
> `exe/vc/`以下のディレクトリには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのバッチファイルも多数用意しています。  
> また、Visual Studio 上から、「スタートアッププロジェクト」を設定すれば、デバッグ実行することも可能です。  

6. サンプルプログラムの実行【Unix系環境+GCCの場合】  
> `exe/gcc/`以下のディレクトリから、`（サンプル名）`の実行ファイルを実行することで、サンプルプログラムを実行可能です。  
> `exe/gcc/`以下のディレクトリには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのシェルスクリプトも多数用意しています。  

ディレクトリ構成
--------
####サブモジュール／ライブラリ本体####
    [gasha_examples/gasha_proj]  ... ライブラリサンプル／ビルドプロジェクト用リポジトリ
     |  
     |-[sub]                     ... サブモジュール用
        |
        `-[gasha]                ... サブモジュール：ライブラリ用リポジトリ
           |
           |-[include]           ... インクルードファイル用
           |  |-...
           |
           |-[lib]               ... ライブラリファイル用
           |  |
           |  |-[gcc]            ... GCC用
           |  |  |
           |  |  |- gasha_x86.a
           |  |  `- gasha_x86_debug.a
           |  |
           |  `-[vc]             ... Visual C++用
           |     |
           |     |- gasha_x86.lib
           |     |- gasha_x86_debug.lib
           |     |- gasha_x64.lib
           |     `- gasha_x64_debug.lib
           |
           `-[proj]
              |
              `- Makefile_common ... GCC用メイクファイルの共通部分
                                     （ライブラリ／サンプルプログラム共用）

####サブモジュール／ライブラリ挙動設定####
    [gasha_examples/gasha_proj]  ... ライブラリサンプル／ビルドプロジェクト用リポジトリ
     |  
     |-[sub]                     ... サブモジュール用
        |
        `-[gasha_settings]       ... サブモジュール：ライブラリ挙動設定用リポジトリ
           |
           `-[include]           ... ライブラリインクルードファイル用
              |
              |- project_build_settings_first.h ... プロジェクト固有のライブラリ設定（先行設定）
              |                                     ※ネームスペース設定など
              `- project_build_settings.h       ... プロジェクト固有のライブラリ設定
                                                    ※SSE有効化設定など

####サブモジュール／ライブラリソース####
    [gasha_examples/gasha_proj]  ... ライブラリサンプル／ビルドプロジェクト用リポジトリ
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
    [gasha_examples]             ... ライブラリサンプル用リポジトリ
     |  
     |-[exe]                     ... 実行ファイル用
        |
        |-[gcc]                  ... GCCでビルドした実行ファイルの置き場
        |  |
        |  `-[x86]               ... x86(32bit)向け
        |     |
        |     |-[sh]             ... サンプル実行と実行ログ記録用
        |     |  |
        |     |  |-[log]
        |     |  |  |
        |     |  |  `-（サンプル名）.log
        |     |  |
        |     |  |-（サンプル名）.sh
        |     |  |
        |     |  `- run.sh                      ... 全サンプル実行用シェルスクリプト
        |     |
        |     |-（サンプル名）.exe
        |     |
        |     `-[Debug]          ... デバッグビルド用
        |        |- ...
        |
        `-[vc]                   ... Visual C++でビルドした実行ファイルの置き場
           |                         ※GCC用と同様の構成
           |                         ※シェルスクリプトの代わりにバッチファイル使用
           |
           |-[x86]               ... x86(32bit)向け
           |  |- ...
           |
           `-[x64]               ... x64(64bit)向け
              |- ...

####各サンプルプログラムのソース####
    [gasha_examples]             ... ライブラリサンプル用リポジトリ
     |  
     |-[src]                     ... サンプルプログラムソースファイル用
        |
        |-[（サンプル名）]       ... 各サンプルプログラムソースファイル用
        |  |
        |  |- *.cpp/.h

####ライブラリおよび各サンプルプログラムのビルドプロジェクト####
    [gasha_examples]             ... ライブラリサンプル用リポジトリ
     |  
     |-[proj]                    ... ライブラリ／サンプルプログラムビルドプロジェクト用
        |
        |-[（サンプル名）]       ... 各サンプルプログラムビルドプロジェクト用
        |  |
        |  |- main.cpp                          ... サンプル用メイン関数
        |  |- standard.h/.cpp                   ... サンプル用強制インクルード／プリコンパイル済みヘッダー
        |  |
        |  |- （サンプル名）.vcxproj            ... Visual C++用サンプルビルドプロジェクト
        |  |- Makefile                          ... GCC用サンプルビルドメイクファイル
        |  `- mk.sh                             ... GCC用サンプルビルドシェルスクリプト
        |
        |- Makefile_example_common              ... GCC用サンプルメイクファイルの共通部分
        |
        |- gasha_examples.sln                   ... Visual C++用ライブラリ＆全サンプルビルドソリューション
        `- mk.sh                                ... GCC用ライブラリ＆全サンプルビルドシェルスクリプト

####ライブラリおよび各サンプルプログラムのビルドプロジェクト####
    [gasha_proj]                 ... ライブラリビルドプロジェクト用リポジトリ
     |  
     |-[proj]                    ... ライブラリビルドプロジェクト用
        |
        |- gasha.sln                            ... Visual C++用ライブラリビルドソリューション
        `- mk.sh                                ... GCC用ライブラリビルドシェルスクリプト

ライブラリの利用手順
--------
1. xxx
> xxx

2. xxx
> xxx

サブモジュールのバージョン更新方法
--------
xxx

■■以上
