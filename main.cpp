#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

// Заглушки для функций прогноза погоды
std::string getCurrentWeather() {
    return "Текущая погода: Солнечно, +20°C";
}

std::string getForecastWeather() {
    return "Прогноз на 3 дня:\nДень 1: Облачно, +18°C\nДень 2: Дождь, +15°C\nДень 3: Ясно, +22°C";
}

int main() {
    int choice;
    
    // Для Windows: устанавливаем широкие символы для консоли
    // _setmode(_fileno(stdout), _O_U16TEXT);
    // _setmode(_fileno(stdin), _O_U16TEXT);
    
    // Устанавливаем локаль для поддержки кириллицы
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    
    std::cout << "=== СтандартWX - Прогноз погоды ===" << std::endl;
    
    do {
        
        std::cout << "\nВыберите опцию:" << std::endl;
        std::cout << "1. Текущая погода" << std::endl;
        std::cout << "2. Прогноз погоды на 3 дня" << std::endl;
        std::cout << "3. Выход" << std::endl;
        std::cout << "Ваш выбор: ";
        
        if (!(std::cin >> choice)) {
            std::cout << "\nОшибка ввода. Пожалуйста, введите число." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            choice = 0;
        }
        
        switch(choice) {
            case 1:
                std::cout << "\n" << getCurrentWeather() << std::endl;
                break;
            case 2:
                std::cout << "\n" << getForecastWeather() << std::endl;
                break;
            case 3:
                std::cout << "\nДо свидания!" << std::endl;
                break;
            default:
                std::cout << "\nНеверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while(choice != 3);
    
    return 0;
}