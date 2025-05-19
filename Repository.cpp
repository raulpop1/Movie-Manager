#include "Repository.h"
#include <iostream>
#include <fstream>
#include <sstream>





int Repository::loadMoviesFromFile()
{
    movieList.clear();
    int loadedMoviesCount = 0;

    if (movieFileName == "")
        return loadedMoviesCount;

    std::ifstream fin(movieFileName);
    if (!fin.is_open()) {
        return loadedMoviesCount;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(line);

        std::string title, genre, trailer;
        int year = 0, likes = 0;

        // Parse title
        if (std::getline(tokenStream, token, ',')) {
            title = token;
            title.erase(0, title.find_first_not_of(" \t"));
            title.erase(title.find_last_not_of(" \t") + 1);
        }

        // Parse genre
        if (std::getline(tokenStream, token, ',')) {
            genre = token;
            genre.erase(0, genre.find_first_not_of(" \t"));
            genre.erase(genre.find_last_not_of(" \t") + 1);
        }

        // Parse year
        if (std::getline(tokenStream, token, ',')) {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            try {
                year = std::stoi(token);
            } catch (...) {
                year = 2020;
            }
        }

        // Parse likes
        if (std::getline(tokenStream, token, ',')) {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            try {
                likes = std::stoi(token);
            } catch (...) {
                likes = 0;
            }
        }

        // Parse trailer
        if (std::getline(tokenStream, trailer)) {
            // Trim whitespace
            trailer.erase(0, trailer.find_first_not_of(" \t"));
            trailer.erase(trailer.find_last_not_of(" \t") + 1);
        }

        Movie movie(title, genre, year, likes, trailer);
        if (std::find(movieList.begin(), movieList.end(), movie) == movieList.end()) {
            movieList.push_back(movie);
            loadedMoviesCount++;
        }
    }

    fin.close();
    return loadedMoviesCount;
}


//Write movies to given file
void Repository::writeMoviesToFile()
{
    if (movieFileName == "")
        return;

    std::ofstream fout(movieFileName);
    if (!fout.is_open()) {
        return;
    }

    for (const Movie& movieToWrite : movieList) {
        fout << movieToWrite.getTitle() << ", "
             << movieToWrite.getGenre() << ", "
             << movieToWrite.getYearOfRelease() << ", "
             << movieToWrite.getNumberOfLikes() << ", "
             << movieToWrite.getTrailer() << "\n";
    }

    fout.close();
}

//Repository constructor
Repository::Repository(const std::string& nameOfTheFileUsed)
{
    movieFileName = nameOfTheFileUsed;

    // Only attempt to load if a filename was provided
    if (!movieFileName.empty()) {
        int moviesLoaded = loadMoviesFromFile();
        std::cout << "Loaded " << moviesLoaded << " movies from file." << std::endl;
    }

}

//Repository destructor
Repository::~Repository()
{
    writeMoviesToFile();
}


//Add given movie to list
int Repository::addMovie(const Movie& movieToAdd)
{
    auto iteratorWhereMovieIsFound = std::find(movieList.begin(), movieList.end(),  movieToAdd);
    if (iteratorWhereMovieIsFound !=  movieList.end() && movieList.size() != 0)
        throw RepositoryException("Movie already exists in the repository!");
    movieList.push_back(movieToAdd);
    writeMoviesToFile();
    return 1;
}



//Delete a movie from the movieList
int Repository::deleteMovie(const Movie& movieToDelete)
{
    auto iteratorWhereMovieIsFound = std::find(movieList.begin(), movieList.end(),  movieToDelete);
    if (iteratorWhereMovieIsFound == movieList.end())
        throw RepositoryException("Movie does not exist in the repository!");
    movieList.erase(iteratorWhereMovieIsFound);
    writeMoviesToFile();
    return 1;
}


//Update a movie from the movieList
int Repository::updateMovie(const Movie& movieToUpdateWith)
{
    auto iteratorWhereMovieIsFound = std::find(movieList.begin(), movieList.end(),  movieToUpdateWith);
    if (iteratorWhereMovieIsFound == movieList.end())
        throw RepositoryException("Movie does not exist in the repository!");
    *iteratorWhereMovieIsFound = movieToUpdateWith;
    writeMoviesToFile();
    return 1;
}

