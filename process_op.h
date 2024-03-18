#pragma once
#include <iostream>
#include <string>
#include <array>
#include <windows.h>
#include <chrono>
#include <tlhelp32.h>
#include <vector>
#include <random>
#include <initializer_list>
#include <unordered_set>
using namespace std;

struct CodeStruct//代码结构体
{
	unsigned char* code;
	unsigned int length;
	vector<unsigned int> callAddr;
};


enum class Reg : unsigned int//寄存器对应编号
{
    EAX = 0,
    EBX = 3,
    ECX = 1,
    EDX = 2,
    ESI = 6,
    EDI = 7,
    EBP = 5,
    ESP = 4,
};

DWORD GetProcessId(wstring& processName);// 获取进程ID
HANDLE GetHandle();//获取目标pvz的句柄
void CodeInject(CodeStruct*);//代码注入与执行
/*代码的生成操作系列*/
CodeStruct* Init();

void AddByte(CodeStruct*, unsigned char);//添加一个字节的数据
void AddWord(CodeStruct*, unsigned short);//添加一个字的数据
void AddDword(CodeStruct*, unsigned int);//添加一个双字的数据

void AddList(CodeStruct*, initializer_list<unsigned char>);//添加初始化列表的数据
template <typename... Args>
void AddList(CodeStruct*, Args...);

void PushByte(CodeStruct*, unsigned char); //一个字节数据入栈
void PushDword(CodeStruct*, unsigned int); //一个双子数据入栈

void MovReg(CodeStruct*, Reg, unsigned int); //mov双字数据到reg
void MovRegDwordptr(CodeStruct*, Reg, unsigned int); //将数据mov到reg所指的内存地址
void MovRegDwordptrOff(CodeStruct*, Reg, unsigned int); //这里再加上偏移值

void PushReg(CodeStruct*, Reg);//将寄存器的值压入栈
void PopReg(CodeStruct*, Reg); //出栈
void MovRegtoReg(CodeStruct*, Reg, Reg);  // mov reg,reg

void Call(CodeStruct*, unsigned int);//向汇编代码中添加调用指定地址的函数

void Ret(CodeStruct*);//添加返回指令