import QtQuick 2.15
import QtQuick.Layouts 1.15

// Компонент — одна картка задачі у ListView.
// Отримує дані через "model" автоматично від ListView.
Rectangle {
    id: root

    width: ListView.view ? ListView.view.width : 400
    height: 52
    radius: 8
    // Колір залежить від стану задачі (model.state)
    color: {
        switch (model.state) {
            case 0: return colorPalette.bgColorPending  // Pending  — сірий
            case 1: return colorPalette.bgColorRunning  // Running  — синій
            case 2: return colorPalette.bgColorDone     // Done     — зелений
            case 3: return colorPalette.bgColorError    // Error    — червоний
            default: return colorPalette.bgColorPending
        }
    }
    border.width: 1
    border.color: {
        switch (model.state) {
            case 1: return colorPalette.borderColorRunning
            case 2: return colorPalette.borderColorDone
            case 3: return colorPalette.borderColorError
            default: return colorPalette.borderColorPending
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        // Індикатор стану
        Rectangle {
            width: 8
            height: 8
            radius: 4
            color: {
                switch (model.state) {
                    case 1: return colorPalette.indColorRunning
                    case 2: return colorPalette.indColorDone
                    case 3: return colorPalette.indColorError
                    default: return colorPalette.indColorPending
                }
            }
            // Анімація для стану Running
            SequentialAnimation on opacity {
                running: model.state === 1
                loops: Animation.Infinite
                NumberAnimation { to: 0.3; duration: 600 }
                NumberAnimation { to: 1.0; duration: 600 }
            }
        }

        Column {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: model.filename
                font.pixelSize: 13
                font.weight: Font.Medium
                color: colorPalette.filenameTextColor
                elide: Text.ElideMiddle
                width: parent.width
            }

            Text {
                text: model.result || model.stateText
                font.pixelSize: 11
                color: colorPalette.resultTextColor
                elide: Text.ElideRight
                width: parent.width
            }
        }

        Text {
            text: model.stateText
            font.pixelSize: 11
            color: {
                switch (model.state) {
                    case 1: return colorPalette.stateTextColorRunning
                    case 2: return colorPalette.stateTextColorDone
                    case 3: return colorPalette.stateTextColorError
                    default: return colorPalette.stateTextColorPending
                }
            }
        }
    }

    QtObject {
        id: colorPalette

        readonly property color bgColorPending: "#F1EFE8"
        readonly property color bgColorRunning: "#E6F1FB"
        readonly property color bgColorDone: "#E1F5EE"
        readonly property color bgColorError: "#FCEBEB"
        readonly property color borderColorRunning: "#B5D4F4"
        readonly property color borderColorDone: "#9FE1CB"
        readonly property color borderColorError: "#F7C1C1"
        readonly property color borderColorPending: "#D3D1C7"
        readonly property color indColorRunning: "#378ADD"
        readonly property color indColorDone: "#1D9E75"
        readonly property color indColorError: "#E24B4A"
        readonly property color indColorPending: "#888780"
        readonly property color filenameTextColor: "#2C2C2A"
        readonly property color resultTextColor: "#5F5E5A"
        readonly property color stateTextColorRunning: "#185FA5"
        readonly property color stateTextColorDone: "#0F6E56"
        readonly property color stateTextColorError: "#A32D2D"
        readonly property color stateTextColorPending: "#888780"
    }
}