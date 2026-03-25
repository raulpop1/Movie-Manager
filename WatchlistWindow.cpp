// WatchlistWindow.cpp
#include "WatchlistWindow.h"

WatchlistWindow::WatchlistWindow(Repository& repo, QWidget* parent)
        : QWidget(parent), repository(repo) {

    setWindowTitle("Watchlist");
    resize(600, 400);

    auto* layout = new QVBoxLayout(this);

    // Create table view and model
    tableView = new QTableView(this);
    model = new WatchlistTableModel(repository, this);
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(tableView);

    // Create buttons
    auto* buttonLayout = new QHBoxLayout();
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);

    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);
    layout->addLayout(buttonLayout);

    // Connect signals
    connect(undoButton, &QPushButton::clicked, this, &WatchlistWindow::undoRequested);
    connect(redoButton, &QPushButton::clicked, this, &WatchlistWindow::redoRequested);
}

void WatchlistWindow::refreshWatchlist() {
    model->refresh();
}