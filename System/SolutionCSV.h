#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// CSV��͊֐�
std::string ReadDataFromCSV(const char* _FileName, unsigned int ReadRow);
std::vector<std::string> ReadDataCSVAll(const char* _FileName);
// �����񕪊��֐�
std::vector<std::string> SeparateString(std::string _str, char _Separatechar);