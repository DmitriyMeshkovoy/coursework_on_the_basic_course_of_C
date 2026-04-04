//
// Created by meshk on 31.03.2026.
//

#include "temp_api.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

//REPORT_BY_MONTH = 1
// статистика за месяц за конкретный год
Report *report_by_month(Temperature_data *arr, int size, int year, Month month) {
    float sum_temp = 0.0f;
    int count = 0;
    int8_t min_temp = MAX_TEMP;
    int8_t max_temp = MIN_TEMP;
    int found = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i].year == year && arr[i].month == (uint8_t)month) {
            found = 1;
            int8_t temp = arr[i].temperature;

            sum_temp += temp;
            count++;

            if (temp < min_temp) {
                min_temp = temp;
            }
            if (temp > max_temp) {
                max_temp = temp;
            }
        }
    }

    float avg_temp = sum_temp / count;

    Report *report = (Report*)malloc(sizeof(Report));
    report->arr = (Statistics*)malloc(sizeof(Statistics));

    report->arr[0].year = year;
    report->arr[0].month = month;
    report->arr[0].avg_temp = avg_temp;
    report->arr[0].min_temp = min_temp;
    report->arr[0].max_temp = max_temp;

    report->type = REPORT_BY_MONTH;
    report->elements_count = 1;

    return report;
}

//REPORT_BY_YEAR= 2
//статистика за год в целом
Report *report_by_year(Temperature_data *arr, int size, int year) {
    float sum_temp = 0.0f;
    int count = 0;
    int8_t min_temp = MAX_TEMP;
    int8_t max_temp = MIN_TEMP;
    int found = 0;

    for (int i = 0; i < size; i++) {
        if (arr[i].year == year) {
            found = 1;
            int8_t t = arr[i].temperature;
            sum_temp += t;
            count++;
            if (t < min_temp) min_temp = t;
            if (t > max_temp) max_temp = t;
        }
    }

    float avg_temp = sum_temp / count;

    Report *report = (Report*)malloc(sizeof(Report));
    if (!report) return NULL;
    report->arr = (Statistics*)malloc(sizeof(Statistics));

    report->arr[0].year = year;
    report->arr[0].month = 0;  // 0 означает "не месяц"
    report->arr[0].avg_temp = avg_temp;
    report->arr[0].min_temp = min_temp;
    report->arr[0].max_temp = max_temp;
    report->type = REPORT_BY_YEAR;
    report->elements_count = 1;

    return report;
}

//REPORT_BY_MONTH_FOR_THE_YEAR = 3
//статистика за год в разрезе месяцев
Report *report_by_month_for_the_year(Temperature_data *arr, int size, int year) {
    return NULL;
}

//REPORT_BY_YEAR_ALL_PERIOD = 4
//статистика за весь период в разрезе всех лет
Report *report_by_year_all_period(Temperature_data *arr, int size){
    return NULL;
}

//REPORT_BY_ALL_YEARS_AND_MONTHS = 5
//статистика за весь период в разрезе всех лет, каждый год в разрезе месяцев
Report *report_all_years_and_all_months (Temperature_data *arr, int size){
    return NULL;
}

/*
    + REPORT_BY_MONTH: нужны year и month
    + REPORT_BY_YEAR: нужен year
    потом
    - REPORT_BY_MONTH_FOR_THE_YEAR: нужен year
    - REPORT_BY_YEAR_ALL_PERIOD: игнорируем все параметры
    - REPORT_BY_ALL_YEARS_AND_MONTHS: параметры не нужны
*/
Report *return_statistic(const Request *request) {
    switch (request->type) {
        case REPORT_BY_MONTH: //нужны year и month
            return report_by_month(request->array, request->size, request->year, request->month);
        case REPORT_BY_YEAR: //нужен year
            return report_by_year(request->array, request->size, request->year);
        case REPORT_BY_MONTH_FOR_THE_YEAR: //нужен year
            return report_by_month_for_the_year(request->array, request->size, request->year);
        case REPORT_BY_YEAR_ALL_PERIOD: //игнорируем все параметры year/month
            return report_by_year_all_period(request->array, request->size);
        case REPORT_BY_ALL_YEARS_AND_ALL_MONTHS: //игнорируем все параметры year/month
            return report_all_years_and_all_months (request->array, request->size);
        default:
            return NULL;
    }

}

//вывод в консоль статистики
void print_statistic(const Report *report) {
    for (int i = 0; i < report->elements_count; i++) {
        const Statistics *s = &report->arr[i];
        if (report->type == REPORT_BY_MONTH || report->type == REPORT_BY_MONTH_FOR_THE_YEAR) {
            printf("Year: %d, month: %d\n", s->year, s->month);
        } else if (report->type == REPORT_BY_YEAR) {
            printf("year: %d\n", s->year);
        } else if (report->type == REPORT_BY_YEAR_ALL_PERIOD) {
            printf("year: %d\n", s->year);
        } else if (report->type == REPORT_BY_ALL_YEARS_AND_ALL_MONTHS) {
            printf("Year: %d, month: %d\n", s->year, s->month);
        }
        printf("  avg: %.2f\n", s->avg_temp);
        printf("  Min: %d\n", s->min_temp);
        printf("  Max: %d\n", s->max_temp);
        printf("-------------------\n");
    }
}


