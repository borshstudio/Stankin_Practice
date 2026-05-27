#include <gtest/gtest.h>
#include "api.h"

const std::string MY_API_KEY = "e8e4e26c5b3e29892b3310b6f8dcbc21";

// 1. Тест: Правильный ключ + правильный город
TEST(ApiIntegrationTest, CorrectKeyValidCity) {
    ApiResult result = requestWeatherFromApi("Москва", MY_API_KEY);
    
    EXPECT_TRUE(result.success) << "Ошибка: " << result.errorMessage;
    EXPECT_EQ(result.statusCode, 200);
    EXPECT_FALSE(result.weather.city.empty());
}

// 2. Тест: Правильный ключ + несуществующий город
TEST(ApiIntegrationTest, CorrectKeyInvalidCity) {
    ApiResult result = requestWeatherFromApi("СлучайныйГород", MY_API_KEY);
    
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.statusCode, 404);
    EXPECT_EQ(result.errorMessage, "Город не найден.");
}

// 3. Тест: Проверка структуры данных
TEST(ApiTest, StructInitialization) {
    WeatherInfo info;

    // 1. Присваиваем значения всем полям структуры
    info.city = "Тест";
    info.temperature = 22.5;
    info.feelsLike = 21.5;
    info.humidity = 60;
    info.pressure = 1000;
    info.windSpeed = 5.4;
    info.description = "небольшой дождь";

    // 2. Проверяем строковые значения (EXPECT_EQ)
    EXPECT_EQ(info.city, "Тест");
    EXPECT_EQ(info.description, "небольшой дождь");

    // 3. Проверяем целочисленные значения (EXPECT_EQ)
    EXPECT_EQ(info.humidity, 60);
    EXPECT_EQ(info.pressure, 1000);

    // 4. Проверяем числа с плавающей точкой (EXPECT_NEAR)
    // Используем погрешность 0.001 для точности
    EXPECT_NEAR(info.temperature, 22.5, 0.001);
    EXPECT_NEAR(info.feelsLike, 21.5, 0.001);
    EXPECT_NEAR(info.windSpeed, 5.4, 0.001);
}