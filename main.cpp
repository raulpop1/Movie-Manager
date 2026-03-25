#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QSplitter>
#include <QTextBrowser>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QUrl>
#include <QShortcut>
#include <QKeySequence>
#include "Repository.h"
#include "AdminService.h"
#include "UserService.h"
#include "FileManager.h"
#include "WatchlistModel.h"
#include "WatchlistWindow.h"

// Admin Widget - Handles administrator functionality
class AdminWidget : public QWidget {
private:
    AdminService& adminService;
    QTableWidget* movieTable;

    // Form fields
    QLineEdit* titleEdit;
    QLineEdit* genreEdit;
    QLineEdit* yearEdit;
    QLineEdit* likesEdit;
    QLineEdit* trailerEdit;

    // Buttons
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* updateButton;
    QLineEdit* fileNameEdit;
    QPushButton* loadButton;
    QPushButton* undoButton;
    QPushButton* redoButton;

    void setupUI() {
        auto* mainLayout = new QVBoxLayout(this);

        // Create movie table
        movieTable = new QTableWidget();
        movieTable->setColumnCount(5);
        movieTable->setHorizontalHeaderLabels({"Title", "Genre", "Year", "Likes", "Trailer"});
        movieTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        movieTable->setSelectionBehavior(QTableWidget::SelectRows);
        movieTable->setEditTriggers(QTableWidget::NoEditTriggers);
        mainLayout->addWidget(movieTable);

        // File loading section
        auto* fileBox = new QGroupBox("Repository File");
        auto* fileLayout = new QHBoxLayout(fileBox);
        fileNameEdit = new QLineEdit();
        loadButton = new QPushButton("Load");
        fileLayout->addWidget(new QLabel("File:"));
        fileLayout->addWidget(fileNameEdit);
        fileLayout->addWidget(loadButton);
        mainLayout->addWidget(fileBox);

        // Form for movie data
        auto* formBox = new QGroupBox("Movie Details");
        auto* formLayout = new QFormLayout(formBox);

        titleEdit = new QLineEdit();
        genreEdit = new QLineEdit();
        yearEdit = new QLineEdit();
        likesEdit = new QLineEdit();
        trailerEdit = new QLineEdit();

        formLayout->addRow("Title:", titleEdit);
        formLayout->addRow("Genre:", genreEdit);
        formLayout->addRow("Year:", yearEdit);
        formLayout->addRow("Likes:", likesEdit);
        formLayout->addRow("Trailer:", trailerEdit);

        mainLayout->addWidget(formBox);

        // Button controls
        auto* buttonLayout = new QHBoxLayout();
        addButton = new QPushButton("Add");
        updateButton = new QPushButton("Update");
        deleteButton = new QPushButton("Delete");

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(updateButton);
        buttonLayout->addWidget(deleteButton);
        mainLayout->addLayout(buttonLayout);

        auto* undoRedoLayout = new QHBoxLayout();
        undoButton = new QPushButton("Undo");
        redoButton = new QPushButton("Redo");
        undoRedoLayout->addWidget(undoButton);
        undoRedoLayout->addWidget(redoButton);
        mainLayout->addLayout(undoRedoLayout);

        connectSignalsSlots();
        populateMovieList();
    }

