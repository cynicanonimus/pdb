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
#include "searchrequest.h"
#include <QRegExp>

SearchRequest::SearchRequest(QObject *parent) :
    QObject(parent)
{
    reset();
}

void SearchRequest::reset()
{
    m_bCaseSensitive            = false;
    m_bEnableMinMatch           = false;
    //
    m_bWholeWord                = false;
    m_bIsRegularExp             = false;
    m_bCurrentTreeOnly          = false;
    //
    m_bSearchInNodeNames        = false;
    m_bSearchInNodeDescriptor   = false;
    m_bSearchInAttachmentNames  = false;
    m_bSearchInAtttachmentBody  = false;
    //
    m_strSearchIt=tr("");
    //
    m_bIsRegularExp             = false;
    //
    m_bUseBeginSearch           = false;
    m_bUseEndSearch             = false;
}

const SearchRequest& SearchRequest::operator=(const SearchRequest&o)
{
    this->m_bCaseSensitive              = o.m_bCaseSensitive;
    this->m_bWholeWord                  = o.m_bWholeWord;
    this->m_bIsRegularExp               = o.m_bIsRegularExp;
    this->m_bCurrentTreeOnly            = o.m_bCurrentTreeOnly;
    //
    this->m_bSearchInNodeNames          = o.m_bSearchInNodeNames;
    this->m_bSearchInNodeDescriptor     = o.m_bSearchInNodeDescriptor;
    this->m_bSearchInAttachmentNames    = o.m_bSearchInAttachmentNames;
    this->m_bSearchInAtttachmentBody    = o.m_bSearchInAtttachmentBody;
    this->m_strSearchIt                 = o.m_strSearchIt;
    //
    this->m_dtBeginSearch               = o.m_dtBeginSearch;
    this->m_bUseBeginSearch             = o.m_bUseBeginSearch;
    //
    this->m_enRegExpSyntax              = o.m_enRegExpSyntax;
    //
    this->m_dtEndSearch                 = o.m_dtEndSearch;
    this->m_bUseEndSearch               = o.m_bUseEndSearch;
    return (*this);
}


void SearchRequest::setBeginSearchDateTime      (const QDateTime& dt_begin)
{
    m_dtBeginSearch = dt_begin;
    m_bUseBeginSearch = true;
}

const QDateTime*    SearchRequest::getBeginSearchDateTime () const
{
    if (m_bUseBeginSearch)
        return &m_dtBeginSearch;
    //
    return NULL;
}
void SearchRequest::dropBeginSearchDateTime ()
{
    m_bUseBeginSearch = false;
}

void SearchRequest::setEndSearchDateTime (const QDateTime& dt_begin)
{
    m_dtEndSearch = dt_begin;
    m_bUseEndSearch = true;
}

const QDateTime* SearchRequest::getEndSearchDateTime () const
{
    if (m_bUseEndSearch)
        return &m_dtEndSearch;
    //
    return NULL;
}

void SearchRequest::dropEndSearchDateTime       ()
{
    m_bUseEndSearch = false;
}
//-----------------------------------------------------------------------------


bool SearchRequest::isSearchAreaDefined() const
{
    if (m_bSearchInNodeNames || m_bSearchInNodeDescriptor || m_bSearchInAttachmentNames || m_bSearchInAtttachmentBody)
        return true;
    //
    return false;
}

Qt::CaseSensitivity  SearchRequest::getCaseSensitivity() const
{

    if (m_bCaseSensitive)
        return Qt::CaseSensitive;
    else
        return Qt::CaseInsensitive;
};

bool SearchRequest::isRegExpValid () const
{
    if (false == m_bIsRegularExp)
        return true; //nothing to check, this is not regexp
    //
    QRegExp rx(m_strSearchIt);
    //
    rx.setMinimal(m_bEnableMinMatch);
    rx.setPatternSyntax(m_enRegExpSyntax);
    //
    if ( rx.isValid() )
        return true;
    //
    return false;
}

void  SearchRequest::setRegExpSyntax (QRegExp::PatternSyntax en_syntax)
{
    m_enRegExpSyntax = en_syntax;
    m_bIsRegularExp = true;
}
