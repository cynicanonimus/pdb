#ifndef WAITER_H
#define WAITER_H
//
// waiter.h
//
#include <QObject>

class Waiter : public QObject
{
    Q_OBJECT
public:
    explicit Waiter(QObject *parent = 0);
    
    static void wait (int i_sec);

signals:
    
public slots:
    
};

#endif // WAITER_H
