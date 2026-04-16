//
// Created by Admin on 10.04.2026.
//

#include "UpdaterClass.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include "../Utils/utils.h"

#pragma comment(lib, "urlmon.lib")

void UpdaterClass::checkUpdates() {
    std::string VersionGit_url = "https://raw.githubusercontent.com/bogdashs/PassManager/refs/heads/master/version.txt";
    std::string DownloadGit_url = "https://github.com/bogdashs/PassManager/releases/latest/download/PassManager.exe";

    std::string tempVersion = "Data\\check.tmp";
    std::string tempDownload = "Data\\PassManager_new.exe";
    setColor(11);
    print("[ИНФО] Проверка обновлений");
    setColor(7);

    if (URLDownloadToFileA(NULL,VersionGit_url.c_str(),tempVersion.c_str(),0,NULL) == S_OK) {

        std::ifstream file(tempVersion);
        std::string remoteVersion;
        file >> remoteVersion;
        remoteVersion.erase(remoteVersion.find_last_not_of('\n\r\t') + 1);
        file.close();
        DeleteFileA(tempVersion.c_str());

        if (!remoteVersion.empty() && remoteVersion != CURRENT_VERSION) {
            setColor(14);
            print("[ОБНОВЛЕНИЕ] Доступна новая версия: " + remoteVersion);
            setColor(7);
            std::string choice = input("Желаете обновиться? (y/n): ");

            if (choice == "y" || choice == "Y") {
                setColor(11);
                print("[ИНФО] Доступна новая версия\n");
                setColor(7);

                if (URLDownloadToFileA(NULL, DownloadGit_url.c_str(),tempDownload.c_str(),0,NULL) == S_OK) {
                    setColor(10);
                    print("[УСПЕХ] Версия скачена!");
                    Sleep(2000);
                    setColor(7);

                    std::string args = "PassManager.exe " + tempDownload;

                    ShellExecuteA(NULL,"open","Updater.exe",args.c_str(),NULL,SW_SHOW);

                    exit(0);

                } else {
                    setColor(12);
                    print("[УСПЕХ]");
                    system("pause");
                    exit(0);
                }

            }


        } else {
            setColor(11);
            print("[ИНФО] У вас установлена актуальная версия");
            setColor(7);
        }



    }

}
