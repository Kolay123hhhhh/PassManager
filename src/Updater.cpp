#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utils/utils.h"

int main(int argc,char* argv[]) {

    // FreeConsole();

    if (argc < 3) {
        return 1;
    }

    std::string OriginalExe = argv[1];
    std::string DownloadZIP = argv[2];

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

    std::string psCommand = "powershell -Command \"Expand-Archive -Path '" + DownloadZIP + "' -DestinationPath '.' -Force\"";

    print("Unpacking Update...");

    int res = system(psCommand.c_str());

    if (res == 0) {
        print("Unpacking finish!");

        DeleteFileA(DownloadZIP.c_str());

        ShellExecuteA(NULL, "open",OriginalExe.c_str(),NULL,NULL,SW_SHOW);

        // if (MoveFileA(DownloadZIP.c_str(), OriginalExe.c_str())) {
        //     ShellExecuteA(NULL,"open",OriginalExe.c_str(),NULL,NULL,SW_SHOW);
        // } else {
        //     print("Failed to move");
        // }
    } else {
        print("Unpacking error!");
    }
}
