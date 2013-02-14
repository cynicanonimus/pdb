#include "dbstructuredescriptor.h"
//
#include <QMessageBox>
//
//the class contains database structure from the xml-file. 1 instance of the class per database
//
DbStructureDescriptor::DbStructureDescriptor(const QString &str_diver_name, const QString& str_show_tables_request, QObject *parent) :
    QObject(parent)
{
    m_strDiverName          = str_diver_name;
    m_strShowTablesRequest  = str_show_tables_request;
}

bool DbStructureDescriptor::addStatement(int id, const QString& str_statement)
{
    if ( m_SQLStatements.contains(id) )
    {
        QString str_message = QString("statement with id N %1 exsits more than once. Please fix it and try again.").arg(id);
        QMessageBox::critical(NULL,"Error in dbdescription.xml",str_message, QMessageBox::Ok);
        return false;
    };
    //
    m_SQLStatements[id] = str_statement;
    //
    return true;
}
