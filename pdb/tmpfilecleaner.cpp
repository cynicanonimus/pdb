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
#include "tmpfilecleaner.h"
#include "logger.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QMutexLocker>
#include <QFile>
#include <QSettings>
#include <QProcess>


TmpFileCleaner::TmpFileCleaner(QObject *parent) :
    QObject(parent)
{
    getShredstatus();
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bDeleteOnExit = settings.value( g_str_TMP_DELETE ).toBool();
}

TmpFileCleaner::~TmpFileCleaner()
{

}

TmpFileCleaner& TmpFileCleaner::getInstance()
{
    static TmpFileCleaner instance;
    return instance;
}

void TmpFileCleaner::deleteFile(const QString& str_tmp, bool b_immediately)
{
    if (b_immediately)
    {
        if ( QFile::exists(str_tmp) )
        {
            if (mbUseShred)
            {
                shred_it(str_tmp);
            }else
            {
                QFile::remove(str_tmp);
            };
        };
        //
        return;
    };
    //
    if (m_bDeleteOnExit)
    {
        QMutexLocker lock(&m_ListAccessBlocker);
        m_vTmpFilesList.push_back(str_tmp);
    };
}

void TmpFileCleaner::getShredstatus()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    mbUseShred          = settings.value(g_str_SEC_SCHRED).value<bool>();
    m_strShredCommand   = settings.value(g_str_SEC_SCHRED_CMD).value<QString>();
    //
    if (mbUseShred)
    {
        if (m_strShredCommand.length() == 0)
        {
            const QString str_msg = QString("Command for shreder not defined. Use classic delete:");
            Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
            mbUseShred = false;
        }else if ( m_strShredCommand.indexOf("%file", 0, Qt::CaseInsensitive) == -1)
        {
            const QString str_msg = QString("Command for shreder has wrong format '%file' not found. 'Classic'' delete will be used:");
            Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
            mbUseShred = false;
        };
    };
}

void TmpFileCleaner::shred_it ( const QString& str_file_name )
{
    QString str_final_file_name = str_file_name;
    //
    str_final_file_name = "\"" + str_final_file_name + "\"";
    //
    QString str_shred_command = m_strShredCommand;
    str_shred_command.replace("%file",str_final_file_name, Qt::CaseInsensitive);
    /*int i_exit_code = */QProcess::execute(str_shred_command);
    return;
}

void TmpFileCleaner::clearAll()
{
    QMutexLocker lock(&m_ListAccessBlocker);
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if (settings.value( g_str_TMP_DELETE ).toBool() == false)
        return;
    //
    std::vector<QString>::iterator itr;
    for (itr = m_vTmpFilesList.begin(); itr != m_vTmpFilesList.end(); ++itr)
    {
        if (QFile::exists(*itr))
        {
            if (mbUseShred)
            {
                shred_it(*itr);
            }else
            {
                QFile::remove(*itr);
            };
        };
    };
    //
    m_vTmpFilesList.erase( m_vTmpFilesList.begin(), m_vTmpFilesList.end() );
}
