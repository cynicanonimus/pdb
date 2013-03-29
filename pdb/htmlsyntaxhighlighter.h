#ifndef HTMLSYNTAXHIGHLIGHTER_H
#define HTMLSYNTAXHIGHLIGHTER_H

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
