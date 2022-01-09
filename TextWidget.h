#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "Highlighter.h"

class TextWidget : public QWidget{

    Q_OBJECT

public:

    TextWidget(QWidget* parent = nullptr);

protected:

    void createWidgets();
    void createConnections();
    void toggleFindWidget(bool visible);

protected slots:

    void keyPressEvent(QKeyEvent* event) override;
    void highlightText();
    void goToNextHighlight();
    void goToPrevHighlight();
    void TextEditRefreshHighlighter(int cursorIndex);

protected:

    QVBoxLayout m_mainVLayout;
    QTextEdit   m_textEdit;

    QHBoxLayout m_findWidgetHLayout;
    QWidget     m_findWidget;
    QLineEdit   m_findTextLineEdit;
    QPushButton m_nextButton;
    QPushButton m_previousButton;

    Highlighter m_textHighlighter;

};

#endif // TEXTWIDGET_H
