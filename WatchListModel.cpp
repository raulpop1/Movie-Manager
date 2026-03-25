// WatchListModel.cpp
#include "WatchListModel.h"

WatchlistTableModel::WatchlistTableModel(Repository& repo, QObject* parent)
        : QAbstractTableModel(parent), repository(repo) {
}

int WatchlistTableModel::rowCount(const QModelIndex& parent) const {
    return repository.getAllWatchListMovies().size();
}

int WatchlistTableModel::columnCount(const QModelIndex& parent) const {
    return 5; // Title, Genre, Year, Likes, Trailer
}

QVariant WatchlistTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= repository.getAllWatchListMovies().size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        const Movie& movie = repository.getAllWatchListMovies()[index.row()];

        switch(index.column()) {
            case 0: return QString::fromStdString(movie.getTitle());
            case 1: return QString::fromStdString(movie.getGenre());
            case 2: return movie.getYearOfRelease();
            case 3: return movie.getNumberOfLikes();
            case 4: return QString::fromStdString(movie.getTrailer());
            default: return QVariant();
        }
    }
    return QVariant();
}

QVariant WatchlistTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section) {
            case 0: return QString("Title");
            case 1: return QString("Genre");
            case 2: return QString("Year");
            case 3: return QString("Likes");
            case 4: return QString("Trailer");
            default: return QVariant();
        }
    }
    return QVariant();
}

void WatchlistTableModel::refresh() {
    beginResetModel();
    endResetModel();
}