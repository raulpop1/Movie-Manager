#pragma once
#include "Repository.h"

class UserService
{
private:
    Repository& repository;
    std::vector<Movie> currentMoviesByGenre;
    int currentMoviePosition;

public:
    UserService(Repository& repository, int currentMoviePosition = 0);
    std::vector<Movie> userGetMovieList();
    std::vector<Movie> userGetWatchList();
    int listMoviesByGenre(const std::string& genreGiven);
    int addMovieToWatchList();
    int addMovieToWatchListByTitle(const std::string& titleOfTheMovieToAdd);
    void goToNextMovieByGenre();
    Movie getCurrentMovie();
    int getWatchListLength();
    int getCurrentMoviesByGenreSize();
    int getCurrentPosition();
    int deleteMovieFromWatchlist(const std::string& title, bool like = false);
    void setWatchlistFileManager(FileManager* fileManager);
    void saveWatchlist() const;
    void displayWatchlist() const;
};
