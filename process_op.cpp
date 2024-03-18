#include "process_op.h"
extern wstring pvzname;
// ��ȡ����ID
DWORD GetProcessId(wstring& processName) {
    PROCESSENTRY32 processEntry; // ������̿��սṹ��
    processEntry.dwSize = sizeof(PROCESSENTRY32); // ���ýṹ���С

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); // �������̿���
    if (Process32First(snapshot, &processEntry)) { // ��ȡ��һ�����̿���
        do {
            if (processName.compare(processEntry.szExeFile) == 0) { // �ȽϽ������Ƿ�ƥ��
                CloseHandle(snapshot); // �رս��̿��վ��
                return processEntry.th32ProcessID; // ���ؽ���ID
            }
        } while (Process32Next(snapshot, &processEntry)); // ��ȡ��һ�����̿���
    }

    CloseHandle(snapshot); // �رս��̿��վ��
    return 0; // δ�ҵ�ƥ��Ľ��̣�����0
}

HANDLE GetHandle()
{
    DWORD pvzPid = GetProcessId(pvzname);
    return OpenProcess(PROCESS_ALL_ACCESS, false, pvzPid);
}

void CodeInject(CodeStruct* target)
{
    HANDLE pvzHand = GetHandle();
    // ����һ�����̵������ַ�ռ��з����ڴ�
    LPVOID addr = VirtualAllocEx(pvzHand, nullptr, target->length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (addr == nullptr) return;

    //�Ժ�������ָ�����������ȷ���ڲ�ͬ�ڴ��ַ��ִ��ʱ��Ȼ�ܹ���ȷ�ص��õ�Ŀ�꺯��
    for (size_t i = 0; i < target->callAddr.size(); i++)
    {
        unsigned int pos = target->callAddr[i];
        int& CallAddr = (int&)target->code[pos];
        CallAddr = CallAddr - ((int)addr + pos + 4);
    }

    //д�����
    SIZE_T write_size = 0;
    BOOL ret = WriteProcessMemory(pvzHand, addr, target->code, target->length, &write_size);
    if (ret == 0 || write_size != target->length)
    {
        VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);
        return;
    }

    //����Զ���̣߳��Ա�ִ������д��Ĵ���
    HANDLE thread = CreateRemoteThread(pvzHand, nullptr, 0, LPTHREAD_START_ROUTINE(addr), nullptr, 0, nullptr);
    if (thread == nullptr)
    {
        VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);
        return;
    }
    //��Զ���̵߳�ִ����ɣ�������ɺ�ر��߳̾���Լ��ͷ���ǰ������ڴ�ռ䡣
    DWORD wait_status = WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);

}

CodeStruct* Init()
{
    CodeStruct* newCode = new CodeStruct;
    newCode->code = new unsigned char[4096 * 256]; //������ڴ��С
    newCode->length = 0;
    newCode->callAddr.clear();
    return newCode;
}

void AddByte(CodeStruct* Code, unsigned char value)
{
    Code->code[Code->length++] = value;
}

void AddWord(CodeStruct* Code, unsigned short value)
{
    (unsigned short&)Code->code[Code->length] = value;
    Code->length += 2;
}

void AddDword(CodeStruct* Code, unsigned int value)
{
    (unsigned int&)Code->code[Code->length] = value;
    Code->length += 4;
}

void AddList(CodeStruct* Code, initializer_list<unsigned char> value)
{
    for (auto iter = value.begin(); iter != value.end(); iter++)
        AddByte(Code, *iter);
}
template <typename... Args>
void AddList(CodeStruct* Code, Args...);

void PushByte(CodeStruct* Code, unsigned char value)
{
    AddByte(Code,0x6a);
    AddByte(Code, value);
}

void PushDword(CodeStruct* Code, unsigned int value)
{
    AddByte(Code, 0x68);
    AddDword(Code, value);
}

void MovReg(CodeStruct* Code, Reg reg, unsigned int value)
{
    AddByte(Code, 0xb8 + static_cast<unsigned int>(reg));
    AddDword(Code, value);
}

void MovRegDwordptr(CodeStruct* Code, Reg reg, unsigned int value)
{
    AddByte(Code, 0x8b);
    AddByte(Code, 0x05 + static_cast<unsigned int>(reg) * 8);
    AddDword(Code, value);
}

void MovRegDwordptrOff(CodeStruct* Code, Reg reg, unsigned int value)
{
    AddByte(Code, 0x8b);
    AddByte(Code, 0x80 + static_cast<unsigned int>(reg) * (8 + 1));
    if (reg == Reg::ESP)
        AddByte(Code, 0x24);
    AddDword(Code, value);
}

void PushReg(CodeStruct* Code, Reg reg)
{
    AddByte(Code, 0x50 + static_cast<unsigned int>(reg));
}

void PopReg(CodeStruct* Code, Reg reg)
{
    AddByte(Code, 0x58 + static_cast<unsigned int>(reg));
}

void MovRegtoReg(CodeStruct* Code, Reg reg_to, Reg reg_from)
{
    AddByte(Code, 0x8b);
    AddByte(Code, 0xc0 + static_cast<unsigned int>(reg_to) * 8 + static_cast<unsigned int>(reg_from));
}

void Call(CodeStruct* Code, unsigned int addr)
{
    AddByte(Code, 0xe8);
    Code->callAddr.push_back(Code->length);
    AddDword(Code, addr);
}

void Ret(CodeStruct* Code)
{
    AddByte(Code, 0xc3);
}
