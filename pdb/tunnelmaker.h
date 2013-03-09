#ifndef TUNNELMAKER_H
#define TUNNELMAKER_H
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
//
// class is responsible for making the ssh tunnel if it is defined in the configuration.
//
#include <QObject>
#include <QProcess>
#include <QMutex>
#include <QMutexLocker>
//
//
//
class TunnelMaker : public QObject
{
    Q_OBJECT
public:
    explicit        TunnelMaker     ( QObject *parent = 0 );
                    ~TunnelMaker    ();
    //
    void            makeTunnel      ();
    void            destroyTunnel   ();
    //
signals:
    
public slots:

private slots:
    void            onTunnelError       ( QProcess::ProcessError );
    void            onTunnelStart       ();
    void            onTunnelFinish      (int);

private:
    void            setTunnelActive     (bool b_active);
    bool            getTunnelActive     ();
private:

    QProcess*       m_ptrTunnelProcess;
    QString         m_strRawCommand;
    bool            m_bTunnelIsActiveNow;
    bool            m_bTunnelNeedToCreate;
    //
    QMutex          m_TunnelStatusLocker;
    
};

#endif // TUNNELMAKER_H
