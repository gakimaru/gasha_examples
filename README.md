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

**名前の由来**  
> 名前の由来は「[がしゃどくろ][GASHADOKURO]」から。  
> その「骨格」的な意味合いに、自身のハンドル名「Gakimaru」の頭文字「GA」を重ねて命名しました。
[GASHADOKURO]: http://ja.wikipedia.org/wiki/%E3%81%8C%E3%81%97%E3%82%83%E3%81%A9%E3%81%8F%E3%82%8D

ライブラリ概要
---
####【環境系】####
* `<build_settings.h>`  
    ビルド環境／言語に応じた自動環境設定。  

> コンパイラの種類やバージョンを判別し、必要に応じて `nullptr`, `override`, `alignas`, `thread_local` などのC++11仕様に合わせた処理の独自実装版を有効化し、コード互換性の向上に寄与します。  
> 同様に、`__FUNCTION__`, `__PRETTY_FUNCTION__` なども各コンパイラで共通利用可能にします。  
> 通常このヘッダーは、強制インクルード ファイルに設定して利用します。  
> 資料：[本当にちょっとしたプログラミングTips.pdf][本当にちょっとしたプログラミングTips.pdf]  
[本当にちょっとしたプログラミングTips.pdf]: https://www.dropbox.com/sh/xh1tauzgqhkk7hy/AABn6TJ8yr9B7Kv4Fd1lHY3fa/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E6%9C%AC%E5%BD%93%E3%81%AB%E3%81%A1%E3%82%87%E3%81%A3%E3%81%A8%E3%81%97%E3%81%9F%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips.pdf

####【算術系】####
* `<crc32>`  
    CRC32計算。  

> `constexpr`, `ユーザー定義リテラル`によるメタプログラミング版と、SSE版にも対応します。  
> 資料：[効果的なテンプレートテクニック.pdf][効果的なテンプレートテクニック.pdf]  
[効果的なテンプレートテクニック.pdf]: https://www.dropbox.com/sh/xh1tauzgqhkk7hy/AAB8EiCPNGy9sgfRccpk7AG0a/%E4%BB%95%E6%A7%98%E3%83%BB%E8%A8%AD%E8%A8%88%E6%9B%B8/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0Tips/%E5%8A%B9%E6%9E%9C%E7%9A%84%E3%81%AA%E3%83%86%E3%83%B3%E3%83%97%E3%83%AC%E3%83%BC%E3%83%88%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF.pdf

対象コンパイラ
---
**C++11仕様に対応したコンパイラが必要です。**  
開発には下記のコンパイラを用いています。  
* Visual Studio 2013  
* GCC 4.7  

> 「可変長引数テンプレート」を削除するなど、わずかな改変でC++11以前のコンパイラにも適用できるものが多数あります。  
> 逆に言えば、C++11仕様にさえ対応していれば、上記以外のコンパイラでもほぼそのまま利用できます。  

サンプルプログラム実行手順
---
1. gitでgasha_examplesリポジトリのクローンを作成  
> $ git clone https://github.com/gakimaru/gasha_examples.git

2. サブモジュールのアップデート  
> $ cd gasha_examples  
> $ git submodule update --init  

3. 【Visual Studioの場合】  
proj/gasha_examples.sln を開き、[ビルド]→[バッチビルド]メニューを実行し、
すべての（もしくは任意の）プロジェクト／構成／プラットフォームを選んでビルドを実行してください。  

3. 【Unix系環境の場合】  
proj/mk.sh を実行してください。  
> $ cd proj  
> $ ./mk.sh  

4. 【Visual Studioの場合】  
`gasha_examples\exe\vc\`以下のフォルダから、`（サンプル名）.exe`を実行することで、サンプルプログラムを実行可能です。  
`gasha_examples\exe\vc\`以下のフォルダには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのバッチファイルも多数用意しています。  
また、Visual Studio 上から、「スタートアッププロジェクト」を設定すれば、デバッグ実行することも可能です。  

4. 【Unix系環境の場合】  
`gasha_examples\exe\gcc\`以下のフォルダから、`（サンプル名）`の実行ファイルを実行することで、サンプルプログラムを実行可能です。  
`gasha_examples\exe\gcc\`以下のフォルダには、サンプルプログラムをまとめて実行し、結果をログファイルに記録するためのシェルスクリプトも多数用意しています。  

ライブラリの利用手順
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
