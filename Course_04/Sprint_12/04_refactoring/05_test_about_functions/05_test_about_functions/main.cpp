#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <array>

using namespace std;

//---------------------- LoadPersons ----------------------
struct Person {
    std::string name;
    int age;
};

class DBLogLevel {

};

class DBConnector {
    bool allow_exceptions_;
    DBLogLevel log_level_;

public:
    DBConnector(const bool allow_exceptions, const DBLogLevel log_level) : allow_exceptions_(allow_exceptions), log_level_(log_level) {};
    
    DBHandler ConnectTmp(std::string_view name, int connection_timeout) {
        return {};
    }

    DBHandler Connect(std::string_view name, int connection_timeout) {
        return {};
    }
};

class DBQuery;

class DBHandler {
public:
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

class DBQuery {
    std::string query_;
public:
    DBQuery(std::string_view query) : query_(query) {};
};
//---------------------- LoadPersons ----------------------
//Дана функция LoadPerson.Оцените, насколько удобно ей пользоваться, тестировать, поддерживать, 
//и выберите наиболее подходящий способ рефакторинга.Реализуйте выбранный способ и сохраните решение в файл LoadPerson.cpp.

vector<Person> LoadPersons(string_view db_name, int db_connection_timeout, bool db_allow_exceptions,
    DBLogLevel db_log_level, int min_age, int max_age, string_view name_filter) {
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

//---------------------- CheckDateTimeValidity ----------------------
//Дана функция CheckDateTimeValidity.Оцените, насколько удобно ей пользоваться, тестировать, поддерживать, 
//и выберите наиболее подходящий способ рефакторинга.Реализуйте выбранный способ и сохраните решение в файл CheckDateTimeValidity.cpp.

void CheckDateTimeValidity(const DateTime& dt) {
    if (dt.year < 1) {
        throw domain_error("year is too small"s);
    }
    if (dt.year > 9999) {
        throw domain_error("year is too big"s);
    }

    if (dt.month < 1) {
        throw domain_error("month is too small"s);
    }
    if (dt.month > 12) {
        throw domain_error("month is too big"s);
    }

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (dt.day < 1) {
        throw domain_error("day is too small"s);
    }
    if (dt.day > month_lengths[dt.month - 1]) {
        throw domain_error("day is too big"s);
    }

    if (dt.hour < 0) {
        throw domain_error("hour is too small"s);
    }
    if (dt.hour > 23) {
        throw domain_error("hour is too big"s);
    }

    if (dt.minute < 0) {
        throw domain_error("minute is too small"s);
    }
    if (dt.minute > 59) {
        throw domain_error("minute is too big"s);
    }

    if (dt.second < 0) {
        throw domain_error("second is too small");
    }
    if (dt.second > 59) {
        throw domain_error("second is too big"s);
    }
}
//---------------------- Function 3 ----------------------
class Language;

class City {
    std::string name, iso_code, phone_code, country_name, country_iso_code, country_time_zone;
    std::vector<Language> languages;
};

struct Country {
    std::string name, iso_code, phone_code, time_zone;
    std::vector<Language> languages;
};
class Node {
    City city_;
public:
    const City AsObject() const{
        return city_;
    }
};

class Json{
    std::vector<Node> cities_;
    Node node_;
public:
    const std::vector<Node> AsList() const {
        return cities_;
    }
    const Node AsObject() const {
        return node_;
    }
};

class Language {

};
//---------------------- Function 3 ----------------------
// Оцените, насколько удобно пользоваться, тестировать и поддерживать функцию ParseCitySubjson, 
// выберите наиболее подходящий способ рефакторинга и переделайте ParseCitySubjson, не меняя функционал. 
// При необходимости измените способ вызова этой функции из ParseCountryJson. Сохраните решение в файл ParseCitySubjson.cpp.
// 
// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const string& country_name,
    const string& country_iso_code, const string& country_phone_code, const string& country_time_zone,
    const vector<Language>& languages) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({ city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages });
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
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
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], country.name, country.iso_code, country.phone_code,
            country.time_zone, country.languages);
    }
}

int main() {
    return 0;
}