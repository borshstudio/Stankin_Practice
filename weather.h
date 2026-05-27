#ifndef WEATHER_H
#define WEATHER_H

#include <string>
#include <vector>

std::string readUtf8Line();
void showWeatherByCity(const std::string& apiKey);
void showWeatherForCity(const std::string& city, const std::string& apiKey);

std::vector<std::string> loadFavorites();
void saveFavorites(const std::vector<std::string>& favorites);

void favoritesMenu(const std::string& apiKey);
void aboutApp();

std::string translateCityToEnglish(const std::string& input);
bool isCityInFavorites(const std::string& city);
int safeStoi(const std::string& str, int fallback = -1);

#endif