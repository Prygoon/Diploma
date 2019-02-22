#include "tracksectionproxymodel.h"

TrackSectionProxyModel::TrackSectionProxyModel(QObject *parent) :
    QSortFilterProxyModel (parent)
{

}

QModelIndex TrackSectionProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (sourceModel()) {
        return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
    } else {
        return QModelIndex();
    }
}

QModelIndex TrackSectionProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return index(sourceIndex.column(), sourceIndex.row());
}

QModelIndex TrackSectionProxyModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column, static_cast<void*>( nullptr));
}

QModelIndex TrackSectionProxyModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int TrackSectionProxyModel::rowCount(const QModelIndex &) const
{
    return sourceModel() ? sourceModel()->columnCount() : 0;
}

int TrackSectionProxyModel::columnCount(const QModelIndex &) const
{
    return sourceModel() ? sourceModel()->rowCount() : 0;
}

QVariant TrackSectionProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!sourceModel()) { return QVariant(); }
    Qt::Orientation new_orientation = orientation == Qt::Horizontal ?
                Qt::Vertical : Qt::Horizontal;
    return sourceModel()->headerData(section, new_orientation, role);
}
