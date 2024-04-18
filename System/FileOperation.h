#pragma once
#include <iostream>
#include <fstream>

// ファイルの書き込み関数
class FileOperator
{
private:
	// ファイル
	std::ofstream File;

public:
	FileOperator();
	FileOperator(const char* _FileName, bool _isOverWrite = true);
	~FileOperator();
	// ファイルを開く
	void Open(const char* _FileName, bool _isOverWrite = true);
	// 書き込み
	void Write(const char* _data, bool _isNewLine = true);
	void Write(int _data, bool _isNewLine = true);
	void Write(unsigned int _data, bool _isNewLine = true);
	void Write(float _data, bool _isNewLine = true);
	void Write(double _data, bool _isNewLine = true);
	void Write(std::string _data, bool _isNewLine = true);
	void NewLine();
	// ファイルを閉じる
	void Close();
private:
	// ファイルが開かれているかのチェック
	bool Check();
};