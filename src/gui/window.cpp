// window.cpp
#include "gui/window.h"
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <vector>

Window::Window(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Warships GUI");
    setFixedSize(1200, 600);
}

void Window::closeEvent(QCloseEvent *event)
{
    event->accept();
}

GameWindow::GameWindow(QWidget* parent)
    : Window(parent)
{ setupUI(); }

void GameWindow::setupUI()
{
    auto *hlay = new QHBoxLayout(this);

    QFrame *left = makeFieldFrame();
    populateField(left, false);

    QFrame *mid = new QFrame(this);
    mid->setFrameShape(QFrame::StyledPanel);

    auto *midLayout = new QVBoxLayout(mid);
    midLayout->setContentsMargins(10,10,10,10);
    midLayout->setSpacing(0);

    midLayout->addStretch(1);

    midLabel = new QLabel("Press a cell", mid);
    midLabel->setAlignment(Qt::AlignCenter);
    midLabel->setWordWrap(true);
    midLayout->addWidget(midLabel, 0, Qt::AlignHCenter);

    midLayout->addStretch(1);

    QFrame *line = new QFrame(mid);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    midLayout->addWidget(line);

    loseButton = new QPushButton("Get Lose", mid);
    loseButton->setFixedHeight(60);
    loseButton->setStyleSheet("font-size: 18px; padding: 6px;");
    midLayout->addSpacing(100);
    midLayout->addWidget(loseButton);

    // Done button is used in drag/drop window only; leave here if needed

    connect(loseButton, &QPushButton::clicked, this, [this]() {
        emit losePressed();
    });

    QFrame *rightFrame = makeFieldFrame();
    populateField(rightFrame, true);

    hlay->addWidget(left, 1);
    hlay->addWidget(mid, 1);
    hlay->addWidget(rightFrame, 1);
}

QFrame* GameWindow::makeFieldFrame()
{
    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    return frame;
}

void GameWindow::populateField(QFrame* frame, bool clickable)
{
    auto *grid = new QGridLayout(frame);
    grid->setSpacing(2);
    grid->setContentsMargins(6,6,6,6);

    const int N = 10;
    QVector<QVector<QPushButton*>> buttons(N, QVector<QPushButton*>(N));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            QPushButton* btn = new QPushButton(frame);
            btn->setFixedSize(40, 40);
            btn->setStyleSheet("background:#cfe8ff; border:1px solid #7aa7d9;");

            if (clickable) {
                connect(btn, &QPushButton::clicked, this, [this, r, c]() {
                    QString coord = QString("%1%2").arg(r).arg(c);
                    emit cellClicked(Point(coord.toStdString()));
                });
            } else {
                btn->setEnabled(false);
            }

            grid->addWidget(btn, r, c);
            buttons[r][c] = btn;
        }
    }

    if (clickable) rightButtons = buttons;
    else leftButtons = buttons;
}

void GameWindow::setMiddleText(const std::string& text)
{
    if (midLabel)
        midLabel->setText(QString::fromStdString(text));
}

void GameWindow::setCellColor(Point coord, bool rightField, const QString& color)
{
    if (coord[0] < 0 || coord[0] >= 10 ||
        coord[1] < 0 || coord[1] >= 10)
        return;

    QPushButton* btn = rightField ?
        rightButtons[coord[1]][coord[0]] :
        leftButtons[coord[1]][coord[0]];

    if (btn)
        btn->setStyleSheet(
            QString("background:%1; border:1px solid #7aa7d9;").arg(color)
        );
}

void GameWindow::setCellColor(Point coord, bool rightField)
{ setCellColor(coord, rightField, "red"); }

void GameWindow::setRightClickable(bool enabled)
{
    for (int r = 0; r < rightButtons.size(); ++r) {
        for (int c = 0; c < rightButtons[r].size(); ++c) {
            if (rightButtons[r][c])
                rightButtons[r][c]->setEnabled(enabled);
        }
    }
}


DragDropWindow::DragDropWindow(QWidget *parent)
    : Window(parent)
{
    setAcceptDrops(true);
    setWindowTitle("Ship placement");
    setFixedSize(1200, 600);

    createLayout();
}

void DragDropWindow::createLayout()
{
    auto *main = new QHBoxLayout(this);
    main->setContentsMargins(6,6,6,6);
    main->setSpacing(8);

    leftFrame = makeLeftFieldFrame();
    rightFrame = makeRightShipsFrame();

    leftFrame->setFixedWidth(width() / 2 - 20);
    rightFrame->setMinimumWidth(300);

    main->addWidget(leftFrame);
    main->addWidget(rightFrame);
    setLayout(main);

    doneButton = new QPushButton("Done", this);
    doneButton->setFixedHeight(40);
    doneButton->setEnabled(false);
    connect(doneButton, &QPushButton::clicked, this, [this]() {
        emit shipsPlaced();
        this->close();
    });
    if (this->layout()) this->layout()->addWidget(doneButton);
}

