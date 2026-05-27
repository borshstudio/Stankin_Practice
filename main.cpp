#include "api.h"
#include "weather.h"

#include <iostream>
#include <fstream>
#include <string>
// #include <codecvt>
// #include <locale>

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif




std::string loadApiKey() {
    std::ifstream file("api_key.txt");
    std::string apiKey;
    if (file.is_open()) {
        std::getline(file, apiKey);
    }
    return apiKey;
}

void saveApiKey(const std::string& apiKey) {
    std::ofstream file("api_key.txt");
    file << apiKey;
}

std::string getValidApiKey() {
    std::string apiKey = loadApiKey();

    if (!apiKey.empty()) {
        std::cout << "[INFO] Найден сохраненный API-ключ. Проверяем...\n";
        if (checkApiKey(apiKey)) {
            std::cout << "[OK] API-ключ работает.\n";
            return apiKey;
        }
        std::cout << "[ERROR] Сохраненный API-ключ не работает.\n";
    }

    while (true) {
        std::cout << "\nВведите API-ключ OpenWeatherMap (0 — выход): ";
        apiKey = readUtf8Line();

        if (apiKey == "0") {
            return "";
        }

        if (apiKey.empty()) {
            std::cout << "[ERROR] API-ключ не может быть пустым.\n";
            continue;
        }

        std::cout << "[INFO] Проверяем API-ключ...\n";

        if (checkApiKey(apiKey)) {
            saveApiKey(apiKey);
            std::cout << "[OK] API-ключ сохранен.\n";
            return apiKey;
        }

        std::cout << "[ERROR] API-ключ неправильный.\n";
    }
}

void mainMenu(const std::string& apiKey) {
    while (true) {
        std::cout << "\n=== МЕНЮ ===\n";
        std::cout << "1) Поиск погоды по городу\n";
        std::cout << "2) Избранные города\n";
        std::cout << "3) О приложении\n";
        std::cout << "0) Выход\n";
        std::cout << "Ваш выбор: ";

        std::string choice;
        choice = readUtf8Line();

        if (choice == "1") {
            showWeatherByCity(apiKey);
        }
        else if (choice == "2") {
            favoritesMenu(apiKey);
        }
        else if (choice == "3") {
            aboutApp();
        }
        else if (choice == "0") {
            std::cout << "\nЗавершение программы.\n";
            break;
        }
        else {
            std::cout << "[ERROR] Неверный пункт меню.\n";
        }
    }
}

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    setlocale(LC_ALL, ".UTF-8");

    std::cout << "Добро пожаловать в приложение прогноза погоды!\n";

    std::string apiKey = getValidApiKey();
    if (apiKey.empty()) {
        std::cout << "Выход из приложения.\n";
        return 0;
    }

    mainMenu(apiKey);
    return 0;
}