//Change the file name of the repository
void Repository::changeFileName(const std::string& nameOfTheFileUsed)
{

    movieFileName = nameOfTheFileUsed;
    writeMoviesToFile();
}


//Get the movie at a certain position
Movie Repository::getMovieAtPosition(int positionOfMovie)
{
    if (positionOfMovie < 0 || positionOfMovie >= movieList.size())
        throw RepositoryException("Invalid position for movie access!");
    return movieList[positionOfMovie];
}

//Get the number of movies in the list
int Repository::getNumberOfMovies()
{
    return movieList.size();
}

//Get all the movies with a given genre
std::vector<Movie> Repository::getMoviesByGenre(const std::string& genreGiven)
{
    std::vector<Movie> moviesWithGenre;
    if (genreGiven == "")
        return movieList;
    std::copy_if(movieList.begin(), movieList.end(), std::back_inserter(moviesWithGenre), [&genreGiven](const Movie& movie) {return movie.getGenre() == genreGiven; });
    return moviesWithGenre;
}

//Get all the movies
std::vector<Movie> Repository::getAllMovies()
{
    return movieList;
}


//Get all the movies in the watch list
std::vector<Movie> Repository::getAllWatchListMovies()
{
    return userWatchList;
}


//Add a certain movie to the watch list
int Repository::addMovieToWatchlist(const Movie& movieToAdd)
{
    auto iteratorWhereMovieIsFound = std::find(userWatchList.begin(), userWatchList.end(), movieToAdd);
    if (iteratorWhereMovieIsFound != userWatchList.end())
        throw RepositoryException("Movie already exists in watchlist!");
    userWatchList.push_back(movieToAdd);
    return 1;
}


//Add a certain movie to the watch list by the title
int Repository::addMovieToWatchListByTitle(const std::string& titleOfTheMovieToAdd)
{
    auto iteratorWhereMovieFound = std::find_if(movieList.begin(), movieList.end(), [&titleOfTheMovieToAdd](const Movie& movie) {return movie.getTitle() == titleOfTheMovieToAdd; });
    if (iteratorWhereMovieFound == movieList.end())
        throw RepositoryException("Movie with title '" + titleOfTheMovieToAdd + "' does not exist in repository!");
    userWatchList.push_back(*iteratorWhereMovieFound);
    return 1;
}

//Get the number of movies in the watch list
int Repository::getNumberOfMoviesWatchList()
{
    return userWatchList.size();
}

//Delete a movie from the watch list by title
int Repository::deleteMovieFromWatchlist(const std::string& title)
{
    auto iterator = std::find_if(userWatchList.begin(), userWatchList.end(),
                                 [&title](const Movie& movie) { return movie.getTitle() == title; });

    if (iterator == userWatchList.end())
        throw RepositoryException("Movie with title '" + title + "' does not exist in watchlist!");

    userWatchList.erase(iterator);
    return 1;
}

//Increase likes for a movie in the repository
int Repository::likeMovie(const std::string& title)
{
    auto iterator = std::find_if(movieList.begin(), movieList.end(),
                                 [&title](const Movie& movie) { return movie.getTitle() == title; });

    if (iterator == movieList.end())
        throw RepositoryException("Movie with title '" + title + "' does not exist in repository!");

    Movie updatedMovie = *iterator;
    updatedMovie.setNumberOfLikes(updatedMovie.getNumberOfLikes() + 1);

    *iterator = updatedMovie;
    writeMoviesToFile();
    return 1;
}

void Repository::setWatchlistFileManager(FileManager* fileManager) {
    watchListFileManager = fileManager;
}

void Repository::saveWatchlist() const {
    if (watchListFileManager) {
        watchListFileManager->saveToFile(userWatchList);
    } else {
        throw FileException("No watchlist file manager has been set");
    }
}

void Repository::displayWatchlist() const {
    if (watchListFileManager) {
        watchListFileManager->displayFile();
    } else {
        throw FileException("No watchlist file manager has been set");
    }
}
