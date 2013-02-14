#ifndef TREESTATISTICDLG_H
#define TREESTATISTICDLG_H

#include <QDialog>

namespace Ui {
class TreeStatisticDlg;
}

class TreeStatisticDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeStatisticDlg(QWidget *parent = 0);
    ~TreeStatisticDlg();

public:
    void keyPressEvent          (QKeyEvent * event);
    void setNodeAmount          (unsigned int ui_node_amount);
    void setAttachmentsAmount   (unsigned int ui_attachment_amount);
    void setAttachmentsSize     (unsigned int ui_attachment_size);

private slots:
    void onOK();


private:
    Ui::TreeStatisticDlg *ui;
};

#endif // TREESTATISTICDLG_H
