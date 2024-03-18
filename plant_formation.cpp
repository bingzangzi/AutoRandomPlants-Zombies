#include "plant_formation.h"

#define call_place_plant  0x0040d120
#define call_place_zombie  0x0042a0f0
#define challenge  0x160
#define lawn 0x6a9ec0
#define board 0x768

const char* plant[48] = {
	"豌豆射手","向日葵","樱桃炸弹","坚果","土豆雷","寒冰射手","大嘴花","双发射手",
	"小喷菇","阳光菇","大喷菇","墓碑吞噬者","魅惑菇","胆小菇","寒冰菇","毁灭菇",
	"睡莲","窝瓜","三线射手","缠绕海草","火爆辣椒","地刺","火炬","高坚果",
	"海蘑菇","路灯花","仙人掌","三叶草","裂荚射手","杨桃","南瓜头","磁力菇",
	"卷心菜投手","花盆","玉米投手","咖啡豆","大蒜","叶子保护伞","金盏花","西瓜投手",
	"机枪射手","双子向日葵","忧郁蘑菇","香蒲","冰西瓜投手","吸金磁","地磁王","玉米加农炮"
}; //原版图鉴植物
int MaxCol;//列
int MaxRow;//行
bool plantIfAllow[48];//设定的有用植物判断
bool plantIfGift[48];//设定的附赠的植物判断，如睡莲、花盆等
unordered_set<int> Forbid_one = { 3,22,23,36,46,10,42 }; //第一格禁种植物
unordered_set<int> Forbid_two = { 10 };//第二格禁种植物

void RandInit(vector<int> allow, vector<int> gift) {
	memset(plantIfAllow, true, sizeof(bool) * 48);
	memset(plantIfGift, false, sizeof(bool) * 48);
	for (int i = 0; i < allow.size(); i++) plantIfAllow[allow[i]] = false;
	for (int i = 0; i < gift.size(); i++) plantIfGift[gift[i]] = true;
}


int JudgeIfOk(int** form,int num, int r, int l) {
	if (!plantIfAllow[num]) return 0;
	else if (plantIfGift[num]) return 1;
	else if (l == 0 && Forbid_one.find(num) != Forbid_one.end()) return 0;
	else if (l == 1 && Forbid_two.find(num) != Forbid_two.end()) return 0;
	else if (num == 22) {//判断火炬的逻辑，火炬前是否有豌豆射手、双发、裂荚或机枪射手
		for (int i = 0; i < l; i++) if (form[r][i] == 0 || form[r][i] == 7 || form[r][i] == 40 || form[r][i] == 28) return 2;
		return 0;
	}
	else return 2;
}

int JudgeIfOk(int num, int r, int l) {
	if (!plantIfAllow[num]) return 0;
	else if (plantIfGift[num]) return 1;
	else if (l == 0 && Forbid_one.find(num) != Forbid_one.end()) return 0;
	else if (l == 1 && Forbid_two.find(num) != Forbid_two.end()) return 0;
	else return 2;
}

void RandForm() {
	int** randform=new int*[MaxRow];
	for (int i = 0; i < MaxRow; i++) {
		randform[i] = new int[MaxCol];
	}
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 47);
	for (int i = 0; i < MaxRow; i++)
		for (int j = 0; j < MaxCol; j++) {
			int temp;
			while (true) {
				temp = dis(gen);
				switch (JudgeIfOk(randform, temp, i , j )) {
				case 0:continue;
				case 1:PlacePlant(i, j, temp); continue;
				case 2:break;
				}
				PlacePlant(i, j, temp);
				randform[i][j] = temp;
				break;
			}
		}
}

void PlacePlant(int row, int col, int type)
{
	CodeStruct* newPlant = Init();
	PushDword(newPlant, -1);
	PushDword(newPlant, type);
	MovReg(newPlant, Reg::EAX, row);
	PushDword(newPlant, col);
	MovRegDwordptr(newPlant, Reg::EBP, lawn);
	MovRegDwordptrOff(newPlant, Reg::EBP, board);
	PushReg(newPlant, Reg::EBP);
	Call(newPlant, call_place_plant);
	Ret(newPlant);
	CodeInject(newPlant);
}

void CustomForm()
{
	int** formation = new int* [MaxRow];
	for (int i = 0; i < MaxRow; i++) {
		formation[i] = new int[MaxCol];
	}
	for (int i = 0; i < MaxRow; i++)
		for (int j = 0; j < MaxCol; j++)
			cin >> formation[i][j];
	for (int i = 0; i < MaxRow; i++)
		for (int j = 0; j < MaxCol; j++) {
			if (formation[i][j] < 48 && formation[i][j] >= 0)
				PlacePlant(i, j, formation[i][j]);
		}
}

void RandSingle()
{
	int row, col;
	printf("请输入植物所在行数（从1开始）：\n");
	cin >> row;
	row--;
	printf("请输入植物所在列数（从1开始）：\n");
	cin >> col;
	col--;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 47);
	int temp;
	while (true) {
		temp = dis(gen);
		switch (JudgeIfOk(temp, row, col)) {
		case 0:continue;
		case 1:PlacePlant(row, col, temp); continue;
		case 2:break;
		}
		PlacePlant(row, col, temp);
		break;
	}
}

void CustomSingle()
{
	int row, col, type;
	printf("请输入植物所在行数（从1开始）：\n");
	cin >> row;
	row--;
	printf("请输入植物所在列数（从1开始）：\n");
	cin >> col;
	col--;
	printf("请输入植物的编号：\n");
	cin >> type;
	PlacePlant(row, col, type);
}

