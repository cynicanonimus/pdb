#ifndef GLOBALTREEOPERATIONSHANDLER_H
#define GLOBALTREEOPERATIONSHANDLER_H
//
// class is responsible for global operations with the trees: create new, rename, delete
//
#include <QObject>
#include <QComboBox>
#include <QStatusBar>
#include "mytree.h"
//
class RootOfTree;
//
class GlobalTreeOperationsHandler : public QObject
{
    Q_OBJECT
public:
    explicit GlobalTreeOperationsHandler(QObject *parent = 0);

public:
    void        setPtrToComboBox    (QComboBox*     ptr_combo);
    void        setPtrToTree        (MyTree*        ptr_to_tree);
    void        setPtrToStatusBar   (QStatusBar*    ptr_to_statusbar);

signals:
    
public slots:
    void    onCreateNewTree     ();
    void    onRenameCurrentTree ();
    void    onInfoAboutTree     ();
    void    onDeleteTree        ();

private:
    //
    // incoming parameters: name of the tree, name of the root node
    // outgoing parameters: ** to the root node
    //
    RootOfTree* createNewTree_DB( const QString& str_tree_name, const QString& str_root_node_name );
    //
    void        requestTreeInfo_DB (unsigned int& node_amount,
                                    unsigned int& attachments_amount,
                                    unsigned int& attachments_size,
                                    unsigned int  tree_id);

private:
    QComboBox*       m_ptrCombo;
    MyTree*          m_ptrTree;
    QStatusBar*      m_ptrStatusBar;
};

#endif // GLOBALTREEOPERATIONSHANDLER_H
