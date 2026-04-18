//
// Created by meshk on 31.03.2026.
//

#include "temp_functions.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//REPORT_BY_MONTH = 1
// статистика за месяц за конкретный год
Report *report_by_month(Temperature_data *arr, int size, int year, Month month) {
    // 1. Проверка входных параметров
    if (arr == NULL || size <= 0) {
        fprintf(stderr, "report_by_month: invalid array or size\n");
        return NULL;
    }

    if (year < 1900 || year > 2100) {
        fprintf(stderr, "report_by_month: invalid year %d\n", year);
        return NULL;
    }

    if (month < JAN || month > DEC) {
        fprintf(stderr, "report_by_month: invalid month %d\n", month);
        return NULL;
    }

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
    if (!found) {
        fprintf(stderr, "report_by_month: no data found  %d-%d\n", year, month);
        return NULL;
    }
    float avg_temp = sum_temp / count;

    Report *report = (Report*)malloc(sizeof(Report));
    if (report == NULL) {
        fprintf(stderr, "report_by_month: memory failed\n");
        return NULL;
    }
    report->arr = (Statistics*)malloc(sizeof(Statistics));
    if (report->arr == NULL) {
        fprintf(stderr, "report_by_month: memory failed\n");
        free(report);
        return NULL;
    }

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
    if (arr == NULL || size <= 0) {
        fprintf(stderr, "report_by_year: invalid array or size\n");
        return NULL;
    }
    if (year < 1900 || year > 2100) {
        fprintf(stderr, "report_by_year: invalid year %d\n", year);
        return NULL;
    }

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

    if (!found) {
        fprintf(stderr, "report_by_year: no data for %d\n", year);
        return NULL;
    }

    float avg_temp = sum_temp / count;

    Report *report = (Report*)malloc(sizeof(Report));
    if (!report) return NULL;
    report->arr = (Statistics*)malloc(sizeof(Statistics));
    if (!report->arr) {
        free(report);
        return NULL;
    }

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
    (void)arr; (void)size; (void)year;
    return NULL;
}

//REPORT_BY_YEAR_ALL_PERIOD = 4
//статистика за весь период в разрезе всех лет
Report *report_by_year_all_period(Temperature_data *arr, int size) {
    (void)arr; (void)size;
    return NULL;
}

//REPORT_BY_ALL_YEARS_AND_MONTHS = 5
//статистика за весь период в разрезе всех лет, каждый год в разрезе месяцев
Report *report_all_years_and_all_months (Temperature_data *arr, int size) {
    (void)arr; (void)size;
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
    if (request==NULL) {
        fprintf(stderr, "report is NULL");
        return NULL;
    }
    if (request->array ==NULL) {
        fprintf(stderr, "request array is NULL");
        return NULL;
    }
    if (request->size<=0) {
        fprintf(stderr, "request size <= 0. ");
        return NULL;
    }
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
            fprintf(stderr, "unknown report type %d", request->type);
            return NULL;
    }


}

