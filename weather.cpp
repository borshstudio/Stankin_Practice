#include "weather.h"
#include "api.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cstdint>

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

std::string readUtf8Line() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE) {
        std::string fallback;
        std::getline(std::cin, fallback);
        return fallback;
    }

    WCHAR buffer[1024];
    DWORD charsRead = 0;

    if (!ReadConsoleW(hConsole, buffer, 1024, &charsRead, nullptr)) {
        std::string fallback;
        std::getline(std::cin, fallback);
        return fallback;
    }

    while (charsRead > 0 &&
           (buffer[charsRead - 1] == L'\r' || buffer[charsRead - 1] == L'\n')) {
        charsRead--;
    }

    int utf8Len = WideCharToMultiByte(
        CP_UTF8, 0,
        buffer, charsRead,
        nullptr, 0,
        nullptr, nullptr
    );

    std::string result(utf8Len, '\0');

    WideCharToMultiByte(
        CP_UTF8, 0,
        buffer, charsRead,
        &result[0], utf8Len,
        nullptr, nullptr
    );

    return result;

#else
    std::string input;
    std::getline(std::cin, input);
    return input;
#endif
}


void showWeatherForCity(const std::string& city, const std::string& apiKey) {
    ApiResult result = requestWeatherFromApi(city, apiKey);

    if (!result.success) {
        printError("[ERROR] " + result.errorMessage);
        return;
    }

    WeatherInfo w = result.weather;

    std::cout << "\n==============================\n";
    std::cout << "Сегодня в городе: " << w.city << "\n";
    std::cout << "==============================\n";
    std::cout << "[T] Температура:     " << w.temperature << " °C\n";
    std::cout << "[~] Ощущается как:   " << w.feelsLike << " °C\n";
    std::cout << "[W] Погода:          " << w.description << "\n";
    std::cout << "[H] Влажность:       " << w.humidity << " %\n";
    std::cout << "[V] Ветер:           " << w.windSpeed << " м/с\n";
    std::cout << "[P] Давление:        " << w.pressure << " гПа\n";
    std::cout << "==============================\n";
}



void showWeatherByCity(const std::string& apiKey) {
    while (true) {
        std::string city;
        std::cout << "\nВведите название города (0 — назад): ";
        city = readUtf8Line();

        if (city == "0") return;
        if (city.empty()) {
            printError("[ERROR] Название города не может быть пустым.");
            continue;
        }

        city = translateCityToEnglish(city);

        ApiResult result = requestWeatherFromApi(city, apiKey);
        if (!result.success) {
            printError("[ERROR] " + result.errorMessage);
            continue;
        }

        showWeatherForCity(city, apiKey);
        return;
    }
}



std::vector<std::string> loadFavorites() {
    std::vector<std::string> favorites;
    std::ifstream file("favorites.txt");
    std::string city;

    while (std::getline(file, city)) {
        if (!city.empty()) favorites.push_back(city);
    }
    return favorites;
}



void saveFavorites(const std::vector<std::string>& favorites) {
    std::ofstream file("favorites.txt");
    for (const auto& city : favorites) {
        file << city << "\n";
    }
}



void favoritesMenu(const std::string& apiKey) {
    while (true) {
        std::cout << "\n=== ИЗБРАННЫЕ ГОРОДА ===\n";
        std::cout << "1) Показать список\n";
        std::cout << "2) Добавить город\n";
        std::cout << "3) Удалить город\n";
        std::cout << "4) Показать погоду\n";
        std::cout << "0) Назад\n";
        std::cout << "Ваш выбор: ";

        std::string choice;
        choice = readUtf8Line();
        auto favorites = loadFavorites();
        if (choice == "1") {
            if (favorites.empty()) {
                std::cout << "Список пуст.\n";
            }
            else {
                for (int i = 0; i < favorites.size(); ++i)
                    std::cout << i + 1 << ") " << favorites[i] << "\n";
            }
        }
        else if (choice == "2") {
            std::string city;
            std::cout << "Введите город: ";
            city = readUtf8Line();
            if (!city.empty()) {
                city = translateCityToEnglish(city);
                if (isCityInFavorites(city)) {
                    printWarning("[!] Этот город уже в избранном.");
                }
                else {
                    favorites.push_back(city);
                    saveFavorites(favorites);
                    printOk("[OK] Город добавлен.");
                }
            }
        }
        else if (choice == "3") {
            if (favorites.empty()) {
                std::cout << "Список пуст.\n";
                continue;
            }
            std::cout << "Введите номер: ";
            std::string n;
            n = readUtf8Line();
            int idx = safeStoi(n) - 1;
            if (idx >= 0 && idx < (int)favorites.size()) {
                favorites.erase(favorites.begin() + idx);
                saveFavorites(favorites);
                printOk("[OK] Город удален.");
            }
            else {
                printError("[ERROR] Неверный номер.");
            }
        }
        else if (choice == "4") {
            if (favorites.empty()) continue;
            std::cout << "Введите номер: ";
            std::string n;
            n = readUtf8Line();
            int idx = safeStoi(n) - 1;
            if (idx >= 0 && idx < (int)favorites.size()) {
                showWeatherForCity(favorites[idx], apiKey);
            }
            else {
                printError("[ERROR] Неверный номер.");
            }
        }
        else if (choice == "0") {
            return;
        }
    }
}



