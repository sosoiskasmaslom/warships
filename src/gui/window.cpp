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
    populateField(left, false);

    QFrame *mid = new QFrame(this);
    mid->setFrameShape(QFrame::StyledPanel);

    auto *midLayout = new QVBoxLayout(mid);
    midLayout->setContentsMargins(10,10,10,10);
    midLayout->setSpacing(0);

    // Верхняя пустота
    midLayout->addStretch(1);

    // Лейбл
    midLabel = new QLabel("Press a cell", mid);
    midLabel->setAlignment(Qt::AlignCenter);
    midLabel->setWordWrap(true);
    midLayout->addWidget(midLabel, 0, Qt::AlignHCenter);

    // Нижняя пустота
    midLayout->addStretch(1);

    // Разделительная линия
    QFrame *line = new QFrame(mid);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    midLayout->addWidget(line);

    // Кнопка снизу
    loseButton = new QPushButton("Get Lose", mid);
    loseButton->setFixedHeight(60);
    loseButton->setStyleSheet("font-size: 18px; padding: 6px;");
    midLayout->addSpacing(100);
    midLayout->addWidget(loseButton);

    connect(loseButton, &QPushButton::clicked, this, [this]() {
        emit losePressed();
    });

    QFrame *rightFrame = makeFieldFrame();
    populateField(rightFrame, true); 

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
                    QString coord = QString("%1%2").arg(c).arg(r);
                    midLabel->setText(coord);
                    // qDebug() << "Clicked:" << coord;
                    emit cellClicked(coord);
                });
            } else {
                btn->setEnabled(false); // делаем кнопку некликабельной
            }

            grid->addWidget(btn, r, c);
            buttons[r][c] = btn;
        }
    }

    if (clickable) {
        rightButtons = buttons;   // кликабельное → правое
    } else {
        leftButtons = buttons;    // некликабельное → левое
    }
}

void Window::setMiddleText(const std::string& text)
{
    if (midLabel)
        midLabel->setText(QString::fromStdString(text));
}

void Window::setCellColor(Point coord, bool rightField)
{
    if (coord[0] < 0 || coord[0] >= 10 ||
        coord[1] < 0 || coord[1] >= 10)
        return;

    QPushButton *btn = nullptr;

    if (rightField)
        btn = rightButtons[coord[0]][coord[1]];
    else
        btn = leftButtons[coord[0]][coord[1]];

    if (btn) {
        btn->setStyleSheet("background:red; border:1px solid #7aa7d9;");
    }
}
