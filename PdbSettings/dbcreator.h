#ifndef DBCREATOR_H
#define DBCREATOR_H

#include <QObject>
#include <QMap>
#include <QStringList>
/*
class is responsible for reading, checking and creating database structure.
*/

class DBSettings;
class QDomDocument;
class QDomElement;
class DbStructureDescriptor;

typedef QMap<QString, DbStructureDescriptor*>  DbDescriptors;

class DBCreator : public QObject
{
    Q_OBJECT
public:
    explicit                DBCreator               ( QObject *parent = NULL);
                            ~DBCreator              ();
public:
    //
    void                    setDbSettings           ( DBSettings *ptr_db_settings );
    bool                    readDbFileDescription   ();
    const QString&          getDescriptorFileName   ()                                      { return m_strDescriptorFileName; }
    const DbDescriptors&    getDescriptorsList      () const                                { return m_DBDescriptionStorage;  }
    const QString&          getDefaultDbDriverName  () const;
    const QStringList&      getMandatoryTablesList  () const                                { return m_lstMandatoryTables;    }
    //
signals:
    
public slots:

private:
    bool        parseXMLDocument        ( QDomDocument* ptr_xml_document );
    //
    // this one is common for any type of database
    //
    bool        getMandatoryTablesList  ( QDomElement* ptr_element );
    bool        getDbType               ( QDomElement* ptr_element, QString& str_db_type );
    bool        getDriverName           ( QDomElement* ptr_element, QString& str_driver_name );
    bool        getDbStatements         ( QDomElement* ptr_element, DbStructureDescriptor* ptr_db_structure);
    bool        getShowTablesQuery      ( QDomElement* ptr_element, QString& str_show_tables_request );
private:
    const QString   m_strDescriptorFileName;
    QString         m_strErrHeader;
    DBSettings*     m_ptrDbSettings;
    bool            m_bStatusOK;
    QStringList     m_lstMandatoryTables;
    //
    //key of the map is database type, value DbStructureDescriptor
    //
    DbDescriptors   m_DBDescriptionStorage;
};

#endif // DBCREATOR_H
