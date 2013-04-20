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
#include "htmlsyntaxhighlighter.h"

HTMLSyntaxHighlighter::HTMLSyntaxHighlighter(QTextDocument* parent)
        : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    //
    bracketFormat.setForeground(Qt::darkRed);
    bracketFormat.setFontWeight(QFont::Bold);
    //
    QStringList bracketPatterns;
    bracketPatterns <<  "/>"<< ">"<< "<"<< "</";

    foreach (const QString &pattern, bracketPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = bracketFormat;
        highlightingRules.append(rule);
    };

}

void HTMLSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        //
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        };
    };
}
