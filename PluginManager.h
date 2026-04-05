//
// Created by Admin on 05.04.2026.
//

#ifndef PASSMANAGER_PLUGINMANAGER_H
#define PASSMANAGER_PLUGINMANAGER_H


class PluginManager {
public:

    virtual void onInit() {};
    virtual void onMenu() {};
    virtual void onUpdate() {};
    virtual void onAuthSuccess() {};
    virtual void onAuthFailure() {};
    virtual void onPreExit() {};
    virtual std::string getMenuItemName() {
        return "Неизвестный плагин";
    };

    virtual void onMenuSelected() {}

    int pluginID = 0;
    void setPluginID(int id) { pluginID = id; }


    virtual bool handleChoice(const std::string& choice) {
        return false;
    }

    virtual ~PluginManager() {}
};


typedef PluginManager* (__cdecl *CreatePluginFunc)();

#endif //PASSMANAGER_PLUGINMANAGER_H