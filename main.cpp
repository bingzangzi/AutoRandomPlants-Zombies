#include "headfile.h"
extern int MaxCol;
extern int MaxRow;
wstring pvzname = L"PlantsVsZombies.exe";

void main_menu();

void plant_menu();
void randform_menu();
void customform_menu();
void randsingle_menu();
void customsingle_menu();

void zombie_menu();
void autozombie_menu();
void columplace_menu();
void singleplace_menu();

int main()
{
	vector<int> allow = { 1,2,4,9,11,14,15,17,19,20,21,25,27,31,35,37,38,41,45,47 };
	vector<int> gift = { 16,30,33 };
	RandInit(allow, gift);
	main_menu();
	printf_s("��ӭ�´�ʹ�ã�\n");
	this_thread::sleep_for(chrono::seconds(3));
	//RandForm();
	return 0;
}

void main_menu()
{
	printf_s("��ӭʹ��ֲ���ս��ʬ�����и�����������\n");
	printf_s("��ǰ�汾��ver.0.1 BETA\n");
	printf_s("Made By bilibili@������_\n\n");
	system("pause");
	while (true)
	{
		system("cls");
		printf_s("��ѡ���ܣ�\n");
		printf_s("1.���ֲ�ﲼ��\n");
		printf_s("2.�����ʬ�Զ�����\n");
		printf_s("0.�˳�����\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls");return;
		case 1:plant_menu(); break;
		case 2:zombie_menu(); break;
		default:printf_s("���ִ������������룡\n"); system("pause"); continue;
		}
	}
	
}
void plant_menu() {
	while (true)
	{
		system("cls");
		printf_s("��ѡ����ģʽ:\n");
		printf_s("1.�������\n");
		printf_s("2.�Զ��岼��\n");
		printf_s("3.���������ֲ\n");
		printf_s("4.�Զ��嵥����ֲ\n");
		printf_s("0.�����ϼ��˵�\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls"); return;
		case 1:randform_menu();  break;
		case 2:customform_menu();  break;
		case 3:randsingle_menu();  break;
		case 4:customsingle_menu(); break;
		default:printf_s("���ִ������������룡\n"); system("pause"); continue;
		}
	}
}

void randform_menu()
{
	system("cls");
	printf_s("������������\n");
	cin >> MaxRow;
	printf_s("������������\n");
	cin >> MaxCol;
	printf_s("�����Զ�������󡭡�\n");
	RandForm();
	printf_s("���������ϣ�\n");
	system("pause");
}
void customform_menu()
{
	system("cls");
	printf_s("������������\n");
	cin >> MaxRow;
	printf_s("������������\n");
	cin >> MaxCol;
	printf_s("�밴������˳����������ֲ��ı�ţ���ĳ��ĳ�в���ֲ��������-1�����ⳬ����ŵ����ֽ����Զ����ԣ���\n");
	CustomForm();
	printf_s("������ϣ�\n");
	system("pause");
}
void randsingle_menu()
{
	while (true) {
		system("cls");
		RandSingle();
		printf_s("�Ƿ������ֲ��\n1.��\n2.��\n");
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:continue;
		default:return;
		}
	}
}
void customsingle_menu()
{
	while (true) {
		system("cls");
		CustomSingle();
		printf_s("�Ƿ������ֲ��\n1.��\n2.��\n");
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:continue;
		default:return;
		}
	}
}

void zombie_menu() {
	while (true)
	{
		system("cls");
		printf_s("��ѡ��ʬ����ģʽ:\n");
		printf_s("1.�������Զ�����\n");
		printf_s("2.�������з���\n");
		printf_s("3.���ε�������\n");
		printf_s("0.�����ϼ��˵�\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls"); return;
		case 1:autozombie_menu();  break;
		case 2:columplace_menu();  break;
		case 3:singleplace_menu();  break;
		default:printf_s("���ִ������������룡\n"); system("pause"); continue;
		}
	}
}

void autozombie_menu()
{
	autoZombiePlace();
}
void columplace_menu()
{
	ZombieColumPlace();
}
void singleplace_menu()
{
	ZombieSinglePlace();
}