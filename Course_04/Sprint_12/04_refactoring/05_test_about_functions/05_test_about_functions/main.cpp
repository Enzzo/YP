#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <array>
#include <list>
#include <variant>
#include <map>

using namespace std;

//---------------------- LoadPersons ----------------------

class DBQuery;

struct Person {
    std::string name;
    int age;
};

class DBLogLevel {

};

class DBHandler {
    std::string name_;
    int connection_timeout_;

public:
    DBHandler() = default;
    DBHandler(std::string_view name, int connection_timeout) : name_(name), connection_timeout_(connection_timeout) {};

    bool IsOK() {
        return false;
    }

    std::string Quote(std::string_view name_filter) {
        return {};
    }

    template<typename T1, typename T2>
    std::vector<std::pair<T1, T2>> LoadRows(const DBQuery query) {

    }
};

class DBConnector {
    bool allow_exceptions_;
    DBLogLevel log_level_;

public:
    DBConnector(const bool allow_exceptions, const DBLogLevel log_level) : allow_exceptions_(allow_exceptions), log_level_(log_level) {};
    
    DBHandler ConnectTmp(std::string_view name, int connection_timeout) {
        return DBHandler(name, connection_timeout);
    }

    DBHandler Connect(std::string_view name, int connection_timeout) {
        return DBHandler(name, connection_timeout);
    }
};



class DBQuery {
    std::string query_;
public:
    DBQuery(std::string_view query) : query_(query) {};
};

//---------------------- LoadPersons ----------------------

struct PersonsParams {
    std::string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    std::string_view name_filter;
};

vector<Person> LoadPersons(const PersonsParams& p) {
    string_view db_name = p.db_name;
    int db_connection_timeout = p.db_connection_timeout;
    bool db_allow_exceptions = p.db_allow_exceptions;
    DBLogLevel db_log_level = p.db_log_level;
    int min_age = p.min_age;
    int max_age = p.max_age;
    string_view name_filter = p.name_filter;

    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    }
    else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << min_age << " and "s << max_age << " "s
        << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}
//---------------------- CheckDateTimeValidity ----------------------

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
    CHECK_LOWER_BORDER(dt.day,   "day",   1);
    CHECK_UPPER_BORDER(dt.day,   "day",   month_lengths[dt.month - 1]);
    CHECK_LOWER_BORDER(dt.hour,  "hour",  0);
    CHECK_UPPER_BORDER(dt.hour,  "hour",  23);
    CHECK_LOWER_BORDER(dt.minute,"minute",0);
    CHECK_UPPER_BORDER(dt.minute,"minute",59);
    CHECK_LOWER_BORDER(dt.second,"second",0);
    CHECK_UPPER_BORDER(dt.second,"second",59);
}
//---------------------- Function 3 ----------------------
struct CountryParams {
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string time_zone;
    std::vector<Language> languages;
};

struct CityParams{
    std::string country_name;
    std::string counrty_iso_code;
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string time_zone;
    std::vector<Language> languages;
};

class City {
public:
    CityParams params;
};

struct Country {
    CountryParams params;
};

void ParseCitySubjson(
    vector<City>& cities,
    const Json& json,
    const CountryParams country_params
    ) {

    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();

        cities.push_back({country_params.name,
            country_params.iso_code,
            city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            country_params.phone_code + city_obj["phone_code"s].AsString(),
            country_params.time_zone,
            country_params.languages });
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.params.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(
            cities, 
            country_obj["cities"s], 
            country.params);
    }
}