    void connectSignalsSlots() {
        connect(loadButton, &QPushButton::clicked, this, &AdminWidget::handleLoadFile);
        connect(addButton, &QPushButton::clicked, this, &AdminWidget::handleAddMovie);
        connect(updateButton, &QPushButton::clicked, this, &AdminWidget::handleUpdateMovie);
        connect(deleteButton, &QPushButton::clicked, this, &AdminWidget::handleDeleteMovie);
        connect(movieTable, &QTableWidget::itemSelectionChanged, this, &AdminWidget::handleMovieSelection);
        connect(undoButton, &QPushButton::clicked, this, &AdminWidget::handleUndo);
        connect(redoButton, &QPushButton::clicked, this, &AdminWidget::handleRedo);

        // Add undo/redo shortcuts
        auto* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
        auto* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
        connect(undoShortcut, &QShortcut::activated, this, &AdminWidget::handleUndo);
        connect(redoShortcut, &QShortcut::activated, this, &AdminWidget::handleRedo);
    }

private slots:
    void handleUndo() {
        try {
            adminService.undo();
            populateMovieList();
            clearForm();
            QMessageBox::information(this, "Success", "Operation undone successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

    void handleRedo() {
        try {
            adminService.redo();
            populateMovieList();
            clearForm();
            QMessageBox::information(this, "Success", "Operation redone successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }


public:
    AdminWidget(AdminService& service, QWidget* parent = nullptr)
            : QWidget(parent), adminService(service) {
        setupUI();
    }

    void populateMovieList() {
        movieTable->setRowCount(0);

        auto movies = adminService.adminGetMovieList();
        movieTable->setRowCount(movies.size());

        int row = 0;
        for (const auto& movie : movies) {
            movieTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(movie.getTitle())));
            movieTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(movie.getGenre())));
            movieTable->setItem(row, 2, new QTableWidgetItem(QString::number(movie.getYearOfRelease())));
            movieTable->setItem(row, 3, new QTableWidgetItem(QString::number(movie.getNumberOfLikes())));
            movieTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(movie.getTrailer())));
            row++;
        }
    }

private slots:
    void handleLoadFile() {
        try {
            QString fileName = fileNameEdit->text();
            if (fileName.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter a file name");
                return;
            }
            adminService.changeRepositoryFileName(fileName.toStdString());
            populateMovieList();
            QMessageBox::information(this, "Success", "Repository file loaded successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

    void handleAddMovie() {
        try {
            QString title = titleEdit->text();
            QString genre = genreEdit->text();
            QString yearStr = yearEdit->text();
            QString likesStr = likesEdit->text();
            QString trailer = trailerEdit->text();

            if (title.isEmpty() || genre.isEmpty() || yearStr.isEmpty() || likesStr.isEmpty() || trailer.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required");
                return;
            }

            int year = yearStr.toInt();
            int likes = likesStr.toInt();

            adminService.adminAddMovie(title.toStdString(), genre.toStdString(), year, likes, trailer.toStdString());
            populateMovieList();
            clearForm();
            QMessageBox::information(this, "Success", "Movie added successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

    void handleUpdateMovie() {
        try {
            QString title = titleEdit->text();
            QString genre = genreEdit->text();
            QString yearStr = yearEdit->text();
            QString likesStr = likesEdit->text();
            QString trailer = trailerEdit->text();

            if (title.isEmpty() || genre.isEmpty() || yearStr.isEmpty() || likesStr.isEmpty() || trailer.isEmpty()) {
                QMessageBox::warning(this, "Error", "All fields are required");
                return;
            }

            int year = yearStr.toInt();
            int likes = likesStr.toInt();

            adminService.adminUpdateMovie(title.toStdString(), genre.toStdString(), year, likes, trailer.toStdString());
            populateMovieList();
            clearForm();
            QMessageBox::information(this, "Success", "Movie updated successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

    void handleDeleteMovie() {
        try {
            QString title = titleEdit->text();

            if (title.isEmpty()) {
                QMessageBox::warning(this, "Error", "Title is required");
                return;
            }

            adminService.adminDeleteMovie(title.toStdString());
            populateMovieList();
            clearForm();
            QMessageBox::information(this, "Success", "Movie deleted successfully");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

    void handleMovieSelection() {
        auto selectedItems = movieTable->selectedItems();
        if (selectedItems.empty()) return;

        int row = movieTable->row(selectedItems[0]);

        titleEdit->setText(movieTable->item(row, 0)->text());
        genreEdit->setText(movieTable->item(row, 1)->text());
        yearEdit->setText(movieTable->item(row, 2)->text());
        likesEdit->setText(movieTable->item(row, 3)->text());
        trailerEdit->setText(movieTable->item(row, 4)->text());
    }

    void clearForm() {
        titleEdit->clear();
        genreEdit->clear();
        yearEdit->clear();
        likesEdit->clear();
        trailerEdit->clear();
    }
};

class UserWidget : public QWidget {
private:
    UserService& userService;
    FileManager* fileManager = nullptr;

    // Browse movies section
    QGroupBox* browseBox;
    QLineEdit* genreFilterEdit;
    QPushButton* filterButton;
    QTextBrowser* currentMovieDisplay;
    QPushButton* nextButton;
    QPushButton* addToWatchlistButton;
    QPushButton* openTrailerButton;

    // Watchlist section
    QGroupBox* watchlistBox;
    QTableView* watchlistTableView;
    QPushButton* deleteFromWatchlistButton;
    QCheckBox* likeMovieCheckBox;

    // Watchlist file options
    QGroupBox* fileBox;
    QComboBox* fileFormatCombo;
    QLineEdit* watchlistFileEdit;
    QPushButton* saveWatchlistButton;
    QPushButton* displayWatchlistButton;

    // Watchlist model
    WatchlistTableModel* watchlistModel;

    void setupUI() {
        auto* mainLayout = new QVBoxLayout(this);
        auto* splitter = new QSplitter(Qt::Vertical);
        mainLayout->addWidget(splitter);

        // Browse movies section
        browseBox = new QGroupBox("Browse Movies");
        auto* browseLayout = new QVBoxLayout(browseBox);

        auto* filterLayout = new QHBoxLayout();
        genreFilterEdit = new QLineEdit();
        filterButton = new QPushButton("Filter by Genre");
        filterLayout->addWidget(new QLabel("Genre:"));
        filterLayout->addWidget(genreFilterEdit);
        filterLayout->addWidget(filterButton);
        browseLayout->addLayout(filterLayout);

        currentMovieDisplay = new QTextBrowser();
        currentMovieDisplay->setMinimumHeight(100);
        browseLayout->addWidget(currentMovieDisplay);

        auto* browseButtonLayout = new QHBoxLayout();
        nextButton = new QPushButton("Next Movie");
        addToWatchlistButton = new QPushButton("Add to Watchlist");
        openTrailerButton = new QPushButton("Open Trailer");
        browseButtonLayout->addWidget(nextButton);
        browseButtonLayout->addWidget(addToWatchlistButton);
        browseButtonLayout->addWidget(openTrailerButton);
        browseLayout->addLayout(browseButtonLayout);

        splitter->addWidget(browseBox);

        // Watchlist section
        watchlistBox = new QGroupBox("My Watchlist");
        auto* watchlistLayout = new QVBoxLayout(watchlistBox);

        // Create QTableView with custom model
        watchlistTableView = new QTableView();
        watchlistModel = new WatchlistTableModel(userService.getRepository(), watchlistTableView);
        watchlistTableView->setModel(watchlistModel);
        watchlistTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        watchlistTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        watchlistTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        watchlistLayout->addWidget(watchlistTableView);

        auto* watchlistButtonLayout = new QHBoxLayout();
        deleteFromWatchlistButton = new QPushButton("Delete from Watchlist");
        likeMovieCheckBox = new QCheckBox("Like this movie");
        watchlistButtonLayout->addWidget(deleteFromWatchlistButton);
        watchlistButtonLayout->addWidget(likeMovieCheckBox);
        watchlistLayout->addLayout(watchlistButtonLayout);

        splitter->addWidget(watchlistBox);

        // File options section
        fileBox = new QGroupBox("Watchlist File Options");
        auto* fileLayout = new QVBoxLayout(fileBox);

        auto* formatLayout = new QHBoxLayout();
        formatLayout->addWidget(new QLabel("File Format:"));
        fileFormatCombo = new QComboBox();
        fileFormatCombo->addItem("HTML");
        fileFormatCombo->addItem("CSV");
        formatLayout->addWidget(fileFormatCombo);
        fileLayout->addLayout(formatLayout);

        auto* fileNameLayout = new QHBoxLayout();
        fileNameLayout->addWidget(new QLabel("File Name:"));
        watchlistFileEdit = new QLineEdit("watchlist");
        fileNameLayout->addWidget(watchlistFileEdit);
        fileLayout->addLayout(fileNameLayout);

        auto* fileButtonLayout = new QHBoxLayout();
        saveWatchlistButton = new QPushButton("Save Watchlist");
        displayWatchlistButton = new QPushButton("Display Watchlist");
        fileButtonLayout->addWidget(saveWatchlistButton);
        fileButtonLayout->addWidget(displayWatchlistButton);
        fileLayout->addLayout(fileButtonLayout);

        mainLayout->addWidget(fileBox);

        connectSignalsSlots();
        updateFileManager();

        // start with all movies
        try {
            userService.listMoviesByGenre("");
            updateCurrentMovieDisplay();
        } catch (const std::exception& e) {
            currentMovieDisplay->setText("No movies available");
        }
    }

    void connectSignalsSlots() {
        connect(filterButton, &QPushButton::clicked, this, &UserWidget::handleFilterByGenre);
        connect(nextButton, &QPushButton::clicked, this, &UserWidget::handleNextMovie);
        connect(addToWatchlistButton, &QPushButton::clicked, this, &UserWidget::handleAddToWatchlist);
        connect(openTrailerButton, &QPushButton::clicked, this, &UserWidget::handleOpenTrailer);
        connect(deleteFromWatchlistButton, &QPushButton::clicked, this, &UserWidget::handleDeleteFromWatchlist);
        connect(saveWatchlistButton, &QPushButton::clicked, this, &UserWidget::handleSaveWatchlist);
        connect(displayWatchlistButton, &QPushButton::clicked, this, &UserWidget::handleDisplayWatchlist);
        connect(fileFormatCombo, &QComboBox::currentTextChanged, this, &UserWidget::handleFileFormatChanged);
    }

    void updateCurrentMovieDisplay() {
        try {
            Movie currentMovie = userService.getCurrentMovie();
            QString movieInfo = QString::fromStdString(currentMovie.getOutputForm());
            currentMovieDisplay->setText(movieInfo);
        } catch (const std::exception& e) {
            currentMovieDisplay->setText(e.what());
        }
    }

    void updateFileManager() {
        if (fileManager) {
            delete fileManager;
            fileManager = nullptr;
        }

        QString basePath = watchlistFileEdit->text();
        if (fileFormatCombo->currentText() == "HTML") {
            fileManager = new HTMLFileManager(basePath.toStdString());
        } else {
            fileManager = new CSVFileManager(basePath.toStdString());
        }

        userService.setWatchlistFileManager(fileManager);
    }

public:
    UserWidget(UserService& service, QWidget* parent = nullptr)
            : QWidget(parent), userService(service) {
        setupUI();
    }

    ~UserWidget() {
        if (fileManager) {
            delete fileManager;
        }
        delete watchlistModel;
    }

private slots:
    void handleFilterByGenre() {
        try {
            QString genre = genreFilterEdit->text();
            userService.listMoviesByGenre(genre.toStdString());
            updateCurrentMovieDisplay();
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleNextMovie() {
        try {
            userService.goToNextMovieByGenre();
            updateCurrentMovieDisplay();
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleAddToWatchlist() {
        try {
            userService.addMovieToWatchList();
            watchlistModel->refresh();
            QMessageBox::information(this, "Success", "Movie added to watchlist");
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleOpenTrailer() {
        try {
            Movie currentMovie = userService.getCurrentMovie();
            QString trailerUrl = QString::fromStdString(currentMovie.getTrailer());
            QDesktopServices::openUrl(QUrl(trailerUrl));
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleDeleteFromWatchlist() {
        auto selectedIndexes = watchlistTableView->selectionModel()->selectedIndexes();
        if (selectedIndexes.empty()) {
            QMessageBox::warning(this, "Error", "Please select a movie to delete");
            return;
        }

        int row = selectedIndexes.first().row();
        QModelIndex index = watchlistModel->index(row, 0);
        QString title = watchlistModel->data(index, Qt::DisplayRole).toString();

        try {
            bool like = likeMovieCheckBox->isChecked();
            userService.deleteMovieFromWatchlist(title.toStdString(), like);
            watchlistModel->refresh();

            QString message = like ?
                              "Movie deleted from watchlist and liked! The number of likes has been increased." :
                              "Movie deleted from watchlist";
            QMessageBox::information(this, "Success", message);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleSaveWatchlist() {
        try {
            updateFileManager();
            userService.saveWatchlist();
            QMessageBox::information(this, "Success",
                                     "Watchlist saved to " + watchlistFileEdit->text() +
                                     "." + fileFormatCombo->currentText().toLower());
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleDisplayWatchlist() {
        try {
            userService.displayWatchlist();
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Error", e.what());
        }
    }

    void handleFileFormatChanged(const QString& format) {
        updateFileManager();
    }
};

// Main application window with tabs
class MainWindow : public QMainWindow {
Q_OBJECT

private:
    QTabWidget* tabWidget;
    Repository repository;
    AdminService adminService;
    UserService userService;
    AdminWidget* adminWidget;
    WatchlistWindow* watchlistWindow;

public:
    MainWindow(QWidget* parent = nullptr)
            : QMainWindow(parent),
              repository("movies.txt"),
              adminService(repository),
              userService(repository) {
        // Window setup
        setWindowTitle("Movie Manager Application");
        resize(800, 600);

        // Create tabs
        tabWidget = new QTabWidget();

        // Create and add admin widget
        adminWidget = new AdminWidget(adminService);
        tabWidget->addTab(adminWidget, "Administrator");

        // Create and add user widget
        auto* userWidget = new UserWidget(userService);
        tabWidget->addTab(userWidget, "User");

        // Create watchlist window
        watchlistWindow = new WatchlistWindow(repository);
        QPushButton* openWatchlistButton = new QPushButton("Open Watchlist");
        connect(openWatchlistButton, &QPushButton::clicked, [this]() {
            watchlistWindow->show();
            watchlistWindow->refreshWatchlist();
        });

        // Layout for main window
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(tabWidget);
        mainLayout->addWidget(openWatchlistButton);

        QWidget* central = new QWidget();
        central->setLayout(mainLayout);
        setCentralWidget(central);

        // Connect signals and slots
        connect(tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
            if (index == 0) {
                adminWidget->populateMovieList();
            }
        });

        connect(watchlistWindow, &WatchlistWindow::undoRequested, [this]() {
            adminService.undo();
            adminWidget->populateMovieList();
            watchlistWindow->refreshWatchlist();
        });

        connect(watchlistWindow, &WatchlistWindow::redoRequested, [this]() {
            adminService.redo();
            adminWidget->populateMovieList();
            watchlistWindow->refreshWatchlist();
        });
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
#include "main.moc"