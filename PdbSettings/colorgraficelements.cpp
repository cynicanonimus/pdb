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
#include "colorgraficelements.h"
//
#include <QColorDialog>
#include <QSettings>
//
#include "../CommonInclude/pdb/pdb_style.h"
//
ColorGraficElements::ColorGraficElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject* parent) :
    AbstractGraficeElements(ptr_parent_frame, ptr_layout, parent)
{
    m_pParentFrame  = ptr_parent_frame;
    m_pGridLayout   = ptr_layout;
    //
    m_cl_NormalTreeElementBack.setNamedColor("white");
    m_cl_NormalTreeElementFront.setNamedColor("black");
    //
    m_cl_DelTreeElementBack.setNamedColor("white");
    m_cl_DelTreeElementFront.setNamedColor("black");
    //
    m_cl_NormalAttachmentElementBack.setNamedColor("white");
    m_cl_NormalAttachmentElementFront.setNamedColor("black");
    //
    m_cl_DelAttachmentElementBack.setNamedColor("white");
    m_cl_DelAttachmentElementFront.setNamedColor("black");
    //
    m_cl_UpdatingAttachmentElementBack.setNamedColor("white");
    m_cl_UpdatingAttachmentElementFront.setNamedColor("black");
    //
    m_cl_FailAttachmentElementBack.setNamedColor("white");
    m_cl_FailAttachmentElementFront.setNamedColor("black");
    //
    m_strAppStyleName = g_str_ar_APP_STYLES[4];
    //
    m_strFrameName = tr("Color settings");
    //
    deleteElementsOnExit(true);
    //
    readData();
    //
    createLayout();
    createLinks();
    //
    updateData(false, false);
}

ColorGraficElements::~ColorGraficElements()
{

};

void ColorGraficElements::createLinks()
{
    QObject::connect( m_pComboStyle, SIGNAL(currentIndexChanged(int)), this, SLOT( onChangeComboBoxSelection(int) ) );
    //
    QObject::connect( m_pBtnChange_B_NormalTreeELement, SIGNAL(clicked()), this, SLOT( onChangeNormalElementBackground() ) );
    QObject::connect( m_pBtnChange_F_NormalTreeELement, SIGNAL(clicked()), this, SLOT( onChangeNormalElementForeground() ) );
    //
    QObject::connect( m_pBtnDel_B_NormalTreeELement, SIGNAL(clicked()), this, SLOT( onChangeDelElementBackground() ) );
    QObject::connect( m_pBtnDel_F_NormalTreeELement, SIGNAL(clicked()), this, SLOT( onChangeDelElementForeground() ) );
    //
    QObject::connect( m_pBtnChange_B_AttachmentELement, SIGNAL(clicked()), this, SLOT( onChangeAttachNormalElementBackground() ) );
    QObject::connect( m_pBtnChange_F_AttachmentELement, SIGNAL(clicked()), this, SLOT( onChangeAttachNormalElementForeground() ) );
    //
    QObject::connect( m_pBtnDeleted_B_AttachmentELement, SIGNAL(clicked()), this, SLOT( onChangeAttachDelElementBackground() ) );
    QObject::connect( m_pBtnDeleted_F_AttachmentELement, SIGNAL(clicked()), this, SLOT( onChangeAttachDelElementForeground() ) );
    //
    QObject::connect( m_pBtnUpdating_B_AttachmentELement, SIGNAL(clicked()), this, SLOT (onChangeAttachUpdateElementBackground() ) );
    QObject::connect( m_pBtnUpdating_F_AttachmentELement, SIGNAL(clicked()), this, SLOT (onChangeAttachUpdateElementForeground() ) );
    //
    QObject::connect( m_pBtnFail_B_AttachmentELement, SIGNAL(clicked()), this, SLOT (onChangeAttachFailElementBackground() ) );
    QObject::connect( m_pBtnFail_F_AttachmentELement, SIGNAL(clicked()), this, SLOT (onChangeAttachFailElementForeground() ) );
};

void ColorGraficElements::onChangeComboBoxSelection(int index)
{
   m_strAppStyleName = m_pComboStyle->itemText( index );
};