void aboutApp() {
    std::cout << "\nО приложении:\n";
    std::cout << "Консольное приложение прогноза погоды.\n";
    std::cout << "Используется сервис OpenWeatherMap.\n\n";
    std::cout << "Авторы приложения:\n - Левашов Дмитрий\n - Новиков Юрий \n - Грачев Александр \n - Гаврилов Роман\n\n";
}



uint32_t decodeUtf8(const std::string& s, size_t& i) {
    if (i >= s.size()) return 0;
    unsigned char c = static_cast<unsigned char>(s[i]);
    uint32_t cp = 0;
    size_t len = 0;
    if ((c & 0x80) == 0) {
        cp = c;
        len = 1;
    }
    else if ((c & 0xE0) == 0xC0) {
        if (i + 1 >= s.size()) return 0;
        cp = (c & 0x1F) << 6;
        cp |= (static_cast<unsigned char>(s[i + 1]) & 0x3F);
        len = 2;
    }
    else if ((c & 0xF0) == 0xE0) {
        if (i + 2 >= s.size()) return 0;
        cp = (c & 0x0F) << 12;
        cp |= (static_cast<unsigned char>(s[i + 1]) & 0x3F) << 6;
        cp |= (static_cast<unsigned char>(s[i + 2]) & 0x3F);
        len = 3;
    }
    else if ((c & 0xF8) == 0xF0) {
        if (i + 3 >= s.size()) return 0;
        cp = (c & 0x07) << 18;
        cp |= (static_cast<unsigned char>(s[i + 1]) & 0x3F) << 12;
        cp |= (static_cast<unsigned char>(s[i + 2]) & 0x3F) << 6;
        cp |= (static_cast<unsigned char>(s[i + 3]) & 0x3F);
        len = 4;
    }
    else {
        return 0;
    }
    i += len;
    return cp;
}