QWidget* DragDropWindow::makeLeftFieldFrame()
{
    QWidget *frame = new QWidget(this);
    frame->setObjectName("leftField");
    frame->setAcceptDrops(true);

    auto *grid = new QGridLayout(frame);
    grid->setSpacing(4);
    grid->setContentsMargins(8,8,8,8);

    const int N = 10;
    fieldButtons = QVector<QVector<QPushButton*>>(N, QVector<QPushButton*>(N, nullptr));

    for (int r=0; r<N; ++r) {
        for (int c=0; c<N; ++c) {
            QPushButton *b = new QPushButton(frame);
            b->setFixedSize(46, 46);
            b->setStyleSheet("background:#cfe8ff; border:1px solid #7aa7d9;");
            b->setAcceptDrops(true);
            grid->addWidget(b, r, c);
            fieldButtons[r][c] = b;
        }
    }

    frame->setLayout(grid);
    return frame;
}

QWidget* DragDropWindow::makeRightShipsFrame()
{
    QWidget *frame = new QWidget(this);
    frame->setObjectName("rightShips");
    auto *vl = new QVBoxLayout(frame);
    vl->setContentsMargins(10,10,10,10);
    vl->setSpacing(12);

    QLabel *title = new QLabel("Ships (drag to place). Press R to rotate.", frame);
    vl->addWidget(title);

    int id = 0;
    for (int s : ships) {
        createShipWidget(frame, s, id++);
    }

    vl->addStretch(1);
    frame->setLayout(vl);
    return frame;
}

void DragDropWindow::createShipWidget(QWidget *parent, int size, int id)
{
    QLabel *ship = new QLabel(QString("Ship %1").arg(size), parent);
    ship->setFixedSize(size*40, 40);
    ship->setAlignment(Qt::AlignCenter);
    ship->setStyleSheet("background:#666; color:white; border:2px solid black; border-radius:4px;");
    ship->setProperty("shipSize", size);
    ship->setObjectName(QString("ship_%1").arg(id));
    ship->setFocusPolicy(Qt::StrongFocus);

    // ship will start drag when clicked -> install eventFilter on this window
    ship->installEventFilter(this);

    shipWidgets.push_back(ship);
    // add to right frame layout (parent has layout)
    if (parent->layout()) parent->layout()->addWidget(ship);
}

// Перехват событий корабля: mouse press => старт drag
bool DragDropWindow::eventFilter(QObject *obj, QEvent *event)
{
    QLabel *ship = qobject_cast<QLabel*>(obj);
    if (!ship)
        return QWidget::eventFilter(obj, event);

    // ----- MOUSE PRESS: start drag -----
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        if (me->button() != Qt::LeftButton)
            return false;

        int size = ship->property("shipSize").toInt();
        bool vertical = ship->property("vertical").toBool();

        // --- Rotate ship if needed ---
        if (vertical != currentVertical)
        {
            vertical = currentVertical;
            ship->setProperty("vertical", vertical);

            if (vertical)
                ship->setFixedSize(40, size * 40);
            else
                ship->setFixedSize(size * 40, 40);

            ship->updateGeometry();
            ship->update();
        }

        draggingSize = size;

        // ---- Start the drag ----
        QMimeData *mime = new QMimeData();
        mime->setText(QString::number(size));
        mime->setData("application/x-ship-vertical", currentVertical ? "1" : "0");

        QDrag *drag = new QDrag(ship);
        drag->setMimeData(mime);

        QPixmap pm(ship->size());
        pm.fill(Qt::darkGray);
        drag->setPixmap(pm);
        drag->exec(Qt::MoveAction);
        return true;
    }

    return QWidget::eventFilter(obj, event);
}


void DragDropWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R) {
        currentVertical = !currentVertical;
        // visual feedback: briefly change cursor or title
        QString t = currentVertical ? "Vertical" : "Horizontal";
        qDebug() << "Rotation now:" << t;
    }
    QWidget::keyPressEvent(event);
}

// Drag enter: accept if we have ship text
void DragDropWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
        event->acceptProposedAction();
}