bool ColorGraficElements::getDialogColor(QColor& t_color)
{
    QColor temp_color = QColorDialog::getColor(Qt::white, m_pParentFrame );
    //
    if ( temp_color.isValid() == false )
    {
        return false;
    };
    int r = 0;
    int g = 0;
    int b = 0;
    temp_color.getRgb(&r,&g, &b);
    //
    t_color = temp_color;
    return true;
};

void ColorGraficElements::onChangeAttachUpdateElementBackground()
{
    bool b_res = getDialogColor( m_cl_UpdatingAttachmentElementBack);
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeAttachUpdateElementForeground()
{
    bool b_res = getDialogColor( m_cl_UpdatingAttachmentElementFront);
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeAttachFailElementBackground()
{
    bool b_res = getDialogColor( m_cl_FailAttachmentElementBack);
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeAttachFailElementForeground()
{
    bool b_res = getDialogColor( m_cl_FailAttachmentElementFront);
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeAttachNormalElementBackground()
{
    bool b_res = getDialogColor( m_cl_NormalAttachmentElementBack );
    //
    if(b_res )
    {
        updateData(false);
    };
}

void ColorGraficElements::onChangeAttachNormalElementForeground()
{
    bool b_res = getDialogColor( m_cl_NormalAttachmentElementFront );
    //
    if(b_res )
    {
        updateData(false);
    };
}

void ColorGraficElements::onChangeAttachDelElementBackground()
{
    bool b_res = getDialogColor( m_cl_DelAttachmentElementBack );
    //
    if(b_res )
    {
        updateData(false);
    };
}

void ColorGraficElements::onChangeAttachDelElementForeground()
{
    bool b_res = getDialogColor( m_cl_DelAttachmentElementFront );
    //
    if(b_res )
    {
        updateData(false);
    };
}


void ColorGraficElements::onChangeNormalElementBackground()
{
    bool b_res = getDialogColor( m_cl_NormalTreeElementBack );
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeNormalElementForeground()
{
    bool b_res = getDialogColor( m_cl_NormalTreeElementFront );
    //
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::onChangeDelElementBackground()
{
    bool b_res = getDialogColor( m_cl_DelTreeElementBack );
    //
    //
    if(b_res )
    {
        updateData(false);
    };
}


void ColorGraficElements::onChangeDelElementForeground()
{
    bool b_res = getDialogColor( m_cl_DelTreeElementFront );
    //
    //
    if(b_res )
    {
        updateData(false);
    };
};

void ColorGraficElements::createLayout()
{
    m_pGridLayout->setSpacing(1);
    //
    const QString str_back_label = tr("Change background");
    const QString str_front_label= tr("Change foreground");
    //-------------------------------
    int i_row = 0;
    //
    m_pStyleNameLabel = new QLabel ("Main application style:");
    m_pStyleNameLabel->setAlignment(Qt::AlignLeft);
    addAndRegisterElement(m_pStyleNameLabel, i_row,0,1,1);
    //
    m_pComboStyle = new QComboBox(m_pParentFrame);
    //"windows", "motif", "cde", "plastique", "windowsxp", or "macintosh"
    const unsigned int ui_styles_amount = sizeof ( g_str_ar_APP_STYLES ) / sizeof (QString);
    //
   unsigned int ui_selected_style_index = 0;
   //
    for (unsigned int ui_num = 0; ui_num < ui_styles_amount; ui_num++)
    {
        m_pComboStyle->addItem( g_str_ar_APP_STYLES[ui_num] );
        //
        if (g_str_ar_APP_STYLES[ui_num].localeAwareCompare ( m_strAppStyleName ) == 0)
            ui_selected_style_index = ui_num;
    };
    //
    m_pComboStyle->setCurrentIndex(ui_selected_style_index);
    //
    addAndRegisterElement(m_pComboStyle, i_row,1,1,2);
    //-----------------
    i_row++;
    m_pLblNormalTreeElement = new QLabel ("Normal node");
    m_pLblNormalTreeElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblNormalTreeElement, i_row,0,1,1);
    //
    m_pBtnChange_B_NormalTreeELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnChange_B_NormalTreeELement, i_row,1,1,1);
    //
    m_pBtnChange_F_NormalTreeELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnChange_F_NormalTreeELement, i_row,2,1,1);
    //-----------------
    i_row++;
    m_pLblDelTreeElement = new QLabel ("Deleted node");
    m_pLblDelTreeElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblDelTreeElement, i_row,0,1,1);
    //
    m_pBtnDel_B_NormalTreeELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnDel_B_NormalTreeELement, i_row,1,1,1);
    //
    m_pBtnDel_F_NormalTreeELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnDel_F_NormalTreeELement, i_row,2,1,1);
    //-----------------------------------------
    i_row++;
    m_pLblNormalAttachmentElement = new QLabel ("Normal attachment");
    m_pLblNormalAttachmentElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblNormalAttachmentElement, i_row,0,1,1);
    //
    m_pBtnChange_B_AttachmentELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnChange_B_AttachmentELement, i_row,1,1,1);
    //
    m_pBtnChange_F_AttachmentELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnChange_F_AttachmentELement, i_row,2,1,1);
    //-----------------------------------------
    i_row++;
    m_pLblDeletedAttachmentElement = new QLabel ("Deleted attachment");
    m_pLblDeletedAttachmentElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblDeletedAttachmentElement, i_row,0,1,1);
    //
    m_pBtnDeleted_B_AttachmentELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnDeleted_B_AttachmentELement, i_row,1,1,1);
    //
    m_pBtnDeleted_F_AttachmentELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnDeleted_F_AttachmentELement, i_row,2,1,1);
    //-----------------------------------------
    i_row++;
    m_pLblUpdatingAttachmentElement = new QLabel ("Attachment during update");
    m_pLblUpdatingAttachmentElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblUpdatingAttachmentElement, i_row,0,1,1);
    //
    m_pBtnUpdating_B_AttachmentELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnUpdating_B_AttachmentELement, i_row,1,1,1);
    //
    m_pBtnUpdating_F_AttachmentELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnUpdating_F_AttachmentELement, i_row,2,1,1);
    //-----------------------------------------
    i_row++;
    m_pLblFailAttachmentElement = new QLabel ("Failed attachment");
    m_pLblFailAttachmentElement->setAlignment(Qt::AlignCenter);
    addAndRegisterElement(m_pLblFailAttachmentElement, i_row,0,1,1);
    //
    m_pBtnFail_B_AttachmentELement = new QPushButton(str_back_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnFail_B_AttachmentELement, i_row,1,1,1);
    //
    m_pBtnFail_F_AttachmentELement = new QPushButton(str_front_label,  m_pParentFrame);
    addAndRegisterElement(m_pBtnFail_F_AttachmentELement, i_row,2,1,1);
};

