//
// Created by meshk on 31.03.2026.
//

/*
 Поддерживаемые ключи:
-h                     описание (справка)
-f file.csv          загрузить данные из CSV (обязательный)
-t <type>              тип отчёта: month, year, month-for-year, year-all-period, full
-m <month>             номер месяца (1-12) для типа month
-y <year>              год для типов month, year, month-for-year
-s                     сортировать данные по дате после загрузки
-p                     вывести загруженный массив на экран
*/
/*
Примеры вызовов:
-f data.csv -p                         только печать
-f data.csv -s -p                      сортировка + печать
-f data.csv -t month -y 2026 -m 1      статистика за январь 2026
-f data.csv -t year -y 2026            статистика за 2024 год
-f data.csv -t month-for-year -y 2026  год в разрезе месяцев (заглушка)
-f data.csv -t year-all-period         все годы (заглушка)
-f data.csv -t full                    все годы и месяцы (заглушка)
-h
TO DO для ясности надо потом поправить наименования функций, чтобы они совпадали с ключами
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_api.h"

#define MAX_RECORDS 10000

//Подскажите, правильно ли оставлять такие функции в main или в отдельный все же лучше?
void print_help(char *progname) {
    printf("Usage: %s [options]\n", progname);
    printf("Options:\n");
    printf("  -h                     Show this\n");
    printf("  -f <file.csv>          Load temperature data from CSV (required)\n");
    printf("  -t <type>              Report type: month, year(required for statistics),\n");
    printf("                         month-for-year, year-all-period, full (maybe I'll do it latter)\n");
    printf("  -m <month>             Month number (1-12) for 'month' type\n");
    printf("  -y <year>              Year for 'month', 'year', 'month-for-year' types\n");
    printf("  -s                     sort data by date (after loading)\n");
    printf("  -p                     print all records\n");
}

int main(int argc, char *argv[]) {
    const char *filename = NULL;
    const char *report_type = NULL;
    int month = 0;
    int year = 0;
    int do_sort = 0;
    int do_print = 0;
    int show_help = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            show_help = 1;
        } else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i+1 < argc) {
            report_type = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            month = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-y") == 0 && i+1 < argc) {
            year = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0) {
            do_sort = 1;
        } else if (strcmp(argv[i], "-p") == 0) {
            do_print = 1;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if (show_help) {
        print_help(argv[0]);
        return 0;
    }

    if (!filename) {
        print_help(argv[0]);
        return 1;
    }

    Temperature_data array[MAX_RECORDS];
    int size = load_from_csv(filename, array, MAX_RECORDS);
    if (size <= 0) {
        return 1;
    }

    if (do_sort) {
        sort_by_date(array, size);
    }

    if (do_print) {
        printf("\n--- Temperature records in array---\n");
        print_array(array, size);
        printf("---------------------------\n");
    }

    // Если тип отчёта не указан, но есть -m или -y — ошибка (требуется -t)
    if (report_type == NULL && (month != 0 || year != 0)) {
        return 1;
    }

    // печать в консоль: статистика в зависимости от типа. Уберу, если не понадобится в дальнейшем
    //надо будет добавить потом проверки, в том числе и для других функций. доработать перед сдачей К/р.
    if (report_type != NULL) {
        if (strcmp(report_type, "month") == 0) {
            printf("\n--- Statistics for month %d, year %d ---\n", month, year);
            Report *r = report_by_month(array, size, year, (Month)month);
            if (r) {
                print_statistic(r);
                free_report(r);
            } else {
                printf("No data found.\n");
            }
        }
        else if (strcmp(report_type, "year") == 0) {
            if (year == 0) {
                return 1;
            }
            printf("\n--- Statistics for year %d ---\n", year);
            Report *r = report_by_year(array, size, year);
            if (r) {
                print_statistic(r);
                free_report(r);
            } else {
                printf("No data found.\n");
            }
        }
        else if (strcmp(report_type, "month-for-year") == 0) {
            printf("\n--- Statistics for year %d (by month) ---\n", year);
            printf("  (not implemented yet)\n");
            // Report *r = report_by_month_for_the_year(array, size, year);
        }
        else if (strcmp(report_type, "year-all-period") == 0) {
            printf("\n--- Statistics for all years (without month breakdown) ---\n");
            printf("  (not implemented yet)\n");
            // Report *r = report_by_year_all_period(array, size);
        }
        else if (strcmp(report_type, "full") == 0) {
            printf("\n--- Full statistics: all years and months ---\n");
            printf("  (not implemented yet)\n");
            // Report *r = report_all_years_and_all_months(array, size);
        }
        else {
            fprintf(stderr, "Error: unknown report type '%s'\n", report_type);
            print_help(argv[0]);
            return 1;
        }
    } else {
        // когда нет ни статистики, ни печати, ни сортировки, то напоминание
        if (!do_print && !do_sort) {
            printf("-p to print data or -t for statistics.\n");
        }
    }

    return 0;
}







//old
/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "temp_api.h"

#define MAX_RECORDS 400

int main(int argc, char *argv[]) {
    Temperature_data array[MAX_RECORDS];
    Request request_month;
    request_month.type = REPORT_BY_MONTH;
    request_month.array = array;
    request_month.size = MAX_RECORDS;
    request_month.year = 2024;
    request_month.month = JAN;

    Report *report_month = return_statistic(&request_month);
    print_statistic(report_month);

    return 0;
}
*/