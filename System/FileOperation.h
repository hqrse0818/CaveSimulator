#pragma once
#include <iostream>
#include <fstream>

// �t�@�C���̏������݊֐�
class FileOperator
{
private:
	// �t�@�C��
	std::ofstream File;

public:
	FileOperator();
	FileOperator(const char* _FileName, bool _isOverWrite = true);
	~FileOperator();
	// �t�@�C�����J��
	void Open(const char* _FileName, bool _isOverWrite = true);
	// ��������
	void Write(const char* _data, bool _isNewLine = true);
	void Write(int _data, bool _isNewLine = true);
	void Write(unsigned int _data, bool _isNewLine = true);
	void Write(float _data, bool _isNewLine = true);
	void Write(double _data, bool _isNewLine = true);
	void Write(std::string _data, bool _isNewLine = true);
	void NewLine();
	// �t�@�C�������
	void Close();
private:
	// �t�@�C�����J����Ă��邩�̃`�F�b�N
	bool Check();
};