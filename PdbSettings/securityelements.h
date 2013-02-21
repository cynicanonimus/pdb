#ifndef SECURITYELEMENTS_H
#define SECURITYELEMENTS_H
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
//class is responsible for representing security-related user interface
//
#include "abstractgraficeelements.h"
//
#include <QObject>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
//

class SecurityElements : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit SecurityElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout);
    virtual ~SecurityElements();

    void writeData();
signals:
    
public slots:

private slots:
    void    onClickShred        ();
    void    onEncryptModeChanged(int);
    //

private:
    void    createLayout        ();
    void    readData            ();
    void    updateData          ( bool b_from_dialog );   //exchange between dialog and variables
    void    createLinks         ();
    //

private:
    //--------------------------------------------------
    QGroupBox*      m_ptrAttachmentGroupBox;
    QGridLayout*    m_ptrAttachmentGrid;
    //
    QLabel*         m_ptrEncryptModeLabel;
    QComboBox*      m_ptrEncryptMode;
    unsigned int    m_uiEncryptMode;
    //
    QCheckBox*      m_ptrMarkCryptByDefault;
    bool            m_bMarkCryptByDefault;
    //--------------------------------------------------
    QLabel*         m_ptrBackupLabel;
    QLineEdit*      m_ptrEditBackup;

    QGroupBox*      m_ptrProtectionGroupBox;
    QGridLayout*    m_ptrProtectionGrid;
    QCheckBox*      m_ptrProtectionByDefault;
    //..
    bool            m_bProtectByDefault;
    //--------------------------------------------------
    QGroupBox*      m_ptrTmpFilesGroupBox;
    QGridLayout*    m_ptrTmpFilesGrid;
    //
    QCheckBox*      m_ptrCheckboxUseShredder;
    QLabel*         m_ptrShredderLabel;
    QLineEdit*      m_ptrEditShredderCmdLine;
    //..
    bool            m_bShred;
    QString         m_strSchredCommandLine;
    //
    bool            m_bSuccessEncryptDecryptIndicator;
};

#endif // SECURITYELEMENTS_H
