// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 在此处引用程序需要的其他头文件
#include <ObjBase.h>
#include <UIlib.h>

using namespace DuiLib;


#include <icrsint.h>
#import "C:\\program files\\common files\\system\\ado\\msadox.dll"
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" \
no_namespace rename("EOF", "adoEOF") rename("Error","adoError")

#ifdef _DEBUG
#pragma comment (lib, "duilib_ud.lib")
#else
#pragma comment (lib, "duilib_u.lib")
#endif
