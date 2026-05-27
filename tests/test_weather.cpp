#include <gtest/gtest.h>
#include "weather.h"
#include <vector>
#include <string>
/*
std::string readUtf8Line() {
    return "";
}
*/
// Тест: Корректное сохранение и загрузка избранного
TEST(WeatherLogicTest, FavoritesSaveAndLoad) {
    std::vector<std::string> originalFavs = loadFavorites();

    std::vector<std::string> testFavs = {"Москва", "Саратов", "Санкт-Петербург"};
    
    saveFavorites(testFavs);
    std::vector<std::string> loadedFavs = loadFavorites();
    
    ASSERT_EQ(testFavs.size(), loadedFavs.size());
    for (size_t i = 0; i < testFavs.size(); ++i) {
        EXPECT_EQ(testFavs[i], loadedFavs[i]);
    }

    saveFavorites(originalFavs);
}

// Тесты: проверка правильного перевода городов
TEST(WeatherLogicTest, CityTranslationDictionary) {
    EXPECT_EQ(translateCityToEnglish("Москва"), "Moscow");
}

TEST(WeatherLogicTest, CityTransliteration) {
    EXPECT_EQ(translateCityToEnglish("Тюмень"), "Tyumen");
}