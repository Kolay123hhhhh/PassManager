//
// Created by Admin on 05.04.2026.
//

#ifndef PASSMANAGER_PLUGINMANAGER_H
#define PASSMANAGER_PLUGINMANAGER_H


#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>

class PluginManager {
public:

    struct PluginAPI {
        void (*print)(std::string text) = [](std::string text) {
            std::cout << text << std::endl;
        };

        void (*setColor)(int color) = [](int color) {
            HANDLE hmod = GetStdHandle(STD_OUTPUT_HANDLE);

            SetConsoleTextAttribute(hmod, color);
        };

        std::string (*input)(std::string promt) = [](std::string promt) {
            std::string value;
            std::cout << promt;
            std::cin >> value;
            return value;
        };
        // std::string (*getDataPath)(std::string promt) = [](std::string promt) {
        //
        // };
        void (*addPassword)(std::string name,std::string login,std::string email,std::string pass) = [](std::string name,std::string login,std::string email,std::string pass) {};
        void (*clearDB)() = []() {};
        void (*findPass)(std::string target_mame) = [](std::string target_mame) {};
        void (*exportDB)(std::string name_file) = [](std::string name_file) {};
        void (*createFile)(std::string name_of_file) = [](std::string name_of_file) {};



        void (*cls)() = []() {system("cls");};
        void (*pause)() = []() {system("pause");};

    };

    virtual void onInit() {};
    virtual void onMenu() {};
    virtual void onUpdate() {};
    virtual void onAuthSuccess() {};
    virtual void onAuthFailure() {};
    virtual void onHwidSuccess() {};
    virtual void onHwidFailure() {};
    virtual void onPreExit() {};
    virtual void onClearDB() {};


    virtual void onPasswordAdded(std::string name,std::string login,std::string email,std::string pass) {};

    virtual std::string getMenuItemName() {
        return "Неизвестный плагин";
    };


    virtual void onMenuSelected(PluginAPI api) {}

    int pluginID = 0;
    void setPluginID(int id) { pluginID = id; }


    virtual bool handleChoice(const std::string& choice) {
        return false;
    }

    virtual ~PluginManager() {}
};


typedef PluginManager* (__cdecl *CreatePluginFunc)();

#endif //PASSMANAGER_PLUGINMANAGER_H