#include "UserService.h"
#include <stdexcept>
#include <algorithm>

UserService::UserService(Repository& repository)
    : repository{repository}, currentMovieIndex{0} {}

std::vector<Movie> UserService::userGetMovieList() {
    return repository.getAllMovies();
}

std::vector<Movie> UserService::userGetWatchList() {
    return repository.getAllWatchListMovies();
}

void UserService::listMoviesByGenre(const std::string& genre) {
    moviesFilteredByGenre = repository.getMoviesByGenre(genre);
    if (moviesFilteredByGenre.empty()) {
        throw std::runtime_error("No movies found for the given genre!");
    }
    currentMovieIndex = 0;
}

Movie UserService::getCurrentMovie() const {
    if (moviesFilteredByGenre.empty()) {
        throw std::runtime_error("No movies available!");
    }
    
    if (currentMovieIndex < 0 || currentMovieIndex >= moviesFilteredByGenre.size()) {
        throw std::runtime_error("Invalid movie index!");
    }
    
    return moviesFilteredByGenre[currentMovieIndex];
}

void UserService::goToNextMovieByGenre() {
    if (moviesFilteredByGenre.empty()) {
        throw std::runtime_error("No movies available!");
    }
    
    currentMovieIndex++;
    if (currentMovieIndex == moviesFilteredByGenre.size()) {
        currentMovieIndex = 0;
    }
}

int UserService::addMovieToWatchList() {
    if (moviesFilteredByGenre.empty()) {
        throw std::runtime_error("No movies available to add to watchlist!");
    }
    
    if (currentMovieIndex < 0 || currentMovieIndex >= moviesFilteredByGenre.size()) {
        throw std::runtime_error("Invalid movie index!");
    }
    
    return repository.addMovieToWatchlist(moviesFilteredByGenre[currentMovieIndex]);
}

int UserService::deleteMovieFromWatchlist(const std::string& title, bool likeMovie) {
    Movie movieToDelete;
    auto watchlist = repository.getAllWatchListMovies();
    auto it = std::find_if(watchlist.begin(), watchlist.end(),
                           [&title](const Movie& m) { return m.getTitle() == title; });

    if (it == watchlist.end()) {
        throw std::runtime_error("Movie not found in watchlist!");
    }

    movieToDelete = *it;

    if (likeMovie) {
        // Increase likes for the movie in the main repository
        auto allMovies = repository.getAllMovies();
        for (auto& movie : allMovies) {
            if (movie.getTitle() == title) {
                Movie updatedMovie = movie;
                updatedMovie.setNumberOfLikes(movie.getNumberOfLikes() + 1);
                repository.updateMovie(updatedMovie);
                break;
            }
        }
    }

    // Fix: Pass the title string instead of the Movie object
    return repository.deleteMovieFromWatchlist(movieToDelete.getTitle());
}

void UserService::setWatchlistFileManager(FileManager* manager) {
    fileManager = manager;
}

void UserService::saveWatchlist() {
    if (!fileManager) {
        throw std::runtime_error("No file manager set!");
    }

    // Fix: Change writeToFile to saveToFile
    fileManager->saveToFile(repository.getAllWatchListMovies());
}

void UserService::displayWatchlist() {
    if (!fileManager) {
        throw std::runtime_error("No file manager set!");
    }
    
    fileManager->displayFile();
}
