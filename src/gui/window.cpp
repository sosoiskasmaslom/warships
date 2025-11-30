// window.cpp
#include "gui/window.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>
#include <QDebug>

Window::Window(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Warships GUI");
    setFixedSize(1200, 600);
    setupUI();
}

void Window::closeEvent(QCloseEvent *event) 
{ event->accept(); }

void Window::setupUI() 
{
    auto *hlay = new QHBoxLayout(this);

    QFrame *left = makeFieldFrame();
    populateField(left, true);

    QFrame *mid = new QFrame(this);
    mid->setFrameShape(QFrame::StyledPanel);
    auto *midLayout = new QHBoxLayout(mid);
    midLabel = new QLabel("Press a cell", mid);
    midLabel->setAlignment(Qt::AlignCenter);
    midLabel->setWordWrap(true);
    midLayout->addWidget(midLabel);

    rightFrame = makeFieldFrame();
    populateField(rightFrame, false); // правый фрейм некликабельный

    hlay->addWidget(left, 1);
    hlay->addWidget(mid, 1);
    hlay->addWidget(rightFrame, 1);
}

QFrame* Window::makeFieldFrame() 
{
    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    return frame;
}

void Window::populateField(QFrame* frame, bool clickable) 
{
    auto *grid = new QGridLayout(frame);
    grid->setSpacing(2);
    grid->setContentsMargins(6,6,6,6);

    const int N = 10;
    QVector<QVector<QPushButton*>> buttons(N, QVector<QPushButton*>(N, nullptr));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            QPushButton *btn = new QPushButton(frame);
            btn->setFixedSize(40, 40);
            btn->setStyleSheet("background:#cfe8ff; border:1px solid #7aa7d9;");

            if (clickable) {
                connect(btn, &QPushButton::clicked, this, [this, r, c]() {
                    QString coord = QString("%1%2").arg(r).arg(c);
                    midLabel->setText(coord);
                    qDebug() << "Clicked:" << coord;
                    emit cellClicked(coord);
                });
            } else {
                btn->setEnabled(false); // делаем кнопку некликабельной
            }

            grid->addWidget(btn, r, c);
            buttons[r][c] = btn;
        }
    }

    if (!clickable) {
        rightButtons = buttons; // сохраняем кнопки правого поля
    }
}

void Window::setRightCellColor(int row, int col, const QColor& color) 
{
    // row и col от 1 до 10
    if (row < 1 || row > 10 || col < 1 || col > 10) return;
    QPushButton *btn = rightButtons[row-1][col-1];
    if (btn) {
        btn->setStyleSheet(QString("background:%1; border:1px solid #7aa7d9;").arg(color.name()));
    }
}
