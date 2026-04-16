#include <complex>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "Utils/utils.h"
#include <filesystem>
#include "SHA256.h"
#include "PluginSystem/PluginSystem.h"
#include "UpdateSystem/UpdaterClass.h"
#include "Security/RunSetup.h"



// master = master key
// id = hwid pc
// db = database password
// SHA256 library by https://github.com/kibonga/sha256-cpp?ysclid=mnjbap0dl3233927588
// Project use GPL-3.0 license

PluginSystem pm;

char generateXORKEY(std::string hwid) {
    char dynamicKey = 0;
    for (char c :hwid) {
        dynamicKey ^= c;
    }
    if (dynamicKey == 0) {
        dynamicKey = 123;
    }
    return dynamicKey;
}

std::string get_self_path() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::string getHWID() {
    DWORD hwid = 0;
    GetVolumeInformationA(NULL,NULL,0,&hwid,NULL,NULL,NULL,0);
    return std::to_string(hwid);
}


void check_sec() {
    if (IsDebuggerPresent()) {
        setColor(12);
        print("[ВЗЛОМ] ОБНАРУЖЕН ОТЛАДЧИК!,Доступ запрещен!");
        setColor(7);
        Sleep(700);
        exit(0);
    }
}

void ya_pedik() {
    setColor(11);
    print("\n[ИНФО] Программа сделана bogdashs/Keu1oi");
    print("[ИНФО] Мой github: https://github.com/bogdashs");
    print("[ИНФО] Программа полностью бесплатна\n");
    setColor(7);
}


void showmenu() {
    print("\n[+] ==== PASS MANAGER ====");
    print("[+] 1.Записать новый пароль");
    print("[+] 2.Посмотреть пароль");
    print("[+] 3.Очистка базы данных");
    print("[+] 4.Бэкап базы данных");
    print("[+] 5.Показать все сервисы");
    print("[+] 6.Переключить API");
    print("[+] 7.Выход\n");

    if (pm.hasPlugins()) {
        if (pm.getApiStatus() == TRUE) {
            print("[+] ==== PLUGIN MENU ====");
            pm.showMenu();
        }
    }
}

std::string enc(std::string data,char key) {
    std::string output = data;
    for (int i = 0; i < output.size(); i++) {
        output[i] = data[i] ^ key;
    }
    return output;
}

void add_to_db(std::string name,std::string login,std::string email,std::string pass) {

    std::ofstream file("Data\\__runtime_cache__.dll",std::ios::app); //db
    if (file.is_open()) {
        char key = generateXORKEY(getHWID());
        std::string ENCpass = enc(pass,key);

        file << name << "\n";
        file << login << "\n";
        file << email << "\n";
        file << ENCpass << "\n";
        file.close();

        pm.onbrodcastPasswordAdded(name,login,email,pass);

        setColor(10);
        print("\n[УСПЕХ] Данные успешно сохранены!");
        setColor(7);
    } else {
        setColor(12);
        print("[ОШИБКА] Проверьте состояние database.txt !");
        setColor(7);
    }
}




void find_pass(std::string target_name) {
    std::ifstream file("Data\\__runtime_cache__.dll"); //db
    std::string name, login,email,encpass;
    char key= generateXORKEY(getHWID());
    bool found = false;
    if (!file.is_open()) {
        setColor(12);
        print("[ОШИБКА] Поток с базой данных не найден");
        setColor(7);
        return;
    }

    while (std::getline(file,name) && std::getline(file,login) && std::getline(file,email) && std::getline(file,encpass)) {
        if (name == target_name) {
            std::string dec = enc(encpass,key);
            setColor(11);
            print("\n\n[РЕЗУЛЬТАТ]");
            print("Сервис: " + name);
            print("Логин: " + login);
            print("Почта: " + email);
            print("Пароль: " + dec);
            setColor(7);

            dec.assign(dec.size(), '\0');

            found = true;
            break;
        }
    }
    if (!found) {
        setColor(12);
        print("[ОШИБКА] Аккаунт не найден");
        setColor(7);
    }

}

void clear_db() {
    std::ofstream file("Data\\__runtime_cache__.dll", std::ios::trunc); //db
    if (file.is_open()) {
        file.close();
        setColor(11);
        print("[ИНФО] База данных полностью очищена!");
        setColor(7);

        pm.onbrodcastClearDB();

    } else {
        setColor(12);
        print("[ОШИБКА] Не удалось обнаружить базу данных!");
        setColor(7);
    }
}

