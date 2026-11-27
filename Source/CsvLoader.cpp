#include "CsvLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<std::vector<std::string>> CsvLoader::Load(const std::string& fileName)
{
	std::vector<std::vector<std::string>> data;
	std::ifstream file(fileName);       // CSVファイルを読み込む
	if (!file.is_open()) return data;   // CSVファイルが読み込めないなら終了
	std::string line;
	std::getline(file, line);      // 1行目を捨てる
	// CSVファイルを1行ずつ読み込む
	while (std::getline(file, line)) {
		std::stringstream ss(line);     	// 1行のCSVデータを解析するためのストリームを生成
		std::vector<std::string> row;
		std::string cell;
		// CSVの情報をコンマごとにコンテナに格納
		while (std::getline(ss, cell, ',')) {
			row.push_back(cell);
		}
		data.push_back(row);  // 取得した文字データのコンテナを格納する
	}
	file.close();  	// ファイルを閉じる
	return data;
}