// window.h
#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget* parent = nullptr);

    // Функция для перекраски клетки в правом поле
    void setRightCellColor(int row, int col, const QColor& color);

signals:
    void cellClicked(const QString& coord);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QLabel *midLabel;
    QFrame *rightFrame; // сохраним правый фрейм
    QVector<QVector<QPushButton*>> rightButtons; // кнопки правого поля

    void setupUI();
    QFrame* makeFieldFrame();
    void populateField(QFrame* frame, bool clickable = true);
};
