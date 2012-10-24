#include "trackmodel.h"
#include "../hardware.h"
#include <QHash>
#include <QDebug>
#include <QByteArray>
#include "pointerconverter.h"

TrackModel::TrackModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";//I want Align plugin from vim, but end up using QtCreator :-(
    roles[RecordingRole] = "recording";
    roles[RecordingChannelsRole] = "channel";
    roles[PatternsRole] = "patterns";
    roles[RecordingSessionRole] = "session";
    roles[PatternArrangementRole] = "arrangement";
    setRoleNames(roles);

    nextTrackID = 1;
}

int TrackModel::rowCount(const QModelIndex& index) const
{
    return trackList.count();
}

QVariant TrackModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > trackList.count())
        return QVariant();

    const Track* track = trackList[index.row()];
    switch(role)
    {
    case NameRole:return track->name;
    case RecordingRole:return (track->recording || track->readyToRecord);
    case RecordingChannelsRole:return QVariant();//TODO set recording channels
    case PatternsRole:return QVariant::fromValue((PatternModel*)(&(track->patternPool)));
    case RecordingSessionRole:return QVariant::fromValue( track->currentRecordingSession );
    case PatternArrangementRole:return QVariant::fromValue((NoteModel*)(&track->arrangement));
    //case PatternArrangementRole:return 0;
    default:return QVariant();
    }
}

bool TrackModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() > trackList.count())
        return false;

    HWLOCK;

    Track* track = trackList[index.row()];
    switch(role)
    {
    case NameRole:
        track->name = value.toString();
        qDebug()<<QString("Changing the name of track #%1 to %2").arg(index.row()).arg(track->name);
        emit dataChanged(index,index);
        return true;
    case RecordingRole:
        track->setRecording(value.toBool());
        emit dataChanged(index,index);
        return true;
    case RecordingChannelsRole:
        return false;//TODO implement this requires understanding of lists in QML. not now
    case PatternsRole:return false;//Do not edit pattern model pointer, shouldn't reach here
    case RecordingSessionRole:return false;//QML cannot edit this. Only passive
    case PatternArrangementRole:return false;//Do not edit pattern group model pointer, shouldn't reach here
    default:return false;
    }
}

bool TrackModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(count!=1)return false;//One shoot one die right on track!
    if(row <0||row >trackList.count())return false;
    beginInsertRows(parent,row,row+count-1);
    Track* track = new Track(2);
    track->model = this;
    int id = nextTrackID++;

    track->name = QString("Track %1").arg(id);
    trackList.insert(row, track);

    HWLOCK;

    Hardware::InstallDevice(track);
    mixerChannelList.insert(row,Hardware::MainMixer->InsertInputDevice(row,track));

    endInsertRows();
    return true;
}

bool TrackModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row <0||row + count > trackList.count())return false;
    if(count == 0)
        return true;
    HWLOCK;
    beginRemoveRows(parent,row,row+count-1);
    for(int i=0;i<count;++i)
    {
        Track *track = trackList.takeAt(row);
        mixerChannelList.removeAt(row);
        Hardware::RemoveDevice(track);//This will automatically desctruct the mixer channel and the track itself.
    }
    endRemoveRows();
    return true;
}

//Used by tracks inside the model to propagate update to views
void TrackModel::pulse(Track *t)
{
    int index = trackList.indexOf(t);
    Q_ASSERT(index >= 0);
    QModelIndex idx = this->index(index);
    emit dataChanged(idx,idx);
}

void TrackModel::clear()
{
    removeRows(0, trackList.count());
    nextTrackID = 1;
}
/*
    enum ItemFlag {
        NoItemFlags = 0,
        ItemIsSelectable = 1,
        ItemIsEditable = 2,
        ItemIsDragEnabled = 4,
        ItemIsDropEnabled = 8,
        ItemIsUserCheckable = 16,
        ItemIsEnabled = 32,
        ItemIsTristate = 64
    };
**/
Qt::ItemFlags TrackModel::flags(const QModelIndex &index) const
{
    if(index.row()<0 || index.row() > trackList.count())
        return 0;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
