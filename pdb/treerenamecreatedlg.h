#ifndef TREERENAMECREATEDLG_H
#define TREERENAMECREATEDLG_H

#include <QDialog>

namespace Ui {
class TreeRenameCreateDlg;
}

class TreeRenameCreateDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeRenameCreateDlg(QWidget *parent = 0);
    ~TreeRenameCreateDlg();

public slots:
    void accept();

public:
    enum DLG_MODE
    {
        RENAME_DLG,
        CREATE_DLG
    };
public:

    void            setDlgMode          (DLG_MODE en_dlg_mode);
    //
    const QString&  getTreeName         ();
    void            setTreeName         (const QString&  str_tree_name);
    //
    const QString&  getRootNodeName     ();
private:
    DLG_MODE        m_enActualDlgMode;
    //
    QString         m_strTreeName;
    QString         m_strRootNodeName;
    //
    Ui::TreeRenameCreateDlg *ui;
};

#endif // TREERENAMECREATEDLG_H
