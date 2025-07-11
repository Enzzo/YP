struct CountryParams {
    std::string name;
    std::string iso_code;
    std::string phone_code;
    std::string time_zone;
    std::vector<Language> languages;
};

struct CityParams {
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

        cities.push_back({ country_params.name,
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