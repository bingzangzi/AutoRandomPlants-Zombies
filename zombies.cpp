#include "zombies.h"
#define call_place_plant  0x0040d120
#define call_place_zombie  0x0042a0f0
#define challenge  0x160
#define lawn 0x6a9ec0
#define board 0x768

void PlaceZombie(int row, int col, int type)
{
	CodeStruct* newZombie = Init();
	PushDword(newZombie, col);
	PushDword(newZombie, type);
	MovReg(newZombie, Reg::EAX, row);
	MovRegDwordptr(newZombie, Reg::ECX, lawn);
	MovRegDwordptrOff(newZombie, Reg::ECX, board);
	MovRegDwordptrOff(newZombie, Reg::ECX, challenge);
	Call(newZombie, call_place_zombie);
	Ret(newZombie);
	CodeInject(newZombie);
}
void ZombieColumPlace()
{
	system("cls");
	int type; int col, maxrow;
	printf_s("请输入你想放置的僵尸类型编号：\n");
	cin >> type;
	printf_s("请输入你想放置的列数（从1开始）：\n");
	cin >> col;
	col--;
	printf_s("请输入当前地图的总行数：\n");
	cin >> maxrow;
	printf_s("正在放置中……\n");
	for (int i = 0; i < maxrow; i++) {
		PlaceZombie(i, col, type);
	}
	printf_s("放置完毕！\n");
}
void ZombieSinglePlace()
{
	system("cls");
	int type; int col, row;
	printf_s("请输入你想放置的僵尸类型编号：\n");
	cin >> type;
	printf_s("请输入你想放置的行数（从1开始）：\n");
	cin >> row;
	row--;
	printf_s("请输入你想放置的列数（从1开始）：\n");
	cin >> col;
	col--;
	printf_s("正在放置中……\n");
	PlaceZombie(row, col, type);
	printf_s("放置完毕！\n");
}

void autoZombiePlace()
{
	system("cls");
	int initrate = 0, maxrow;
	printf_s("请输入当前地图的总行数：\n");
	cin >> maxrow;
	printf_s("请输入初始出怪速率(单位：s)：\n");
	cin >> initrate;
	printf_s("请分别输入出怪速率改变的节点以及变化后的速率（第一个数字单位为分钟，第二数字单位为秒。输入-1 -1表示结束）：\n");
	printf_s("*************************\n");
	printf_s("示例：假设初始速率为8s，如果想在第3分钟加快1s，第6分钟再加快0.5s，此后不变，则应该输入：\n");
	printf_s("3 7\n6 6.5\n-1 -1\n");
	printf_s("*************************\n");
	vector<double> ratetable;
	ratetable.push_back(initrate);
	vector<double> timetable;
	while (true)
	{
		double trate;
		double ttime;
		cin >> ttime;
		cin >> trate;
		if (trate == -1 || ttime == -1) break;
		timetable.push_back(ttime * 60);
		ratetable.push_back(trate);
	}
	timetable.push_back(99999999);
	ratetable.push_back(99999999);
	printf_s("按任意键开始出怪\n");
	system("pause");
	while (true) {
		int countzombies = 0;
		char pauseKey = 'p';
		char stopKey = 's';
		int currentpoint = 0;
		double counttime = 0;
		bool isPaused = false;
		char keyPressed;
		while (true) {
			system("cls");
			if (!isPaused) {
				if (_kbhit())
				{
					keyPressed = _getch();
					if (keyPressed == 'p') isPaused = !isPaused;
					else if (keyPressed == 's') break;
				}
				printf_s("出怪中……\n按S结束出怪，按P暂停出怪……\n当前已出僵尸%d个\n当前出怪速率：%.2lfs\n", countzombies, ratetable[currentpoint]);
				for (int i = 0; i < maxrow; i++) {
					PlaceZombie(i, 10, 2);
				}
				chrono::duration<double> sleepDuration(ratetable[currentpoint]);
				this_thread::sleep_for(sleepDuration);
				countzombies++;
				counttime += ratetable[currentpoint];
				if (counttime > timetable[currentpoint]) currentpoint++;
			}
			else
			{
				printf_s("暂停出怪中……按下P键继续，S键则停止\n");
				chrono::duration<double> sleepDuration(ratetable[currentpoint]);
				this_thread::sleep_for(sleepDuration);
				if (_kbhit())
				{
					keyPressed = _getch();
					if (keyPressed == 'p') isPaused = !isPaused;
					else if (keyPressed == 's') break;
				}
			}

		}
		printf_s("是否重新开始？\n1.是\n2.否且返回上级菜单（将清除已输入的数据）\n");
		int choice;
		cin >> choice;
		if (choice != 1) break;
	}
}