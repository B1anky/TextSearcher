#include "highlighter.h"
#include <QTextDocument>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    m_defaultHighlightingRule.format.setBackground(Qt::yellow);
    m_defaultHighlightingRule.format.setFontWeight(QFont::Bold);

    m_activeHighlightingRule.format.setBackground(QColor(255, 165, 0));
    m_activeHighlightingRule.format.setFontWeight(QFont::Bold);
}

void Highlighter::setWordPattern(const QString& pattern){
    if(pattern != m_findString){
        m_findString = pattern;
        m_defaultHighlightingRule.pattern = QRegularExpression(pattern, QRegularExpression::PatternOption::CaseInsensitiveOption);
        m_activeMatchIndex = 0;
    }
}

void Highlighter::highlightBlock(const QString &text)
{
    if(m_findString.isEmpty()){
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        m_activeMatchIndex = 0;
    }else{
        m_matches = m_defaultHighlightingRule.pattern.globalMatch(text);
        while(m_matches.hasNext()){
            QRegularExpressionMatch match = m_matches.next();
            if(match.hasPartialMatch() || match.hasMatch()){
                int startOffset = match.capturedStart();
                int endOffset   = match.capturedEnd();
                QTextCharFormat format = m_defaultHighlightingRule.format;
                if( m_activeMatchIndex == m_currentMatchIndex ){
                    format = m_activeHighlightingRule.format;
                }
                setFormat(startOffset, endOffset - startOffset, format);
                m_matchMap[m_currentMatchIndex] = match;
                ++m_currentMatchIndex;
            }
        }
    }
}

void Highlighter::customRehighlight(){
    m_matchMap.clear();
    m_currentMatchIndex = 0;
    rehighlight();
}

int Highlighter::setNextMatchStateActive(){
    int startIndex = -1;
    if(m_activeMatchIndex + 1 < m_matchMap.size()){
        ++m_activeMatchIndex;
        startIndex = m_matchMap[m_activeMatchIndex].capturedEnd();
        customRehighlight();
    }
    return startIndex;
}

int Highlighter::setPrevMatchStateActive(){
    int startIndex = -1;
    if(m_activeMatchIndex - 1 >= 0){
        --m_activeMatchIndex;
        startIndex = m_matchMap[m_activeMatchIndex].capturedEnd();
        customRehighlight();
    }
    return startIndex;
}
