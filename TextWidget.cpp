#include "TextWidget.h"
#include <QDebug>

TextWidget::TextWidget(QWidget *parent) : QWidget(parent)
{

    createWidgets();
    createConnections();
}

void TextWidget::createWidgets(){

    setLayout(&m_mainVLayout);

    m_mainVLayout.addWidget(&m_textEdit);
    m_mainVLayout.addWidget(&m_findWidget);
    m_textHighlighter.setDocument(m_textEdit.document());

    m_findWidget.setLayout(&m_findWidgetHLayout);
    m_findWidgetHLayout.addWidget(&m_findTextLineEdit);
    m_findWidgetHLayout.addWidget(&m_nextButton);
    m_findWidgetHLayout.addWidget(&m_previousButton);

    m_nextButton.setText("Next");
    m_previousButton.setText("Previous");
    m_findTextLineEdit.setPlaceholderText("Find text in summary...");

    m_findWidget.setVisible(false);

}

void TextWidget::createConnections(){

    connect(&m_findTextLineEdit, &QLineEdit::textChanged, this, TextWidget::highlightText,     Qt::DirectConnection );
    connect(&m_textEdit,         &QTextEdit::textChanged, this, TextWidget::highlightText,     Qt::DirectConnection );
    connect(&m_nextButton,       &QPushButton::clicked,   this, TextWidget::goToNextHighlight, Qt::DirectConnection );
    connect(&m_previousButton,   &QPushButton::clicked,   this, TextWidget::goToPrevHighlight, Qt::DirectConnection );

}

void TextWidget::toggleFindWidget(bool visible){

    m_findWidget.setVisible(visible);
    highlightText();

    if(visible){
        m_findTextLineEdit.setFocus();
    }

}

void TextWidget::keyPressEvent(QKeyEvent* event){

    bool ctrlModKeyPressed = event->modifiers() & Qt::ControlModifier;
    switch(event->key()){
        case Qt::Key_F:
            toggleFindWidget(ctrlModKeyPressed);
            break;
        case Qt::Key_Escape:
            toggleFindWidget(false);
            break;
        default:
            break;
    }

    QWidget::keyPressEvent(event);

}

void TextWidget::highlightText()
{

    if(m_findWidget.isVisible()){
        m_textHighlighter.setWordPattern(m_findTextLineEdit.text());
    }else{
        m_textHighlighter.setWordPattern("");
    }

    // For some reason rehighlighting from the context of the QTextEdit itself
    // will cause a recursive infinite loop...
    if(sender() != &m_textEdit){
        m_textHighlighter.rehighlight();
    }

}

void TextWidget::TextEditRefreshHighlighter(int cursorIndex){
    m_textEdit.setFocus();
    if(cursorIndex >= 0){
        QTextCursor currentCursor = m_textEdit.textCursor();
        currentCursor.setPosition(cursorIndex);
        m_textEdit.setTextCursor(currentCursor);
    }
}

void TextWidget::goToNextHighlight()
{
    int cursorIndex = m_textHighlighter.setNextBlockStateCurrent();
    TextEditRefreshHighlighter(cursorIndex);
}

void TextWidget::goToPrevHighlight()
{
    int cursorIndex = m_textHighlighter.setPrevBlockStateCurrent();
    TextEditRefreshHighlighter(cursorIndex);
}
