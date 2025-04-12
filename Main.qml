import QtQuick 2.15
import QtQuick.Controls 2.15
import Sudoku 1.0

Window {
    visible: true
    width: 480
    height: 520
    minimumWidth: 480
    minimumHeight: 520
    title: "Sudoku Game"
    color: "#f5f5f5"

    property int currentDifficulty: 0
    property real scaleFactor: Math.min(width/480, height/520)

    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"

        // Title
        Text {
            id: title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20 * scaleFactor
            text: "SUDOKU"
            font.pixelSize: 28 * scaleFactor
        }

        // Column labels (C1-C9)
        Row {
            id: columnLabels
            anchors.top: parent.top
            anchors.topMargin: 75 * scaleFactor
            anchors.left: sudokuGrid.left
            spacing: 0
            Repeater {
                model: 9
                Text {
                    width: sudokuGrid.width / 9
                    height: 20 * scaleFactor
                    text: "C" + (index + 1)
                    font.pixelSize: 12 * scaleFactor
                    horizontalAlignment: Text.AlignHCenter
                    color: "#666"
                }
            }
        }

        // Row labels (R1-R9)
        Column {
            id: rowLabels
            anchors.top: sudokuGrid.top
            anchors.topMargin: 21 * scaleFactor
            anchors.right: sudokuGrid.left
            spacing: 0
            Repeater {
                model: 9
                Text {
                    width: 25 * scaleFactor
                    height: sudokuGrid.height / 9
                    text: "R" + (index + 1)
                    font.pixelSize: 12 * scaleFactor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#666"
                }
            }
        }

        // Sudoku Grid
        SudokuGrid {
            id: sudokuGrid
            width: Math.min(parent.width - 100 * scaleFactor, parent.height - 200 * scaleFactor)
            height: width
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -20 * scaleFactor
        }

        // Difficulty Buttons
        Row {
            id: difficultyButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: sudokuGrid.bottom
            anchors.topMargin: 30 * scaleFactor
            spacing: 10 * scaleFactor

            Button {
                text: "Débutant"
                width: 70 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    currentDifficulty = 0
                    sudokuBoard.generateNewPuzzle(0)
                }
                background: Rectangle {
                    color: parent.down ? "#4caf50" : "#81c784"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                text: "Facile"
                width: 70 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    currentDifficulty = 1
                    sudokuBoard.generateNewPuzzle(1)
                }
                background: Rectangle {
                    color: parent.down ? "#4caf50" : "#81c784"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                text: "Intermédiaire"
                width: 110 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    currentDifficulty = 2
                    sudokuBoard.generateNewPuzzle(2)
                }
                background: Rectangle {
                    color: parent.down ? "#4caf50" : "#81c784"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                text: "Expert"
                width: 80 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    currentDifficulty = 3
                    sudokuBoard.generateNewPuzzle(3)
                }
                background: Rectangle {
                    color: parent.down ? "#4caf50" : "#81c784"
                    radius: 5 * scaleFactor
                }
            }
        }

        // Action Buttons
        Row {
            id: actionButtons
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: difficultyButtons.bottom
            anchors.topMargin: 20 * scaleFactor
            spacing: 5 * scaleFactor

            Button {
                text: "Recommencer"
                width: 110 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: sudokuBoard.generateNewPuzzle(currentDifficulty)
                background: Rectangle {
                    color: parent.down ? "#42a5f5" : "#64b5f6"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                id: checkSolutionButton
                text: "Vérifier"
                width: 80 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: sudokuBoard.checkSolution()
                background: Rectangle {
                    color: parent.down ? "#42a5f5" : "#64b5f6"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                text: "Sauvegarder"
                width: 110 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    sudokuBoard.saveGame()
                    solutionText.text = "Partie sauvegardée!"
                    solutionText.color = "#42a5f5"
                    solutionText.visible = true
                    messageTimer.restart()
                }
                background: Rectangle {
                    color: parent.down ? "#ff9800" : "#ffb74d"
                    radius: 5 * scaleFactor
                }
            }

            Button {
                text: "Charger"
                width: 80 * scaleFactor
                height: 20 * scaleFactor
                font.pixelSize: 12 * scaleFactor
                onClicked: {
                    if (!sudokuBoard.loadGame()) {
                        solutionText.text = "Aucune partie sauvegardée!"
                        solutionText.color = "#f44336"
                        solutionText.visible = true
                        messageTimer.restart()
                    }
                }
                background: Rectangle {
                    color: parent.down ? "#ff9800" : "#ffb74d"
                    radius: 5 * scaleFactor
                }
            }
        }

        // Solution Feedback
        Text {
            id: solutionText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: title.bottom
            font.pixelSize: 22 * scaleFactor
            font.bold: true
            visible: false
        }

        Timer {
            id: messageTimer
            interval: 1500
            onTriggered: solutionText.visible = false
        }

        Connections {
            target: sudokuBoard ? sudokuBoard : null

            function onPuzzleSolved() {
                solutionText.text = "Correct! Bien joué!"
                solutionText.color = "#4caf50"
                solutionText.visible = true
                messageTimer.restart()
            }

            function onSolutionIncorrect(message) {
                solutionText.text = message
                solutionText.color = "#f44336"
                solutionText.visible = true
                messageTimer.restart()
            }

            function onGameLoaded(success) {
                if (success) {
                    solutionText.text = "Partie chargée!"
                    solutionText.color = "#4caf50"
                    solutionText.visible = true
                    messageTimer.restart()
                }
            }
        }
    }
}
