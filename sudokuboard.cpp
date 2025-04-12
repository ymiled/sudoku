#include <QTime>
#include <algorithm>
#include <random>
#include "sudokuboard.h"
#include <iostream>
using namespace std;

SudokuBoard::SudokuBoard(QObject *parent) : QObject(parent), m_board(81, -1), m_fixedCells(81, false), m_conflictCells(81, false)
{
    srand(QTime::currentTime().msec());
    loadPuzzleData();
    generateNewPuzzle(debutant); // débutant par défaut
}

void SudokuBoard::loadPuzzleData() {
    // Débutant
    QVector<int> debutantIndices = {1, 5, 17, 25, 26, 28, 29, 31, 34, 36, 38, 41, 44, 45, 47, 48, 53, 55, 56, 57, 68, 70, 73, 78, 80};
    QVector<int> debutantValues =  {9, 4,  8,  3,  6,  7,  6,  3,  4,  9,  3,  7,  2,  2,  8,  9,  5,  8,  7,  4,  8,  2,  2,  7,  9};

    // Facile
    QVector<int> facileIndices = {0, 3, 4, 5, 7, 13, 15, 16, 24, 26, 27, 30, 35, 37, 38, 41, 45, 46, 49, 50, 52, 53, 66, 69, 70, 71, 77};
    QVector<int> facileValues =  {4, 2, 7, 5, 8,  1,  4,  7,  9,  5,  7,  3,  4,  2,  1,  4,  8,  3,  9,  1,  5,  2,  8,  5,  9,  7,  2};

    // Intermediaire
    QVector<int> intermediaireIndices = {2, 3, 7, 12, 14, 16, 19, 21, 24, 25, 30, 34, 35, 37, 40, 45, 55, 56, 63, 64, 68, 72, 74, 76, 78};
    QVector<int> intermediaireValues =  {4, 8, 1,  9,  1,  2,  6,  2,  4,  3,  3,  8,  9,  1,  7,  7,  4,  2,  3,  9,  8,  6,  1,  2,  3};

    // Expert
    QVector<int> expertIndices = {1, 2, 13, 23, 24, 25, 26, 32, 34, 36, 38, 42, 43, 49, 54, 56, 60, 66, 67, 69, 70, 76, 80};
    QVector<int> expertValues =  {4, 6,  7,  2,  7,  9,  1,  6,  7,  6,  1,  4,  2,  9,  2,  4,  8,  2,  1,  5,  6,  5,  9};

    m_puzzleData[debutant] = qMakePair(debutantIndices, debutantValues);
    m_puzzleData[facile] = qMakePair(facileIndices, facileValues);
    m_puzzleData[intermediaire] = qMakePair(intermediaireIndices, intermediaireValues);
    m_puzzleData[expert] = qMakePair(expertIndices, expertValues);

}
void SudokuBoard::generateNewPuzzle(int difficulty) {
    clearBoard(); // Déjà présent, qui remet tout à zéro
    m_currentDifficulty = difficulty;

    if (!m_puzzleData.contains(difficulty)) {
        qWarning() << "Invalid difficulty level:" << difficulty;
        difficulty = debutant;
    }

    auto puzzle = m_puzzleData[difficulty];
    QVector<int> indices = puzzle.first;
    QVector<int> values = puzzle.second;

    for (int i = 0; i < indices.size(); ++i) {
        int index = indices[i];
        int value = values[i];
        m_board[index] = value;
        m_fixedCells[index] = true;
    }

    // Réinitialiser les conflits explicitement
    m_conflictCells.fill(false); // <-- AJOUT
    emit conflictsChanged(); // <-- AJOUT

    emit boardChanged();
    emit fixedCellsChanged();
}


QVector<int> SudokuBoard::board() const {
    return m_board;
}

QVector<bool> SudokuBoard::fixedCells() const {
    return m_fixedCells;
}

bool SudokuBoard::setCellValue(int index, int value) {
    if (m_fixedCells[index]) {
        emit invalidMove();
        return false;
    }

    if (value < -1 || value > 9) return false;

    m_board[index] = value;
    updateConflictCells();
    emit boardChanged();
    return true;
}



bool SudokuBoard::isValidPlacement(int row, int col, int num) const {
    int index = row * 9 + col;

    // cellule fixée ?
    if (m_fixedCells[index] && m_board[index] != num) {
        return false;
    }

    // lignes
    for (int c = 0; c < 9; c++) {
        if (c != col && m_board[row * 9 + c] == num) return false;
    }

    // collonnes
    for (int r = 0; r < 9; r++) {
        if (r != row && m_board[r * 9 + col] == num) return false;
    }

    // 3x3 box
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int r = boxRow; r < boxRow + 3; r++) {
        for (int c = boxCol; c < boxCol + 3; c++) {
            if (r != row && c != col && m_board[r * 9 + c] == num) return false;
        }
    }

    return true;
}

void SudokuBoard::removeNumbers(int cellsToRemove) {
    m_board.fill(-1, 81);
    m_fixedCells.fill(false, 81);
}

