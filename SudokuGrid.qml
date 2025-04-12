import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

GridLayout {
    id: gridLayout
    rows: 9
    columns: 9
    rowSpacing: 0
    columnSpacing: 0

    // Make the grid square and centered
    property int cellSize: Math.min(width, height) / 9
    width: cellSize * 9
    height: cellSize * 9
    anchors.centerIn: parent

    Repeater {
        model: 81
        delegate: Rectangle {
            id: cell
            width: gridLayout.cellSize
            height: gridLayout.cellSize
            color: {
                if (sudokuBoard && sudokuBoard.conflictCells && sudokuBoard.conflictCells[index])
                    return "#ffcccc";
                else if (sudokuBoard && sudokuBoard.fixedCells && sudokuBoard.fixedCells[index])
                    return "#f0f0f0";
                else
                    return "white";
            }
            border.color: "black"
            border.width: 1

            // Top border for the first row
            Rectangle {
                anchors.top: parent.top
                width: parent.width
                height: 3
                color: "black"
                visible: Math.floor(index / 9) === 0
            }

            // Left border for the first column
            Rectangle {
                anchors.left: parent.left
                width: 3
                height: parent.height
                color: "black"
                visible: index % 9 === 0
            }

            // Right thick borders for 3x3 blocks
            Rectangle {
                anchors.right: parent.right
                width: 3
                height: parent.height
                color: "black"
                visible: index % 9 === 2 || index % 9 === 5
            }

            // Bottom thick borders for 3x3 blocks
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 3
                color: "black"
                visible: Math.floor(index / 9) === 2 || Math.floor(index / 9) === 5
            }

            Text {
                id: cellText
                anchors.centerIn: parent
                text: (sudokuBoard && sudokuBoard.board && sudokuBoard.board[index] !== -1) ?
                      sudokuBoard.board[index] : ""
                color: "black" // (sudokuBoard && sudokuBoard.fixedCells && sudokuBoard.fixedCells[index]) ?
                       //"red" : "black"
                font {
                    pixelSize: Math.min(parent.width, parent.height) * 0.5
                    bold: true
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (sudokuBoard && !sudokuBoard.fixedCells[index]) {
                        var current = sudokuBoard.board[index]
                        var newValue = (current === -1) ? 1 : ((current + 1) % 10)
                        if (newValue === 0) newValue = -1
                        sudokuBoard.setCellValue(index, newValue)
                    }
                }
            }
            Connections {
                target: sudokuBoard
                function onConflictsChanged() {
                    if (sudokuBoard != null){
                    cell.color = Qt.binding(function() {
                        return sudokuBoard.conflictCells[index] ? "#ffcccc" :
                            sudokuBoard.fixedCells[index] ? "#f0f0f0" : "white";

                    });
                    }
                }
            }
        }
    }
}