void ColorGraficElements::writeData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    settings.setValue(g_str_CLR_NORNAL_FG, m_cl_NormalTreeElementFront);
    settings.setValue(g_str_CLR_NORNAL_BG, m_cl_NormalTreeElementBack);
    //
    settings.setValue(g_str_CLR_DELETED_FG, m_cl_DelTreeElementFront);
    settings.setValue(g_str_CLR_DELETED_BG, m_cl_DelTreeElementBack);
    //
    settings.setValue(g_str_CLR_ATTACH_NORNAL_FG, m_cl_NormalAttachmentElementFront);
    settings.setValue(g_str_CLR_ATTACH_NORNAL_BG, m_cl_NormalAttachmentElementBack);
    //
    settings.setValue(g_str_CLR_ATTACH_DELETED_FG, m_cl_DelAttachmentElementFront);
    settings.setValue(g_str_CLR_ATTACH_DELETED_BG, m_cl_DelAttachmentElementBack);
    //
    settings.setValue(g_str_CLR_ATTACH_UPDATING_FG, m_cl_UpdatingAttachmentElementFront );
    settings.setValue(g_str_CLR_ATTACH_UPDATING_BG, m_cl_UpdatingAttachmentElementBack );
    //
    settings.setValue(g_str_CLR_ATTACH_FAIL_FG, m_cl_FailAttachmentElementFront );
    settings.setValue(g_str_CLR_ATTACH_FAIL_BG, m_cl_FailAttachmentElementBack );
    //
    m_bChanged = false;
};

