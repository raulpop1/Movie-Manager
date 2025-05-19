#include "UserService.h"
#include <iostream>
/*
    UserService constructor - repository - repository used
                              currentMoviePosition - the index of the movie listed
*/
UserService::UserService(Repository& repository, int currentMoviePosition) : repository{ repository }, currentMoviePosition{ currentMoviePosition }, currentMoviesByGenre{repository.getAllMovies()}{}

//Get the list of all movies
std::vector<Movie> UserService::userGetMovieList()
{
    return repository.getAllMovies();
}

//Get the list of movies in the watch list
std::vector<Movie> UserService::userGetWatchList()
{
    return repository.getAllWatchListMovies();
}

//Get the list of movies with a given genre
int UserService::listMoviesByGenre(const std::string& genreGiven)
{
    currentMoviesByGenre = repository.getMoviesByGenre(genreGiven);
    if (currentMoviesByGenre.size() == 0)
        throw MovieAppException("No movies found with genre '" + genreGiven + "'");
    currentMoviePosition = 0;
    return 1;
}

//Add a movie to the watch list by the current movie position
int UserService::addMovieToWatchList()
{
    if (currentMoviesByGenre.empty())
        throw MovieAppException("No movies available to add to watchlist");

    if (currentMoviePosition < 0 || currentMoviePosition >= currentMoviesByGenre.size())
        throw MovieAppException("Invalid movie position");

    return repository.addMovieToWatchlist(currentMoviesByGenre[currentMoviePosition]);
}

//Add a movie to the watch list by the given title
int UserService::addMovieToWatchListByTitle(const std::string& titleOfTheMovieToAdd)
{
    return repository.addMovieToWatchListByTitle(titleOfTheMovieToAdd);
}

//Iterate to the next movie in the list
void UserService::goToNextMovieByGenre()
{
    if (currentMoviesByGenre.empty())
        throw MovieAppException("No movies available in the current list");

    currentMoviePosition++;
    if(currentMoviePosition == currentMoviesByGenre.size())
        currentMoviePosition = 0;
}

//Get the length of the watch list
int UserService::getWatchListLength()
{
    return repository.getNumberOfMoviesWatchList();
}

//Get the current movie in the list
Movie UserService::getCurrentMovie()
{
    if (currentMoviesByGenre.empty())
        throw MovieAppException("No movies available in the current list");

    if (currentMoviePosition < 0 || currentMoviePosition >= currentMoviesByGenre.size())
        throw MovieAppException("Invalid movie position");

    return currentMoviesByGenre[currentMoviePosition];
}

//Get the size of the currentMoviesByGenre list
int UserService::getCurrentMoviesByGenreSize()
{
    return currentMoviesByGenre.size();
}

//Get the current position in the movie list
int UserService::getCurrentPosition()
{
    return currentMoviePosition;
}

//Delete a movie from watch list with option to like it
int UserService::deleteMovieFromWatchlist(const std::string& title, bool like)
{
    try {
        if (like)
            repository.likeMovie(title);
    }
    catch (const RepositoryException& e) {
        // If liking fails, we still want to try to delete from watchlist
        // so we just continue
    }

    return repository.deleteMovieFromWatchlist(title);
}

void UserService::setWatchlistFileManager(FileManager* fileManager) {
    repository.setWatchlistFileManager(fileManager);
}

void UserService::saveWatchlist() const {
    repository.saveWatchlist();
}

void UserService::displayWatchlist() const {
    repository.displayWatchlist();
}
