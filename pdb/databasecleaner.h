#ifndef DATABASECLEANER_H
#define DATABASECLEANER_H

#include <QObject>
#include <QRunnable>
class DatabaseCleaner : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DatabaseCleaner(QObject *parent = 0);
    
signals:
    
public slots:

public:
    void run();

private:
    bool clearAttachments();
    bool clearNodes();
    bool clearTrees();
    
};

#endif // DATABASECLEANER_H
