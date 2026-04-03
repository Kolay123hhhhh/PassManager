#include <complex>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "utils.h"
#include <filesystem>

// master = master key
// id = hwid pc
// db = database password

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
    GetVolumeInformationA("C:\\",NULL,0,&hwid,NULL,NULL,NULL,0);
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
    print("[+] 6.Выход\n");
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
        setColor(10);
        print("[УСПЕХ] Данные успешно сохраненны!");
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
    } else {
        setColor(12);
        print("[ОШИБКА] Не удалось обнаружить базу данных!");
        setColor(7);
    }
}

void login() {
    std::string master_path = "Data\\__libs__.master"; //master
    std::string saved_master,input_pass;
    char key = generateXORKEY(getHWID());

    std::ifstream check_file(master_path);

    if (!check_file.is_open()) {
        print("\n\n==== ПЕРВЫЙ ЗАПУСК ====");
        input_pass = input("Введите МАСТЕР-КЛЮЧ: ");
        std::ofstream out(master_path);
        if (out.is_open()) {
            out << enc(input_pass,key);
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

            if (enc(input_pass,key) == saved_master) {
                setColor(10);
                print("\n[MASTER-KEY] ДОСТУП РАЗРЕШЕН\n");
                setColor(7);
                Sleep(3000);
                system("cls");
                return;
            } else {
                setColor(12);
                print("\n[MASTER-KEY] ДОСТУП НЕ РАЗРЕШЕН\n");
                setColor(7);
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

void export_to_db() {
    std::string db_path = "Data\\__runtime_cache__.dll"; //db
    std::ifstream file(db_path);

    if (!file.is_open()) {
        setColor(12);
        print("[ОШИБКА] База данных не найдена");
        setColor(7);
        return;
    }
    std::ofstream backup("pass.txt");
    if (!backup.is_open()) {
        setColor(12);
        print("[ОШИБКА] Не удалось создать Бэкап!");
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
    print("[ИНФО] ФАЙЛ 'pass.txt' создан рядом с программой\n");
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
        print("[ИНФО] Всего аккаунтов: " + std::to_string(count));
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

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ya_pedik();
    login();

    createFileSystem();

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
            print("[ОШИБКА] Доступ запрещен!");
            setColor(7);
            exit(0);
        } else {
            setColor(10);
            print("\n[HWID] Доступ разрешен!\n");
            setColor(7);
        }
    }

    while (true) {

        check_sec();

        if (isDebuggerPresent) {
            exit(0);
        }

        showmenu();

        choice = input("Выберите действие: ");
        if (choice.empty()) {
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
            export_to_db();
            continue;
        }
        if (choice == "5") {
            show_all_servis();
            continue;
        }
        if (choice == "6") {
            setColor(11);
            print("[ИНФО] Завершаю работу!");
            setColor(7);
            Sleep(1000);
            exit(0);
        }

        if (choice == "1488") {
            full_reset();
        }

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