std::string translateCityToEnglish(const std::string& input) {

    static const std::unordered_map<std::string, std::string> cityDict = {
        {"Москва", "Moscow"},
        {"Санкт-Петербург", "Saint Petersburg"},
        {"Санкт Петербург", "Saint Petersburg"},
        {"Питер", "Saint Petersburg"},
        {"Новосибирск", "Novosibirsk"},
        {"Екатеринбург", "Yekaterinburg"},
        {"Нижний Новгород", "Nizhny Novgorod"},
        {"Казань", "Kazan"},
        {"Челябинск", "Chelyabinsk"},
        {"Омск", "Omsk"},
        {"Самара", "Samara"},
        {"Уфа", "Ufa"},
        {"Красноярск", "Krasnoyarsk"},
        {"Пермь", "Perm"},
        {"Воронеж", "Voronezh"},
        {"Волгоград", "Volgograd"},
        {"Краснодар", "Krasnodar"},
        {"Саратов", "Saratov"},
        {"Тюмень", "Tyumen"},
        {"Ижевск", "Izhevsk"},
        {"Барнаул", "Barnaul"},
        {"Иркутск", "Irkutsk"},
        {"Хабаровск", "Khabarovsk"},
        {"Ярославль", "Yaroslavl"},
        {"Владивосток", "Vladivostok"},
        {"Томск", "Tomsk"},
        {"Кемерово", "Kemerovo"},
        {"Рязань", "Ryazan"},
        {"Астрахань", "Astrakhan"},
        {"Пенза", "Penza"},
        {"Киров", "Kirov"},
        {"Калининград", "Kaliningrad"},
        {"Тула", "Tula"},
        {"Тверь", "Tver"},
        {"Сочи", "Sochi"},
        {"Мурманск", "Murmansk"},
        {"Париж", "Paris"},
        {"Лондон", "London"},
        {"Рим", "Rome"},
        {"Берлин", "Berlin"},
        {"Мадрид", "Madrid"},
        {"Пекин", "Beijing"},
        {"Токио", "Tokyo"},
        {"Минск", "Minsk"},
        {"Киев", "Kyiv"},
        {"Астана", "Astana"}
    };

    static const std::unordered_map<uint32_t, std::string> translit = {
        {0x0410,"A"}, {0x0430,"a"},
        {0x0411,"B"}, {0x0431,"b"},
        {0x0412,"V"}, {0x0432,"v"},
        {0x0413,"G"}, {0x0433,"g"},
        {0x0414,"D"}, {0x0434,"d"},
        {0x0415,"E"}, {0x0435,"e"},
        {0x0401,"Yo"},{0x0451,"yo"},
        {0x0416,"Zh"},{0x0436,"zh"},
        {0x0417,"Z"}, {0x0437,"z"},  
        {0x0418,"I"}, {0x0438,"i"}, 
        {0x0419,"Y"}, {0x0439,"y"},
        {0x041A,"K"}, {0x043A,"k"},
        {0x041B,"L"}, {0x043B,"l"},   
        {0x041C,"M"}, {0x043C,"m"},  
        {0x041D,"N"}, {0x043D,"n"},
        {0x041E,"O"}, {0x043E,"o"}, 
        {0x041F,"P"}, {0x043F,"p"},
        {0x0420,"R"}, {0x0440,"r"}, 
        {0x0421,"S"}, {0x0441,"s"},
        {0x0422,"T"}, {0x0442,"t"},
        {0x0423,"U"}, {0x0443,"u"}, 
        {0x0424,"F"}, {0x0444,"f"}, 
        {0x0425,"Kh"},{0x0445,"kh"}, 
        {0x0426,"Ts"},{0x0446,"ts"},
        {0x0427,"Ch"},{0x0447,"ch"},
        {0x0428,"Sh"},{0x0448,"sh"},
        {0x0429,"Shch"},{0x0449,"shch"}, 
        {0x042A,""},  {0x044A,""},
        {0x042B,"Y"}, {0x044B,"y"}, 
        {0x042C,""},  {0x044C,""},   
        {0x042D,"E"}, {0x044D,"e"}, 
        {0x042E,"Yu"},{0x044E,"yu"}, 
        {0x042F,"Ya"},{0x044F,"ya"}   
    };

    auto cityIt = cityDict.find(input);
    if (cityIt != cityDict.end()) {
        return cityIt->second;
    }

    bool hasCyrillic = false;
    size_t pos = 0;
    while (pos < input.size()) {
        size_t oldPos = pos;
        uint32_t cp = decodeUtf8(input, pos);
        if (cp >= 0x0400 && cp <= 0x04FF) {
            hasCyrillic = true;
            break;
        }
    }
    if (!hasCyrillic) return input;

    std::string result;
    pos = 0;
    while (pos < input.size()) {
        size_t oldPos = pos;
        uint32_t cp = decodeUtf8(input, pos);
        if (cp == 0) {
            result += input[oldPos];
            pos = oldPos + 1;
            continue;
        }
        auto t = translit.find(cp);
        if (t != translit.end()) {
            result += t->second;
        }
        else {
            result += input.substr(oldPos, pos - oldPos);
        }
    }
    return result;
}



bool isCityInFavorites(const std::string& city) {
    auto favorites = loadFavorites();
    for (const auto& fav : favorites) {
        if (fav == city) 
            return true;
    }
    return false;
}



int safeStoi(const std::string& str, int fallback) {
    try {
        return std::stoi(str);
    }
    catch (...) {
        return fallback;
    }
}