#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// CSV解析関数
std::string ReadDataFromCSV(const char* _FileName, unsigned int ReadRow);
std::vector<std::string> ReadDataCSVAll(const char* _FileName);
// 文字列分割関数
std::vector<std::string> SeparateString(std::string _str, char _Separatechar);