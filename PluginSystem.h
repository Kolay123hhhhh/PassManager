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
#include "utils.h"

extern void add_to_db(std::string name,std::string login,std::string email,std::string pass);
extern void clear_db();
extern void find_pass(std::string target_mame);
extern void export_to_db(std::string name_file);
extern void createFile(std::string name_of_file);

class PluginSystem {
    private:
    std::pmr::vector<PluginManager*> plugins;
    std::vector<HMODULE> library;
    public:

    void loadPlugins(const std::string& folder_name) {
        namespace fs = std::filesystem;
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string exePath(buffer);
        std::string exeDir = exePath.substr(0, exePath.find_last_of("\\/"));

        std::string fullPath = exeDir + "\\" + folder_name;

        std::cout << "Loading plugins from: " << fullPath << std::endl;

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
        int currentIndex = 7;
        for (auto& plugin : plugins) {

            std::cout << "[+] "<< currentIndex << "." << plugin->getMenuItemName() << std::endl;
            currentIndex++;
        }
    }

    void UpdateAll() {
        for (auto plugin : plugins) {
            plugin->onUpdate();
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

                    PluginManager::PluginAPI api;

                    plugins[index]->onMenuSelected(api);

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

                }

            }

        } catch (const std::invalid_argument& e) {}
        return false;
    }



    void onbrodcastAuthSuccess() {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onAuthSuccess();
            }
        }
    }


    void onbrodcastClearDB() {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onClearDB();
            }
        }
    }

    void onbrodcastAuthFailure() {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onAuthSuccess();
            }
        }
    }

    void onbrodcastHwidSuccess() {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onHwidSuccess();
            }
        }
    }

    void onbrodcastHwidFailure() {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onHwidFailure();
            }
        }
    }

    void onbrodcastPasswordAdded(std::string name,std::string login,std::string email,std::string pass) {
        for (auto* plugin : plugins) {
            if (plugin) {
                plugin->onPasswordAdded(name, login,email,pass);
            }
        }
    }

    void onbrodcastExit() {
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