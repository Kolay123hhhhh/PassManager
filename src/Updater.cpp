#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "utils.h"

int main(int argc,char* argv[]) {

    // FreeConsole();

    if (argc < 3) {
        return 1;
    }

    std::string OriginalExe = argv[1];
    std::string DownloadExe = argv[2];

    std::this_thread::sleep_for(std::chrono::seconds(3));

    bool deleted = false;

    for (int i =0; i < 5;i++) {

        if (DeleteFileA(OriginalExe.c_str())) {
            deleted = true;
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    if (!deleted) {
        print("Dont delete,close manually please!");
        return 1;
    }

    if (MoveFileA(DownloadExe.c_str(), OriginalExe.c_str())) {
        ShellExecuteA(NULL,"open",OriginalExe.c_str(),NULL,NULL,SW_SHOW);
    } else {
        print("Failed to move");
    }
}
