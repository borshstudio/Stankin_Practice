//
// Created by ASUS on 24.03.2026.
//

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>


using std::string;
using std::round;


#define KEL 273.15
#define KOF_MM 0.75006


typedef enum { max_to4nost = 3, min_to4nost = 1, to4nost = 2 } TO4NOST;


// перевод температуры из Кульвинов в Цельсия
double ubersetzen_kel_4el(double a)
{
    return a - KEL;
}

// перевод паскалей в мм рт ст
double ubersetzen_pas_mm(double a)
{
    return a * KOF_MM;
}


// форматированный вывод
double format_value(double value, TO4NOST to4)
{
    if (to4 == max_to4nost)
        return std::round(value * 100.0) / 100.0;
    if (to4 == to4nost)
        return std::round(value * 10.0) / 10.0;
    if (to4 == min_to4nost)
        return std::round(value);
    return std::round(value);
}

// форматированное время
string format_time(long long time_n, int our_pois)
{
    time_t loc_time = (time_t)(time_n + our_pois);
    struct tm* time_info = std::gmtime(&loc_time);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", time_info);

    return string(buffer);
}




// класс погоды
class Weather_Now
{
private:
    string city_name;   // название города

    double temp_kel;    // температура в кельвинах
    double temp_4el;    // температура в градусах цельсия
    int vlaga;   // влажность
    double dav_pas;     // давление в паскалях
    double dav; // давление в мм

    double speed_wind;  // скорость ветра
    int wind_deg; // направление ветра
    double gust; // порывы ветра

    int time_zona;  // часовой пояс
    long long time_3amer;   // время замера
    long long time_3akat, time_pacvet;  //время заката и рассвета

    string opic;    // описание




public:
    Weather_Now(string name, double t_k) { // упрощенное объявление
        city_name = name;
        temp_kel = t_k;
        temp_4el = ubersetzen_kel_4el(t_k);

        vlaga = -1;
        dav_pas = -1;
        dav = -1;

        speed_wind = -1;
        wind_deg = -1;
        gust = -1;

        time_zona = -1;
        time_3amer = -1;
        time_3akat = -1;
        time_pacvet = -1;

        opic = "Нет данных.";
    }

    // полноценное объявление
    Weather_Now(string name, double t_k = -1.0, int vlag = -1, double davlen = -1.0, double sp_w = -1.0,
        int deg = -1, double gg = -1.0, int t_z = -1, long long t = -1,
        long long t_3 = -1, long long t_p = -1) {
        city_name = name;

        temp_kel = t_k;
        if (temp_kel != -1.0)
            temp_4el = ubersetzen_kel_4el(temp_kel);
        else
            temp_4el = -1.0;
        vlaga = vlag;
        dav_pas = davlen;
        if (dav_pas != -1.0)
            dav = ubersetzen_pas_mm(davlen);
        else
            dav = -1.0;

        speed_wind = sp_w;
        wind_deg = deg;
        gust = gg;

        time_zona = t_z;
        time_3amer = t;
        time_3akat = t_3;
        time_pacvet = t_p;

        opic = "Нет данных.";
    }


    // выдать базу
    // выдать город
    string get_city_name() const {
        return city_name;
    }

    // выдать температура в кельвиназх
    double get_temp_kel(TO4NOST to4) const {
        if (temp_kel != -1.0)
            return format_value(temp_kel, to4);
        else
            return -1.0;
    }

    // выдать темпераутра в цельсия
    double get_temp_4el(TO4NOST to4) const {
        if (temp_4el != -1.0)
            return format_value(temp_4el, to4);
        else
            return -1.0;
    }

    // выдать влажность
    int get_vlaga() const {
        if (vlaga != -1.0)
            return round((double)vlaga);
        else
            return -1;
    }

    // выдать скорость ветра
    double get_speed_wind(TO4NOST to4) const {
        if (speed_wind != -1.0)
            return format_value(speed_wind, to4);
        else
            return -1.0;
    }

    // выдать описание
    string get_opic() const {
        return opic;
    }

    // выдавать направление ветра
    string get_wind_direction_text() const {
        if (wind_deg >= 337.5 || wind_deg < 22.5)   return "Северный";
        if (wind_deg >= 22.5 && wind_deg < 67.5)   return "Северо-восточный";
        if (wind_deg >= 67.5 && wind_deg < 112.5)  return "Восточный";
        if (wind_deg >= 112.5 && wind_deg < 157.5)  return "Юго-восточный";
        if (wind_deg >= 157.5 && wind_deg < 202.5)  return "Южный";
        if (wind_deg >= 202.5 && wind_deg < 247.5)  return "Юго-западный";
        if (wind_deg >= 247.5 && wind_deg < 292.5)  return "Западный";
        if (wind_deg >= 292.5 && wind_deg < 337.5)  return "Северо-западный";
        return "Неизвестно";
    }

    // выдать давление в паскалях
    double get_dav_pas(TO4NOST to4) {
        if (dav_pas != -1.0)
            return format_value(dav_pas, to4);
        else
            return -1.0;
    }

    // выдать давление
    double get_dav(TO4NOST to4) {
        if (dav != -1.0)
            return format_value(dav, to4);
        else
            return -1.0;
    }

    // выдать порывы ветра
    double get_gust(TO4NOST to4) {
        if (gust != -1.0)
            return format_value(gust, to4);
        else
            return -1.0;
    }

    // выдатль время замера
    string get_time_3amer() const {
        if (time_3amer != -1.0)
            return format_time(time_3amer, time_zona);
        else
            return "Нет данных.";
    }

    // выдать время рассвета 
    string get_time_pacvet() const {
        if (time_pacvet != -1.0)
            return format_time(time_pacvet, time_zona);
        else
            return "Нет данных.";
    }

    // выдать время заката
    string get_time_3akat() const {
        if (time_3akat != -1.0)
            return format_time(time_3akat, time_zona);
        else
            return "Нет данных.";
    }

    //день ли это
    bool is_daytime() const {
        if (time_3amer == -1 || time_pacvet == -1 || time_3akat == -1) {
            return true;
        }

        return (time_3amer >= time_pacvet && time_3amer <= time_3akat);
    }
};