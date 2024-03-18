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

//植物阵容操作
void RandInit(vector<int> allow, vector<int> gift);//设定有用和附赠植物
int JudgeIfOk(int **,int num, int r, int l);//判断当前的植物是否符合种植规则
int JudgeIfOk(int num, int r, int l);
void RandForm(); 
void PlacePlant(int row, int col, int type);
void CustomForm();
void RandSingle();
void CustomSingle();