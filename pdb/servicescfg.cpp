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
#include "servicescfg.h"
#include "dbacccesssafe.h"
#include "logger.h"
//
#include <QtSql>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QByteArray>
//

ServicesCfg& ServicesCfg::getInstance()
{
    static ServicesCfg instance;

    return instance;
}

//
ServicesCfg::ServicesCfg(QObject *parent) :
    QObject(parent)
{
    m_bIsAnotherInstanceActive = false;
}

ServicesCfg::~ServicesCfg()
{

}

void    ServicesCfg::getDataAndCheckInstance()
{
    getServiceTableData_DB(m_bIsAnotherInstanceActive, m_strPasswordHash);
    //
    if (false ==  m_bIsAnotherInstanceActive)
    {
        setInstanceInUse_DB();
    }else //somebody else working now
    {
        m_bIsAnotherInstanceActive = true;
#ifdef QT_NO_DEBUG  //because it is really annoying
        emit DbInUse ();
#endif
    };
    //
    if (m_strPasswordHash.length() > 0)
        emit PasswordExists(true);
    else
        emit PasswordExists(false);
    //
    //emit PasswordCorrect()

}

void ServicesCfg::createRecord_DB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //INSERT INTO into service_tbl (is_already_in_use ) values (1);
    //
    QString str_insert_string = "INSERT INTO service_tbl (is_already_in_use) values (1);";
    //
    if (!qry.prepare( str_insert_string ))
    {
        qDebug() << qry.lastError();
        return;
    };
    //
    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        return;
    };
    //
    qDebug( "Inserted!" );
}

void ServicesCfg::setInstanceInUse_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string;
    //
    str_update_string = "UPDATE service_tbl SET is_already_in_use = 1;";
    //
    qry.prepare( str_update_string );
    //
    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        return;
    };
    //
    return;
}

void ServicesCfg::getServiceTableData_DB ( bool& b_another_instance_active, QString& str_password_hash )
{
    bool            b_record_exist      = false;

    { // use this brackets because 1 connection is locked here and can not be used in the
      // createRecord_DB and setInstanceInUse_DB functions
      //
        DBAcccessSafe   db_safe;
        QMessageBox     box;
        int             id_already_in_use   = 0;
        //
        b_another_instance_active   =   false;
        //
        QSqlDatabase* ptr_db = db_safe.getDB();
        Q_ASSERT(ptr_db);
        if (NULL == ptr_db)
            return;
        //
        QSqlQuery qry(*ptr_db);
        //-----------------------------------------------------------------------------------------
        QString str_select_str = "select is_already_in_use, password from service_tbl;";
        //
        bool b_sql_result = qry.prepare( str_select_str );
        //
        if (false == b_sql_result)
        {
            box.setText( "Unable to get prepare the query. "+ qry.lastError().text() );
            box.exec();
            const QString str_msg = QString("Unable prepare query %1").arg( qry.lastError().text() );
            Logger::getInstance().logIt(en_LOG_ERRORS,str_msg, &str_select_str);
            return;
        };
        //
        b_sql_result = qry.exec();
        //
        if( false == b_sql_result )
        {
            box.setText( "Unable to get exec the query. " + qry.lastError().text() );
            box.exec();
            const QString str_msg = QString("Unable exec query %1").arg( qry.lastError().text() );
            Logger::getInstance().logIt(en_LOG_ERRORS,str_msg, &str_select_str);
            return;
        };
        //
        while( qry.next() )
        {
            b_record_exist = true;
            //
            id_already_in_use    = qry.value(0).toInt();
            str_password_hash    = qry.value(1).toString();
            //
            if (id_already_in_use == 1)
                b_another_instance_active = true;
        }; // while( qry.next() )
        //
    }

    if (false == b_record_exist)
    {
        createRecord_DB();
        setInstanceInUse_DB();
    };
    //
    return;
}

void ServicesCfg::dropInstance()
{
    if (true == m_bIsAnotherInstanceActive)
        return;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string;
    //
    str_update_string = "UPDATE service_tbl SET is_already_in_use = 0;";
    //
    qry.prepare( str_update_string );
    //
    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        return;
    };
    //
    m_bIsAnotherInstanceActive = false;
    return;
}

bool ServicesCfg::setNewPassword ( const QString& str_password )
{
    if (str_password.length() == 0)
    {
        const QString str_msg = QString("Unable to set empty password");
        Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
        return false;
    };
    //
    QString str_old_password = m_strMainPassword;
    m_strMainPassword = str_password;
    //
    if ( setNewPassword_DB() == false )
    {
        m_strMainPassword = str_old_password;
        return false;
    };
    //
    return true;
}

bool ServicesCfg::setNewPassword_DB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string;
    //
    QByteArray  ba_entered_password = m_strMainPassword.toUtf8();
    const QString str_new_pwd_hash  = QCryptographicHash::hash(ba_entered_password, QCryptographicHash::Sha1).toBase64();
    //
    str_update_string = QString ("UPDATE service_tbl SET password='%1';").arg(str_new_pwd_hash);
    //
    qry.prepare( str_update_string );
    //
    if( !qry.exec() )
    {
        qDebug() << qry.lastError();
        return false;
    };
    //
    emit PasswordCreated (true);
    //
    return true;
}

void ServicesCfg::enterExistingPassword (const QString& str_password)
{
    m_strMainPassword = str_password;
    emit PasswordExists(true);
}
