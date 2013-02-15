//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
#include "dbcreator.h"
#include "../CommonInclude/pdb/DBSettings.h"
#include "dbstructuredescriptor.h"
//
#include <QDir>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QMessageBox>
#include <QCoreApplication>

namespace
{
const QString empty_str("empty");
}

DBCreator::DBCreator(QObject *parent) :
    QObject(parent)
  ,m_strDescriptorFileName("dbdescription.xml")
{
    m_bStatusOK     = false;
    m_ptrDbSettings = NULL;
    //
    m_strErrHeader = QString("Error in %1").arg(m_strDescriptorFileName);
}

DBCreator::~DBCreator ()
{
    DbDescriptors::Iterator itr = m_DBDescriptionStorage.begin();
    //
    while ( itr != m_DBDescriptionStorage.end() )
    {
        delete itr.value();
        itr++;
    };
}

const QString& DBCreator::getDefaultDbDriverName  () const
{
    DbDescriptors::ConstIterator itr = m_DBDescriptionStorage.begin();
    //
    while ( itr != m_DBDescriptionStorage.end() )
    {
        return itr.value()->getDriverName();
    };
    //
    return empty_str;
}


void DBCreator::setDbSettings   ( DBSettings* ptr_db_settings )
{
    m_ptrDbSettings = ptr_db_settings;
}

bool DBCreator::parseXMLDocument ( QDomDocument* ptr_xml_document )
{
    //get the root element
    QDomElement root_element = ptr_xml_document->documentElement();
    QString str_root_name = root_element.tagName();
    //
    if (str_root_name.localeAwareCompare("databases") != 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader, "root node must has the name 'databases'", QMessageBox::Ok);
        return false;
    };
    //
    if ( getMandatoryTablesList(&root_element) == false)
        return false;
    //
    QDomNodeList databases_list = root_element.elementsByTagName("database");
    //
    for (int i = 0; i < databases_list.size(); ++i)
    {
        QDomElement current_db = databases_list.at(i).toElement();
        //
        QString str_current_database_type;
        //
        if ( getDbType (&current_db, str_current_database_type) == false)
            return false;
        str_current_database_type = str_current_database_type.simplified();
        //
        QString str_diver_name;
        //
        if ( getDriverName(&current_db, str_diver_name) == false )
            return false;
        str_diver_name = str_diver_name.simplified();
        //
        QString str_show_tables_request;
        if ( getShowTablesQuery(&current_db, str_show_tables_request) == false)
            return false;
        str_show_tables_request = str_show_tables_request.simplified();
        //
        DbStructureDescriptor* ptr_db_structure = new DbStructureDescriptor(str_diver_name, str_show_tables_request);
        //
        if ( getDbStatements(&current_db, ptr_db_structure) == false )
        {
            if( ptr_db_structure )
                delete ptr_db_structure;
            return false;
        };
        //
        if ( m_DBDescriptionStorage.contains(str_current_database_type) == true )
        {
            QString str_message = QString("Database type '%1' described more than once. Please corrent it and try again").arg(str_current_database_type);
            QMessageBox::critical(NULL,"Error in dbdescription.xml",str_message, QMessageBox::Ok);
            return false;
        };
        //
        m_DBDescriptionStorage[str_current_database_type] = ptr_db_structure;
    };
    //
    return true;
}

bool DBCreator::getMandatoryTablesList ( QDomElement* ptr_element )
{
        //mandatory_tables
    QDomNodeList mandatory_tables_list = ptr_element->elementsByTagName("mandatory_tables");
    //
    if (mandatory_tables_list.size() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'mandatory_tables' must exist", QMessageBox::Ok);
        return false;
    };
    //
    QDomNode meta_tables_list  = mandatory_tables_list.at(0);
    QDomNodeList table_list = meta_tables_list.childNodes();
    //
    if (table_list.size() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'mandatory_tables' can not be empty", QMessageBox::Ok);
        return false;
    };
    //
    for (int j = 0; j < table_list.size(); ++j)
    {
        QString str_tag_name       = table_list.at(j).nodeName();
        QString str_tag_text       =table_list.at(j).toElement().text();
        //
        if ( (str_tag_name.compare("table") == 0) && (str_tag_text.length() > 0) )
        {
            m_lstMandatoryTables<<str_tag_text;
        };
    };
    //
    if (m_lstMandatoryTables.size() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'mandatory_tables' must contains non-empty entries 'table'", QMessageBox::Ok);
        return false;
    };
    //
    return true;
}

