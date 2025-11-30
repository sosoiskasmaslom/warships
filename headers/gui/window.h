
#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include "point.h"

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget* parent = nullptr);

    // Функция для перекраски клетки в поле
    void setCellColor(Point coord, bool rightField);
    void setMiddleText(const std::string& text);

signals:
    void cellClicked(const QString& coord);
    void losePressed();


protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QLabel *midLabel;
    QVector<QVector<QPushButton*>> leftButtons;
    QVector<QVector<QPushButton*>> rightButtons;
    QPushButton* loseButton;

    void setupUI();
    QFrame* makeFieldFrame();
    void populateField(QFrame* frame, bool clickable = true);
};
