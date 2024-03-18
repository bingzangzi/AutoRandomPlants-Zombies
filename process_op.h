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

struct CodeStruct//����ṹ��
{
	unsigned char* code;
	unsigned int length;
	vector<unsigned int> callAddr;
};


enum class Reg : unsigned int//�Ĵ�����Ӧ���
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

DWORD GetProcessId(wstring& processName);// ��ȡ����ID
HANDLE GetHandle();//��ȡĿ��pvz�ľ��
void CodeInject(CodeStruct*);//����ע����ִ��
/*��������ɲ���ϵ��*/
CodeStruct* Init();

void AddByte(CodeStruct*, unsigned char);//���һ���ֽڵ�����
void AddWord(CodeStruct*, unsigned short);//���һ���ֵ�����
void AddDword(CodeStruct*, unsigned int);//���һ��˫�ֵ�����

void AddList(CodeStruct*, initializer_list<unsigned char>);//��ӳ�ʼ���б������
template <typename... Args>
void AddList(CodeStruct*, Args...);

void PushByte(CodeStruct*, unsigned char); //һ���ֽ�������ջ
void PushDword(CodeStruct*, unsigned int); //һ��˫��������ջ

void MovReg(CodeStruct*, Reg, unsigned int); //mov˫�����ݵ�reg
void MovRegDwordptr(CodeStruct*, Reg, unsigned int); //������mov��reg��ָ���ڴ��ַ
void MovRegDwordptrOff(CodeStruct*, Reg, unsigned int); //�����ټ���ƫ��ֵ

void PushReg(CodeStruct*, Reg);//���Ĵ�����ֵѹ��ջ
void PopReg(CodeStruct*, Reg); //��ջ
void MovRegtoReg(CodeStruct*, Reg, Reg);  // mov reg,reg

void Call(CodeStruct*, unsigned int);//�����������ӵ���ָ����ַ�ĺ���

void Ret(CodeStruct*);//��ӷ���ָ��