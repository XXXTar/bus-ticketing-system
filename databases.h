#ifndef DATABASES_H
#define DATABASES_H

#include <QtSql>
extern QString g_username;
class databases
{
public:
    databases();
    void initializeDatabase();
    QSqlDatabase m_db;
};


#endif // DATABASES_H
