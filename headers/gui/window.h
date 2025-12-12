// window.h
#pragma once
#include <QWidget>

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVector>

#include "point.h"

//
// ==== Base window ====
class Window : public QWidget
{
    Q_OBJECT
public:
    // Конструктор базового окна
    explicit Window(QWidget* parent = nullptr);

protected:
    // Обрабатывает закрытие окна
    void closeEvent(QCloseEvent *event) override;
};


//
// ==== GameWindow ====
// — игровое окно с двумя полями 
class GameWindow : public Window
{
    Q_OBJECT
public:
    // Создаёт игровое окно
    explicit GameWindow(QWidget* parent = nullptr);
    void setMiddleText(const std::string& text);
    
public slots:
    // Меняет цвет клетки на левом или правом поле (invokable via Qt)
    void setCellColor(Point coord, bool rightField, const QString& color);
    void setCellColor(Point coord, bool rightField);
    // Enable or disable clicking on the right field
    void setRightClickable(bool enabled);

signals:
    // Сигнал при нажатии на клетку (правое поле)
    void cellClicked(const Point& coord);

    // Сигнал при нажатии кнопки "Lose"
    void losePressed();

private:
    QLabel* midLabel = nullptr;                 // центральный текст
    QPushButton* loseButton = nullptr;          // кнопка снизу
    QVector<QVector<QPushButton*>> leftButtons; // левое поле
    QVector<QVector<QPushButton*>> rightButtons;// правое поле

    // Создаёт всё расположение UI
    void setupUI();

    // Делает рамку под игровое поле
    QFrame* makeFieldFrame();

    // Заполняет рамку сеткой кнопок-клеток
    void populateField(QFrame* frame, bool clickable);
};


// ==== DragDropWindow — окно расстановки кораблей ====
class DragDropWindow : public Window
{
    Q_OBJECT
public:
    // Создаёт окно с полем слева и кораблями справа
    explicit DragDropWindow(QWidget *parent = nullptr);

signals:
    // Emitted when the user finishes placing ships and confirms
    void shipsPlaced();

public:
    // Returns placed ships as tuples {length, y, x, vertical}
    std::vector<std::vector<unsigned>> getPlacedShips() const;

public slots:
    // Validate current placement and enable/disable Done button
    void validatePlacement();

protected:
    QVector<int> ships = {4,4,3,3,2,2,2,2};
    // Перехватывает события виджетов кораблей (начало драга)
    bool eventFilter(QObject *obj, QEvent *event) override;

    // Разрешает перетаскивание внутрь области
    void dragEnterEvent(QDragEnterEvent *event) override;

    // Следит за тем, куда движется драг
    void dragMoveEvent(QDragMoveEvent *event) override;

    // Обрабатывает сброс корабля на поле
    void dropEvent(QDropEvent *event) override;

    // Переключает ориентацию корабля (вертикально ↔ горизонтально)
    void keyPressEvent(QKeyEvent *event) override;

private:
    // Создаёт левое и правое окно + раскладывает их
    void createLayout();

    // Создаёт левую часть — поле 10×10
    QWidget* makeLeftFieldFrame();

    // Создаёт правую часть — кораблики
    QWidget* makeRightShipsFrame();

    // Создаёт сетку клеток внутри левого фрейма
    void populateField(QWidget *frame);

    // Создаёт модель корабля заданного размера
    void createShipWidget(QWidget *parent, int size, int id);

    // Преобразует координаты дропа в индекс клетки
    bool widgetToCell(QWidget* w, int &row, int &col) const;

    // Убирает подсветку всех ячеек
    void clearHighlight();

    // Подсвечивает возможную позицию корабля при драге
    void highlightPlacement(int row, int col, int size, bool vertical, bool ok);

private:
    QWidget *leftFrame = nullptr;               // поле слева
    QWidget *rightFrame = nullptr;              // корабли справа

    QVector<QVector<QPushButton*>> fieldButtons;// клетки поля 10×10
    QVector<QLabel*> shipWidgets;               // графические корабли

    bool currentVertical = false;               // ориентация корабля
    int draggingSize = 0;                       // размер корабля в текущем перетаскивании

    bool placementConfirmed = false;            // whether auto-confirm was emitted
};

class WinWindow : public Window
{
    Q_OBJECT
public:
    explicit WinWindow(QWidget* parent = nullptr);
    // Set displayed message (winner name / text)
    void setMessage(const QString& msg);

private:
    QLabel* label = nullptr;
};