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
	printf_s("欢迎下次使用！\n");
	this_thread::sleep_for(chrono::seconds(3));
	//RandForm();
	return 0;
}

void main_menu()
{
	printf_s("欢迎使用植物大战僵尸斗蛐蛐辅助布阵器！\n");
	printf_s("当前版本：ver.0.1 BETA\n");
	printf_s("Made By bilibili@饼藏子_\n\n");
	system("pause");
	while (true)
	{
		system("cls");
		printf_s("请选择功能：\n");
		printf_s("1.随机植物布阵\n");
		printf_s("2.随机僵尸自动放置\n");
		printf_s("0.退出程序\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls");return;
		case 1:plant_menu(); break;
		case 2:zombie_menu(); break;
		default:printf_s("数字错误，请重新输入！\n"); system("pause"); continue;
		}
	}
	
}
void plant_menu() {
	while (true)
	{
		system("cls");
		printf_s("请选择布阵模式:\n");
		printf_s("1.随机布阵\n");
		printf_s("2.自定义布阵\n");
		printf_s("3.随机单个种植\n");
		printf_s("4.自定义单个种植\n");
		printf_s("0.返回上级菜单\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls"); return;
		case 1:randform_menu();  break;
		case 2:customform_menu();  break;
		case 3:randsingle_menu();  break;
		case 4:customsingle_menu(); break;
		default:printf_s("数字错误，请重新输入！\n"); system("pause"); continue;
		}
	}
}

void randform_menu()
{
	system("cls");
	printf_s("请输入行数：\n");
	cin >> MaxRow;
	printf_s("请输入列数：\n");
	cin >> MaxCol;
	printf_s("正在自动随机布阵……\n");
	RandForm();
	printf_s("随机布阵完毕！\n");
	system("pause");
}
void customform_menu()
{
	system("cls");
	printf_s("请输入行数：\n");
	cin >> MaxRow;
	printf_s("请输入列数：\n");
	cin >> MaxCol;
	printf_s("请按照行列顺序依次输入植物的编号（若某行某列不种植物请输入-1，此外超出编号的数字将会自动忽略）：\n");
	CustomForm();
	printf_s("布阵完毕！\n");
	system("pause");
}
void randsingle_menu()
{
	while (true) {
		system("cls");
		RandSingle();
		printf_s("是否继续种植？\n1.是\n2.否\n");
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
		printf_s("是否继续种植？\n1.是\n2.否\n");
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
		printf_s("请选择僵尸放置模式:\n");
		printf_s("1.斗蛐蛐自动放置\n");
		printf_s("2.单次整列放置\n");
		printf_s("3.单次单个放置\n");
		printf_s("0.返回上级菜单\n");
		int choice;
		cin >> choice;
		switch (choice) {
		case 0:system("cls"); return;
		case 1:autozombie_menu();  break;
		case 2:columplace_menu();  break;
		case 3:singleplace_menu();  break;
		default:printf_s("数字错误，请重新输入！\n"); system("pause"); continue;
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