#pragma once

#include "movie.h"
#include <vector>
#include <algorithm>
#include "Exceptions.h"
#include "FileManager.h"


class Repository
{
private:
    std::vector<Movie> movieList;
    std::vector<Movie> userWatchList;
    std::string movieFileName;
    FileManager* watchListFileManager = nullptr;


public:
    int loadMoviesFromFile();
    void writeMoviesToFile();
    Repository(const std::string& nameOfTheFileUsed = "");
    ~Repository();
    int addMovie(const Movie& movieToAdd);
    int deleteMovie(const Movie& movieToDelete);
    int updateMovie(const Movie& ToUpdateWith);
    void changeFileName(const std::string& nameOfTheFileUsed = "");
    Movie getMovieAtPosition(int positionOfMovie);
    int getNumberOfMovies();
    std::vector<Movie> getMoviesByGenre(const std::string& genreGiven = "");
    std::vector<Movie> getAllMovies();
    std::vector<Movie> getAllWatchListMovies();
    int addMovieToWatchlist(const Movie& movieToAdd);
    int addMovieToWatchListByTitle(const std::string& titleOfTheMovieToAdd);
    int getNumberOfMoviesWatchList();
    int deleteMovieFromWatchlist(const std::string& title);
    int likeMovie(const std::string& title);
    void setWatchlistFileManager(FileManager* fileManager);
    void saveWatchlist() const;
    void displayWatchlist() const;
};
