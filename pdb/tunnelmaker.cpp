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
#include "tunnelmaker.h"
#include "../CommonInclude/pdb/pdb_style.h"
#include "../CommonInclude/pdb/DBSettings.h"
//
#include <QSettings>
#include <QMessageBox>
#include <QTime>
#include <QCoreApplication>
//
TunnelMaker::TunnelMaker(QObject *parent) :
    QObject(parent)
{
    m_ptrTunnelProcess      = NULL;
    setTunnelActive(false);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    QVariant var            = settings.value(g_str_DB_SETTINGS);
    DBSettings db_settings  = var.value<DBSettings>();
    //
    m_bTunnelNeedToCreate   = db_settings.m_vMakeTunnel[db_settings.getCurrentPage()];
    m_strRawCommand         = db_settings.m_TunnelList[db_settings.getCurrentPage()];
    //
    return;
}

TunnelMaker::~TunnelMaker ()
{
    destroyTunnel();
}

void TunnelMaker::makeTunnel      ()
{
    if (false == m_bTunnelNeedToCreate)
        return;
    //
    QStringList str_split_list = m_strRawCommand.split(" ",QString::SkipEmptyParts);
    //
    QString program = str_split_list.at(0);
    str_split_list.removeAt(0);
    QStringList arguments = str_split_list;
    //
    m_ptrTunnelProcess = new QProcess(this);
    //
    QObject::connect(m_ptrTunnelProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT( onTunnelError  ( QProcess::ProcessError)  ));
    QObject::connect(m_ptrTunnelProcess, SIGNAL(started()),                     this, SLOT( onTunnelStart  ()                         ));
    QObject::connect(m_ptrTunnelProcess, SIGNAL(finished(int)),                 this, SLOT( onTunnelFinish (int)                      ));
    //
    m_ptrTunnelProcess->start(program, arguments);
    //
    QTime dieTime= QTime::currentTime().addSecs(3);
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //
        if ( true == getTunnelActive() )  //tunnel is active now
            break;
    };
    //
//    Logger::getInstance().logIt(en_LOG_ERRORS, "tunnel created");
    return;
}

void TunnelMaker::destroyTunnel ()
{
    if (false == m_bTunnelNeedToCreate)
        return;
    //
    if (m_ptrTunnelProcess)
    {
        setTunnelActive(false);
        m_ptrTunnelProcess->terminate();
        m_ptrTunnelProcess->kill();
        //
        QTime dieTime= QTime::currentTime().addSecs(3);
        while( QTime::currentTime() < dieTime )
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        };
        //
        delete m_ptrTunnelProcess;
        m_ptrTunnelProcess = NULL;        
    };
}

void TunnelMaker::onTunnelFinish(int)
{
    setTunnelActive(false);
//    Logger::getInstance().logIt(en_LOG_ERRORS, "tunnel destroyed");
}

void TunnelMaker::onTunnelStart ()
{
    setTunnelActive(true);
}

void TunnelMaker::onTunnelError ( QProcess::ProcessError err)
{
    if ( false == getTunnelActive() ) //because when we terminate the task, we always get the error
        return;
    //
    //
    QString str_msg;
    QMessageBox box;
    //
    switch(err)
    {
    case QProcess::FailedToStart:
        str_msg = "Can not make tunnel. Process failed to start.";
        break;
    case QProcess::Crashed:
        str_msg = "Tunnel crashed";
        break;
    case QProcess::Timedout:
        str_msg = "Process timeout expired";
        break;
    case QProcess::WriteError:
        str_msg = "Can not write to process";
        break;
    case QProcess::ReadError:
        str_msg = "Can not read from process";
        break;
    default:
        str_msg = "Unknown error. Process stopped.";
        break;
    };
    //
    box.setText(str_msg);
    box.exec();
}

void TunnelMaker::setTunnelActive ( bool b_active )
{
    QMutexLocker locker ( &m_TunnelStatusLocker );
    m_bTunnelIsActiveNow = b_active;
}

bool TunnelMaker::getTunnelActive ()
{
    bool b_active_status = false;
    //
    {
        QMutexLocker locker ( &m_TunnelStatusLocker );
        b_active_status = m_bTunnelIsActiveNow;
    };
    //
    return b_active_status;
}
