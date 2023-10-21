#include <iostream>
#include <string>


#define DECLARE_INT_PARAM(Name) \
    struct Name {               \
        int value;              \
        explicit Name(int v)    \
            : value(v) {        \
        }                       \
        operator int() const {  \
            return value;       \
        }                       \
    } 

#define DECLARE_BOOL_PARAM(Name) \
    struct Name {                \
        bool value;              \
        explicit Name(bool v)    \
            : value(v) {         \
        }                        \
        operator bool() const {  \
            return value;        \
        }                        \
    } 

DECLARE_INT_PARAM(DbConnectionTimeout);
DECLARE_INT_PARAM(MinAge);
DECLARE_INT_PARAM(MaxAge);

DECLARE_BOOL_PARAM(DbAllowExceptions);


using namespace std;

vector<Person> LoadPersons(string_view db_name, DbConnectionTimeout db_connection_timeout, DbAllowExceptions db_allow_exceptions,
    DBLogLevel db_log_level, MinAge min_age, MaxAge max_age, string_view name_filter) {

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