void ColorGraficElements::readData ()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    const QColor    white_color(255,255,255);
    const QColor    black_color(0,0,0);
    //
    QColor          actual_color;
    //
    actual_color = settings.value(g_str_CLR_NORNAL_FG).value<QColor>();
    m_cl_NormalTreeElementFront = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_NORNAL_BG).value<QColor>();
    m_cl_NormalTreeElementBack  = actual_color.isValid() ? actual_color : white_color;
    //
    actual_color = settings.value(g_str_CLR_DELETED_FG).value<QColor>();
    m_cl_DelTreeElementFront = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_DELETED_BG).value<QColor>();
    m_cl_DelTreeElementBack = actual_color.isValid() ? actual_color : white_color;
    //
    actual_color = settings.value(g_str_CLR_ATTACH_NORNAL_FG).value<QColor>();
    m_cl_NormalAttachmentElementFront   = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_ATTACH_NORNAL_BG).value<QColor>();
    m_cl_NormalAttachmentElementBack    = actual_color.isValid() ? actual_color : white_color;
    //
    actual_color = settings.value(g_str_CLR_ATTACH_DELETED_FG).value<QColor>();
    m_cl_DelAttachmentElementFront      = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_ATTACH_DELETED_BG).value<QColor>();
    m_cl_DelAttachmentElementBack       = actual_color.isValid() ? actual_color : white_color;
    //
    actual_color = settings.value(g_str_CLR_ATTACH_UPDATING_FG).value<QColor>();
    m_cl_UpdatingAttachmentElementFront = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_ATTACH_UPDATING_BG).value<QColor>();
    m_cl_UpdatingAttachmentElementBack  = actual_color.isValid() ? actual_color : white_color;
    //
    actual_color = settings.value(g_str_CLR_ATTACH_FAIL_FG).value<QColor>();
    m_cl_FailAttachmentElementFront     = actual_color.isValid() ? actual_color : black_color;
    actual_color = settings.value(g_str_CLR_ATTACH_FAIL_BG).value<QColor>();
    m_cl_FailAttachmentElementBack      = actual_color.isValid() ? actual_color : white_color;
}

void ColorGraficElements::updateData (bool b_from_dialog, bool b_data_changed)
{
    if(b_from_dialog)
    {


    }else
    {
        //
        //set color of label depends of the stored settings
        //
        const QString str_begin_style = "QLabel { ";
        const QString str_end_style   = "; }";
        //
        const QString str_back_type = "background-color :";
        const QString str_front_type = "color :";

        m_pLblNormalTreeElement->setStyleSheet(str_begin_style                                  +
                                               str_back_type                                    +
                                               m_cl_NormalTreeElementBack.name()   +
                                               "; "                                             +
                                               str_front_type                                   +
                                               m_cl_NormalTreeElementFront.name()  +
                                               str_end_style
                                              );
        //

        m_pLblDelTreeElement->setStyleSheet(str_begin_style                                  +
                                            str_back_type                                    +
                                            m_cl_DelTreeElementBack.name()      +
                                            "; "                                             +
                                            str_front_type                                   +
                                            m_cl_DelTreeElementFront.name()     +
                                            str_end_style
                                           );
        //
        m_pLblNormalAttachmentElement->setStyleSheet(str_begin_style                                        +
                                                     str_back_type                                          +
                                                     m_cl_NormalAttachmentElementBack.name()   +
                                                     "; "                                                   +
                                                     str_front_type                                         +
                                                     m_cl_NormalAttachmentElementFront.name()  +
                                                     str_end_style
                                                    );
        //
        m_pLblDeletedAttachmentElement->setStyleSheet(str_begin_style                                        +
                                                      str_back_type                                          +
                                                      m_cl_DelAttachmentElementBack.name()      +
                                                      "; "                                                   +
                                                      str_front_type                                         +
                                                      m_cl_DelAttachmentElementFront.name()     +
                                                      str_end_style);
        //
        m_pLblUpdatingAttachmentElement->setStyleSheet(str_begin_style                                        +
                                                       str_back_type                                          +
                                                       m_cl_UpdatingAttachmentElementBack.name()      +
                                                       "; "                                                   +
                                                       str_front_type                                         +
                                                       m_cl_UpdatingAttachmentElementFront.name()     +
                                                       str_end_style);
        //
        m_pLblFailAttachmentElement->setStyleSheet(str_begin_style                                        +
                                                   str_back_type                                          +
                                                   m_cl_FailAttachmentElementBack.name()      +
                                                   "; "                                                   +
                                                   str_front_type                                         +
                                                   m_cl_FailAttachmentElementFront.name()     +
                                                   str_end_style);

    };
    //
    if (b_data_changed)
        m_bChanged = true;
};