void login() {
    std::string master_path = "Data\\__libs__.master"; //master
    std::string db_path = "Data\\__runtime_cache__.dll"; //db
    std::string id_path = "Data\\assets_v2.bin"; //id
    std::string saved_master,input_pass;
    char key = generateXORKEY(getHWID());

    std::ifstream check_file(master_path);
    std::ifstream check_file2(id_path);
    std::ifstream db(db_path);

    if (db.is_open() && (!check_file.is_open() || !check_file2.is_open())) {
        setColor(12);
        print("[ОШИБКА] ВЗЛОМ БАЗЫ ДАННЫХ");
        print("[ОШИБКА] УДАЛЕНИЕ БАЗЫ ДАННЫХ");
        std::ofstream wipe_db(db_path,std::ios::trunc);
        wipe_db.close();
        Sleep(1500);
        exit(0);
    }

    if (!check_file.is_open()) {
        print("\n\n==== ПЕРВЫЙ ЗАПУСК ====");
        input_pass = input("Введите МАСТЕР-КЛЮЧ: ");
        std::ofstream out(master_path);

        if (out.is_open()) {
            out << sha256(input_pass);
            out.close();
            setColor(10);
            print("[УСПЕХ] Программа будет перезапущена!");
            setColor(7);
            Sleep(600);
            exit(0);
        }
    } else {
        check_file >> saved_master;
        check_file.close();

        if (saved_master.empty()) {
            setColor(12);
            print("[ОШИБКА] БАЗА ДАННЫХ ПОВРЕЖДЕНА");
            print("[ОШИБКА] НЕМЕДЛЕННО ЗАКРЫВАЮ ПРОГРАММУ");
            setColor(7);
            Sleep(500);
            exit(0);
        }

        while (true) {
            system("cls");
            print("\n==== ВХОД В СИСТЕМУ ====");
            input_pass = input("Введите ваш МАСТЕР-КЛЮЧ: ");

            if (sha256(input_pass) == saved_master) {
                setColor(10);
                print("\n[MASTER-KEY] ДОСТУП РАЗРЕШЕН\n");
                setColor(7);
                pm.onbrodcastAuthSuccess();
                Sleep(3000);
                system("cls");
                return;
            } else {
                setColor(12);
                print("\n[MASTER-KEY] ДОСТУП НЕ РАЗРЕШЕН\n");
                setColor(7);
                pm.onbrodcastAuthFailure();
                Sleep(700);
                exit(0);
            }
        }
    }

}

void full_reset() {
    setColor(5);
    std::string confirm = input("Вы уверены, что хотите удалить все данные? (y/n): ");
    setColor(7);
    if (confirm == "y" | confirm == "Y") {
        std::string paths[] = {
            "Data\\__runtime_cache__.dll", //db
            "Data\\assets_v2.bin", //id
            "Data\\__libs__.master", //master
        };
        for (const std::string& p : paths) {
            std::ofstream f(p,std::ios::trunc);
            f.close();
        }


        std::ofstream id_file("Data\\assets_v2.bin"); //id
        if (id_file.is_open()) {
            id_file << getHWID();
            id_file.close();
        }
        setColor(11);
        print("[ИНФО] Все данные стерты, Программа закрывается!");
        setColor(7);
        Sleep(1000);
        exit(0);
    }
}

void export_to_db(std::string name_file) {
    std::string db_path = "Data\\__runtime_cache__.dll"; //db
    std::ifstream file(db_path);

    if (!file.is_open()) {
        setColor(12);
        print("\n[ОШИБКА] База данных не найдена");
        setColor(7);
        return;
    }
    std::ofstream backup(name_file);
    if (!backup.is_open()) {
        setColor(12);
        print("\n[ОШИБКА] Не удалось создать Бэкап!");
        setColor(7);
    }

    std::string name,login,email,enc_pass;
    int count = 0;
    char key = generateXORKEY(getHWID());

    backup << "==== ЭКСПОРТ ПАРОЛЕЙ ====\n\n";

    while (std::getline(file,name) && std::getline(file,login) && std::getline(file,email) && std::getline(file,enc_pass)) {
        std::string raw_pass = enc(enc_pass,key);
        backup << "Сервис: " << name << "\n";
        backup << "Логин: " << login << "\n";
        backup << "Почта: " << email << "\n";
        backup << "Пароль: " << raw_pass << "\n";
        backup << "--------------------------------\n";
        count++;
    }
    file.close();
    backup.close();
    setColor(10);
    print("[УСПЕХ] Экспортировано аккаунтов: " + std::to_string(count));
    setColor(11);
    print("[ИНФО] ФАЙЛ " + name_file + " был создан рядом с програмой");
    setColor(7);
}

void show_all_servis() {
    std::string db_path = "Data\\__runtime_cache__.dll"; //db
    std::ifstream file(db_path);

    if (!file.is_open()) {
        setColor(12);
        print("\n[ОШИБКА] База данных не найдена");
        setColor(7);
        return;
    }

    std::string name,login,email,enc_pass;

    int count = 0;

    while (std::getline(file,name) && std::getline(file,login) && std::getline(file,email) && std::getline(file,enc_pass)) {
        count++;
        print(std::to_string(count) + ". " + name);
    }

    if (count == 0) {
        setColor(12);
        print("[ОШИБКА] В базе нет данных");
        setColor(7);
    } else {
        setColor(14);
        print("\n[ИНФО] Всего аккаунтов: " + std::to_string(count));
        setColor(7);
    }

    file.close();

}

