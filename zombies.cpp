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
	printf_s("������������õĽ�ʬ���ͱ�ţ�\n");
	cin >> type;
	printf_s("������������õ���������1��ʼ����\n");
	cin >> col;
	col--;
	printf_s("�����뵱ǰ��ͼ����������\n");
	cin >> maxrow;
	printf_s("���ڷ����С���\n");
	for (int i = 0; i < maxrow; i++) {
		PlaceZombie(i, col, type);
	}
	printf_s("������ϣ�\n");
}
void ZombieSinglePlace()
{
	system("cls");
	int type; int col, row;
	printf_s("������������õĽ�ʬ���ͱ�ţ�\n");
	cin >> type;
	printf_s("������������õ���������1��ʼ����\n");
	cin >> row;
	row--;
	printf_s("������������õ���������1��ʼ����\n");
	cin >> col;
	col--;
	printf_s("���ڷ����С���\n");
	PlaceZombie(row, col, type);
	printf_s("������ϣ�\n");
}

void autoZombiePlace()
{
	system("cls");
	int initrate = 0, maxrow;
	printf_s("�����뵱ǰ��ͼ����������\n");
	cin >> maxrow;
	printf_s("�������ʼ��������(��λ��s)��\n");
	cin >> initrate;
	printf_s("��ֱ�����������ʸı�Ľڵ��Լ��仯������ʣ���һ�����ֵ�λΪ���ӣ��ڶ����ֵ�λΪ�롣����-1 -1��ʾ��������\n");
	printf_s("*************************\n");
	printf_s("ʾ���������ʼ����Ϊ8s��������ڵ�3���Ӽӿ�1s����6�����ټӿ�0.5s���˺󲻱䣬��Ӧ�����룺\n");
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
	printf_s("���������ʼ����\n");
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
				printf_s("�����С���\n��S�������֣���P��ͣ���֡���\n��ǰ�ѳ���ʬ%d��\n��ǰ�������ʣ�%.2lfs\n", countzombies, ratetable[currentpoint]);
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
				printf_s("��ͣ�����С�������P��������S����ֹͣ\n");
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
		printf_s("�Ƿ����¿�ʼ��\n1.��\n2.���ҷ����ϼ��˵������������������ݣ�\n");
		int choice;
		cin >> choice;
		if (choice != 1) break;
	}
}