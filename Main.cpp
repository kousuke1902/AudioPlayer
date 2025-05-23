# include <Siv3D.hpp> // OpenSiv3D v0.6.15


//音声ファイル判定
bool IsAllAudio(const FilePath& path)
{
	String extension = FileSystem::Extension(path);

	return (extension == U"wav" || extension == U"mp3" || extension == U"m4a" || extension == U"ogg" ||
		extension == U"opus" || extension == U"mid" || extension == U"wma" || extension == U"flac" ||
		extension == U"aif" || extension == U"aiff" || extension == U"aifc");

}



void Main()
{
	//ウィンドウ名
	Window::SetTitle(U"Light Audio Player ver.1.0.0");
	// 背景の色を設定 | Set background color
	Scene::SetBackground(Palette::Midnightblue);

	// 通常のフォントを作成 | Create a new font
	const Font font30{ 30 };
	const Font font20{ 20 };

	//オーディオ
	Audio audio;

	//ボリューム
	double volume = 0.3;

	//再生位置変更の有無
	bool seeking = false;

	//ファイルパス
	Optional<FilePath> filepath;

	//ディレクトリネーム
	Optional<FilePath> directorypath;

	//音楽ファイルリスト
	Array<Optional<FilePath>> filenamelist;

	//ファイルネーム
	String filename;
	String nextfilename;

	//設定ファイル
	INI ini{U"startup.ini"};

	//ディスク加速度
	double deltadisc = 0;

	//ディスク位置
	double discpos = 0;

	//ファイル番号
	size_t filenum = 0;

	//前回のファイルと再生位置をロード
	if (not ini)
	{		

	}

	else
	{
		filepath = Parse<FilePath>(ini[U"Audio.FilePath"]);
		directorypath = FileSystem::ParentPath(*filepath);
		filename = FileSystem::BaseName(*filepath);
		audio = Audio{ Audio::Stream, *filepath };
		audio.seekSamples(Parse<size_t>(ini[U"Audio.Time"]));
		volume = Parse<double>(ini[U"Volume.Volume"]);

		//フォルダ内の音楽ファイルを取得
		for (const auto& path : FileSystem::DirectoryContents(*directorypath, Recursive::No).filter(IsAllAudio))
		{
			filenamelist << path;
			if (filenamelist.back() == *filepath) filenum = filenamelist.size() - 1;
		}

		//次の曲取得
		if (filenamelist.size() > filenum + 1)nextfilename = FileSystem::BaseName(*filenamelist[filenum + 1]);
		else nextfilename = U"End of Audio";
	}


	while (System::Update())
	{
		//リズム数値
		const double rythm = Periodic::Sawtooth0_1(0.4s) * 5 * audio.isPlaying();

		//オーディオタイトル
		font30(U"♪", filename).draw(20, 25, Palette::White);
		font20(U"Next→", nextfilename).draw(80, 60, Palette::White);

		//ディスク加速
		discpos += deltadisc;

		//四角枠
		RoundRect{ Arg::center(400, 285), 330 + rythm, 330 + rythm, 10 }.draw(Palette::Black);
		Circle{ 265 - rythm, 150 - rythm, 6 }.draw();
		Circle{ 535 + rythm, 150 - rythm, 6 }.draw();
		Circle{ 265 - rythm, 420 + rythm, 6 }.draw();
		Circle{ 535 + rythm, 420 + rythm, 6 }.draw();

		//ディスク回転
		Circle{ 400, 285, 150 + rythm}.drawArc(0_deg + discpos, 160_deg, 120, 0, Palette::Gold).drawArc(160_deg + discpos, 20_deg, 120, 0, Palette::White)
			.drawArc(180_deg + discpos, 180_deg, 120, 0, Palette::Gold).drawArc(340_deg + discpos, 21_deg, 120, 0, Palette::White);
		Circle{ 400, 285, 40 }.drawFrame(15, 0, Palette::Goldenrod);

		//スピーカー
		Circle{ 120, 330, 100 + rythm }.draw(Palette::Black).drawFrame(15, 0);
		Circle{ 680, 330, 100 + rythm }.draw(Palette::Black).drawFrame(15, 0);
		Circle{ 120, 330, 50 + rythm }.draw();
		Circle{ 680, 330, 50 + rythm }.draw();


		//再生時間
		const String time = FormatTime(SecondsF{ audio.posSec() }, U"M:ss") + U'/' + FormatTime(SecondsF{ audio.lengthSec() }, U"M:ss");

		//プログレスバーの進行度
		double progress = static_cast<double>(audio.posSample()) / audio.samples();

		//再生中か否か
		if (audio.isPlaying())
		{

			//一時停止
			if (SimpleGUI::Button(U"Pause", Vec2{ 40, 500 }, 120)) audio.pause(0.2s);

			//ディスク加速
			if (deltadisc < 1.0)deltadisc += 0.5 * Scene::DeltaTime();



			//次曲自動再生
			if (audio.lengthSec() - 0.125 <= audio.posSec())
			{

				audio.pause(0.5s);

				//次の曲
				if (filenamelist.size() > filenum + 1)
				{
					filenum++;
					filepath = *filenamelist[filenum];
					audio = Audio{ Audio::Stream, *filepath };
					filename = FileSystem::BaseName(*filepath);
					if (filenamelist.size() > filenum + 1)nextfilename = FileSystem::BaseName(*filenamelist[filenum + 1]);
					else nextfilename = U"End of Audio";
				}
				

				audio.play(0.2s);
				audio.setVolume(volume);

			}

		}

		else
		{
			//再生
			if (SimpleGUI::Button(U"Play", Vec2{ 40, 500 }, 120, !audio.isEmpty()))
			{
				audio.play(0.2s);
				audio.setVolume(volume);
			}

			//ディスク減速
			if (deltadisc > 0)deltadisc -= 0.25 * Scene::DeltaTime();
			else if (deltadisc < 0)deltadisc = 0;
				
			
		}

		//停止
		if (SimpleGUI::Button(U"Stop", Vec2{ 170, 500 }, 120))audio.stop();


		//フォルダから音楽ファイルを開く
		if (SimpleGUI::Button(U"Open", Vec2{300, 500}, 120))
		{

			audio.pause(0.5s);
			//音楽ファイルのファイルパス
			if (Optional<FilePath> buf = Dialog::OpenFile({ FileFilter::AllAudioFiles(), FileFilter::WAVE(), FileFilter::MP3(), FileFilter::AAC(), FileFilter::OggVorbis(), FileFilter::Opus(),
				FileFilter::MIDI(), FileFilter::WMA(), FileFilter::WMA(),FileFilter::FLAC(), FileFilter::AIFF() }))
			{
				filepath = buf;
				audio = Audio{ Audio::Stream, *filepath };
				directorypath = FileSystem::ParentPath(*filepath);
				filename = FileSystem::BaseName(*filepath);

				filenamelist.clear();

				//フォルダ内の音楽ファイルを取得
				for (const auto& path : FileSystem::DirectoryContents(*directorypath, Recursive::No).filter(IsAllAudio))
				{
					filenamelist << path;
					if (filenamelist.back() == *filepath) filenum = filenamelist.size() - 1;
				}

				//次の曲
				if (filenamelist.size() > filenum + 1)nextfilename = FileSystem::BaseName(*filenamelist[filenum + 1]);
				else nextfilename = U"End of Audio";
			}			
			//audio = Dialog::OpenAudio();
			audio.play(0.2s);
			audio.setVolume(volume);

		}

		//ボリューム操作
		if (SimpleGUI::Slider(U"{:3.0f}"_fmt(volume * 100), volume, Vec2{430, 500}, 40, 290)) audio.setVolume(volume);


		//スライダー操作
		if (SimpleGUI::Slider(time, progress, Vec2{ 40, 540 }, 130, 590, !audio.isEmpty()))
		{

			audio.pause(0.05s);

			//停止まで待機
			while (audio.isPlaying())System::Sleep(0.01s);

			//再生位置変更
			audio.seekSamples(static_cast<size_t>(audio.samples() * progress));

			seeking = true;

		}

		//再生再開
		else if (seeking && MouseL.up())
		{

			audio.play(0.05s);
			seeking = false;
			audio.setVolume(volume);
		}


	}

	if (audio.isPlaying())
	{
		audio.fadeVolume(0.0, 0.3s);
		System::Sleep(0.3s);
	}

	//閉じたときの状況のセーブ
	ini.addSection(U"Audio");
	ini.addSection(U"Volume");

	if (audio.isEmpty())
	{
		ini.write(U"AudioData", U"FilePath", U"none");
		ini.write(U"AudioData", U"Time", 0);

	}

	else
	{

		ini.write(U"Audio", U"FilePath", *filepath);
		ini.write(U"Audio", U"Time", audio.posSample());
	}

	ini.write(U"Volume", U"Volume", volume);

	ini.save(U"startup.ini");

}


