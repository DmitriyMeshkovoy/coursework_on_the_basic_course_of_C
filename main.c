//
// Created by meshk on 31.03.2026.
//

/*
 Поддерживаемые ключи:
-h                     описание (справка)
-f file.csv            загрузить данные из CSV (обязательный)
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
-f data.csv -t year -y 2026            статистика за 2026 год
-f data.csv -t month-for-year -y 2026  год в разрезе месяцев (заглушка)
-f data.csv -t year-all-period         все годы (заглушка)
-f data.csv -t full                    все годы и месяцы (заглушка)
-h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_functions.h"

//#define MAX_RECORDS 10000 убрал, т.к. свыше 500 тыс записей на стеке хранить наверное не стоит,
//хотя на линуксе запускал-работает. Сделал хранение на куче

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
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            report_type = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            month = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) {
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

    // Динамическое выделение памяти для массива записей
    Temperature_data *array = NULL;
    int size = 0;
    if (load_from_csv(filename, &array, &size) <= 0) {
        fprintf(stderr, "Failed to load data from %s\n", filename);
        return 1;
    }
    printf("Loaded %d records.\n", size);

    if (do_sort) {
        sort_by_date(array, size);
        printf("Data sorted by date.\n");
    }

    if (do_print) {
        printf("\n--- Temperature records in array---\n");
        print_array(array, size);
        printf("---------------------------\n");
    }

    // ------------------------------------------------------------------
    // Поведение по ТЗ (ключи -h, -f, -m) и расширенные ключи (-t, -y, -s, -p)
    // Если -t НЕ задан, работаем по ТЗ:
    //   - если есть -m: выводим статистику только за этот месяц
    //   - иначе: выводим статистику за все месяцы и за год
    //   - исключение: если есть -p (и нет -m), статистику не выводим
    // Если -t задан: используем расширенную логику
    // ------------------------------------------------------------------
    if (report_type == NULL) {
        // Режим строгого ТЗ
        if (month != 0) {
            // ключ -m задан
            if (month < 1 || month > 12) {
                fprintf(stderr, "Invalid month: %d\n", month);
                free(array);
                return 1;
            }
            if (size == 0) {
                printf("No data\n");
                free(array);
                return 0;
            }
            int data_year = array[0].year; // по ТЗ все данные за один год
            printf("\n--- Statistics for month %d, year %d ---\n", month, data_year);
            Report *r = report_by_month(array, size, data_year, (Month) month);
            if (r) {
                print_statistic(r);
                free_report(r);
            } else {
                printf("No data found.\n");
            }
        } else {
            // Нет -m и нет -t
            // Если запрошена печать (даже с сортировкой) и нет -m, статистику не выводим
            if (do_print) {
                // только печать (уже выполнена выше), больше ничего не делаем
                ;
            } else if (size > 0) {
                print_full_statistics(array, size);
            } else {
                if (!do_print && !do_sort) {
                    printf("No data. Use -p to print or -t for specific reports.\n");
                }
            }
        }
    } else {
        // Расширенный режим с -t
        if (strcmp(report_type, "month") == 0) {
            if (year == 0 || month < 1 || month > 12) {
                fprintf(stderr, "Error: 'month' requires -y <year> and -m <1-12>\n");
                free(array);
                return 1;
            }
            printf("\n--- Statistics for month %d, year %d ---\n", month, year);
            Report *r = report_by_month(array, size, year, (Month) month);
            if (r) {
                print_statistic(r);
                free_report(r);
            } else {
                printf("No data found.\n");
            }
        } else if (strcmp(report_type, "year") == 0) {
            if (year == 0) {
                fprintf(stderr, "Error: 'year' requires -y <year>\n");
                free(array);
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
        } else if (strcmp(report_type, "month-for-year") == 0) {
            printf("\n--- Statistics for year %d (by month) ---\n", year);
            printf("  (not implemented yet)\n");
        } else if (strcmp(report_type, "year-all-period") == 0) {
            printf("\n--- Statistics for all years (without month breakdown) ---\n");
            printf("  (not implemented yet)\n");
        } else if (strcmp(report_type, "full") == 0) {
            printf("\n--- Full statistics: all years and months ---\n");
            printf("  (not implemented yet)\n");
        } else {
            fprintf(stderr, "Error: unknown report type '%s'\n", report_type);
            print_help(argv[0]);
            free(array);
            return 1;
        }
    }

    free(array);
    return 0;
}