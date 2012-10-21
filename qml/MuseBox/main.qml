// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "TransposeComponents"

Rectangle {
    id: main
    width: 1024
    height: 768
    Transpose{
        id:transpose
        y: 670
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        z: 1
    }
//    MouseArea {
//        anchors.fill: parent
//        onClicked: {
//            bridge.OpenConfigurationDialog()
//        }
//    }
    Item{
        Keys.onSpacePressed: {
                museBox.togglePlayStop();
            }
    }

    Image {
        id: image1
        anchors.fill: parent
        fillMode: Image.Tile
        source: "office.png"
    }

    Timer{
        id:guiTimer
        interval: 30
        running:true
        repeat: true
        onTriggered: {
            var bar = musebox.getBar()
            var beat = musebox.getBeat()
            var min = musebox.getMinute()
            var sec = musebox.getSecond()
            var mil = musebox.getMillisecond()
            var beatPos = musebox.getPositionInBeat()

            transpose.updateTimeAndBeat(min,sec,mil,bar,beat)
            trackView.setCurrentPos(bar,beat,beatPos)

            transpose.updateMasterDbMeter(musebox.masterL(), musebox.masterR())
        }
    }

    Flipable {

        function updateTrackViewCurrentPosition(bar,beat,beatPos)
        {
            trackView.setCurrentPos(bar,beat,beatPos)
        }

        id: trackFlip//Track flip contains track view and mixer
        y: 0
        height: 658
        anchors.right: patternFlip.left
        anchors.rightMargin: 0
        anchors.bottom: transpose.top
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        front: TrackView {
            anchors.fill: parent
            id: trackView
        }
    }

    Flipable {
        id: patternFlip
        x: 744
        y: 0
        width: 280
        height: 658
        anchors.bottom: transpose.top
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        front: Rectangle{
            anchors.fill: parent
        }
    }

    //v--------doesn't work!
    Keys.onPressed: {
        if(event.key === Qt.Key_Space)
        {
            transpose.togglePlay()
            event.accepted = true
        }
    }
}
