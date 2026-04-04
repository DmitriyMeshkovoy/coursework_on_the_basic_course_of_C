//
// Created by meshk on 31.03.2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "temp_api.h"

#define MAX_RECORDS 400

int main(void) {
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