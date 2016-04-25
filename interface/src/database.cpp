#include "database.h"

#include <qfile.h>

const std::string Database::c_sqlCreateDb = "CREATE TABLE editors("
                                                "id INTEGER PRIMARY KEY,"
                                                "name TEXT UNIQUE NOT NULL);"
                                            "CREATE TABLE games("
                                                "id INTEGER PRIMARY KEY,"
                                                "name TEXT UNIQUE NOT NULL,"
                                                "year INT,"
                                                "editor INT,"
                                                "img BLOB,"
                                                "analysis_img BLOB NOT NULL,"
                                                "description TEXT,"
                                                "FOREIGN KEY(editor) REFERENCES editors(id));";

const std::string Database::c_sqlInsertEditor = "INSERT INTO editors(name) VALUES(?);";
const std::string Database::c_sqlEditorId     = "SELECT id FROM editors WHERE name=?;";
const std::string Database::c_sqlEditorName   = "SELECT name FROM editors WHERE id=?;";
const std::string Database::c_sqlInsertGame   = "INSERT INTO games(name, year, editor, description, img, analysis_img) VALUES(?,?,?,?,?,?);";
const std::string Database::c_sqlGameNames    = "SELECT name FROM games;";
const std::string Database::c_sqlGameUpdate   = "UPDATE games SET "
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

    if(sqlite3_prepare_v2(m_db, c_sqlInsertEditor.c_str(), c_sqlInsertEditor.length()+1, &m_insertEditorHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_finalize(m_gameNamesHandle);
        sqlite3_finalize(m_gameHandle);
        sqlite3_finalize(m_gameUpdateHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    if(sqlite3_prepare_v2(m_db, c_sqlEditorId.c_str(), c_sqlEditorId.length()+1, &m_editorIdHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_finalize(m_gameNamesHandle);
        sqlite3_finalize(m_gameHandle);
        sqlite3_finalize(m_gameUpdateHandle);
        sqlite3_finalize(m_insertEditorHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }

    if(sqlite3_prepare_v2(m_db, c_sqlEditorName.c_str(), c_sqlEditorName.length()+1, &m_editorNameHandle, nullptr) != SQLITE_OK) {
        std::string errstr(sqlite3_errmsg(m_db));
        sqlite3_finalize(m_insertStmtHandle);
        sqlite3_finalize(m_gameNamesHandle);
        sqlite3_finalize(m_gameHandle);
        sqlite3_finalize(m_gameUpdateHandle);
        sqlite3_finalize(m_insertEditorHandle);
        sqlite3_finalize(m_editorIdHandle);
        sqlite3_close(m_db);
        throw std::runtime_error(errstr);
    }
}

Database::~Database() {
    sqlite3_finalize(m_insertStmtHandle);
    sqlite3_finalize(m_gameNamesHandle);
    sqlite3_finalize(m_gameHandle);
    sqlite3_finalize(m_gameUpdateHandle);
    sqlite3_finalize(m_insertEditorHandle);
    sqlite3_finalize(m_editorIdHandle);
    sqlite3_finalize(m_editorNameHandle);
    sqlite3_close(m_db);
}

std::vector<QString> Database::games() {
    sqlite3_reset(m_gameNamesHandle);
    std::vector<QString> v;
    int sqliteCode = sqlite3_step(m_gameNamesHandle);
    while(sqliteCode != SQLITE_DONE) {
        checkSqliteCall(sqliteCode, SQLITE_ROW);

        const unsigned char* str = sqlite3_column_text(m_gameNamesHandle, 0);

        v.push_back(QString(reinterpret_cast<const char*>(str)));

        sqliteCode = sqlite3_step(m_gameNamesHandle);
    }
    return v;
}

Game Database::game(const QString& name) {
    checkSqliteCall(sqlite3_bind_text(m_gameHandle, 1, name.toStdString().c_str(), name.length(), SQLITE_STATIC), SQLITE_OK);
    checkSqliteCall(sqlite3_step(m_gameHandle), SQLITE_ROW);
    QImage analysis = QImage::fromData(reinterpret_cast<const char*>(sqlite3_column_blob(m_gameHandle, 4), sqlite3_column_bytes(m_gameHandle, 4)));
    bool nullEditor = (sqlite3_column_type(m_gameHandle, 1) == SQLITE_NULL),
         nullYear   = (sqlite3_column_type(m_gameHandle, 0) == SQLITE_NULL),
         nullImg    = (sqlite3_column_type(m_gameHandle, 3) == SQLITE_NULL),
         nullDesc   = (sqlite3_column_type(m_gameHandle, 2) == SQLITE_NULL);
    QString* editor = nullEditor ? nullptr : new QString(editor_name(sqlite3_column_int(m_gameHandle, 1)));
    QString* desc   = nullDesc   ? nullptr : new QString(reinterpret_cast<const char*>(sqlite3_column_text(m_gameHandle, 2)));
    int* year = nullYear ? nullptr : new int(sqlite3_column_int(m_gameHandle, 0));
    QImage img = nullImg ? QImage() : QImage::fromData(reinterpret_cast<const char*>(sqlite3_column_blob(m_gameHandle, 3), sqlite3_column_bytes(m_gameHandle, 3)));
    return Game(name, analysis, editor, desc, img, year);
}

void Database::update_game(const Game& g) {
    if(g.year())
        checkSqliteCall(sqlite3_bind_int(m_gameUpdateHandle, 1, *(g.year())), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 1), SQLITE_OK);

    if(g.editor()) {
        int* id = editor_id(*g.editor());
        if(!id) {
            insert_editor(*g.editor());
            id = editor_id(*g.editor());
        }
        checkSqliteCall(sqlite3_bind_int(m_gameUpdateHandle, 2, *id), SQLITE_OK);
        delete id;
    }
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 2), SQLITE_OK);

    if(!g.gameImg().isNull())
        checkSqliteCall(sqlite3_bind_blob(m_gameUpdateHandle, 3, g.gameImg().constBits(), g.gameImg().byteCount(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 3), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_blob(m_gameUpdateHandle, 4, g.analysis().constBits(), g.analysis().byteCount(), SQLITE_STATIC), SQLITE_OK);

    if(g.description())
        checkSqliteCall(sqlite3_bind_text(m_gameUpdateHandle, 5, g.description()->toStdString().c_str(), g.description()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_gameUpdateHandle, 5), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 6, g.name().toStdString().c_str(), g.name().length(), SQLITE_STATIC), SQLITE_OK);

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
    checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 1, g.name().toStdString().c_str(), g.name().length(), SQLITE_STATIC), SQLITE_OK);

    if(g.year())
        checkSqliteCall(sqlite3_bind_int(m_insertStmtHandle, 2, *(g.year())), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 2), SQLITE_OK);

    if(g.editor()) {
        int* id = editor_id(*g.editor());
        if(id) {
            checkSqliteCall(sqlite3_bind_int(m_insertStmtHandle, 3, *id), SQLITE_OK);
            delete id;
        }
        else
            checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 3), SQLITE_OK);
    }
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 3), SQLITE_OK);

    if(g.description())
        checkSqliteCall(sqlite3_bind_text(m_insertStmtHandle, 4, g.description()->toStdString().c_str(), g.description()->length(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 4), SQLITE_OK);

    checkSqliteCall(sqlite3_bind_blob(m_insertStmtHandle, 6, g.analysis().constBits(), g.analysis().byteCount(), SQLITE_STATIC), SQLITE_OK);

    if(!g.gameImg().isNull())
        checkSqliteCall(sqlite3_bind_blob(m_insertStmtHandle, 5, g.gameImg().constBits(), g.gameImg().byteCount(), SQLITE_STATIC), SQLITE_OK);
    else
        checkSqliteCall(sqlite3_bind_null(m_insertStmtHandle, 5), SQLITE_OK);

    // Parameters bound, run the statement
    checkSqliteCall(sqlite3_step(m_insertStmtHandle), SQLITE_DONE);
    checkSqliteCall(sqlite3_reset(m_insertStmtHandle), SQLITE_OK);
}

