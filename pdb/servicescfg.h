#ifndef SERVICESCFG_H
#define SERVICESCFG_H
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
/*
The class is responsible for reading service_tbl and doing some actions:
1. check if database is already in use
2. get-set general password into database
*/
#include <QObject>
#include <QByteArray>

class ServicesCfg : public QObject
{
    Q_OBJECT
public:

                ~ServicesCfg     ();
    static      ServicesCfg&    getInstance  ();
private:
    explicit    ServicesCfg(QObject *parent = 0);
                ServicesCfg  (ServicesCfg const&);
    void        operator=(ServicesCfg const&);


public:
    const QString&  getPasswordHash         () const            {return m_strPasswordHash;}
    const QString&  getPassword             () const            {return m_strMainPassword;}
    //
    void            enterExistingPassword   ( const QString& str_password ); //only for set right already existing password
    bool            setNewPassword          ( const QString& str_password ); //when user wants to replace password or create it on begin

    //
    void            dropInstance            ();
    void            getDataAndCheckInstance ();

signals:
    void    DbInUse             ();
    void    PasswordExists      ( bool b_exists  );
    void    PasswordCreated     ( bool b_created );
    
public slots:
    
private:
    void    createRecord_DB         ();
    void    setInstanceInUse_DB     ();
    void    getServiceTableData_DB  ( bool& b_another_instance_active, QString& str_password_hash );
    bool    setNewPassword_DB       ();

    //
    bool        m_bIsAnotherInstanceActive;
    QString     m_strMainPassword;
    QString     m_strPasswordHash;
};

#endif // SERVICESCFG_H
