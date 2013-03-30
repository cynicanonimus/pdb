#ifndef HTMLSYNTAXHIGHLIGHTER_H
#define HTMLSYNTAXHIGHLIGHTER_H
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
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class HTMLSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    Q_OBJECT

public:
    HTMLSyntaxHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);


private:

    struct HighlightingRule
    {
        QRegExp         pattern;
        QTextCharFormat format;
    };


private:
    QTextCharFormat             bracketFormat;
    QVector<HighlightingRule>   highlightingRules;
};

#endif // HTMLSYNTAXHIGHLIGHTER_H