int* Database::editor_id(QString const& editor) {
    checkSqliteCall(sqlite3_bind_text(m_editorIdHandle, 1, editor.toStdString().c_str(), editor.length(), SQLITE_STATIC), SQLITE_OK);
    int* r = (sqlite3_step(m_editorIdHandle) == SQLITE_ROW) ? new int(sqlite3_column_int(m_editorIdHandle, 0)) : nullptr;
    sqlite3_reset(m_editorIdHandle);
    return r;
}

QString Database::editor_name(const int id) {
    checkSqliteCall(sqlite3_bind_int(m_editorNameHandle, 1, id), SQLITE_OK);
    if(sqlite3_step(m_editorNameHandle) != SQLITE_ROW)
        throw std::runtime_error("No editor with such id");
    QString name(reinterpret_cast<const char*>(sqlite3_column_text(m_editorNameHandle, 0)));
    sqlite3_reset(m_editorNameHandle);
    return name;
}

void Database::insert_editor(const QString& editor) {
    checkSqliteCall(sqlite3_bind_text(m_insertEditorHandle, 1, editor.toStdString().c_str(), editor.length(), SQLITE_STATIC), SQLITE_OK);
    checkSqliteCall(sqlite3_step(m_insertEditorHandle), SQLITE_DONE);
    sqlite3_reset(m_insertEditorHandle);
}
