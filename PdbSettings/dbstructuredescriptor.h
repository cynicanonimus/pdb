#ifndef DBSTRUCTUREDESCRIPTOR_H
#define DBSTRUCTUREDESCRIPTOR_H

#include <QObject>
#include <QMap>

typedef QMap<int, QString> StatementsMap;

class DbStructureDescriptor : public QObject
{
    Q_OBJECT
public:
    explicit DbStructureDescriptor(const QString& str_diver_name, const QString& str_show_tables_request, QObject *parent = 0);
    
    bool                    addStatement        ( int id, const QString& str_statement );
    const QString&          getDriverName       () const                                                { return m_strDiverName; }
    const QString&          getShowTablesRequest() const                                                { return m_strShowTablesRequest; }
    const StatementsMap&    getStatements       () const                                                { return m_SQLStatements;}

signals:
    
public slots:
    
private:

    StatementsMap       m_SQLStatements;
    QString             m_strDiverName;
    QString             m_strShowTablesRequest;
};

#endif // DBSTRUCTUREDESCRIPTOR_H
