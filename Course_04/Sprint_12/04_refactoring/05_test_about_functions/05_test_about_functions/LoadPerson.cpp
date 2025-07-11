
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