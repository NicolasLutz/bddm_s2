template <class T>
void Database::checkSqliteCall(T const& value, T const& okValue) {
    if(value != okValue) {
        std::string errstr(sqlite3_errmsg(m_db));
        throw std::runtime_error(errstr);
    }
}