//вывод в консоль статистики
void print_statistic(const Report *report) {
    if (report == NULL || report->arr == NULL) {
        printf("No data\n");
        return;
    }

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

//
//почистить за собой
//
bool free_report(Report *report) {
    if (report == NULL) return true;
    if (report->arr != NULL) free(report->arr);
    free(report);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///второе задание курсовой начинается тут, предыдущую часть поторопился (сделал в прошлый раз)
///это пункт 3
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Сравнение для qsort
static int compare_by_date(const void *a, const void *b) {
    const Temperature_data *da = (const Temperature_data*)a;
    const Temperature_data *db = (const Temperature_data*)b;
    if (da->year != db->year) return da->year - db->year;
    if (da->month != db->month) return da->month - db->month;
    if (da->day != db->day) return da->day - db->day;
    if (da->hour != db->hour) return da->hour - db->hour;
    return da->minutes - db->minutes;
}

void sort_by_date(Temperature_data *arr, int size) {
    if (arr == NULL || size <= 1) return;
    qsort(arr, size, sizeof(Temperature_data), compare_by_date);
}


void print_array(const Temperature_data *arr, int size) {
    if (arr == NULL) {
        printf("Array is NULL\n");
        return;
    }
    //нет описания как выводить, поэтомцу сдела лпока так
    printf("Year Month Day Hour Min Temp\n");
    for (int i = 0; i < size; i++) {
        printf("%4hu %2hhu %2hhu %2hhu %2hhu %3hhd\n",
               arr[i].year, arr[i].month, arr[i].day,
               arr[i].hour, arr[i].minutes, arr[i].temperature);
    }
}

int add_record(Temperature_data *arr, int *size, int max_size, const Temperature_data *new_record) {
    if (arr == NULL || size == NULL || new_record == NULL) return -1;
    if (*size >= max_size) return -1;
    arr[*size] = *new_record;
    (*size)++;
    return 0;
}

int delete_record(Temperature_data *arr, int *size, int index) {
    if (arr == NULL || size == NULL) return -1;
    if (index < 0 || index >= *size) return -1;
    for (int i = index; i < *size - 1; i++) {
        arr[i] = arr[i+1];
    }
    (*size)--;
    return 0;
}

int load_from_csv(const char *filename, Temperature_data **arr, int *size) {
    if (!filename || !arr || !size) return -1;
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    int capacity = 1000;  // начальная емкость
    *arr = malloc(capacity * sizeof(Temperature_data));
    if (!*arr) { fclose(f); return -1; }

    int count = 0;
    char line[256];
    int line_no = 0;

    while (fgets(line, sizeof(line), f)) {
        line_no++;
        int y, m, d, h, min, t;
        if (sscanf(line, "%d;%d;%d;%d;%d;%d", &y, &m, &d, &h, &min, &t) != 6) {
            fprintf(stderr, "Parse error line %d\n", line_no);
            continue;
        }
        // валидация полей
        if (y < 1900 || y > 2100) { fprintf(stderr, "Invalid year line %d\n", line_no); continue; }
        if (m < 1 || m > 12) { fprintf(stderr, "Invalid month line %d\n", line_no); continue; }
        if (d < 1 || d > 31) { fprintf(stderr, "Invalid day line %d\n", line_no); continue; }
        if (h < 0 || h > 23) { fprintf(stderr, "Invalid hour line %d\n", line_no); continue; }
        if (min < 0 || min > 59) { fprintf(stderr, "Invalid minute line %d\n", line_no); continue; }
        if (t < MIN_TEMP || t > MAX_TEMP) { fprintf(stderr, "Temp out of range line %d\n", line_no); continue; }

        if (count >= capacity) {
            capacity *= 2;
            Temperature_data *new_arr = realloc(*arr, capacity * sizeof(Temperature_data));
            if (!new_arr) {
                fprintf(stderr, "Memory allocation ERROR\n");
                free(*arr);
                fclose(f);
                return -1;
            }
            *arr = new_arr;
        }
        (*arr)[count].year = y;
        (*arr)[count].month = m;
        (*arr)[count].day = d;
        (*arr)[count].hour = h;
        (*arr)[count].minutes = min;
        (*arr)[count].temperature = t;
        count++;
    }
    fclose(f);
    *size = count;
    // Не уменьшаем память, можно оставить как есть
    return count;
}

//сделать потом
int save_to_csv(const char *filename, const Temperature_data *arr, int size) {
    (void)filename; (void)arr; (void)size;
    return 0;
}

void print_full_statistics(Temperature_data *arr, int size) {
    if (!arr || size <= 0) return;
    int year = arr[0].year; // по ТЗ все данные за один год
    printf("\n=== Monthly statistics for %d ===\n", year);
    for (int m = JAN; m <= DEC; m++) {
        Report *r = report_by_month(arr, size, year, (Month)m);
        if (r) {
            printf("Month %d:\n", m);
            print_statistic(r);
            free_report(r);
        } else {
            printf("Month %d: no data\n", m);
        }
    }
    printf("\n=== Yearly statistics for %d ===\n", year);
    Report *yr = report_by_year(arr, size, year);
    if (yr) {
        print_statistic(yr);
        free_report(yr);
    } else {
        printf("No data for year %d\n", year);
    }
}