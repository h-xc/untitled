import QtQuick 2.0

Item {
    Text {
        id: text1
        x: 48
        y: 98
        width: 24
        height: 11
        text: qsTr("Text")
        font.pixelSize: 12
    }

    TextEdit {
        id: textEdit1
        x: 20
        y: 165
        width: 80
        height: 20
        text: qsTr("1458454165")
        font.pixelSize: 12
    }

    GridView {
        id: gridView1
        x: 170
        y: 118
        width: 140
        height: 140
        delegate: Item {
            x: 5
            height: 50
            Column {
                spacing: 5
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        cellWidth: 70
        cellHeight: 70
    }

}

