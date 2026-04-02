//
// Created by ASUS on 24.03.2026.
//

// При использовании с русским текстом рекомендуется установить локаль:
// setlocale(LC_ALL, "Russian");

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <optional>


using std::string;
using std::round;
using std::optional;
using std::nullopt;


constexpr double KELVIN_OFFSET = 273.15;
constexpr double PASCAL_TO_MMHG = 0.75006;


typedef enum { HUNDREDTHS = 3, INTEGER = 1, TENTHS = 2 } Precision;


namespace WeatherUtils {
    // перевод температуры из Кельвина в Цельсия
    double kelvin_To_Celsius(double kelvin)
    {
        return kelvin - KELVIN_OFFSET;
    }

    // перевод давления из Паскаль в мм рт ст
    double pascal_To_MmHg(double pascal)
    {
        return pascal * PASCAL_TO_MMHG;
    }


    // форматированный вывод
    double formatValue(double value, Precision precision)
    {
        if (precision == HUNDREDTHS)
            return std::round(value * 100.0) / 100.0;
        if (precision == TENTHS)
            return std::round(value * 10.0) / 10.0;
        if (precision == INTEGER)
            return std::round(value);
        return std::round(value);
    }

    // форматированное вывод времени
    string formatTimestamp(long long timestamp, int timezoneOffset)
    {
        time_t loc_time = (time_t)(timestamp + timezoneOffset);
        struct tm* time_info = std::gmtime(&loc_time);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", time_info);

        return string(buffer);
    }
}



// класс погоды_сейчас
class Weather_Now
{
private:
    string cityName;                              // название города

    optional<double> temperatureKelvin;           // температура в кельвинах
    optional<double> temperatureCelsius;          // температура в градусах цельсия
    optional<int> humidity;                       // влажность
    optional<double> pressurePascal;              // давление в паскалях
    optional<double> pressureMmHg;                // давление в мм

    optional<double> windSpeed;                   // скорость ветра
    optional<int> windDegrees;                    // направление ветра
    optional<double> windGust;                    // порывы ветра

    optional<int> timezoneOffset;                 // часовой пояс
    optional<long long> measurementTime;          // время замера
    optional<long long> sunsetTime, sunriseTime;  //время заката и рассвета

    string description;                           // описание




public:
    // конструктор по умолчанию
    Weather_Now(const string& name) : cityName(name) {
        description = "Нет данных.";
    }

    // конструктор полноценный
    Weather_Now(
        const string& name,
        optional<double> kelvin = nullopt,
        optional<int> hum = nullopt,
        optional<double> pascal = nullopt,
        optional<double> windSpd = nullopt,
        optional<int> windDeg = nullopt,
        optional<double> gust = nullopt,
        optional<int> tzOffset = nullopt,
        optional<long long> measTime = nullopt,
        optional<long long> sunset = nullopt,
        optional<long long> sunrise = nullopt,
        const string& desc = "Нет данных.") : 
        cityName(name),
        temperatureKelvin(kelvin),
        humidity(hum),
        pressurePascal(pascal),
        windSpeed(windSpd),
        windDegrees(windDeg),
        windGust(gust),
        timezoneOffset(tzOffset),
        measurementTime(measTime),
        sunsetTime(sunset),
        sunriseTime(sunrise),
        description(desc)
    {
        if (temperatureKelvin.has_value()) {
            temperatureCelsius = WeatherUtils::kelvin_To_Celsius(temperatureKelvin.value());
        }

        if (pressurePascal.has_value()) {
            pressureMmHg = WeatherUtils::pascal_To_MmHg(pressurePascal.value());
        }
    }



    // выдать название города
    string get_cityName() const {
        return cityName;
    }

    // выдать температуру в кельвинах
    optional<double> get_temperatureKelvin(Precision precision = TENTHS) const {
        if (!temperatureKelvin.has_value()) return nullopt;
        return WeatherUtils::formatValue(temperatureKelvin.value(), precision);
    }

    // выдать температуру в цельсия
    optional<double> get_temperatureCelsius(Precision precision = TENTHS) const {
        if (!temperatureCelsius.has_value()) return nullopt;
        return WeatherUtils::formatValue(temperatureCelsius.value(), precision);
    }

    // выдать влажность
    optional<int> get_humidity() const {
        return humidity;
    }

    // выдать скорость ветра
    optional<double> get_windSpeed(Precision precision = TENTHS) const {
        if (!windSpeed.has_value()) return nullopt;
        return WeatherUtils::formatValue(windSpeed.value(), precision);
    }

    // выдать описание
    string get_description() const {
        return description;
    }

    // выдать направление ветра (текст)
    string get_windDirectionText() const {
        if (!windDegrees.has_value()) return "Неизвестно";

        int deg = windDegrees.value();
        if (deg >= 337.5 || deg < 22.5)   return "Северный";
        if (deg >= 22.5 && deg < 67.5)    return "Северо-восточный";
        if (deg >= 67.5 && deg < 112.5)   return "Восточный";
        if (deg >= 112.5 && deg < 157.5)  return "Юго-восточный";
        if (deg >= 157.5 && deg < 202.5)  return "Южный";
        if (deg >= 202.5 && deg < 247.5)  return "Юго-западный";
        if (deg >= 247.5 && deg < 292.5)  return "Западный";
        if (deg >= 292.5 && deg < 337.5)  return "Северо-западный";
        return "Неизвестно";
    }

    // выдать давление в паскалях
    optional<double> get_pressurePascal(Precision precision = INTEGER) const {
        if (!pressurePascal.has_value()) return nullopt;
        return WeatherUtils::formatValue(pressurePascal.value(), precision);
    }

    // выдать давление в мм рт ст
    optional<double> get_pressureMmHg(Precision precision = TENTHS) const {
        if (!pressureMmHg.has_value()) return nullopt;
        return WeatherUtils::formatValue(pressureMmHg.value(), precision);
    }

    // выдать порывы ветра
    optional<double> get_windGust(Precision precision = TENTHS) const {
        if (!windGust.has_value()) return nullopt;
        return WeatherUtils::formatValue(windGust.value(), precision);
    }

    // выдать время замера
    string get_measurementTime() const {
        if (!measurementTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(measurementTime.value(), timezoneOffset.value());
    }

    // выдать время рассвета
    string get_sunriseTime() const {
        if (!sunriseTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(sunriseTime.value(), timezoneOffset.value());
    }
    // выдать время заката
    string get_sunsetTime() const {
        if (!sunsetTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(sunsetTime.value(), timezoneOffset.value());
    }

    // проверка дня/ночи
    bool isDaytime() const {
        if (!measurementTime.has_value() || !sunriseTime.has_value() || !sunsetTime.has_value()) {
            return true;  // если данных нет, считаем что день
        }
        return (measurementTime.value() >= sunriseTime.value() &&
            measurementTime.value() <= sunsetTime.value());
    }

};