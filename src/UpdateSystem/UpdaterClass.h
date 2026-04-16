//
// Created by Admin on 10.04.2026.
//

#ifndef PASSMANAGER_UPDATERCLASS_H
#define PASSMANAGER_UPDATERCLASS_H
#include <string>


class UpdaterClass {

    static inline const std::string CURRENT_VERSION = "1.0.1";
public:
    static void checkUpdates();

};


#endif //PASSMANAGER_UPDATERCLASS_H