bool SudokuBoard::checkSolution() {
    for (int i = 0; i < 81; ++i) {
        if (m_board[i] == -1) {
            emit solutionIncorrect(tr("Il y a encore des cases vides !"));
            return false;
        }
    }

    for (int i = 0; i < 9; ++i) {
        QSet<int> rowSet, colSet, boxSet;

        for (int j = 0; j < 9; ++j) {
            // lignes :
            int rowVal = m_board[i * 9 + j];
            if (rowSet.contains(rowVal)) {
                emit solutionIncorrect(tr("Doublon à la ligne %1").arg(i+1));
                return false;
            }
            rowSet.insert(rowVal);

            // colonnes :
            int colVal = m_board[j * 9 + i];
            if (colSet.contains(colVal)) {
                emit solutionIncorrect(tr("Doublon à la colonne %1").arg(i+1));
                return false;
            }
            colSet.insert(colVal);

            // vérification des box :
            int boxRow = 3 * (i / 3) + (j / 3);
            int boxCol = 3 * (i % 3) + (j % 3);
            int boxVal = m_board[boxRow * 9 + boxCol];
            if (boxSet.contains(boxVal)) {
                emit solutionIncorrect(tr("Doublon dans le box à (%1,%2)").arg(boxRow+1).arg(boxCol+1));
                return false;
            }
            boxSet.insert(boxVal);
        }
    }


    emit puzzleSolved();
    return true;
}

void SudokuBoard::clearBoard() {
    m_board.fill(-1, 81);
    m_fixedCells.fill(false, 81);
    m_conflictCells.fill(false, 81); // S'assurer que les conflits sont réinitialisés

    // Émettre tous les signaux nécessaires
    emit boardChanged();
    emit fixedCellsChanged();
    emit conflictsChanged();
}


void SudokuBoard::saveGame() {
    QSettings settings;

    // Sauvegarder le tableau actuel
    QVariantList boardList;
    for (int value : m_board) {
        boardList << value;
    }
    settings.setValue("savedGame/board", boardList);

    // Sauvegarder les cellules fixes
    QVariantList fixedCellsList;
    for (bool fixed : m_fixedCells) {
        fixedCellsList << fixed;
    }
    settings.setValue("savedGame/fixedCells", fixedCellsList);

    QVariantList conflictCellsList;
    for (bool conflict : m_conflictCells) {
        conflictCellsList << conflict;
    }
    settings.setValue("savedGame/conflictCells", conflictCellsList);

    // Sauvegarder la difficulté
    settings.setValue("savedGame/difficulty", m_currentDifficulty);

}
bool SudokuBoard::loadGame() {
    QSettings settings;

    if (!settings.contains("savedGame/board")) {
        emit gameLoaded(false);
        return false;
    }

    // Charger le tableau
    QVariantList boardList = settings.value("savedGame/board").toList();
    for (int i = 0; i < boardList.size() && i < 81; ++i) {
        m_board[i] = boardList[i].toInt();
    }

    // Charger les cellules fixes
    QVariantList fixedCellsList = settings.value("savedGame/fixedCells").toList();
    for (int i = 0; i < fixedCellsList.size() && i < 81; ++i) {
        m_fixedCells[i] = fixedCellsList[i].toBool();
    }

    if (settings.contains("savedGame/conflictCells")) {
        QVariantList conflictCellsList = settings.value("savedGame/conflictCells").toList();
        for (int i = 0; i < conflictCellsList.size() && i < 81; ++i) {
            m_conflictCells[i] = conflictCellsList[i].toBool();
        }
    } else {
        m_conflictCells.fill(false);
    }

    // Charger la difficulté
    m_currentDifficulty = settings.value("savedGame/difficulty", 0).toInt();

    // Recalculer les conflits après chargement
    updateConflictCells(); // <-- AJOUT IMPORTANT

    emit boardChanged();
    emit fixedCellsChanged();
    emit conflictsChanged(); // <-- Émettre le signal des conflits
    emit gameLoaded(true);
    return true;
}

void SudokuBoard::updateConflictCells() {
    QVector<bool> newConflicts(81, false);

    for (int i = 0; i < 81; ++i) {
        if (m_board[i] == -1 || m_fixedCells[i]) continue;

        int row = i / 9;
        int col = i % 9;
        int value = m_board[i];

        // Check row and column
        for (int j = 0; j < 9; ++j) {
            if (j != col && m_board[row * 9 + j] == value) {
                newConflicts[i] = true;
                newConflicts[row * 9 + j] = true;
            }
            if (j != row && m_board[j * 9 + col] == value) {
                newConflicts[i] = true;
                newConflicts[j * 9 + col] = true;
            }
        }

        // Check 3x3 box
        int boxRow = row - row % 3;
        int boxCol = col - col % 3;
        for (int r = boxRow; r < boxRow + 3; ++r) {
            for (int c = boxCol; c < boxCol + 3; ++c) {
                int idx = r * 9 + c;
                if (idx != i && m_board[idx] == value) {
                    newConflicts[i] = true;
                    newConflicts[idx] = true;
                }
            }
        }
    }

    // Only update and emit if something changed
    if (m_conflictCells != newConflicts) {
        m_conflictCells = newConflicts;

        for (int i = 0; i < 81; ++i) {
            if (m_conflictCells[i]) {
                int row = i / 9 + 1;
                int col = i % 9 + 1;
            }
        }

        emit conflictsChanged();  // This triggers the QML update
    }
}


QVector<bool> SudokuBoard::conflictCells() const {
    return m_conflictCells;
}

