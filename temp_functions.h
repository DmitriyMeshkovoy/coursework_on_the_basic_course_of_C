//
// Created by meshk on 31.03.2026.
//
#include <stdbool.h>
#include <stdint.h>
#ifndef TEMP_API_H
#define TEMP_API_H
#define MIN_TEMP -99
#define MAX_TEMP 99
//по работе с массивом
/////////////////////////////////////////////////////////////////////////////////////////
//1. общее
/////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7, AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12
} Month;

//для обозначения типа статистики
typedef enum {
    REPORT_BY_MONTH = 1,                //статистика за месяц за конкретный год
    REPORT_BY_YEAR= 2,                  //статистика за год в целом
    REPORT_BY_MONTH_FOR_THE_YEAR = 3,   //статистика за год в разрезе месяцев
    REPORT_BY_YEAR_ALL_PERIOD = 4,      //статистика за весь период в разрезе всех лет
    REPORT_BY_ALL_YEARS_AND_ALL_MONTHS = 5  //статистика за весь период в разрезе всех лет, каждый год в разрезе месяцев
} Report_type;

//одна запись
//тут упаковка не нужна, т.к. работаем с оперативной памятью
typedef struct {
    uint16_t year; //2б
    uint8_t month; //1б
    uint8_t day; //1б
    uint8_t hour; //1б
    uint8_t minutes; //1б
    int8_t temperature; //1б
} Temperature_data; //итого 7 байт

/////////////////////////////////////////////////////////////////////////////////////////
//2.запрос
/////////////////////////////////////////////////////////////////////////////////////////
//структура для запроса
typedef struct {
    Report_type type;
    Temperature_data *array;
    int size;
    int year;
    Month month;
} Request;

/////////////////////////////////////////////////////////////////////////////////////////
//3. ответ
/////////////////////////////////////////////////////////////////////////////////////////
//элемент репорта
typedef struct {
    int year;
    Month month;
    float avg_temp;
    int8_t min_temp;
    int8_t max_temp;
} Statistics;
//сам отчет
typedef struct {
    Report_type type;
    int elements_count;
    Statistics *arr;
} Report;

/////////////////////////////////////////////////////////////////////////////////////////
//4. прототипы функций
/////////////////////////////////////////////////////////////////////////////////////////
/*
статистика за период
    REPORT_BY_MONTH: нужны year и month
    REPORT_BY_YEAR: нужен year
    REPORT_BY_MONTH_FOR_THE_YEAR: нужен year
    REPORT_BY_YEAR_ALL_PERIOD: игнорируем все параметры
    REPORT_BY_ALL_YEARS_AND_ALL_MONTHS: параметры не нужны
*/
//собрать статистику
Report* return_statistic (const Request *request);

//в зависимости от типа отчета будет вызываться соответсвующая функция

//REPORT_BY_MONTH = 1
// статистика за месяц за конкретный год
Report* report_by_month(Temperature_data *arr, int size, int year, Month month);

//REPORT_BY_YEAR= 2
//статистика за год в целом
Report* report_by_year(Temperature_data *arr, int size, int year);

//REPORT_BY_MONTH_FOR_THE_YEAR = 3
//статистика за год в разрезе месяцев
Report* report_by_month_for_the_year(Temperature_data *arr, int size, int year);

//REPORT_BY_YEAR_ALL_PERIOD = 4
//т.е.статистика за весь период в разрезе всех лет
Report* report_by_year_all_period(Temperature_data *arr, int size);

//REPORT_BY_ALL_YEARS_AND_ALL_MONTHS = 5
//статистика за весь период в разрезе всех лет, каждый год в разрезе месяцев
Report* report_all_years_and_all_months (Temperature_data *arr, int size);

//вывод в консоль статистики
void print_statistic (const Report *report);

//очитска
bool free_report(Report *report);
/////////////////////////////////////////////////////////////////////////////////////////
//5. утилитные функции для работы с массивом
/////////////////////////////////////////////////////////////////////////////////////////

void print_array(const Temperature_data *arr, int size);
int add_record(Temperature_data *arr, int *size, int max_size, const Temperature_data *new_record);
int delete_record(Temperature_data *arr, int *size, int index);
void sort_by_date(Temperature_data *arr, int size);
// int load_from_csv(const char *filename, Temperature_data *arr, int max_size); //так было со статическим массивом
int load_from_csv(const char *filename, Temperature_data **arr, int *size);
void print_full_statistics(Temperature_data *arr, int size);
//следующим этапом
int save_to_csv(const char *filename, const Temperature_data *arr, int size);

#endif // TEMP_API_H