bool DBCreator::getDbStatements ( QDomElement* ptr_element, DbStructureDescriptor* ptr_db_structure)
{
    QDomNodeList db_statements = ptr_element->elementsByTagName("statement");
    //
    //check statements array here
    //
    if (db_statements.size() == 0)
    {
        QMessageBox::critical(NULL,"Error in dbdescription.xml","every 'database' must has the child 'statement'", QMessageBox::Ok);
        return false;
    };
    //
    for (int j = 0; j < db_statements.size(); ++j)
    {
        QString str_statement       = db_statements.at(j).toElement().text();
        str_statement = str_statement.simplified();
        //
        QString str_statement_id    = db_statements.at(j).toElement().attribute("id");
        str_statement_id = str_statement_id.simplified();
        //
        if (str_statement.length() == 0)
        {
            QMessageBox::critical(NULL,m_strErrHeader,"entry 'statement' can not be empty", QMessageBox::Ok);
            return false;
        };
        //
        if (str_statement_id.length() == 0)
        {
            QMessageBox::critical(NULL,m_strErrHeader,"attribute 'id' must exists in entry 'statement'", QMessageBox::Ok);
            return false;
        };
        //
        bool b_add_statement = ptr_db_structure->addStatement(str_statement_id.toInt(), str_statement);
        //
        if (false == b_add_statement)
            return false;
    };
    //
    return true;
}

bool DBCreator::getDbType ( QDomElement* ptr_element, QString& str_db_type )
{
    str_db_type = ptr_element->attribute("type"); //1
    //
    if (str_db_type.length() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'database' must has the type ('MySql, Oracle...')", QMessageBox::Ok);
        return false;
    };
    //
    return true;
}

bool DBCreator::getShowTablesQuery (QDomElement* ptr_element, QString& str_show_tables_request )
{
    QDomNodeList show_tables_list = ptr_element->elementsByTagName("show_tables_request");
    //
    if (show_tables_list.size() > 0)
    {
        str_show_tables_request = show_tables_list.at(0).toElement().text();
    }else
    {
        QMessageBox::critical(NULL,m_strErrHeader,"every 'database' must has the 'show_tables_request' entry", QMessageBox::Ok);
        return false;
    };
    //
    if (str_show_tables_request.length() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'show_tables_request' can not be empty", QMessageBox::Ok);
        return false;
    };
    //
    return true;
}

bool DBCreator::getDriverName ( QDomElement* ptr_element, QString &str_driver_name )
{
    QDomNodeList db_driver_list = ptr_element->elementsByTagName("driver");
    //
    if (db_driver_list.size() > 0)
    {
        str_driver_name = db_driver_list.at(0).toElement().text();
    }else
    {
        QMessageBox::critical(NULL,m_strErrHeader,"every 'database' must has the 'driver' entry", QMessageBox::Ok);
        return false;
    };
    //
    if (str_driver_name.length() == 0)
    {
        QMessageBox::critical(NULL,m_strErrHeader,"entry 'driver' can not be empty", QMessageBox::Ok);
        return false;
    };
    //
    return true;
}

bool DBCreator::readDbFileDescription()
{
    QString str_source_file =  QCoreApplication::applicationDirPath();

    str_source_file += "/";
    str_source_file += m_strDescriptorFileName;
    //
    QFile* ptr_xml_file = new QFile(str_source_file);
    //
    if (ptr_xml_file->open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        QMessageBox::critical(NULL, QString("Load XML file problem"), QString("Couldn't open %1 to load settings").arg(m_strDescriptorFileName), QMessageBox::Ok);
        if (ptr_xml_file)
            delete ptr_xml_file;
        return false;
    };
    //declare xml-document
    QDomDocument xml_document ("database_descriptor");
    xml_document.setContent(ptr_xml_file);
    //
    bool b_xml_res = parseXMLDocument(&xml_document);
    //
    if (ptr_xml_file)
        delete ptr_xml_file;
    //
    return b_xml_res;
};
