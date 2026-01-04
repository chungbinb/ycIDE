.版本 2

.DLL命令 ECodeParser_Create, 整数型, "ECodeParser.dll", "@ECodeParser_Create", 公开
    .参数 eroot, 文本型

.DLL命令 ECodeParser_Destroy, , "ECodeParser.dll", "@ECodeParser_Destroy", 公开
    .参数 parser, 整数型

.DLL命令 ECodeParser_AddEvent, , "ECodeParser.dll", "@ECodeParser_AddEvent", 公开
    .参数 parser, 整数型
    .参数 name, 文本型
    .参数 e, 子程序指针

.DLL命令 ECodeParser_Parse, 整数型, "ECodeParser.dll", "@ECodeParser_Parse", 公开
    .参数 parser, 整数型
    .参数 data, 字节集
    .参数 size, 整数型
    .参数 enc, 逻辑型

.DLL命令 ECodeParser_GetDerived, 整数型, "ECodeParser.dll", "@ECodeParser_GetDerived", 公开
    .参数 parser, 整数型
    .参数 tag, 整数型

.DLL命令 ECodeParser_FindEBase, 整数型, "ECodeParser.dll", "@ECodeParser_FindEBase", 公开
    .参数 parser, 整数型
    .参数 tag, 整数型

.DLL命令 ECodeParser_EMethod2Code, 整数型, "ECodeParser.dll", "@ECodeParser_EMethod2Code", 公开
    .参数 parser, 整数型
    .参数 tag, 整数型

.DLL命令 ECodeParser_EForm2Window, 整数型, "ECodeParser.dll", "@ECodeParser_EForm2Window", 公开
    .参数 parser, 整数型
    .参数 tag, 整数型

.DLL命令 Ptr2Arr, 整数型, "ECodeParser.dll", "@Ptr2Arr", 公开, (LocalAlloc) vector => array
    .参数 v, vector, 传址, vector*
    .参数 size, 整数型

.DLL命令 Ptr2Bin, 整数型, "ECodeParser.dll", "@Ptr2Bin", 公开
    .参数 v, vector, 传址, vector*

.DLL命令 Ptr2Str, 整数型, "ECodeParser.dll", "@Ptr2Str", 公开, (LocalAlloc) string => text
    .参数 s, string, 传址, string*

.DLL命令 FreePtr, , "ECodeParser.dll", "@FreePtr", 公开
    .参数 ptr, 整数型

.DLL命令 释放EBase指针, , "ECodeParser.dll", "@FreeEBase", 公开
    .参数 ptr, 整数型, , EBase*

.DLL命令 RtlMoveMemory_ESection_Program_Method, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Program_Method, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Program_Dll, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Program_Dll, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Program_Assembly, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Program_Assembly, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Variable, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Variable, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ETAG, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ETAG, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Resources_Const, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Resources_Const, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Resources_Data, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Resources_Data, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_UserInfo, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_UserInfo, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_ECList_Info, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_ECList_Info, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Library, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Library, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Resources_Form, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Resources_Form, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 RtlMoveMemory_ESection_Resources_FormElement, , "Kernel32.dll", "RtlMoveMemory", 公开
    .参数 Destination, ESection_Resources_FormElement, 传址
    .参数 Source, 整数型
    .参数 Length, 整数型

.DLL命令 LocalFree_111, 整数型, "Kernel32.dll", "LocalFree"
    .参数 未知_136, 整数型

.DLL命令 RtlMoveMemory_1fa, , "Kernel32.dll", "RtlMoveMemory"
    .参数 未知_201, 字节集, 传址
    .参数 未知_202, 整数型
    .参数 未知_203, 整数型

.DLL命令 RtlMoveMemory_24e, , , "RtlMoveMemory"
    .参数 未知_24f, 整数型, 传址 数组
    .参数 未知_250, 整数型
    .参数 未知_251, 整数型
