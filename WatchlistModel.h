// WatchListModel.h
#pragma once
#include <QAbstractTableModel>
#include "Repository.h"

class WatchlistTableModel : public QAbstractTableModel {
Q_OBJECT

private:
    Repository& repository;

public:
    explicit WatchlistTableModel(Repository& repo, QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void refresh();
};