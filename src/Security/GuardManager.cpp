#include <windows.h>
#include <iostream>
#include "../Utils/utils.h"

void hideconsole() {
    HWND console = GetConsoleWindow();
    ShowWindow(console,SW_HIDE);
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    // hideconsole();
    std::string blacklist[] = {
        "Cheat Engine",
        "x64dbg",
        "x32dbg",
    };




    while (true) {
        for (const std::string& name : blacklist ) {
           HWND hwnd = FindWindowA(NULL, name.c_str());
            if (hwnd == NULL) {
                print("[ИНФО] Вредоносные процессы не найдены");
            }
            DWORD pid;
            GetWindowThreadProcessId(hwnd, &pid);
            HANDLE hprocess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

            TerminateProcess(hprocess, 0);
            CloseHandle(hprocess);
        }

        Sleep(2500);
    }

    return 0;
}
