#pragma once
#include "Repository.h"
#include "FileManager.h"
#include <vector>
#include <string>

class UserService {
private:
    Repository& repository;
    std::vector<Movie> moviesFilteredByGenre;
    int currentMovieIndex;
    FileManager* fileManager = nullptr;

public:
    explicit UserService(Repository& repository);
    Repository& getRepository() { return repository; }
    std::vector<Movie> userGetMovieList();
    std::vector<Movie> userGetWatchList();
    
    void listMoviesByGenre(const std::string& genre);
    Movie getCurrentMovie() const;
    void goToNextMovieByGenre();
    
    int addMovieToWatchList();
    int deleteMovieFromWatchlist(const std::string& title, bool likeMovie);
    
    void setWatchlistFileManager(FileManager* fileManager);
    void saveWatchlist();
    void displayWatchlist();
    
    // Helper method for UI
    int getCurrentMoviesByGenreSize() const { return moviesFilteredByGenre.size(); }
};
