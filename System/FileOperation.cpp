#include "FileOperation.h"

FileOperator::FileOperator()
{
}

FileOperator::FileOperator(const char* _FileName, bool _isOverWrite)
{
	Open(_FileName, _isOverWrite);
}

FileOperator::~FileOperator()
{
	if (Check())
	{
		Close();
	}
}

void FileOperator::Open(const char* _FileName, bool _isOverWrite)
{
	if (!Check())
	{
		if (_isOverWrite)
		{
			File = std::ofstream(_FileName);
		}
		else
		{
			File = std::ofstream(_FileName, std::ios::app);
		}
	}
	else
	{
		File.close();
		if (_isOverWrite)
		{
			File = std::ofstream(_FileName);
		}
		else
		{
			File = std::ofstream(_FileName, std::ios::app);
		}
	}
}

void FileOperator::Write(const char* _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}

void FileOperator::Write(int _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}

void FileOperator::Write(unsigned int _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}

void FileOperator::Write(float _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}

void FileOperator::Write(double _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}

void FileOperator::Write(std::string _data, bool _isNewLine)
{
	if (Check())
	{
		if (_isNewLine)
		{
			File << std::endl << _data;
		}
		else
		{
			File << _data;
		}
	}
}


void FileOperator::NewLine()
{
	if (Check())
	{
		File << std::endl;
	}
}

void FileOperator::Close()
{
	if (Check())
	{
		File.close();
	}
}

bool FileOperator::Check()
{
	if (File.is_open())
	{
		return true;
	}
	else
	{
		return false;
	}
}
