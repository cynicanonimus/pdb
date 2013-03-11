#ifndef COLORGRAFICELEMENTS_H
#define COLORGRAFICELEMENTS_H
//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
//
#include <QObject>
//
#include "abstractgraficeelements.h"
//
class ColorGraficElements : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit ColorGraficElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject *parent = 0);
    virtual ~ColorGraficElements();

signals:
    
public slots:
    void        onChangeNormalElementBackground();
    void        onChangeNormalElementForeground();
    //
    void        onChangeDelElementBackground();
    void        onChangeDelElementForeground();
    //
    void        onChangeAttachNormalElementBackground();
    void        onChangeAttachNormalElementForeground();
    //
    void        onChangeAttachDelElementBackground();
    void        onChangeAttachDelElementForeground();
    //
    void        onChangeAttachUpdateElementBackground();
    void        onChangeAttachUpdateElementForeground();
    //
    void        onChangeAttachFailElementBackground();
    void        onChangeAttachFailElementForeground();
    //
    void        onChangeComboBoxSelection(int index);
public:
//
    void        writeData       ();

protected:

private:
//
    void        readData        ();
    void        updateData      (bool b_from_dialog, bool b_data_changed = true); //exchange between dialog and variables
    void        createLinks     ();
    void        createLayout    ();
    bool        getDialogColor  (QColor& t_color);

//
private:
    QGroupBox*      m_pParentFrame;
    //
    QGridLayout*    m_pGridLayout;
    //
    QLabel*         m_pStyleNameLabel;
    QComboBox*      m_pComboStyle;
    //
    QLabel*         m_pLblNormalTreeElement;
    QPushButton*    m_pBtnChange_B_NormalTreeELement;
    QPushButton*    m_pBtnChange_F_NormalTreeELement;
    //
    QLabel*         m_pLblDelTreeElement;
    QPushButton*    m_pBtnDel_B_NormalTreeELement;
    QPushButton*    m_pBtnDel_F_NormalTreeELement;
    //
    QLabel*         m_pLblNormalAttachmentElement;
    QPushButton*    m_pBtnChange_B_AttachmentELement;
    QPushButton*    m_pBtnChange_F_AttachmentELement;
    //
    QLabel*         m_pLblDeletedAttachmentElement;
    QPushButton*    m_pBtnDeleted_B_AttachmentELement;
    QPushButton*    m_pBtnDeleted_F_AttachmentELement;
    //
    QLabel*         m_pLblUpdatingAttachmentElement;
    QPushButton*    m_pBtnUpdating_B_AttachmentELement;
    QPushButton*    m_pBtnUpdating_F_AttachmentELement;
    //
    QLabel*         m_pLblFailAttachmentElement;
    QPushButton*    m_pBtnFail_B_AttachmentELement;
    QPushButton*    m_pBtnFail_F_AttachmentELement;

//
private:
//
    QString m_strAppStyleName;
    //
    QColor  m_cl_NormalTreeElementBack;
    QColor  m_cl_NormalTreeElementFront;
    //
    QColor  m_cl_DelTreeElementBack;
    QColor  m_cl_DelTreeElementFront;
    //
    QColor  m_cl_NormalAttachmentElementBack;
    QColor  m_cl_NormalAttachmentElementFront;
    //
    QColor  m_cl_DelAttachmentElementBack;
    QColor  m_cl_DelAttachmentElementFront;
    //
    QColor  m_cl_UpdatingAttachmentElementBack;
    QColor  m_cl_UpdatingAttachmentElementFront;
    //
    QColor  m_cl_FailAttachmentElementBack;
    QColor  m_cl_FailAttachmentElementFront;
};

#endif // COLORGRAFICELEMENTS_H
