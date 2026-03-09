#include "..\include_spec_header.h"
#include <stdio.h>
#include <wchar.h>

// 调用格式: _SDT_NULL 调试输出, 命令说明: "类似于易语言核心库中的"输出调试文本()"命令，向输出面板输出一行调试文本。本命令可接受任意多个通用型(任意基本类型)参数及数组。与"输出调试文本()"不同，本命令的所有参数数据仅输出为一行文本（各参数之间以" | "间隔）——既保持了数据相关性，又有效利用了输出面板的横向宽度。另外，本命令对各种数据的输出格式都颇为友好：如字节集被输出为形如"字节集:n{101,121,117,121,97,110,...}"，其中n为字节集长度，{}之间为字节集数据；如数组被输出为形如"数组:m,n{...}"，其中"m,n"表示该数组为m行n列，{}之间为数组数据。"
// 参数<1>: [欲输出值 数组/非数组 _SDT_ALL], 参数说明: "对于非"文本型"参数，将自动转换为文本。本参数可接受任意基本类型，但不接受数组以及自定义数据类型。"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
SPEC_EXTERN_C void spec_Trace_4_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    // 构建输出文本
    wchar_t buf[4096];
    int pos = 0;

    for (int i = 0; i < nArgCount && pos < 4000; i++) {
        if (i > 0) {
            pos += swprintf(buf + pos, 4096 - pos, L" | ");
        }

        DATA_TYPE dt = pArgInf[i].m_dtDataType;

        if (dt == SDT_TEXT) {
            // 文本型：在 ycIDE 中实际传入的是 wchar_t* 宽字符串
            const wchar_t* pText = (const wchar_t*)pArgInf[i].m_pText;
            if (pText) {
                int remain = 4096 - pos - 1;
                int len = (int)wcslen(pText);
                if (len > remain) len = remain;
                wmemcpy(buf + pos, pText, len);
                pos += len;
            } else {
                pos += swprintf(buf + pos, 4096 - pos, L"(空文本)");
            }
        } else if (dt == SDT_INT) {
            pos += swprintf(buf + pos, 4096 - pos, L"%d", pArgInf[i].m_int);
        } else if (dt == SDT_INT64) {
            pos += swprintf(buf + pos, 4096 - pos, L"%lld", pArgInf[i].m_int64);
        } else if (dt == SDT_FLOAT) {
            pos += swprintf(buf + pos, 4096 - pos, L"%g", (double)pArgInf[i].m_float);
        } else if (dt == SDT_DOUBLE) {
            pos += swprintf(buf + pos, 4096 - pos, L"%g", pArgInf[i].m_double);
        } else if (dt == SDT_BOOL) {
            pos += swprintf(buf + pos, 4096 - pos, pArgInf[i].m_bool ? L"真" : L"假");
        } else if (dt == SDT_BYTE) {
            pos += swprintf(buf + pos, 4096 - pos, L"%u", (unsigned)pArgInf[i].m_byte);
        } else if (dt == SDT_SHORT) {
            pos += swprintf(buf + pos, 4096 - pos, L"%d", (int)pArgInf[i].m_short);
        } else {
            // 未知类型，尝试按整数输出
            pos += swprintf(buf + pos, 4096 - pos, L"%d", pArgInf[i].m_int);
        }
    }

    buf[pos] = L'\0';

    // 输出到 stdout（由 IDE 管道捕获）
    // 使用 WriteFile 直接写 UTF-8，确保窗口程序也能输出
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut && hStdOut != INVALID_HANDLE_VALUE) {
        int utf8Len = WideCharToMultiByte(CP_UTF8, 0, buf, pos, NULL, 0, NULL, NULL);
        if (utf8Len > 0) {
            char* utf8Buf = (char*)_alloca(utf8Len + 2);
            WideCharToMultiByte(CP_UTF8, 0, buf, pos, utf8Buf, utf8Len, NULL, NULL);
            utf8Buf[utf8Len] = '\n';
            DWORD written;
            WriteFile(hStdOut, utf8Buf, utf8Len + 1, &written, NULL);
            FlushFileBuffers(hStdOut);
        }
    }

    // 同时输出到调试器（如果有附加调试器）
    OutputDebugStringW(buf);
    OutputDebugStringW(L"\n");
}

