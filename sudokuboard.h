#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QSettings>

class SudokuBoard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> board READ board NOTIFY boardChanged)
    Q_PROPERTY(QVector<bool> fixedCells READ fixedCells NOTIFY fixedCellsChanged)
    Q_PROPERTY(QVector<bool> conflictCells READ conflictCells NOTIFY conflictsChanged)

public:
    enum Difficulty {
        debutant = 0,
        facile = 1,
        intermediaire = 2,
        expert = 3
    };
    Q_ENUM(Difficulty)

    explicit SudokuBoard(QObject *parent = nullptr);

    QVector<int> board() const;
    QVector<bool> fixedCells() const;

    Q_INVOKABLE bool setCellValue(int index, int value);
    Q_INVOKABLE void generateNewPuzzle(int difficulty = 0);
    Q_INVOKABLE bool checkSolution();
    Q_INVOKABLE void saveGame();
    Q_INVOKABLE bool loadGame();
    Q_INVOKABLE QVector<bool> conflictCells() const;

signals:
    void boardChanged();
    void fixedCellsChanged();
    void puzzleSolved();
    void invalidMove();
    void solutionIncorrect(const QString& message);
    void gameLoaded(bool success);
    void conflictsChanged();

private:
    bool isValidPlacement(int row, int col, int num) const;
    void clearBoard();
    void loadPuzzleData();
    void removeNumbers(int cellsToRemove);

    QVector<int> m_board;
    QVector<bool> m_fixedCells;
    QMap<int, QPair<QVector<int>, QVector<int>>> m_puzzleData;
    int m_currentDifficulty;

    QVector<bool> m_conflictCells;
    void updateConflictCells();
};

#endif // SUDOKUBOARD_H
