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
#include "process_op.h"
using namespace std;

//ֲ�����ݲ���
void RandInit(vector<int> allow, vector<int> gift);//�趨���ú͸���ֲ��
int JudgeIfOk(int **,int num, int r, int l);//�жϵ�ǰ��ֲ���Ƿ������ֲ����
int JudgeIfOk(int num, int r, int l);
void RandForm(); 
void PlacePlant(int row, int col, int type);
void CustomForm();
void RandSingle();
void CustomSingle();