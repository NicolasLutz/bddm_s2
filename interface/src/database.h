#ifndef DATABASE_H
#define DATABASE_H

#include <qstring.h>
#include <sqlite3.h>
#include <vector>

#include "game.h"

class Database
{
    public:
        // Open database
        Database(QString const& filename);

        // Close database and free sqlite resources
        virtual ~Database();

        // Return list of game names
        std::vector<std::string> games();

        // Return the matching Game object, throws if it doesn't exist
        Game game(std::string const& name);
        // Modify the matching Game object, throws if it doesn't exist
        void update_game(Game const& g);
        // Insert a new game in the database, throws if it already exists
        void insert_game(Game const& g);

        // Create new database
        static void create(QString const& filename);

    protected:
        sqlite3 *m_db;

        sqlite3_stmt* m_insertStmtHandle;
        sqlite3_stmt* m_gameNamesHandle;
        sqlite3_stmt* m_gameUpdateHandle;
        sqlite3_stmt* m_gameHandle;

        static const std::string c_sqlCreateDb;
        static const std::string c_sqlInsertGame;
        static const std::string c_sqlGameNames;
        static const std::string c_sqlGameUpdate;
        static const std::string c_sqlGame;

        template <class T>
        void checkSqliteCall(T const& value, T const& okValue);
};

#include "database.inl"

#endif // DATABASE_H
