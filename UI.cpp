#include "UI.h"
#include <iostream>
#include <stdlib.h>
#include <limits>

// Helper function to get valid numeric input
int UI::getValidNumericInput(const std::string& prompt) {
    int value;
    std::string input;
    bool validInput = false;

    while (!validInput) {
        std::cout << prompt;
        std::cin >> input;

        try {
            value = std::stoi(input);
            validInput = true;
        }
        catch (const std::exception&) {
            std::cout << "Error: Please enter a valid number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return value;
}

UI::UI(AdminService& adminService, UserService& userService): adminService { adminService } , userService{ userService } ,watchlistFileManager{ nullptr }
{
    uiSelectFileFormat();
}

UI::~UI()
{
    delete watchlistFileManager;
}

void UI::uiSelectFileFormat() {
    std::string format;
    std::string basePath = "watchlist"; // Default path

    std::cout << "Choose watchlist file format (csv/html): ";
    std::cin >> format;

    if (format == "csv") {
        watchlistFileManager = new CSVFileManager(basePath);
    } else if (format == "html") {
        watchlistFileManager = new HTMLFileManager(basePath);
    } else {
        std::cout << "Invalid format. Defaulting to CSV.\n";
        watchlistFileManager = new CSVFileManager(basePath);
    }

    userService.setWatchlistFileManager(watchlistFileManager);
    std::cout << "Watchlist will be saved as: " << watchlistFileManager->getFilePath() << std::endl;
}

void UI::uiDisplayWatchlist() {
    try {
        userService.displayWatchlist();
    } catch (const FileException& e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << std::endl;
    }
}

void UI::runApp()
{
    std::string applicationMode = "";
    while (1) {
        std::cout << "\nAvailable commands: admin, user, exit\n";
        std::cout << "Choose mode: ";
        std::cin >> applicationMode;
        if (applicationMode == "admin") {
            runAdmin();
            break;
        }
        else if (applicationMode == "user") {
            runUser();
            break;
        }
        else if (applicationMode == "exit")
            break;
        else
            std::cout << "Invalid option!\n";
    }
}

void UI::runAdmin()
{
    std::cout << "Mode changed to Admin!\n";

    std::string consoleInput = "";
    int changeMode = 0;
    while (1 && !changeMode) {
        std::cout << "\nAvailable commands: add, update, delete, list, user, exit\n";
        std::cout << "Admin Input: ";
        std::cin >> consoleInput;
        if (consoleInput == "add")
            uiAdminAdd();
        else if (consoleInput == "update")
            uiAdminUpdate();
        else if (consoleInput == "delete")
            uiAdminDelete();
        else if (consoleInput == "list")
            uiAdminList();
        else if (consoleInput == "user") {
            changeMode = 1;
            runUser();
        }
        else if (consoleInput == "exit")
            break;
        else
            std::cout << "Invalid input! \n";
    }

}

void UI::runUser()
{
    std::cout << "Mode changed to User!\n";
    std::cout << "Available commands: list, admin, next, mylist, delete (watched), exit\n";

    try {
        userService.listMoviesByGenre("");
    }
    catch (const MovieAppException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    Movie currentMovie;
    std::string consoleInput = "";
    int changeMode = 0;

    while (1 && !changeMode) {
        std::cout << "\nAvailable commands: list, admin, next,display, mylist, delete (watched), exit\n";
        std::cout << "User Input: ";
        std::cin >> consoleInput;

        if (consoleInput == "list") {
            uiUserList();
            // No flag set - will continue in this loop after returning from list function
        }
        else if(consoleInput == "display")
            uiDisplayWatchlist();
        else if (consoleInput == "admin") {
            changeMode = 1;
            runAdmin();
        }
        else if (consoleInput == "next") {
            try {
                currentMovie = userService.getCurrentMovie();
                std::cout << currentMovie;
                uiUserNext();
            }
            catch (const MovieAppException& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Unexpected error: " << e.what() << std::endl;
            }
        }
        else if (consoleInput == "mylist")
            uiUserWatchList();
        else if (consoleInput == "delete")
            uiUserDeleteFromWatchlist();
        else if (consoleInput == "exit")
            break;
        else
            std::cout << "Invalid input! \n";
    }
}

void UI::uiAdminAdd()
{
    std::cout << "\n--- Add Movie ---\n";
    std::string title, genre, trailer;
    int yearOfRelease, numberOfLikes;

    std::cout << "Title: ";
    std::cin >> title;

    std::cout << "Genre: ";
    std::cin >> genre;

    yearOfRelease = getValidNumericInput("Year of release: ");

    numberOfLikes = getValidNumericInput("Number of likes: ");

    std::cout << "Trailer: ";
    std::cin >> trailer;

    try {
        adminService.adminAddMovie(title, genre, yearOfRelease, numberOfLikes, trailer);
        std::cout << "Movie added successfully!\n";
    }
    catch (const ValidationException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const RepositoryException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiAdminDelete()
{
    std::cout << "\n--- Delete Movie ---\n";
    std::string title;
    std::cout << "Title of movie to delete: ";
    std::cin >> title;

    try {
        adminService.adminDeleteMovie(title);
        std::cout << "Movie deleted successfully!\n";
    }
    catch (const ValidationException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const RepositoryException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiAdminUpdate()
{
    std::cout << "\n--- Update Movie ---\n";
    std::string title, genre, trailer;
    int yearOfRelease, numberOfLikes;

    std::cout << "Title of movie to update: ";
    std::cin >> title;

    std::cout << "New genre: ";
    std::cin >> genre;

    yearOfRelease = getValidNumericInput("New year of release: ");

    numberOfLikes = getValidNumericInput("New number of likes: ");

    std::cout << "New trailer: ";
    std::cin >> trailer;

    try {
        adminService.adminUpdateMovie(title, genre, yearOfRelease, numberOfLikes, trailer);
        std::cout << "Movie updated successfully!\n";
    }
    catch (const ValidationException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const RepositoryException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiAdminList()
{
    std::cout << "\n--- Movie List ---\n";
    try {
        std::vector<Movie>listOfMovies = adminService.adminGetMovieList();
        if(listOfMovies.empty()) {
            std::cout << "No movies in the database.\n";
            return;
        }

        for (int i = 0; i < listOfMovies.size(); ++i)
            std::cout << listOfMovies[i] << '\n';
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiUserAdd() {
    try {
        userService.addMovieToWatchList();
        userService.saveWatchlist();
        std::cout << "Movie added to watchlist successfully!\n";
    }
    catch (const RepositoryException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiUserWatchList()
{
    std::cout << "\n--- Your Watchlist ---\n";
    try {
        std::vector<Movie>listOfMovies = userService.userGetWatchList();
        if (listOfMovies.empty()) {
            std::cout << "Your watchlist is empty!\n";
            return;
        }

        for (int i = 0; i < listOfMovies.size(); ++i)
            std::cout << listOfMovies[i] << '\n';
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiUserNext() {
    try {
        userService.goToNextMovieByGenre();
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void UI::uiUserList()
{
    std::cout << "\n--- Browse Movies ---\n";
    std::string consoleInput, genre = "";
    Movie currentMovie;

    std::cout << "Enter genre (optional, press Enter for all genres): ";
    std::cin.ignore();
    getline(std::cin, genre);

    try {
        userService.listMoviesByGenre(genre);
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
        return; // Return to main user input prompt immediately
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return;
    }

    // Check if there's only one movie with the given genre
    int movieCount = userService.getCurrentMoviesByGenreSize();
    if (movieCount == 1) {
        try {
            // Show the only movie that exists
            currentMovie = userService.getCurrentMovie();
            std::cout << currentMovie << '\n';

            system((("open " + currentMovie.getTrailer()).c_str()));

            std::cout << "\nOnly one movie available for this genre.\n";
            std::cout << "Options: add (add to watchlist), exit\n";
            std::cout << "Input: ";
            std::cin >> consoleInput;

            if (consoleInput == "add") {
                uiUserAdd();
            }
        }
        catch (const MovieAppException& e) {
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        return; // Return to main user prompt
    }

    // Continue with showing movies of the selected genre (multiple movies)
    bool isFirstIteration = true;

    while (true) {
        try {
            currentMovie = userService.getCurrentMovie();
            std::cout << currentMovie << '\n';

            if (isFirstIteration) {
                system((("open " + currentMovie.getTrailer()).c_str()));
                isFirstIteration = false;
            }

            std::cout << "\nOptions: add (add to watchlist), next (see next movie), exit\n";
            std::cout << "Input: ";
            std::cin >> consoleInput;

            if (consoleInput == "add") {
                uiUserAdd();
                try {
                    userService.goToNextMovieByGenre();
                    // Add this code to open the trailer of the next movie
                    currentMovie = userService.getCurrentMovie();
                    system((("open " + currentMovie.getTrailer()).c_str()));
                }
                catch (const MovieAppException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                    return;
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << std::endl;
                    return;
                }
            }
            else if (consoleInput == "next") {
                try {
                    userService.goToNextMovieByGenre();
                    // Get and open the trailer of the next movie
                    currentMovie = userService.getCurrentMovie();
                    system((("open " + currentMovie.getTrailer()).c_str()));
                }
                catch (const MovieAppException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                    return;
                }
                catch (const std::exception& e) {
                    std::cout << "Unexpected error: " << e.what() << std::endl;
                    return;
                }
            }
            else if (consoleInput == "exit")
                break;
            else
                std::cout << "Invalid option\n";
        }
        catch (const MovieAppException& e) {
            std::cout << "Error: " << e.what() << std::endl;
            return;
        }
        catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << std::endl;
            return;
        }
    }
}

void UI::uiUserDeleteFromWatchlist()
{
    std::cout << "\n--- Delete Movie from Watchlist ---\n";

    try {
        std::vector<Movie> watchlist = userService.userGetWatchList();
        if (watchlist.empty()) {
            std::cout << "Your watchlist is empty!\n";
            return;
        }

        std::cout << "Your watchlist:\n";
        for (int i = 0; i < watchlist.size(); ++i) {
            std::cout << i+1 << ": " << watchlist[i] << "\n";
        }

        std::string title;
        std::cout << "\nEnter the title of the movie you watched: ";
        std::cin.ignore();
        std::getline(std::cin, title);

        char likeChoice;
        std::cout << "Did you like the movie? (y/n): ";
        std::cin >> likeChoice;

        bool like = (likeChoice == 'y' || likeChoice == 'Y');

        userService.deleteMovieFromWatchlist(title, like);
        userService.saveWatchlist();
        std::cout << "Movie deleted from watchlist";
        if (like) {
            std::cout << " and liked! The number of likes has been increased.";
        }
        std::cout << "\n";
    }
    catch (const RepositoryException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const MovieAppException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

