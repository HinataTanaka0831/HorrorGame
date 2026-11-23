#pragma once
#include <vector>
#include <string>

class CsvLoader
{
public:
	// 各CSVファイルを読み込む
	// 入力: fileName(CSVファイルの名前) / 出力: date(読み取ったCSVファイルのコンテナを格納したコンテナ) / 副作用: なし
	static std::vector<std::vector<std::string>>
		Load(std::string fileName);
};