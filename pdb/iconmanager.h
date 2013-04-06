#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <QIcon>

class IconManager : public QObject
{
    Q_OBJECT
public:
                            ~IconManager    ();
    static IconManager&     getInstance ();

signals:
    
public slots:

private:
    /*
    Internal struct for store icon info
    */
    struct DBIcon
    {
        DBIcon();
        ~DBIcon();
        //
        QString m_strHash;
        QIcon*  m_ptrIcon;
    };

private:
    explicit                IconManager(QObject *parent = 0);
                            IconManager (IconManager const&);
    void                    operator=(IconManager const&);

private:
    
};

#endif // ICONMANAGER_H
