#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "sudokuboard.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<SudokuBoard>("Sudoku", 1, 0, "SudokuBoard");

    QQmlApplicationEngine engine;
    SudokuBoard sudokuBoard;
    engine.rootContext()->setContextProperty("sudokuBoard", &sudokuBoard);

    engine.loadFromModule("Sudoku", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
