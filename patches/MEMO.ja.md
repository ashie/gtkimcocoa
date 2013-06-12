translate-JIS-Hiragana-Eisu-keys.patchバグ報告用メモ
====================================================

問題点
------
かなキーや英数キーを押すと、ウィジェットによって以下のいずれかの
問題が発生する。imquartz、GtkIMCocoaいずれでも再現する。

* 期待しないGtkEntryのactivateシグナルが発動されてしまう(GtkEntry)
  アプリ側でこのシグナルを契機に何か処理をしていると、問題が顕在化する。
  (Sylpheedのメール作成ウィンドウではフォーカス移動を行う)
* 期待しないスペースが入力されてしまう。
  (Xamarin Studioのテキストエディタウィジェット)

再現方法
--------
* JISキーボードが無い場合
  * KeyRemap4MacBookをインストール
  * 「For Japanese」の以下の2つにチェックを入れる
    (チェック一つで両方を有効化できる設定もあるが、英語ロケールでも項目名が
    なぜか日本語なので、日本人以外にも伝え易い以下を使用)
  * 「Change Command_L Key」->
    「Command_L to Command_L (+ When you type Command_L only, send EISU)」
  * 「Change Command_R Key」->
    「Command_R to Command_R (+ When you type Command_R only, send KANA)」
* testbet-gtk2 or testbed-gtk3を起動する
* エントリで「英数」キーを押す。JISキーボードが無い場合は「左コマンドキー」。
  * 期待する結果: コンソールに「keyval: Eisu_toggle」と表示される。
    (JISキーボードが無い場合は「Meta_L」も表示される。)
    また、コンソールに「activate」が表示されない。
  * 実際の結果: コンソール「keyval: space」と表示される。
    また、コンソールに「activate」が表示される。
* エントリで「かな」キーを押す。JISキーボードが無い場合は「右コマンドキー」。
  * 期待する結果: コンソールに「keyval: Hiragana」と表示される
    (JISキーボードが無い場合は「Meta_R」も表示される。)
    また、コンソールに「activate」が表示されない。
  * 実際の結果: コンソール「keyval: space」と表示される
    また、コンソールに「activate」が表示される。

原因
----
* 今のGdkQuartzではかなキーや英数キーがGDK_spaceに変換されてしまう
* 今のimquartzでは、以下のような処理になっている:
  * 通常のGDK_spaceの場合はGtkIMContext内で処理し、スペース文字を出力して、キー
    イベントをストップさせる
  * かなキーや英数キーは上記の問題によりキーコードとしてはGDK_spaceだが、
    imquartzではhardware_keycodeで通常のスペースと見分けたうえで、ハンドルせず
    に(スペース文字を出力しないで)returnしている。
    (GtkIMCocoaの場合は見分けていないが、結果的にハンドルしないのは同じ)
* 後者の場合、ウィジェットにイベントが返される。その時の動作はウィジェットの実
  装によって異なる。
  * ウィジェットによってそのイベントがハンドルされない場合は、トップレベルウィ
    ンドウのGtkWidgetクラスにチェーンアップされ、GtkBindingで定義されたキーバイ
    ンドとのマッチングが行われる。デフォルトではスペースがactivateにバインドさ
    れているため、activateシグナルが発動してしまう。
    (かなキーや英数キーはスペースキーではないため、これに反応すべきでは無い)
  * Xamarin Studioでは自力でハンドルしてスペース文字を出力しているものと思われ
    る。

修正案
------
* TODO1: パッチの内容をまとめる
* TODO2: imquartz内のhardware_keycode見てる部分を消す。いらないはず。
