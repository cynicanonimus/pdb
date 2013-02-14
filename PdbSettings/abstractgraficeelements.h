#ifndef ABSTRACTGRAFICEELEMENTS_H
#define ABSTRACTGRAFICEELEMENTS_H

#include <QObject>
//
// prototype for all grafic elements
///abstractgraficeelements.h
//
#include <QGroupBox>
//#include <QLabel>
//#include <QLineEdit>
//#include <QPushButton>
#include <QGridLayout>

#include <vector>

class AbstractGraficeElements : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGraficeElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject *parent = 0);
    virtual ~AbstractGraficeElements();
public:

    typedef std::vector <QWidget*>   GraficCollection;
    //
    virtual void    show                    (bool b_show);
    void            deleteElementsOnExit    (bool b_del = true);
    void            addAndRegisterElement   (QWidget* ptr_widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = 0);
    void            addAndRegisterElement   (QWidget* ptr_widget, int row, int column, Qt::Alignment = 0);

public:
    virtual void    writeData() = 0;

signals:
    
public slots:

protected:
     virtual void   createLayout() = 0;
    virtual void    readData    ()    = 0;
    virtual void    updateData  (bool b_from_dialog) = 0;   //exchange between dialog and variables

    QGroupBox*          m_pParentFrame;
    QGridLayout*        m_pGridLayout;
    //
    GraficCollection    m_vGraficObjectsArray;
    QString             m_strFrameName;
    //
private:
    void                registerElement         (QWidget* ptr_grafic_element);

private:
    bool                m_bDelObjectOnExit;
    
};

#endif // ABSTRACTGRAFICEELEMENTS_H