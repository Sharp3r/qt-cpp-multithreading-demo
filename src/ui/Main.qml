import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material
import ThreadingDemo 1.0

ApplicationWindow {
    id: root

    width: 640
    height: 560
    visible: true
    title: "Threading Demo — Junior"

    function addFile() {
        if (fileInput.text.trim() !== "") {
            controller.addFile(fileInput.text)
            fileInput.clear()
        }
    }

    ThreadController {
        id: controller
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        // Заголовок
        Rectangle {
            Layout.fillWidth: true
            Layout.minimumWidth: 420
            height: 44
            radius: 8
            color: colorPalette.headerColor
            border.color: colorPalette.headerBorderColor
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "Junior — Worker object + QMutex + QAbstractListModel"
                font.pixelSize: 14
                font.weight: Font.Medium
                color: colorPalette.headerTextColor
            }
        }

        // Додавання файлів
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: fileInput

                Layout.fillWidth: true
                placeholderText: "File path..."
                text: "test.txt"
                font.pixelSize: 13
                onAccepted: addFile()
            }

            Button {
                text: "Add"
                enabled: !controller.running
                onClicked: addFile()
            }
        }

        // Кнопки управління
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Button {
                text: "Start All"
                enabled: !controller.running
                onClicked: controller.startAll()
            }

            Button {
                text: "Stop"
                enabled: controller.running
                onClicked: controller.stopAll()
            }

            Button {
                text: "Clear Ready Files"
                enabled: !controller.running
                onClicked: controller.clearCompleted()
            }
        }

        // Список задач — підключений до TaskModel через model: controller.taskModel
        ListView {
            id: taskList

            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 6
            clip: true
            model: controller.taskModel   // ← QAbstractListModel з C++

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }

            delegate: TaskCard {}         // ← наш компонент картки

            // Плавна поява нових елементів
            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
                NumberAnimation { property: "y"; from: -20; duration: 100 }
            }

            Text {
                anchors.centerIn: parent
                visible: taskList.count === 0
                text: "Add files to process"
                color: colorPalette.listTextColor
                font.pixelSize: 13
            }
        }

        // Статус бар
        Rectangle {
            Layout.fillWidth: true
            height: 36
            radius: 8
            color: colorPalette.statusBarColor
            border.color: colorPalette.statusBarBorderColor
            border.width: 1

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: 12
                font.pixelSize: 12
                color: colorPalette.statusBarTextColor
                text: controller.status
            }
        }
    }

    QtObject {
        id: colorPalette

        readonly property color headerColor: "#E6F1FB"
        readonly property color headerBorderColor: "#B5D4F4"
        readonly property color headerTextColor: "#0C447C"
        readonly property color listTextColor: "#888780"
        readonly property color statusBarColor: "#F1EFE8"
        readonly property color statusBarBorderColor: "#D3D1C7"
        readonly property color statusBarTextColor: "#5F5E5A"

    }
}