// drag move: highlight potential placement on left field
void DragDropWindow::dragMoveEvent(QDragMoveEvent *event)
{
    // only handle when cursor over leftFrame or its children
    QPoint posWindow = event->position().toPoint();
    QWidget *w = childAt(posWindow);
    if (!w) {
        clearHighlight();
        return;
    }

    // find which cell (row,col)
    int row=-1, col=-1;
    if (!widgetToCell(w, row, col)) {
        clearHighlight();
        return;
    }

    // read mime
    int size = 0;
    bool vert = currentVertical;
    if (event->mimeData()->hasText())
        size = event->mimeData()->text().toInt();
    if (event->mimeData()->hasFormat("application/x-ship-vertical")) {
        QByteArray b = event->mimeData()->data("application/x-ship-vertical");
        if (!b.isEmpty()) vert = (b[0] == '1');
    }

    // check fit
    bool ok = true;
    for (int i=0;i<size;i++) {
        int rr = row + (vert ? i : 0);
        int cc = col + (vert ? 0 : i);
        if (rr<0||rr>=10||cc<0||cc>=10) { ok = false; break; }
        QString css = fieldButtons[rr][cc]->styleSheet();
        if (css.contains("#444")) { ok = false; break; } // already occupied
    }
    // additionally: ships must not touch other ships (no adjacent cells, including diagonals)
    if (ok) {
        // collect target cells
        QVector<QPair<int,int>> targets;
        for (int i=0;i<size;i++) {
            int rr = row + (vert ? i : 0);
            int cc = col + (vert ? 0 : i);
            targets.append(qMakePair(rr, cc));
        }

        auto isTarget = [&](int r, int c){
            for (auto &p : targets) if (p.first==r && p.second==c) return true;
            return false;
        };

        for (auto &p : targets) {
            for (int dr=-1; dr<=1; ++dr) {
                for (int dc=-1; dc<=1; ++dc) {
                    int nr = p.first + dr;
                    int nc = p.second + dc;
                    if (nr<0||nr>=10||nc<0||nc>=10) continue;
                    if (isTarget(nr,nc)) continue; // neighbor is part of this ship
                    QString css = fieldButtons[nr][nc]->styleSheet();
                    if (css.contains("#444")) { ok = false; break; }
                }
                if (!ok) break;
            }
            if (!ok) break;
        }
    }

    highlightPlacement(row, col, size, vert, ok);
    event->acceptProposedAction();
}

// drop: place ship
void DragDropWindow::dropEvent(QDropEvent *event)
{
    QPoint posWindow = event->position().toPoint();
    QWidget *w = childAt(posWindow);
    if (!w) return;

    int row=-1, col=-1;
    if (!widgetToCell(w, row, col)) return;

    int size = 0;
    bool vert = currentVertical;
    if (event->mimeData()->hasText())
        size = event->mimeData()->text().toInt();
    if (event->mimeData()->hasFormat("application/x-ship-vertical")) {
        QByteArray b = event->mimeData()->data("application/x-ship-vertical");
        if (!b.isEmpty()) vert = (b[0] == '1');
    }

    // verify
    if (size <= 0) return;
    for (int i=0;i<size;i++) {
        int rr = row + (vert ? i : 0);
        int cc = col + (vert ? 0 : i);
        if (rr<0||rr>=10||cc<0||cc>=10) return; // out of bounds
        QString css = fieldButtons[rr][cc]->styleSheet();
        if (css.contains("#444")) return; // overlap
    }

    // adjacency check: ensure no adjacent occupied cells (including diagonals)
    QVector<QPair<int,int>> targets;
    for (int i=0;i<size;i++) {
        int rr = row + (vert ? i : 0);
        int cc = col + (vert ? 0 : i);
        targets.append(qMakePair(rr, cc));
    }
    auto isTarget = [&](int r, int c){ for (auto &p: targets) if (p.first==r && p.second==c) return true; return false; };
    for (auto &p : targets) {
        for (int dr=-1; dr<=1; ++dr) {
            for (int dc=-1; dc<=1; ++dc) {
                int nr = p.first + dr;
                int nc = p.second + dc;
                if (nr<0||nr>=10||nc<0||nc>=10) continue;
                if (isTarget(nr,nc)) continue;
                QString css = fieldButtons[nr][nc]->styleSheet();
                if (css.contains("#444")) return; // adjacent to existing ship
            }
        }
    }

    // place
    for (int i=0;i<size;i++) {
        int rr = row + (vert ? i : 0);
        int cc = col + (vert ? 0 : i);
        fieldButtons[rr][cc]->setStyleSheet("background:#444; border:2px solid black;");
    }

    clearHighlight();
    event->acceptProposedAction();

    // If the drag source was one of the ship widgets, mark it as placed
    QObject* src = event->source();
    QLabel* ship = qobject_cast<QLabel*>(src);
    if (ship) {
        ship->setEnabled(false);
        ship->hide();
    }

    // After every drop, re-validate the placement and enable/disable Done
    validatePlacement();

    // After every drop, re-validate the placement and enable/disable Done
    validatePlacement();
}

