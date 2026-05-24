#ifndef WEATHER_H
#define WEATHER_H

#include <string>

void showWeatherByCity(const std::string& apiKey);
void showWeatherForCity(const std::string& city, const std::string& apiKey);

void favoritesMenu(const std::string& apiKey);
void aboutApp();

std::string translateCityToEnglish(const std::string& input);
int safeStoi(const std::string& str, int fallback = -1);

#endif