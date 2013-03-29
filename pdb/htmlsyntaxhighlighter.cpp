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
