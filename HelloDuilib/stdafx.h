// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