std::vector<std::vector<unsigned>> DragDropWindow::getPlacedShips() const
{
    std::vector<std::vector<unsigned>> out;
    int N = fieldButtons.size();
    if (N == 0) return out;
    int M = fieldButtons[0].size();
    std::vector<std::vector<bool>> seen(N, std::vector<bool>(M, false));

    auto isOccupied = [&](int r, int c)->bool{
        if (r<0||r>=N||c<0||c>=M) return false;
        auto btn = fieldButtons[r][c];
        if (!btn) return false;
        QString css = btn->styleSheet();
        return css.contains("#444");
    };

    for (int r=0;r<N;++r) {
        for (int c=0;c<M;++c) {
            if (seen[r][c]) continue;
            if (!isOccupied(r,c)) continue;

            // determine orientation: check right neighbor
            bool horiz = isOccupied(r, c+1);
            bool vert = isOccupied(r+1, c);

            if (horiz && vert) {
                // ambiguous but treat as single cell (fallback)
                seen[r][c] = true;
                out.push_back({1u, (unsigned)r, (unsigned)c, 0u});
                continue;
            }

            if (horiz) {
                int cc = c;
                while (cc < M && isOccupied(r, cc)) { seen[r][cc] = true; ++cc; }
                unsigned len = cc - c;
                out.push_back({len, (unsigned)r, (unsigned)c, 0u});
            } else if (vert) {
                int rr = r;
                while (rr < N && isOccupied(rr, c)) { seen[rr][c] = true; ++rr; }
                unsigned len = rr - r;
                out.push_back({len, (unsigned)r, (unsigned)c, 1u});
            } else {
                // single cell
                seen[r][c] = true;
                out.push_back({1u, (unsigned)r, (unsigned)c, 0u});
            }
        }
    }

    return out;
}

void DragDropWindow::validatePlacement()
{
    if (!doneButton) return;

    auto placed = getPlacedShips();

    int cnt4 = 0, cnt3 = 0, cnt2 = 0;
    for (auto &t : placed) {
        if (t.size() < 1) continue;
        unsigned len = t[0];
        if (len == 4) ++cnt4;
        else if (len == 3) ++cnt3;
        else if (len == 2) ++cnt2;
        else {
            // ignore or treat as invalid
        }
    }

    bool ok = ((int)placed.size() == ships.size());
    doneButton->setEnabled(ok);
}

// helper: given a child widget (maybe the button), find its (row,col) in fieldButtons
bool DragDropWindow::widgetToCell(QWidget *w, int &row, int &col) const
{
    for (int r=0;r<fieldButtons.size();++r) {
        for (int c=0;c<fieldButtons[r].size();++c) {
            if (fieldButtons[r][c] == w || fieldButtons[r][c]->isAncestorOf(w)) {
                row = r; col = c; return true;
            }
        }
    }
    return false;
}

void DragDropWindow::clearHighlight()
{
    for (int r=0;r<fieldButtons.size();++r)
        for (int c=0;c<fieldButtons[r].size();++c) {
            QString css = fieldButtons[r][c]->styleSheet();
            // do not override already placed ships (#444)
            if (!css.contains("#444"))
                fieldButtons[r][c]->setStyleSheet("background:#cfe8ff; border:1px solid #7aa7d9;");
        }
}

void DragDropWindow::highlightPlacement(int row, int col, int size, bool vertical, bool ok)
{
    clearHighlight();
    QColor color = ok ? QColor(0,255,0,80) : QColor(255,0,0,120);
    QString rgba = color.name(QColor::HexArgb);

    for (int i=0;i<size;i++) {
        int rr = row + (vertical ? i : 0);
        int cc = col + (vertical ? 0 : i);
        if (rr<0||rr>=10||cc<0||cc>=10) continue;
        // don't override placed ships
        if (fieldButtons[rr][cc]->styleSheet().contains("#444")) continue;
        fieldButtons[rr][cc]->setStyleSheet(
            QString("background:%1; border:1px solid black;").arg(rgba)
        );
    }
}


WinWindow::WinWindow(QWidget* parent)
    : Window(parent)
{
    this->setWindowTitle("Victory!");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    label = new QLabel(QString::fromUtf8(u8"хуй вин!!"), this);
    label->setAlignment(Qt::AlignCenter);

    QFont f = label->font();
    f.setPointSize(22);
    f.setBold(true);
    label->setFont(f);

    layout->addWidget(label);

    this->setLayout(layout);
    this->setFixedSize(300, 150);
}
