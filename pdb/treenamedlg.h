#ifndef TREENAMEDLG_H
#define TREENAMEDLG_H
//
// treenamedlg.h
//
#include <QDialog>

namespace Ui {
class TreeNameDlg;
}

class TreeNameDlg : public QDialog
{
    Q_OBJECT

public slots:
    void accept();
public:
    explicit TreeNameDlg(QWidget *parent = 0);
    ~TreeNameDlg();
   //
    void        setTreeNameLabel   (const QString& str_label);
    void        setRootNameLabel   (const QString& str_label);
   //
    void        setTreeName(const QString& str_tree_name);
    QString     getTreeName             () const;
    //
    QString     getRootNodeName         () const;
    //
    void        setRenameMode(bool b_rename_mode_active);
    
private:
    Ui::TreeNameDlg* ui;
    bool        m_bRenameMode;
};

#endif // TREENAMEDLG_H
