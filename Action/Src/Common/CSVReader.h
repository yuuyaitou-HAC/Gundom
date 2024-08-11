#pragma once

#ifndef CSV_READER_H_
#define CSV_READER_H_

#include <vector>
#include <string>

//CSVリーダークラス
class CSVReader {
public:
	//コンストラクタ
	CSVReader() = default;
	//コンストラクタ
	CSVReader(const std::string& file_name);
	//ファイルの読み込み
	void load(const std::string& file_name);
	//データの所得（文字列で所得）
	const std::string& get(int row, int column)const;
	//データの所得（int型に変換して所得）
	int geti(int row, int column)const;
	//データの所得（float型に変換して所得）
	float getf(int row, int column)const;
	//行数を返す
	int rows()const;
	//列数を返す
	int columns(int row = 0)const;

private:
	//1行
	using Row = std::vector<std::string>;
	//行の配列
	using Rows = std::vector<Row>;
	//CSVデータ
	Rows rows_;
};

#endif

