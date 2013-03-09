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
#include "logger.h"
#include "../CommonInclude/pdb/DBSettings.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>
#include <QMutexLocker>
#include <QDateTime>

Logger* p_instance = NULL;

Logger::Logger(QObject *parent) :
    QObject(parent)
{
    m_ptrOutStream = NULL;
    //
    //read registry, set logging settings.
    //
    readSettings();
}

Logger::~Logger()
{

}

void Logger::stopLog()
{
    if (m_ptrOutStream)
    {
        m_ptrOutStream->flush();
        m_File.close();
        delete m_ptrOutStream;
        m_ptrOutStream = NULL;
    };
}

Logger& Logger::getInstance()
{
    if (NULL == p_instance)
        p_instance = new Logger();
    //
    return (*p_instance);
}

void Logger::readSettings()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_strLogPath      = settings.value(g_str_LOG_PATH).toString();
    //
    const QVariant flags =     settings.value(g_str_LOG_FLAGS);
    //
    m_bitFlags = flags .toBitArray();
    //
    if ( (m_bitFlags.size() == 0) || (m_bitFlags.size() < en_BITARRAY_SIZE) )
    {
        m_bitFlags.resize(en_BITARRAY_SIZE);
    };
    //
    if (m_strLogPath.length() == 0)
    {
        m_bitFlags[en_ENABLE_LOG] = false;
    };
}

bool Logger::isLogCodeActive (unsigned int ui_err_code)
{
    //if log enabled
    if (m_bitFlags[en_ENABLE_LOG] == false)
        return false;
    // if error code is valid
    if ( ui_err_code >= (unsigned int) en_BITARRAY_SIZE)//code is not defined, can not log it
        return false;
    // if appropriate bit is ON
    if (m_bitFlags[ui_err_code] )
    {
        return true;
    };
    //
    return false;
}

void Logger::writeBeginLog()
{
    if (m_ptrOutStream)
        return;  //do it only once
    //
    if (
            ( m_strLogPath[m_strLogPath.length() - 1] != '/' ) &&
            (m_strLogPath[m_strLogPath.length() - 1] != '\\')
       )
    {
        m_strLogPath += g_strPATH_DELIMETER;
    };
    //
    const QDateTime now_datetime = QDateTime::currentDateTime();
    //
    const QString str_log_file = QString("%1pdb_%2.log").arg(m_strLogPath).arg(now_datetime.toString("dd_MM_yyyy"));
    //
    m_File.setFileName(str_log_file);
    //
    bool b_res_file_open = false;
    //
    if ( QFile::exists(str_log_file) )
    {
        b_res_file_open = m_File.open ( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    }else
    {
        b_res_file_open = m_File.open ( QIODevice::WriteOnly | QIODevice::Text);
    };
    //
    if (false == b_res_file_open)
        return;
    // get db parameters
    DBSettings db_settings;
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    QVariant var = settings.value(g_str_DB_SETTINGS);
    db_settings = var.value<DBSettings>();
    //

    //
    m_ptrOutStream = new QTextStream(&m_File);
    (*m_ptrOutStream)<<"\n\n==== begin log: [ "<<QDateTime::currentDateTime().toString()        <<" ]   ";
    (*m_ptrOutStream)<<" host: [ "    <<db_settings.m_DbHosts   [db_settings.getCurrentPage()]<<" ]   ";
    (*m_ptrOutStream)<<" database: [ "<<db_settings.m_DbNames   [db_settings.getCurrentPage()]<<" ]   ";
    (*m_ptrOutStream)<<" user: [ "    <<db_settings.m_UserNames [db_settings.getCurrentPage()]<<" ]   ";
    (*m_ptrOutStream)<<" ====";
}

void Logger::logIt(unsigned int ui_err_code, const QString &str_message)
{
    if ( isLogCodeActive(ui_err_code) == false )
        return;
    //
    QMutexLocker locker (&m_FileLocker);
    //
    writeBeginLog(); //it is happens only once
    //
    if ( NULL == m_ptrOutStream )
        return;
    //
    (*m_ptrOutStream)<<"\n"<<QDateTime::currentDateTime().toString()<<" "<<getLogString(ui_err_code)<<": "<<str_message;
}

QString Logger::getLogString (unsigned int ui_err_code) const
{
    QString str_ret;
    //
    switch(ui_err_code)
    {
    case en_LOG_ERRORS:
        str_ret = tr (" Error");
        break;
    case en_LOG_CREATE_NODE:
        str_ret = tr (" Create node");
        break;
    case en_LOG_EXPORT_NODE:
        str_ret = tr (" Export node");
        break;
    case en_LOG_UPDATE_NODE:
        str_ret = tr (" Rename node");
        break;
    case en_LOG_MOVE_NODE:
        str_ret = tr (" Move node");
        break;
    case en_LOG_DESCRIPTOR_NODE:
        str_ret = tr (" Descriptor of node");
        break;
    case en_LOG_DELETE_NODE:
        str_ret = tr (" Delete node");
        break;
    //-------------------------------------
    case en_LOG_UPDATE_ATTACH:
        str_ret = tr (" Update attach");
        break;
    case en_LOG_CREATE_ATTACH:
        str_ret = tr (" Create attach");
        break;
    case en_LOG_VIEW_ATTACH:
        str_ret = tr (" View   attach");
        break;
    case en_LOG_RENAME_ATTACH:
        str_ret = tr (" Rename attach");
        break;
    case  en_LOG_EXPORT_ATTACH:
        str_ret = tr (" Export attach");
        break;
    case  en_LOG_MOVE_ATTACH:
        str_ret = tr (" Move   attach");
        break;
    case  en_LOG_DELETE_ATTACH:
        str_ret = tr (" Delete attach");
        break;
    case  en_LOG_CRYPT_ATTACH:
        str_ret = tr (" Crypt/decrypt attach");
        break;
    //-------------------------------------
    case  en_LOG_CREATE_TREE:
        str_ret = tr (" Create tree");
        break;
    case  en_LOG_RENAME_TREE:
        str_ret = tr (" Rename tree");
        break;
    case  en_LOG_DELETE_TREE:
        str_ret = tr (" Delete tree");
        break;
    default:
        str_ret = tr (" Unknown message");
        break;
    };
    //
    return str_ret;
}
/*
//--------------------------------------------
en_LOG_CRYPT_ATTACH, not implemented yet
*/
