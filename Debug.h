#pragma once
#if defined (DEBUG) | (_DEBUG)
#include <iostream>
#define DEBUGLOG(x) {std::cout << x;}
#define WDEBUGLOG(x) {std::wcout << x;}
#define NEWDEBUGLOG(x) {std::cout << std::endl;std::cout << x;}
#define WNEWDEBULOG(x) {std::wcout << std::endl; std::wcout << x;}
#else
#define DEBUGLOG(x)
#define WDEBUGLOG(x)
#define NEWDEBUGLOG(x)
#define WNEWDEBULOG(x)
#endif