void createFileSystem() {
    namespace fs = std::filesystem;

    if (fs::create_directory("Data")) {
        // print("[ИНФО] Файл Data был создан");
    } else {
        // print("[ИНФО] Файл Data не был создан");
        // print("[ИНФО] Файл Data возможно уже существует");
    }
}

void createFile(std::string name_of_file) {
    namespace fs = std::filesystem;
    if (fs::create_directory(name_of_file));
}


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::string exePath = get_self_path();
    std::string exeDir = exePath.substr(0, exePath.find_last_of("\\/"));

    SetCurrentDirectoryA(exeDir.c_str());

    UpdaterClass::checkUpdates();

    pm.loadPlugins("Plugins");

    createFileSystem();

    ya_pedik();
    login();

    pm.loadSetting();

    // RunSetup::runSetup();

    std::string current_hwid = getHWID();
    std::string pass;
    std::string login;
    std::string email;
    std::string name;
    std::string choice;
    BOOL isDebuggerPresent;
    char key = generateXORKEY(getHWID());


    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent);

    std::ifstream file("Data\\assets_v2.bin"); //id
    if (!file.is_open()) {
        std::ofstream out("Data\\assets_v2.bin"); //id
        out << current_hwid;
        out.close();
        setColor(14);
        print("[HWID] Программа привязана к данному оборудованию");
        setColor(7);
    } else {
        std::string saved_enc_hwid;
        file >> saved_enc_hwid;
        file.close();

        if (saved_enc_hwid != current_hwid) {
            print(saved_enc_hwid);
            print(current_hwid);
            setColor(12);
            print("[HWID] Доступ запрещен!");
            setColor(7);

            pm.onbrodcastHwidFailure();

            Sleep(1000);
            exit(0);
        } else {
            setColor(10);
            print("\n[HWID] Доступ разрешен!\n");
            setColor(7);

            pm.onbrodcastHwidSuccess();

        }
    }

    while (true) {

        pm.UpdateAll();

        check_sec();

        if (isDebuggerPresent) {
            exit(0);
        }

        showmenu();

        choice = input("\nВыберите действие: ");

        if (choice.empty()) {
            continue;
        }

        if (pm.dispatchChoice(choice)) {
            std::cin.clear();
            std::fflush(stdin);
            continue;
        }

        if (choice == "1") {
            name = input("Название соцсети: ");
            login = input("Ваш логин: ");
            email = input("Ваша почта: ");
            pass = input("Ваш пароль: ");

            if (pass.empty() || name.empty()) {
                print("Вы ничего не ввели!");
            }
            add_to_db(name,login,email,pass);
            continue;
        }
        if (choice == "2") {
            std::string choice_name = input("Название соцсети: ");
            find_pass(choice_name);
            continue;
        }

        if (choice == "3") {
            setColor(14);
            std::string confirm = input("Вы уверены, что хотите удалить все пароли? (y/n): ");
            setColor(7);
            if (confirm == "y" || confirm == "Y") {
                clear_db();
            } else {
                setColor(11);
                print("[ИНФО] Очистка отменена!");
                setColor(7);
            }
            continue;
        }
        if (choice == "4") {
            std::string name_of_file = input("Введите имя файла-бэкапа: ");
            export_to_db(name_of_file + ".txt");
            continue;
        }
        if (choice == "5") {
            show_all_servis();
            continue;
        }

        if (choice == "6") {
            setColor(12);
            print("[ПРЕДУПРЕЖДЕНИЕ] Плагины смогут удалять/редактировать ваши пароли, скачивайте только проверенные!");
            setColor(7);

            std::string shoice = input("Вы хотите переключить статус API для плагинов (y/n): ");

            if (shoice == "y" || shoice == "Y") {
                pm.toggleAPI();
                setColor(11);
                print("[ИНФО] Статус API изменен на: " + std::string(pm.getApiStatus() ? "ВКЛ" : "Выкл"));
                setColor(7);
                continue;
            }

        }

        if (choice == "7") {
            setColor(11);
            print("[ИНФО] Завершаю работу!");
            setColor(7);

            pm.onbrodcastExit();

            Sleep(1000);
            exit(0);
        }

        // if (choice == "1488") {
        //     full_reset();
        // }

        continue;
    }
    // print("Вы ввели: " + name);
    // print("Вы ввели: " + pass);
    // std::string ENCpass = enc(pass,35);
    // print("Enc: " + ENCpass);
    // std::string DECpass = enc(ENCpass,35);
    // print("Dec " + DECpass);

    return 0;
}