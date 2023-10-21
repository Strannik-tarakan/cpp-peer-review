
#define THROW_DATE_TIME_ERROR(Name,Min,Max)  \
    if (dt.Name < Min) {                     \
throw domain_error(#Name+" is too small"s);  \
    }                                        \
if (dt.Name > Max) {                         \
throw domain_error(#Name + " is too big"s);  \
}                                            \


void CheckDateTimeValidity(const DateTime& dt) { //this function throws errors when time limits are exceeded

    THROW_DATE_TIME_ERROR(year,1,9999);
    THROW_DATE_TIME_ERROR(month, 1, 12);
    
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    THROW_DATE_TIME_ERROR(day, 1, month_lengths[dt.month - 1]);
    THROW_DATE_TIME_ERROR(hour, 0, 23);
    THROW_DATE_TIME_ERROR(minute, 0, 59);
    THROW_DATE_TIME_ERROR(second, 0, 59);
    
}