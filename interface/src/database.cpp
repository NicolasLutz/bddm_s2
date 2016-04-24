#include "database.h"

#include <qfile.h>

const std::string Database::c_sqlCreateDb = "CREATE TABLE games("
                                              "id INTEGER PRIMARY KEY,"
                                              "name TEXT UNIQUE NOT NULL,"
                                              "year INT,"
                                              "editor TEXT,"
                                              "img BLOB,"
                                              "analysis_img BLOB NOT NULL,"
                                              "description TEXT);";

const std::string Database::c_sqlInsertGame = "INSERT INTO games(name, year, editor, description, img, analysis_img) VALUES(?,?,?,?,?,?);";
const std::string Database::c_sqlGameNames = "SELECT name FROM games;";
const std::string Database::c_sqlGameUpdate = "UPDATE games SET "
                                              "year=?,"
                                              "editor=?,"
                                              "img=?,"
                                              "analysis_img=?,"
                                              "description=? "
                                              "WHERE name=?";
const std::string Database::c_sqlGame = "SELECT year, editor, description, img, analysis_img FROM games WHERE name=?;";

Database::Database(const QString& filename) :
    m_db(nullptr),
    m_insertStmtHandle(nullptr),
    m_gameNamesHandle(nullptr),
    m_gameUpdateHandle(nullptr),
    m_gameHandle(nullptr)
{
    if(sqlite3_open_v2(filename.toStdString().c_str(), &m_db, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        if(m_db)
            sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    // TODO : make sure the db is a sqlite db for this app, not some random sqlite db
    // For this, compare the schema to the table creation SQL, it must match

    // Database open, prepare statements
    if(sqlite3_prepare_v2(m_db, c_sqlInsertGame.c_str(), c_sqlInsertGame.length()+1, &m_insertStmtHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    if(sqlite3_prepare_v2(m_db, c_sqlGameNames.c_str(), c_sqlGameNames.length()+1, &m_gameNamesHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    if(sqlite3_prepare_v2(m_db, c_sqlGame.c_str(), c_sqlGame.length()+1, &m_gameHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_finalize(m_gameNamesHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    if(sqlite3_prepare_v2(m_db, c_sqlGameUpdate.c_str(), c_sqlGameUpdate.length()+1, &m_gameUpdateHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_finalize(m_gameNamesHandle);
        sqlite3_finalize(m_gameHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }
}

Database::~Database() {
    sqlite3_finalize(m_insertStmtHandle);
    sqlite3_finalize(m_gameNamesHandle);
    sqlite3_finalize(m_gameHandle);
    sqlite3_close(m_db);
}

std::vector<std::string> Database::games() {
    sqlite3_reset(m_gameNamesHandle);
    std::vector<std::string> v;
    int sqliteCode = sqlite3_step(m_gameNamesHandle);
    while(sqliteCode != SQLITE_DONE) {
        checkSqliteCall(sqliteCode, SQLITE_ROW);

        v.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_gameNamesHandle, 1))));

        sqliteCode = sqlite3_step(m_gameNamesHandle);
    }
    return v;
}

Game Database::game(const std::string& name) {
    checkSqliteCall(sqlite3_bind_text(m_gameHandle, 1, name.c_str(), name.length(), SQLITE_STATIC), SQLITE_OK);
    checkSqliteCall(sqlite3_step(m_gameHandle), SQLITE_ROW);
    QImage analysis = QImage::fromData(reinterpret_cast<const char*>(sqlite3_column_blob(m_gameHandle, 5), sqlite3_column_bytes(m_gameHandle, 5)));
    bool nullEditor = (sqlite3_column_type(m_gameHandle, 2) == SQLITE_NULL),
         nullYear   = (sqlite3_column_type(m_gameHandle, 1) == SQLITE_NULL),
         nullImg    = (sqlite3_column_type(m_gameHandle, 4) == SQLITE_NULL),
         nullDesc   = (sqlite3_column_type(m_gameHandle, 3) == SQLITE_NULL);
    std::string* editor = nullEditor ? nullptr : new std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_gameHandle, 2)));
    std::string* desc   = nullDesc   ? nullptr : new std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_gameHandle, 3)));
    int* year = nullYear ? nullptr : new int(sqlite3_column_int(m_gameHandle, 1));
    QImage img = nullImg ? QImage() : QImage::fromData(reinterpret_cast<const char*>(sqlite3_column_blob(m_gameHandle, 4), sqlite3_column_bytes(m_gameHandle, 4)));
    return Game(name, analysis, editor, desc, img, year);
}

void Database::update_game(const Game& g) {
    if(g.year())
        checkSqliteCall(sqlite3_bind_int(m_gameUpdateHandle, 1, *(g.year())), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 1), SQLITE_OK);

    if(g.editor())
        checkSqliteCall(sqlite3_bind_text(m_gameUpdateHandle, 2, g.editor()->c_str(), g.editor()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 2), SQLITE_OK);

    if(!g.gameImg().isNull())
        checkSqliteCall(sqlite3_bind_blob(m_gameUpdateHandle, 3, g.gameImg().constBits(), g.gameImg().byteCount(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 3), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_blob(m_gameUpdateHandle, 4, g.analysis().constBits(), g.analysis().byteCount(), SQLITE_STATIC), SQLITE_OK);

    if(g.description())
        checkSqliteCall(sqlite3_bind_text(m_gameUpdateHandle, 5, g.description()->c_str(), g.description()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 5), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 6, g.name().c_str(), g.name().length(), SQLITE_STATIC), SQLITE_OK);

    // Parameters bound, run the statement
    checkSqliteCall(sqlite3_step(m_gameUpdateHandle), SQLITE_DONE);
    checkSqliteCall(sqlite3_reset(m_gameUpdateHandle), SQLITE_OK);
}

void Database::create(const QString& filename) {
    if(QFile(filename).exists()) {
        throw std::runtime_error("Database file already exists");
    }
    sqlite3* db = nullptr;
    if(sqlite3_open_v2(filename.toStdString().c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(db));
        if(db)
            sqlite3_close(db);
        throw std::runtime_error(errstr);
    }
    else {
        char* errmsg = nullptr;
        // Create DB schema
        if(sqlite3_exec(db, c_sqlCreateDb.c_str(), nullptr, nullptr, &errmsg)) {
            std::string errstr(errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error(errstr);
        }
    }
    sqlite3_close(db);
}

void Database::insert_game(const Game& g) {
    checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 1, g.name().c_str(), g.name().length(), SQLITE_STATIC), SQLITE_OK);

    if(g.year())
        checkSqliteCall(sqlite3_bind_int(m_insertStmtHandle, 2, *(g.year())), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 2), SQLITE_OK);

    if(g.editor())
        checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 3, g.editor()->c_str(), g.editor()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 3), SQLITE_OK);

    if(g.description())
        checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 4, g.description()->c_str(), g.description()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 4), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_blob(m_insertStmtHandle, 5, g.analysis().constBits(), g.analysis().byteCount(), SQLITE_STATIC), SQLITE_OK);

    if(!g.gameImg().isNull())
        checkSqliteCall(sqlite3_bind_blob(m_insertStmtHandle, 6, g.gameImg().constBits(), g.gameImg().byteCount(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 6), SQLITE_OK);

    // Parameters bound, run the statement
    checkSqliteCall(sqlite3_step(m_insertStmtHandle), SQLITE_DONE);
    checkSqliteCall(sqlite3_reset(m_insertStmtHandle), SQLITE_OK);
}
