//
// Created by Admin on 10.04.2026.
//

#ifndef PASSMANAGER_UPDATERCLASS_H
#define PASSMANAGER_UPDATERCLASS_H
#include <string>


class UpdaterClass {

    static inline const std::pmr::string CURRENT_VERSION = "1.0.0";

    static void checkUpdates();

};


#endif //PASSMANAGER_UPDATERCLASS_H