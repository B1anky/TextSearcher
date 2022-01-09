#include "highlighter.h"
#include <QTextDocument>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    m_defaultHighlightingRule.format.setBackground(Qt::yellow);
    m_defaultHighlightingRule.format.setFontWeight(QFont::Bold);

    m_activeHighlightingRule.format.setBackground(Qt::red);
    m_activeHighlightingRule.format.setFontWeight(QFont::Normal);
}

void Highlighter::setWordPattern(const QString& pattern){
    if(pattern != m_currentText){
        m_currentText = pattern;
        m_defaultHighlightingRule.pattern = QRegularExpression(pattern, QRegularExpression::PatternOption::CaseInsensitiveOption);
        m_currentMatchIndex = 0;
    }
}

void Highlighter::highlightBlock(const QString &text)
{
    m_matchMap.clear();
    m_current_total_matches = 0;
    if(m_currentText.isEmpty() || text.isEmpty()){
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        m_currentMatchIndex = 0;
    }else{
        m_matches = m_defaultHighlightingRule.pattern.globalMatch(text);
        if(!m_matches.hasNext()){
            setFormat(0, document()->toPlainText().length(), QTextCharFormat());
            m_currentMatchIndex = 0;
        }
        int curMatchIndex = 0;
        while(m_matches.hasNext()){
            QRegularExpressionMatch match = m_matches.next();
            if(match.hasPartialMatch() || match.hasMatch()){
                int startOffset = match.capturedStart();
                int endOffset   = match.capturedEnd();
                QTextCharFormat format = m_defaultHighlightingRule.format;
                if( m_currentMatchIndex == curMatchIndex ){
                    format = m_activeHighlightingRule.format;
                }
                setFormat(startOffset, endOffset - startOffset, format);
                m_matchMap[m_current_total_matches] = match;
                ++m_current_total_matches;
                ++curMatchIndex;
            }
        }
        setCurrentBlockState(0);
    }
}


int Highlighter::setNextBlockStateCurrent(){
    int startIndex = -1;
    if(m_currentMatchIndex + 1 < m_current_total_matches){
        ++m_currentMatchIndex;
        startIndex = m_matchMap[m_currentMatchIndex].capturedEnd();
        rehighlight();
    }
    return startIndex;
}

int Highlighter::setPrevBlockStateCurrent(){
    int startIndex = -1;
    if(m_currentMatchIndex - 1 >= 0){
        --m_currentMatchIndex;
        startIndex = m_matchMap[m_currentMatchIndex].capturedEnd();
        rehighlight();
    }
    return startIndex;
}
