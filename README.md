# AudioPlayer
前回の再生位置を記録できることを目的とした簡易的なオーディオプレイヤー  
ASMRとか聴いているときにオーディオプレイヤーを閉じると前回の再生位置を記録してるわけではないから
その位置まで聴きながら粘るか朧げな記憶を掘り起こしておおよその再生位置まで戻すかという事をしていたので
「癒されたいのに作業なんかしてられるか！」という事で前回の再生した音源と位置を記録する
オーディオプレイヤーを作りました．  


# 使い方
* Play/Pauseボタン 読んで字のごとく再生されている音声ファイルを一時停止または再生します  
* Stopボタン 再生中一時停止中に関わらず停止させ再生位置を最初に戻します．ノリで付けたけどそんなに使うことはなさそう．  
* Open ダイアログを開きます．再生したい音声ファイルを選択すると再生します．ストリーミング再生なので選択から再生まで結構早いです．  
 * 再生可能な音声ファイル WAVE(.wav) MP3(.mp3) AAC(.m4a) OggVorbis(.ogg) Opus(.opus) MIDI(.mid) WMA(.wma/Windosのみ) FLAC(.flac/Windows・Macのみ) AIFF(.aif/.aiff/.aifc/Macのみ)  
* ボリュームスライダー Openボタン右横のスライダー．動かすと音量を調整できます．設定したボリュームはアプリを閉じても記録されます．あると便利だから記録機能付けた．  
* タイムラインスライダー Play/Pauseボタン下のスライダー．現在の再生時間と全体の時間を表示．スライダーは相対的な位置を表示すると同時に動かすと再生位置を変更できます．  
* 名前表記 左上の♪横に現在再生中の音声ファイルの名前を表記します．めちゃくちゃ長い名前だとたぶんはみ出す  
* 次曲表記 左上のNext→の隣に次に再生する音声ファイルの名前を表記します．めちゃk(ry 次がない場合にはEnd of Audioと表記します．EOFになぞらえて  
* 次曲自動再生 今再生中の音声ファイルが最終点まで到達すると自動で次曲を再生します．ない場合は停止します．
* 記録機能 アプリケーションを閉じた際に最後に聴いていた音声ファイル，再生位置，音量を記録します．再度アプリケーションを起動した際に記録した情報から再生できるようになります．一番つけたかった機能．満足．
* おまけ 再生中はCDはぐるぐる回り，スピーカーがリズムを刻みます．  


# 開発環境
Microsoft Visual Studio Community 2022 Version 17.14.2  
Visual C++ 2022
OpenSiv3D 0.6.15  

あらゆる機能の実装にはOpenSiv3Dで実現しています．  
OpenSiv3Dに，Ryo Suzukiさんに感謝  
[OpenSiv3Dホームページ](https://siv3d.github.io/ja-jp/)
