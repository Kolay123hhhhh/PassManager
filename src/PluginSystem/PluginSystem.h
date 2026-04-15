//
// Created by Admin on 05.04.2026.
//

#ifndef PASSMANAGER_PLUGINSYSTEM_H
#define PASSMANAGER_PLUGINSYSTEM_H
#include <vector>

#include "PluginManager.h"
#include <windows.h>
#include <string>
#include <filesystem>
#include "../utils.h"

#include "SHA256.h"

extern void add_to_db(std::string name,std::string login,std::string email,std::string pass);
extern void clear_db();
extern void find_pass(std::string target_mame);
extern void export_to_db(std::string name_file);
extern void createFile(std::string name_of_file);



class PluginSystem {
    private:
    std::pmr::vector<PluginManager*> plugins;
    std::vector<HMODULE> library;

    bool isApiEnabled = false;

    std::string ya_gavnoed = "6767GAZANYEBANTIKTAKSTANDOFF2";

    public:

    void loadSetting() {
        std::ifstream file("Data\\runtime.cfg");
        if (file.is_open()) {
            int savedValue;
            std::string savedHash;

            if (file >> savedValue >> savedHash) {

                std::string currentHash = sha256(std::to_string(savedValue) + ya_gavnoed);

                if (currentHash == savedHash) {
                    isApiEnabled = (bool)savedValue;
                } else {
                    isApiEnabled = false;
                    print("[ИНФО] Файл настройки API был поврежден или изменен!");
                }
            }
            file.close();
        }
    }


    void saveSetting() {
        std::ofstream file("Data\\runtime.cfg");
        file << isApiEnabled << std::endl;

        std::string SecretHash = std::to_string(isApiEnabled) + ya_gavnoed;

        std::string getHash = sha256(SecretHash);

        file << getHash << std::endl;

        file.close();
    }


    void toggleAPI() {
        isApiEnabled = !isApiEnabled;
        saveSetting();
    }

    bool getApiStatus() {
        return isApiEnabled;
    }


    void loadPlugins(const std::string& folder_name) {
        namespace fs = std::filesystem;
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string exePath(buffer);
        std::string exeDir = exePath.substr(0, exePath.find_last_of("\\/"));

        std::string fullPath = exeDir + "\\" + folder_name;

        if (!fs::exists(fullPath)) {
            fs::create_directory(fullPath);
            return;
        }

        for (const auto& entry : fs::directory_iterator(fullPath)) {
            if (entry.path().extension() != ".dll") continue;

            HMODULE hModule = LoadLibraryA(entry.path().string().c_str());
            if (!hModule) {
                std::cout << "Failed to load library: " << entry.path().string()
                          << " Error: " << GetLastError() << std::endl;
                continue;
            }

            auto createFunc = (CreatePluginFunc)GetProcAddress(hModule, "CreatePlugin");

            if (!createFunc) {
                std::cout << "Failed to get CreatePlugin function from: "
                          << entry.path().string() << std::endl;
                FreeLibrary(hModule);
                continue;
            }

            PluginManager* manager = createFunc();
            manager->onInit();

            manager->setPluginID(plugins.size() + 7);

            plugins.push_back(manager);
            library.push_back(hModule);
        }
    }

    void showMenu() {

        if (!getApiStatus()) return;

        int currentIndex = 7;
        for (auto& plugin : plugins) {

            std::cout << "[+] "<< currentIndex << "." << plugin->getMenuItemName() << std::endl;
            currentIndex++;
        }
    }

    void UpdateAll() {

        if (!getApiStatus()) return;

        for (auto plugin : plugins) {
            plugin->onUpdate();
        }
    }

    void unloadPlugin(int index) {
        if (index >= 0 && index < plugins.size()) {

            delete plugins[index];

            plugins.erase(plugins.begin() + index);

            if (library[index] != NULL) {
                FreeLibrary(library[index]);
                library.erase(library.begin() + index);
            }

            print("[ИНФО] Плагин выгружен");

        }
    }

    bool dispatchChoice(const std::string& choice) {
        // for (auto plugin : plugins) {
        //     if (plugin->handleChoice(choice)) {
        //         return true;
        //     }
        // }
        // return false;

        try {
            int userChoice = std::stoi(choice);
            if (userChoice >= 7) {
                int index = userChoice - 7;

                if (index >= 0 && index < plugins.size()) {

                    if (!isApiEnabled) {
                        print("[ИНФО] API выключено пользователем!");
                        return false;
                    }

                    PluginManager::PluginAPI api;

                    api.addPassword = [](std::string n,std::string l,std::string e,std::string p) {
                        add_to_db(n,l,e,p);
                    };

                    api.clearDB = []() {
                        clear_db();
                    };

                    api.findPass = [](std::string target_name) {
                        find_pass(target_name);
                    };

                    api.exportDB = [](std::string file_name) {
                        export_to_db(file_name);
                    };

                    api.createFile = [](std::string file_of_name) {
                        createFile(file_of_name);
                    };

                    api.selfUnload = [this, index]() {
                        this->unloadPlugin(index);
                    };

                    plugins[index]->onMenuSelected(api);
                    return true;

                }

            }

        } catch (const std::invalid_argument& e) {}
        return false;
    }

    void unloadallPlugins() {
        for (auto p : plugins) {
            delete p;
        }

        plugins.clear();

        for (HMODULE hmodule : library) {

            if (hmodule != NULL) {
                FreeLibrary(hmodule);
            }

        }
        library.clear();

        print("[ИНФО] Все плагины выгружены!");

    }

    void onbrodcastAuthSuccess() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onAuthSuccess();
            }
        }
    }


    void onbrodcastClearDB() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onClearDB();
            }
        }
    }

    void onbrodcastAuthFailure() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onAuthSuccess();
            }
        }
    }

    void onbrodcastHwidSuccess() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onHwidSuccess();
            }
        }
    }

    void onbrodcastHwidFailure() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onHwidFailure();
            }
        }
    }

    void onbrodcastPasswordAdded(std::string name,std::string login,std::string email,std::string pass) {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onPasswordAdded(name, login,email,pass);
            }
        }
    }

    void onbrodcastExit() {
        if (!getApiStatus()) return;
        for (auto* plugin : plugins) {
            plugin->onPreExit();
        }
    }


    bool hasPlugins() {
        return !plugins.empty();
    }

    ~PluginSystem() {
        for (auto& plugin : plugins) delete plugin;
        for (auto& librarys : library) FreeLibrary(librarys);
    }

};


#endif //PASSMANAGER_PLUGINSYSTEM_H