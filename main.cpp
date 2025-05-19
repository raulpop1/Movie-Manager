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
#include "Repository.h"
#include "AdminService.h"
#include "UserService.h"
#include "FileManager.h"

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

        connectSignalsSlots();
        populateMovieList();
    }

    void connectSignalsSlots() {
        connect(loadButton, &QPushButton::clicked, this, &AdminWidget::handleLoadFile);
        connect(addButton, &QPushButton::clicked, this, &AdminWidget::handleAddMovie);
        connect(updateButton, &QPushButton::clicked, this, &AdminWidget::handleUpdateMovie);
        connect(deleteButton, &QPushButton::clicked, this, &AdminWidget::handleDeleteMovie);
        connect(movieTable, &QTableWidget::itemSelectionChanged, this, &AdminWidget::handleMovieSelection);
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

// User Widget - Handles user functionality
class UserWidget : public QWidget {
private:
    UserService& userService;

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
    QTableWidget* watchlistTable;
    QPushButton* deleteFromWatchlistButton;
    QCheckBox* likeMovieCheckBox;

    // Watchlist file options
    QGroupBox* fileBox;
    QComboBox* fileFormatCombo;
    QLineEdit* watchlistFileEdit;
    QPushButton* saveWatchlistButton;
    QPushButton* displayWatchlistButton;

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

        watchlistTable = new QTableWidget();
        watchlistTable->setColumnCount(5);
        watchlistTable->setHorizontalHeaderLabels({"Title", "Genre", "Year", "Likes", "Trailer"});
        watchlistTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        watchlistTable->setSelectionBehavior(QTableWidget::SelectRows);
        watchlistTable->setEditTriggers(QTableWidget::NoEditTriggers);
        watchlistLayout->addWidget(watchlistTable);

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
        watchlistFileEdit = new QLineEdit("watchlist.html");
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

public:
    UserWidget(UserService& service, QWidget* parent = nullptr)
            : QWidget(parent), userService(service) {
        setupUI();
    }

private slots:
    void handleFilterByGenre() {
        QMessageBox::information(this, "Filter", "Filtering by genre: " + genreFilterEdit->text());
    }

    void handleNextMovie() {
        QMessageBox::information(this, "Navigation", "Moving to next movie");
    }

    void handleAddToWatchlist() {
        QMessageBox::information(this, "Watchlist", "Movie added to watchlist");
    }

    void handleOpenTrailer() {
        QMessageBox::information(this, "Trailer", "Opening movie trailer");
    }

    void handleDeleteFromWatchlist() {
        QMessageBox::information(this, "Watchlist",
                                 likeMovieCheckBox->isChecked() ?
                                 "Movie deleted from watchlist and liked" :
                                 "Movie deleted from watchlist");
    }

    void handleSaveWatchlist() {
        QMessageBox::information(this, "File Operation",
                                 "Saving watchlist to " + watchlistFileEdit->text());
    }

    void handleDisplayWatchlist() {
        QMessageBox::information(this, "File Operation",
                                 "Opening watchlist file: " + watchlistFileEdit->text());
    }

    void handleFileFormatChanged(const QString& format) {
        QString extension = format.toLower();
        watchlistFileEdit->setText("watchlist." + extension);
    }
};

// Main application window with tabs
class MainWindow : public QMainWindow {
private:
    Repository repository;
    AdminService adminService;
    UserService userService;
    QTabWidget* tabWidget;

public:
    MainWindow(QWidget* parent = nullptr) :
            QMainWindow(parent),
            repository("movies.txt"), // Default repository file
            adminService(repository),
            userService(repository) {

        setWindowTitle("Movie Management Application");
        resize(800, 600);

        // Create tab widget
        tabWidget = new QTabWidget();
        setCentralWidget(tabWidget);

        // Create and add admin widget
        auto* adminWidget = new AdminWidget(adminService);
        tabWidget->addTab(adminWidget, "Administrator");

        // Create and add user widget
        auto* userWidget = new UserWidget(userService);
        tabWidget->addTab(userWidget, "User");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}