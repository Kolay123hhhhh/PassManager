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

void showmenu() {
    print("\n[+] ==== PASS MANAGER ====");
    print("[+] 1.Записать новый пароль");
    print("[+] 2.Посмотреть пароль");
    print("[+] 3.Очистка базы данных");
    print("[+] 4.Выход\n");
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

        print("Все данные стерты, Программа закрывается!");
        Sleep(1000);
        exit(0);
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

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
            print("Завершаю работу!");
            Sleep(1000);
            exit(0);
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