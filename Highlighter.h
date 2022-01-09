#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QMap>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter{
    Q_OBJECT

public:
    Highlighter(QTextDocument* parent = nullptr);
    void setWordPattern(const QString& word);
    void highlightBlock(const QString &text);
    int setNextBlockStateCurrent();
    int setPrevBlockStateCurrent();

private:
struct HighlightingRule{
    QRegularExpression pattern;
    QTextCharFormat format;
};
    QString m_currentText;
    HighlightingRule m_defaultHighlightingRule;
    HighlightingRule m_activeHighlightingRule;
    QRegularExpressionMatchIterator m_matches;
    QMap<int, QRegularExpressionMatch> m_matchMap;
    int m_currentMatchIndex;
    int m_current_total_matches;
};
#endif // HIGHLIGHTER_H
