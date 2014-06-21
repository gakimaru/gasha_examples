GASHA
====
**Gakimaru's researched and standard library for C++**  
Copyright (c) 2014 Itagaki Mamoru  
Released under the [MIT license][MIT].
<https://github.com/gakimaru/gasha_examples/blob/master/LICENSE>
[MIT]: http://www.opensource.org/licenses/mit-license.php

サンプルプログラム用リポジトリ
---
**このリポジトリは、「GASHA」のサンプルプログラム用です。**
> 多数のサンプルプログラムのビルドと実行が可能です。  
> 
> また、サブモジュールにより、ライブラリ用の各リポジトリを適
> 切な位置関係に配置しており、ライブラリファイルのビルド、
> および、ソースコードレベルでのライブラリのデバッグが可能です。

「GASHA」とは？
---
**個人制作による、オープンソースのC++用基本ライブラリです。**  
> 主にゲーム開発での利用を想定し、暗黙的なメモリ確保／解放を行わない点が特徴の一つです。  
> 高速性もしくは生産性を追求し、品質と開発効率の向上を目的とするものです。  

**現時点では、基本的なアルゴリズム、コンテナ、メモリ管理、デバッグ機能などを用意しています。**  
> 他、メタプログラミング向けのテンプレートも用意しています。  
> 個人的な学習・調査・研究に応じて、気まぐれに追加します。  

**【名前の由来】**  
> 名前の由来は「[がしゃどくろ][GASHADOKURO]」から。  
> その「骨格」的な意味合いに、自身のハンドル名「Gakimaru」の頭文字「GA」を重ねて命名しました。
[GASHADOKURO]: http://ja.wikipedia.org/wiki/%E3%81%8C%E3%81%97%E3%82%83%E3%81%A9%E3%81%8F%E3%82%8D

「GASHA」のライブラリ概要
---
**【環境系】**  
* `#include <build_settings.h>`  
    ビルド環境／言語に応じた自動環境設定。  
    alignas, thread_local などの統一仕様を利用可能にし、コードの互換性を高める。

**【算術系】**  
* `#include <crc32>`  
    CRC32計算。  
    メタプログラミング版、SSE版にも対応。

「GASHA」の対象コンパイラ
---
**C++11必須です。下記のいずれかのコンパイラを使用してください。**  
* Visual Studio 2013以降(Windows系)  
* GCC 4.7以降(Unix系)  

「GASHA」サンプルプログラム利用の手順
---
1. gitでgasha_examplesリポジトリのクローンを作成  
> $ git clone https://github.com/gakimaru/gasha_examples.git

2. サブモジュールのアップデート  
> $ cd gasha_examples  
> $ git submodule update --init  

3. 【Visual Studioの場合】  
proj/gasha_examples.sln を開き、[ビルド]→[バッチビルド]メニューを実行し、
すべての（もしくは任意の）プロジェクト／構成／プラットフォームを選んでビルドを実行  

3. 【Unix系環境の場合】  
proj/mk.sh を実行
> $ cd proj  
> $ ./mk.sh  

4. 【Visual Studioの場合】  
`gasha_examples\exe\vc\`以下のフォルダから、`（サンプル名）.exe`を実行することで、サンプルプログラムを実行可能。  
`gasha_examples\exe\vc\`以下のフォルダには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのバッチファイルも多数用意。  
また、Visual Studio 上から、「スタートアッププロジェクト」を設定して実行することも可能。  

4. 【Unix系環境の場合】  
`gasha_examples\exe\gcc\`以下のフォルダから、`（サンプル名）`の実行ファイルを実行することで、サンプルプログラムを実行可能。  
`gasha_examples\exe\gcc\`以下のフォルダには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのシェルスクリプトも多数用意。  

「GASHA」をプロジェクトに組み込んで利用する手順
---

基本的なフォルダ構成とサブモジュールの構成
---
xxx

サブモジュールのバージョン更新方法
---
xxx

サンプルプログラムの構成
---
xxx

■■以上
