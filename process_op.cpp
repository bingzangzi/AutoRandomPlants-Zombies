#include "process_op.h"
extern wstring pvzname;
// 获取进程ID
DWORD GetProcessId(wstring& processName) {
    PROCESSENTRY32 processEntry; // 定义进程快照结构体
    processEntry.dwSize = sizeof(PROCESSENTRY32); // 设置结构体大小

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); // 创建进程快照
    if (Process32First(snapshot, &processEntry)) { // 获取第一个进程快照
        do {
            if (processName.compare(processEntry.szExeFile) == 0) { // 比较进程名是否匹配
                CloseHandle(snapshot); // 关闭进程快照句柄
                return processEntry.th32ProcessID; // 返回进程ID
            }
        } while (Process32Next(snapshot, &processEntry)); // 获取下一个进程快照
    }

    CloseHandle(snapshot); // 关闭进程快照句柄
    return 0; // 未找到匹配的进程，返回0
}

HANDLE GetHandle()
{
    DWORD pvzPid = GetProcessId(pvzname);
    return OpenProcess(PROCESS_ALL_ACCESS, false, pvzPid);
}

void CodeInject(CodeStruct* target)
{
    HANDLE pvzHand = GetHandle();
    // 在另一个进程的虚拟地址空间中分配内存
    LPVOID addr = VirtualAllocEx(pvzHand, nullptr, target->length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (addr == nullptr) return;

    //对函数调用指令进行修正，确保在不同内存地址中执行时仍然能够正确地调用到目标函数
    for (size_t i = 0; i < target->callAddr.size(); i++)
    {
        unsigned int pos = target->callAddr[i];
        int& CallAddr = (int&)target->code[pos];
        CallAddr = CallAddr - ((int)addr + pos + 4);
    }

    //写入代码
    SIZE_T write_size = 0;
    BOOL ret = WriteProcessMemory(pvzHand, addr, target->code, target->length, &write_size);
    if (ret == 0 || write_size != target->length)
    {
        VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);
        return;
    }

    //创建远程线程，以便执行上面写入的代码
    HANDLE thread = CreateRemoteThread(pvzHand, nullptr, 0, LPTHREAD_START_ROUTINE(addr), nullptr, 0, nullptr);
    if (thread == nullptr)
    {
        VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);
        return;
    }
    //待远程线程的执行完成，并在完成后关闭线程句柄以及释放先前分配的内存空间。
    DWORD wait_status = WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    VirtualFreeEx(pvzHand, addr, 0, MEM_RELEASE);

}

CodeStruct* Init()
{
    CodeStruct* newCode = new CodeStruct;
    newCode->code = new unsigned char[4096 * 256]; //分配的内存大小
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
