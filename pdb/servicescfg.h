#ifndef SERVICESCFG_H
#define SERVICESCFG_H
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
