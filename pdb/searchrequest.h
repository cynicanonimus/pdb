#ifndef SEARCHREQUEST_H
#define SEARCHREQUEST_H
//searchrequest.h
#include <QObject>
#include <QRegExp>
/*
serachrequest contains parameter of the search. instance of this class passed from search dialg to the search thread.
*/
#include <QDateTime>

class SearchRequest : public QObject
{
    Q_OBJECT
public:
    explicit SearchRequest(QObject *parent = 0);
    //
    void  setCaseSensitive                          (bool flag = true)              { m_bCaseSensitive = flag;   }
    bool isCaseSensitive                            () const                        { return m_bCaseSensitive;   }
    //
    void  setEnableMinMatching                      (bool flag = true)              { m_bEnableMinMatch = flag;   }
    bool isEnableMinMatching                        () const                        { return m_bEnableMinMatch;   }
    //
    void setWholeWord                               (bool flag = true)              { m_bWholeWord = flag;       }
    bool isWholeWord                                () const                        { return m_bWholeWord;       }
    //
    void dropRegularExp                             ()                              { m_bIsRegularExp = false;   }
    bool isRegularExp                               () const                        { return m_bIsRegularExp;    }
    //
    void setCurrentTreeOnly                         (bool flag = true)              { m_bCurrentTreeOnly = flag; }
    bool isCurrentTreeOnly                          () const                        { return m_bCurrentTreeOnly; }
    //-----------------------------------------------------------------------------
    void setSearchPhrase                            (const QString& str_search)     { m_strSearchIt = str_search;}
    const QString&  getSearchPhrase                 () const                 { return m_strSearchIt;      }
    //-----------------------------------------------------------------------------
    void setSearchInNodeNames                       (bool flag = true)              { m_bSearchInNodeNames = flag;       }
    bool isSearchInNodeNames                        () const                        { return m_bSearchInNodeNames;       }
    //
    void setSearchInNodeDescriptor                  (bool flag = true)              { m_bSearchInNodeDescriptor = flag;  }
    bool isSearchInNodeDescriptor                   () const                        { return m_bSearchInNodeDescriptor;  }
    //
    void setSearchInAttachmentNames                 (bool flag = true)              { m_bSearchInAttachmentNames = flag; }
    bool isSearchInAttachmentNames                  () const                        { return m_bSearchInAttachmentNames; }
    //
    void setSearchInAtttachmentBody                 (bool flag = true)              { m_bSearchInAtttachmentBody = flag; }
    bool isSearchInAtttachmentBody                  () const                        { return m_bSearchInAtttachmentBody; }
    //-----------------------------------------------------------------------------
    bool isSearchAreaDefined                        () const;
    //-----------------------------------------------------------------------------
    void setRegExpSyntax                            (QRegExp::PatternSyntax en_syntax);
    QRegExp::PatternSyntax getRegExpSyntax          () const                            { return m_enRegExpSyntax; }
    //-----------------------------------------------------------------------------
    void                setBeginSearchDateTime      (const QDateTime& dt_begin);
    const QDateTime*    getBeginSearchDateTime      () const;
    void                dropBeginSearchDateTime     ();
    //-----------------------------------------------------------------------------
    void                setEndSearchDateTime        (const QDateTime& dt_begin);
    const QDateTime*    getEndSearchDateTime        () const;
    void                dropEndSearchDateTime       ();
    //-----------------------------------------------------------------------------
    const SearchRequest&  operator=(const SearchRequest&);
    //
    Qt::CaseSensitivity getCaseSensitivity() const;
    //
    bool isRegExpValid              () const;

signals:
    
public slots:
    
   private:
    void reset();
    //
    bool        m_bCaseSensitive;
    bool        m_bEnableMinMatch;
    //
    bool        m_bWholeWord;
    bool        m_bIsRegularExp;
    bool        m_bCurrentTreeOnly;
    //
    bool        m_bSearchInNodeNames;
    bool        m_bSearchInNodeDescriptor;
    bool        m_bSearchInAttachmentNames;
    bool        m_bSearchInAtttachmentBody;
    //
    QDateTime   m_dtBeginSearch;
    bool        m_bUseBeginSearch;
    //
    QDateTime   m_dtEndSearch;
    bool        m_bUseEndSearch;

    //
    QString m_strSearchIt;
    //
    QRegExp::PatternSyntax m_enRegExpSyntax;
};

#endif // SEARCHREQUEST_H
