#ifndef TRACKSECTIONPROXYMODEL_H
#define TRACKSECTIONPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TrackSectionProxyModel : public QSortFilterProxyModel
{
public:
    TrackSectionProxyModel(QObject * parent = nullptr);

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // TRACKSECTIONPROXYMODEL_H
