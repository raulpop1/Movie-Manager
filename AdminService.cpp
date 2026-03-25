#include "AdminService.h"
#include "MovieValidator.h"

//AdminService constructor
AdminService::AdminService(Repository& repository):repository{repository}{}


/*
    Explode a string after certain separators - stringToExplode - string to be separated
                                            separatorUsed - character used to divide the string
*/
const std::vector<std::string> AdminService::explode(const std::string& stringToExplode, const char& separatorUsed)
{
    std::string partialStringObtained{ "" };
    std::vector<std::string> explodedString;

    for (auto iterator : stringToExplode)
    {
        if (iterator != separatorUsed)
            partialStringObtained += iterator;
        else
        if (iterator == separatorUsed && partialStringObtained != ""){
            explodedString.push_back(partialStringObtained);
            partialStringObtained = "";
        }
    }
    if (partialStringObtained != "")
        explodedString.push_back(partialStringObtained);
    return explodedString;
}

/*
    Add a movie in the list - title - title of the movie
                            genre - genre of the movie
                            yearOfRelease - the year when the movie was released
                            numberOfLikes - trailer's number of likes
                            trailer - link to the movie's trailer
*/
int AdminService::adminAddMovie(const std::string& title, const std::string& genre, int yearOfRelease, int numberOfLikes, const std::string& trailer) {
    Movie movieUsed{title, genre, yearOfRelease, numberOfLikes, trailer};
    MovieValidator::validate(movieUsed);

    int result = repository.addMovie(movieUsed);

    // Add to undo stack
    auto action = std::make_shared<AddMovieAction>(repository, movieUsed);
    undoRedoManager.executeAction(action);

    return result;
}


/*
    Delete a movie from the list, by its title
*/
int AdminService::adminDeleteMovie(const std::string& title) {
    if (title.empty())
        throw ValidationException("Title cannot be empty!");

    // Find the movie before deleting to store for undo
    Movie movieToDelete;
    auto allMovies = repository.getAllMovies();
    for (const auto& movie : allMovies) {
        if (movie.getTitle() == title) {
            movieToDelete = movie;
            break;
        }
    }

    int result = repository.deleteMovie(movieToDelete);

    // Add to undo stack
    auto action = std::make_shared<DeleteMovieAction>(repository, movieToDelete);
    undoRedoManager.executeAction(action);

    return result;
}

/*
    Add a movie in the list - title - title of the movie to be updated
                            genre - genre of the movie to update with
                            yearOfRelease - the year when the movie was released to update with
                            numberOfLikes - trailer's number of likes to update with
                            trailer - link to the movie's trailer to update with
*/
int AdminService::adminUpdateMovie(const std::string& title, const std::string& genre, int yearOfRelease, int numberOfLikes, const std::string& trailer) {
    Movie newMovie{title, genre, yearOfRelease, numberOfLikes, trailer};
    MovieValidator::validate(newMovie);

    // Find the old movie before updating
    Movie oldMovie;
    auto allMovies = repository.getAllMovies();
    for (const auto& movie : allMovies) {
        if (movie.getTitle() == title) {
            oldMovie = movie;
            break;
        }
    }

    int result = repository.updateMovie(newMovie);

    // Add to undo stack
    auto action = std::make_shared<UpdateMovieAction>(repository, oldMovie, newMovie);
    undoRedoManager.executeAction(action);

    return result;
}

//Return the list of movies
std::vector<Movie> AdminService::adminGetMovieList()
{
    return repository.getAllMovies();
}

//Change the file name used by repository
int AdminService::changeRepositoryFileName(const std::string& nameOfTheFileUsed)
{
    if (nameOfTheFileUsed.empty())
        throw ValidationException("File name cannot be empty!");

    repository.changeFileName(nameOfTheFileUsed);
    return 1;
}

bool AdminService::canUndo() const {
    return undoRedoManager.canUndo();
}

bool AdminService::canRedo() const {
    return undoRedoManager.canRedo();
}

void AdminService::undo() {
    undoRedoManager.undo();
}

void AdminService::redo() {
    undoRedoManager.redo();
}