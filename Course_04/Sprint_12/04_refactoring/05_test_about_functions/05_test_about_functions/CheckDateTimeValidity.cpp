struct DateTime {
    int year, month, day, hour, minute, second;
};

#define CHECK_LOWER_BORDER(value, name, border)     \
    if(value < border){                             \
        throw domain_error(name+" is too small"s);  \
    }                                               \

#define CHECK_UPPER_BORDER(value, name, border)     \
    if(value > border){                             \
        throw domain_error(name+" is too big"s);    \
    }                                               \

void CheckDateTimeValidity(const DateTime& dt) {

    CHECK_LOWER_BORDER(dt.year, "year", 1);
    CHECK_UPPER_BORDER(dt.year, "year", 9999);
    CHECK_LOWER_BORDER(dt.month, "month", 1);
    CHECK_UPPER_BORDER(dt.month, "month", 12);
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    CHECK_LOWER_BORDER(dt.day, "day", 1);
    CHECK_UPPER_BORDER(dt.day, "day", month_lengths[dt.month - 1]);
    CHECK_LOWER_BORDER(dt.hour, "hour", 0);
    CHECK_UPPER_BORDER(dt.hour, "hour", 23);
    CHECK_LOWER_BORDER(dt.minute, "minute", 0);
    CHECK_UPPER_BORDER(dt.minute, "minute", 59);
    CHECK_LOWER_BORDER(dt.second, "second", 0);
    CHECK_UPPER_BORDER(dt.second, "second", 59);
}