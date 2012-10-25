import QtQuick 1.1

Item{
    anchors.fill:parent
    function trackCount() {
        return headerView.trackCount();
    }
    Component.onCompleted: {
        console.log("track view loaded")
        trackModel.prepareToAddTrack.connect(prepareToAddTrack);
    }
    function checkTrackCount(){
        headerView.checkTrackCount();
    }
    function prepareToAddTrack() {
        console.log("prepareToAddTrack");
        var h =  Math.max((headerView.trackCount()+1)*80, parent.height);
        headerView.setContentHeight(h)
        arrangementView.setContentHeight(h)
    }
    function addTrack() {
        trackModel.addTrack()
        //console.log(headerView.trackCount())
    }
    function updateDbMeter(track,l,r) {
        headerView.updateDbMeter(track,l,r)
    }
    function insertNote(draggedNote){
        console.log("Reached")
        arrangementView.insertNote(draggedNote)
    }
    function setCurrentPos(bar,beat,beatPos) {
        arrangementView.setCurrentPos(bar,beat,beatPos)
    }
    function setLoopPos(lStart,lEnd) {
        arrangementView.setLoopPos(lStart,lEnd)
    }

    function setBeatBarCount(beatInBar,unitInBeat)
    {
        arrangementView.setBeatBarCount(beatInBar,unitInBeat);
    }

    function resetInterface()
    {
        arrangementView.resetInterface()
        headerView.resetInterface()
    }

    TrackArrangementView{
        id:arrangementView
        x:200
        anchors{
            left:headerView.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }
    Rectangle{
        x:0
        width:200
        height:10
        y:0
        color:"#313131"
    }
    TrackHeaderView{
        id:headerView
        x:0
        width:200
        height:parent.height
        onContentYChanged: {
            arrangementView.setContentY(contentY)
        }
        anchors{
            top: parent.top
            bottom: parent.bottom
            left:parent.left
        }
    }
}
