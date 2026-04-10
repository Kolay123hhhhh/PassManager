Pass Manager - менеджер паролей использующий sha256,XOR строящийся на вашем HWID

Первый запуск:

<img width="480" height="229" alt="Снимок экрана 2026-04-03 221123" src="https://github.com/user-attachments/assets/0f543297-9e32-47f3-983f-2d8873301ce4" />

Основное меню:

<img width="306" height="163" alt="Снимок экрана 2026-04-07 224111" src="https://github.com/user-attachments/assets/b5ef636a-f145-4e66-bdca-df8cdaa9c1d2" />

Плагин меню:

<img width="263" height="47" alt="Снимок экрана 2026-04-07 224119" src="https://github.com/user-attachments/assets/069ea14c-770c-4b16-8345-097d0006f0f3" />


- XOR-Шифрование: строиться на основе вашего HWID, то есть на другом компьютере программа не расшифрует пароли
- SHA256-Шифрование: создает хеш (не возможно вернуть в пароль), программа сравнивает два хеша и делает вывод впустить или нет

Программа использует сторонью библиотеку SHA256
- Автор [kibonga/sha256](https://github.com/kibonga/sha256-cpp?ysclid=mnjbap0dl3233927588)
- Author [kibonga/sha256](https://github.com/kibonga/sha256-cpp?ysclid=mnjbap0dl3233927588)

В программе имееться API:
``` cpp
void onAuthFailure() override; // неудача при логине
void onMenu() override; 
void onHwidFailure() override; // неудача при логине хвидом
void onHwidSuccess() override; // удача при логине хвидом
void onAuthSuccess() override; // удача при логине
void onInit() override; // При запуске
void onPasswordAdded(std::string name, std::string login, std::string email, std::string pass) override; // при добавление пароля
void onClearDB() override; // при очистке базы данных
void onPreExit() override; // перед выходом
void onUpdate() override; // в меню, при повторе основного цикла
std::string getMenuItemName() override; // имя твоего плагина в меню
    
void onMenuSelected(PluginAPI api) override; // действия при выборе твоего плагина, Обязательна при создании плагина
```
Так-же есть разные события:
``` cpp
void (*addPassword)(std::string name,std::string login,std::string email,std::string pass) = [](std::string name,std::string login,std::string email,std::string pass) {};
void (*clearDB)() = []() {};
void (*findPass)(std::string target_mame) = [](std::string target_mame) {};
void (*exportDB)(std::string name_file) = [](std::string name_file) {};
void (*createFile)(std::string name_of_file) = [](std::string name_of_file) {};
std::function<void()> selfUnload;

// системные
        
void (*cls)() = []() {system("cls");};
void (*pause)() = []() {system("pause");};
void (*onExit)() = []() { exit(0);};
```

Имеются инструменты:
```cpp
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
```
Примеры:
- Название:
``` cpp
    std::string getMenuItemName() override {
        return "testPlugin";
    }
```
``` text
[+] ==== PLUGIN MENU ====
[+] 7.testPlugin
```
- Главная функция:
``` cpp
    void onMenuSelected(PluginAPI api) override {
        api.print("Hello World!");
    }
```
``` text
Выберите действие: 7
Hello World!
```


p.s. я не рекомендую вам это использовать так как, XOR шифрование вскрывается очень быстро
данная защита, способна уберечь от неопытного хакера/случайно прохожего возле вашего компа

**Автор не несет ответственности за потерянные данные/пароли**