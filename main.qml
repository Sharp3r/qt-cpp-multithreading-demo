import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material

ApplicationWindow {
    id: root

    width: 600
    height: 420
    visible: true
    title: "Threading Demo — Trainee"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        // Header
        Rectangle {
            Layout.fillWidth: true
            height: 44
            radius: 8
            color: colorPalette.lvlHeader
            border.color: colorPalette.lvlHeaderBorder
            border.width: 1

            Text {
                anchors.centerIn: parent
                font.pixelSize: 15
                font.weight: Font.Medium
                color: colorPalette.lvlHeaderText
                text: "Trainee — QThread subclass"
            }
        }

        // Enter field
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TextField {
                id: fileInput

                Layout.fillWidth: true
                placeholderText: "Шлях до файлу, напр. test.txt"
                text: "test.txt"
                font.pixelSize: 14
                enabled: !controller.running
            }

            Button {
                text: controller.running ? "Stop" : "Start"
                font.pixelSize: 13

                background: Rectangle {
                    radius: 6
                    color: controller.running ? colorPalette.controllerStop :
                                                colorPalette.controllerStart
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    if (controller.running)
                        controller.stopProcessing()
                    else
                        controller.startProcessing(fileInput.text)
                }
            }
        }

        // Progress Bar
        Rectangle {
            Layout.fillWidth: true
            height: 8
            radius: 4
            color: colorPalette.border

            Rectangle {
                width: parent.width * (controller.progress / 100)
                height: parent.height
                radius: 4
                color: colorPalette.accent

                Behavior on width {
                    NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
                }
            }
        }

        Text {
            font.pixelSize: 13
            color: colorPalette.controllerProgress
            text: controller.progress + "%"
        }

        // Status
        Rectangle {
            Layout.fillWidth: true
            height: 48
            radius: 8
            color: colorPalette.bgPanel
            border.color: colorPalette.border
            border.width: 1

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 14
                }
                font.pixelSize: 13
                color: colorPalette.controllerStatusText
                elide: Text.ElideRight
                text: controller.status
            }
        }

        // Explanation Text
        Rectangle {
            Layout.fillWidth: true
            height: explanationText.implicitHeight + 24
            radius: 8
            color: colorPalette.explBg
            border.color: colorPalette.explBorder
            border.width: 1

            Text {
                id: explanationText

                anchors.fill: parent
                anchors.margins: 12
                font.pixelSize: 12
                color: colorPalette.explText
                wrapMode: Text.WordWrap
                lineHeight: 1.5
                text: "Механізм: WorkerThread::run() виконується в окремому потоці.\n" +
                      "Q_PROPERTY + NOTIFY → QML binding оновлюється автоматично.\n" +
                      "atomic<bool> m_stopRequested — thread-safe флаг скасування."
            }
        }

        Item { Layout.fillHeight: true }
    }

    QtObject {
        id: colorPalette

        readonly property color accent: "#1D9E75"
        readonly property color bgPanel: "#F8F8F6"
        readonly property color border: "#E0E0DC"
        readonly property color lvlHeader: "#E1F5EE"
        readonly property color lvlHeaderBorder: "#9FE1CB"
        readonly property color lvlHeaderText: "#085041"
        readonly property color controllerStop: "#F0997B"
        readonly property color controllerStart: "#1D9E75"
        readonly property color controllerProgress: "#5F5E5A"
        readonly property color controllerStatusText: "#444441"
        readonly property color explBg: "#FAEEDA"
        readonly property color explBorder: "#FAC775"
        readonly property color explText: "#633806"
    }
}
