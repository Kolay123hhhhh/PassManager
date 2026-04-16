//
// Created by Admin on 16.04.2026.
//

#include "RunSetup.h"
#include <windows.h>
#include <string>

const char* REG_PATH = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

const char* APP_NAME = "PassManager_vault";

void RunSetup::runSetup() {

    char szPath[MAX_PATH];
    GetModuleFileNameA(NULL, szPath, MAX_PATH);

    HKEY hkey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER,REG_PATH,0,KEY_SET_VALUE,&hkey) == ERROR_SUCCESS) {
        RegSetValueExA(hkey,APP_NAME,0,REG_SZ,(const BYTE*) szPath,strlen(szPath) + 1);
        RegCloseKey(hkey);
    }

}

void RunSetup::runDelete() {
    HKEY hkey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER,REG_PATH,0,KEY_SET_VALUE,&hkey) == ERROR_SUCCESS) {
        RegDeleteValueA(hkey,APP_NAME);
        RegCloseKey(hkey);
    }
}

bool RunSetup::isRun() {
    HKEY hkey;
    bool result = false;
    if (RegOpenKeyExA(HKEY_CURRENT_USER,REG_PATH,0,KEY_READ,&hkey) == ERROR_SUCCESS) {
        LSTATUS status= RegQueryValueExA(hkey,APP_NAME,NULL,NULL,NULL,NULL);
        if (status == ERROR_SUCCESS) {
            result = true;
        }
        RegCloseKey(hkey);
    }
    return result;
}