//
// Created by ASUS on 24.03.2026.
//

#ifndef _WX_API_H
#define _WX_API_H

// Базовый URL для OpenWeather API
constexpr const char* OPENWEATHER_API_BASE_URL = "https://api.openweathermap.org/data/2.5";

// Получение API ключа из переменных окружения
const char* getOpenWeatherApiKey() {
    const char* key = std::getenv("OPENWEATHER_API_KEY");
    return key ? key : ""; // возвращаем пустую строку если переменная не найдена
}

#endif //_WX_API_H