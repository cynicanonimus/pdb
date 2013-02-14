#ifndef TUNNELMAKER_H
#define TUNNELMAKER_H
//
// class is responsible for making the ssh tunnel if it is defined in the configuration.
//
#include <QObject>
#include <QProcess>

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

private:

    QProcess*       m_ptrTunnelProcess;
    QString         m_strRawCommand;
    bool            m_bTunnelIsActiveNow;
    bool            m_bTunnelNeedToCreate;
    
};

#endif // TUNNELMAKER_H
