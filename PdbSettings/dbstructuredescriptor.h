#ifndef DBSTRUCTUREDESCRIPTOR_H
#define DBSTRUCTUREDESCRIPTOR_H
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
