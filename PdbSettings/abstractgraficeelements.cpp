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
#include "abstractgraficeelements.h"

AbstractGraficeElements::AbstractGraficeElements(QGroupBox *ptr_parent_frame, QGridLayout *ptr_layout, QObject *parent) :
    QObject(parent)
{
    m_pParentFrame      = ptr_parent_frame;
    m_pGridLayout       = ptr_layout;
    m_bDelObjectOnExit  = false;
    m_bChanged          = false;
}

AbstractGraficeElements::~AbstractGraficeElements()
{
    if (m_bDelObjectOnExit)
    {
        GraficCollection::const_iterator itr = m_vGraficObjectsArray.begin();
        //
        for (; itr != m_vGraficObjectsArray.end(); ++itr)
        {
            delete (*itr);
        };
    }
};

void AbstractGraficeElements::show (bool b_show)
{
    if (b_show)
    {
        m_pParentFrame->setTitle( m_strFrameName );
        //
    }else
    {
        m_pParentFrame->setTitle( QObject::trUtf8("") );
    };

    GraficCollection::const_iterator itr = m_vGraficObjectsArray.begin();
    //
    for (; itr != m_vGraficObjectsArray.end(); ++itr)
    {
        (*itr)->setVisible(b_show);
    };
};

void AbstractGraficeElements::registerElement(QWidget* ptr_grafic_element)
{
    m_vGraficObjectsArray.push_back(ptr_grafic_element);
};

void AbstractGraficeElements::deleteElementsOnExit (bool b_del)
{
    m_bDelObjectOnExit = b_del;
};

void AbstractGraficeElements::addAndRegisterElement (QWidget* ptr_widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment aligment)
{
    registerElement(ptr_widget);
    m_pGridLayout->addWidget(ptr_widget, row,column,rowSpan,columnSpan, aligment);

};

void AbstractGraficeElements::addAndRegisterElement   (QWidget* ptr_widget, int row, int column, Qt::Alignment aligment)
{
    registerElement(ptr_widget);
    m_pGridLayout->addWidget(ptr_widget, row,column, aligment);
};
