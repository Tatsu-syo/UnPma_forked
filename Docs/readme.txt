PMA形式書庫 統合アーカイバ仕様展開 DLL
UnPma32.DLL Version 0.02 Visual Studio 2008 build

Tatsuhiko Shoji 2022

[本プログラムについて]
本プログラムはぺぱあみんと★すたあ氏がCP/M用に作成した圧縮ソフトPMarcで
圧縮した書庫を展開するための統合アーカイバ仕様準拠DLLです。

本プログラムはHAL "Minthe" Pussywillow氏が作成したUnPma32.DLLを
Visual Studio 2008でコンパイルできるようforkし、コマンドライン
展開プログラムをつけたものとなります。

本fork版は原作のUnPma32.DLLがWindowsセキュリティ等でウイルスとして
検知されることからVisual Studio 2008でコンパイルすることによって
これを回避するとともに、コマンドラインプログラムを作ることで
単体でも利用できるようにするため作成しました。
また、forkにあたり、元ソースから生成できないバイナリモジュール
および、時代遅れとなったプログラム用の設定を外しております。

よって、本プログラムはWindows 2000以降のWindowsで動作します。

[プログラム詳細について]
プログラム詳細については、同梱のUnPma32.txtをご覧ください。

[本fork版構成について]
SDKフォルダ
 Api.txt DLLのAPIの説明です。
 Api2.txt メッセージ・コールバック・使用する構造体についての説明です。
 Command.txt UnPma APIのコマンドラインの説明です。
 UnPma32.lib スタティックリンク用のライブラリです。
copying.txt GPL 2.0条文です。
UnPma32.dll UnPma32.dll本体です。
UnPma32.exe UnPma32.dllを使用してPMA形式書庫を展開するプログラムです。
readme.txt 本ドキュメントです。
src.zip ソースファイルです。
UnPma32.txt 原作のドキュメントです。
UnPma32e.txt 原作の英語版ドキュメントです。

[ライセンスについて]
本プログラムはHAL "Minthe" Pussywillow氏がMaarten ter Huurne氏の
UnPMAルーチンを元に書き下ろした時点で
ライセンスにGPL 2.0を採用しているので、
これに従い、GPL 2.0となります。

[fork者]
庄子 達彦(Tatsu)
E-mail Address : tatsu_syo@nifty.com
GitHub : https://github.com/Tatsu-syo
