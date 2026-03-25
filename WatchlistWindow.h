// WatchlistWindow.h
#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include "Repository.h"
#include "WatchListModel.h"

class WatchlistWindow : public QWidget {
Q_OBJECT

private:
    Repository& repository;
    WatchlistTableModel* model;
    QTableView* tableView;
    QPushButton* undoButton;
    QPushButton* redoButton;

public:
    explicit WatchlistWindow(Repository& repo, QWidget* parent = nullptr);
    void refreshWatchlist();

signals:
    void undoRequested();
    void redoRequested();
};