#include <complex>
#include <fstream>
#include <iostream>
#include <windows.h>

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

std::string input(std::string promt) {
    std::string value;
    std::cout << promt;
    std::getline(std::cin, value);
    return value;

}
void print(std::string promt) {
    std::cout << promt << std::endl;
}

void check_sec() {
    if (IsDebuggerPresent()) {
        print("[ВЗЛОМ] ОБНАРУЖЕН ОТЛАДЧИК!,Доступ запрещен!");
        Sleep(700);
        exit(0);
    }
}

void ya_pedik() {
    print("\n[ИНФО] Программа сделана bogdashs/Keu1oi");
    print("[ИНФО] Мой github: https://github.com/bogdashs");
    print("[ИНФО] Программа полностью бесплатна\n");
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
    std::ofstream file(get_self_path() + ":db",std::ios::app);
    if (file.is_open()) {
        char key = 35;
        std::string ENCpass = enc(pass,key);

        file << name << "\n";
        file << login << "\n";
        file << email << "\n";
        file << ENCpass << "\n";
        file.close();
        print("Данные успешно сохраненны!");
    } else {
        print("Проверьте состояние database.txt !");
    }
}

void find_pass(std::string target_name) {
    std::ifstream file(get_self_path() + ":db");
    std::string name, login,email,encpass;
    char key= 35;
    bool found = false;
    if (!file.is_open()) {
        print("Поток с базой данных не найден");
        return;
    }

    while (std::getline(file,name) && std::getline(file,login) && std::getline(file,email) && std::getline(file,encpass)) {
        if (name == target_name) {
            std::string dec = enc(encpass,key);
            print("[РЕЗУЛЬТАТ]");
            print("Сервис: " + name);
            print("Логин: " + login);
            print("Почта: " + email);
            print("Пароль: " + dec);

            dec.assign(dec.size(), '\0');

            found = true;
            break;
        }
    }
    if (!found) {
        print("[ОШИБКА] Аккаунт не найден");
    }

}

void clear_db() {
    std::ofstream file(get_self_path() + ":db", std::ios::trunc);
    if (file.is_open()) {
        file.close();
        print("База данных полностью очищена!");
    } else {
        print("[ОШИБКА] Не удалось обнаружить базу данных!");
    }
}

// void login() {
//     std::string master_path = get_self_path() + ":master";
//     std::string saved_master,input_pass;
//     char key = 77;
//
//     std::ifstream check_file(master_path);
//
//     if (!check_file.is_open()) {
//         print("\n\n==== ПЕРВЫЙ ЗАПУСК ====");
//         input_pass = input("Введите МАСТЕР-КЛЮЧ: ");
//         std::ofstream out(master_path);
//         if (out.is_open()) {
//             out << enc(input_pass,key);
//             out.close();
//             print("[УСПЕХ] Программа будет перезапущена!");
//             Sleep(600);
//             exit(0);
//         } else {
//             std::getline(check_file,saved_master);
//             check_file.close();
//
//             while (true) {
//                 system("cls");
//                 print("\n==== ВХОД В СИСТЕМУ ====");
//                 input_pass = input("Введите ваш МАСТЕР-КЛЮЧ: ");
//
//                 if (enc(input_pass,key) == saved_master) {
//                     print("[ДОСТУП РАЗРЕШЕН]");
//                 } else {
//                     print("[ДОСТУП НЕ РАЗРЕШЕН]");
//                     Sleep(700);
//                     exit(0);
//                 }
//             }
//         }
//     }
//
// }

void full_reset() {
    std::string confirm = input("Вы уверены, что хотите удалить все данные? (y/n): ");
    if (confirm == "y" | confirm == "Y") {
        std::string paths[] = {
            get_self_path() + ":db",
            get_self_path() + ":id",
            get_self_path() + ":master",
        };
        for (const std::string& p : paths) {
            std::ofstream f(p,std::ios::trunc);
            f.close();
        }


        std::ofstream id_file(get_self_path() + ":id");
        if (id_file.is_open()) {}
        id_file << getHWID();
        id_file.close();

        print("Все данные стерты, Программа закрывается!");
        Sleep(1000);
        exit(0);
    }
}

void export_to_db() {
    std::string db_path = get_self_path() + ":db";
    std::ifstream file(db_path);

    if (!file.is_open()) {
        print("[ОШИБКА] База данных не найдена");
        return;
    }
    std::ofstream backup("pass.txt");
    if (!backup.is_open()) {
        print("[ОШИБКА] Не удалось создать Бэкап!");
    }

    std::string name,login,email,enc_pass;
    int count = 0;
    char key = 35;

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

    print("[УСПЕХ] Экспортировано аккаунтов: " + std::to_string(count));
    print("[ИНФО] ФАЙЛ 'pass.txt' создан рядом с программой\n");
}

void show_all_servis() {
    std::string db_path = get_self_path() + ":db";
    std::ifstream file(db_path);

    if (!file.is_open()) {
        print("[ОШИБКА] База данных не найдена");
        return;
    }

    std::string name,login,email,enc_pass;

    int count = 0;

    while (std::getline(file,name) && std::getline(file,login) && std::getline(file,email) && std::getline(file,enc_pass)) {
        count++;
        print(std::to_string(count) + ". " + name);
    }

    if (count == 0) {
        print("[!] В базе нет данных");
    } else {
        print("Всего аккаунтов: " + std::to_string(count));
    }

    file.close();

}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    ya_pedik();
    // login();

    std::string current_hwid = getHWID();
    std::string pass;
    std::string login;
    std::string email;
    std::string name;
    std::string choice;
    BOOL isDebuggerPresent;
    char key = 35;


    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent);

    std::ifstream file(get_self_path() + ":id");
    if (!file.is_open()) {
        std::ofstream out(get_self_path() + ":id");
        out << current_hwid;
        out.close();

        print("Программа привязана к данному оборудованию");
    } else {
        std::string saved_enc_hwid;
        file >> saved_enc_hwid;
        file.close();

        if (saved_enc_hwid != current_hwid) {
            print(saved_enc_hwid);
            print(current_hwid);
            print("[ОШИБКА] Доступ запрещен!");
            exit(0);
        } else {
            print("Доступ разрешен!");
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
            std::string confirm = input("Вы уверены, что хотите удалить все пароли? (y/n): ");
            if (confirm == "y" || confirm == "Y") {
                clear_db();
            } else {
                print("Очистка отменена!");
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
            print("Завершаю работу!");
            Sleep(1000);
            exit(0);
        }

        if (choice == "1488") {
            full_reset();
        }

        break;
    }
    // print("Вы ввели: " + name);
    // print("Вы ввели: " + pass);
    // std::string ENCpass = enc(pass,35);
    // print("Enc: " + ENCpass);
    // std::string DECpass = enc(ENCpass,35);
    // print("Dec " + DECpass);

    return 0;
}