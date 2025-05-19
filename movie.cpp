#include "movie.h"
#include <iostream>

//Movie constructors

Movie::Movie(const std::string& title, const std::string& genre, int yearOfRelease, int numberOfLikes, const std::string& trailer): title(title),
                                                                                                                                    genre(genre),
                                                                                                                                    yearOfRelease(yearOfRelease),
                                                                                                                                    numberOfLikes(numberOfLikes),
                                                                                                                                    trailer(trailer) {}


Movie::Movie(const Movie& movieToUse):title{movieToUse.title},
                                      genre{movieToUse.genre},
                                      yearOfRelease{movieToUse.yearOfRelease},
                                      numberOfLikes{movieToUse.numberOfLikes},
                                      trailer{movieToUse.trailer}
{}
//End of Movie constructors



void Movie::setTitle(const std::string& title)
{
    if(title.empty())
        throw ValidationException("Title cannot be empty!");
    Movie::title = title;

}

void Movie::setGenre(const std::string& genre)
{
    if(genre.empty())
        throw ValidationException("Genre cannot be empty!");
    Movie::genre = genre;
}

void Movie::setYearOfRelease(int yearOfRelease)
{
    if(yearOfRelease < 0 || yearOfRelease > 2025)
        throw ValidationException("Year of release must be between 0 and 2025!");
    Movie::yearOfRelease = yearOfRelease;
}

void Movie::setNumberOfLikes(int numberOfLikes)
{
    if(numberOfLikes < 0)
        throw ValidationException("Number of likes cannot be negative!");
    Movie::numberOfLikes = numberOfLikes;
}

void Movie::setTrailer(const std::string& trailer)
{
    if(trailer.empty())
        throw ValidationException("Trailer cannot be empty!");
    Movie::trailer = trailer;
}

const std::string& Movie::getTitle() const
{
    return title;
}

int Movie::getYearOfRelease() const
{
    return yearOfRelease;
}

const std::string& Movie::getGenre() const
{
    return genre;
}

int Movie::getNumberOfLikes() const
{
    return numberOfLikes;
}

const std::string& Movie::getTrailer() const
{
    return trailer;
}

//Operators overload

bool Movie::operator==(const Movie& movieToCheck) const
{
    return title == movieToCheck.title; //maybe there can be 2 movies with the same title
}

void Movie::operator=(const Movie& movieToGetValuesFrom)
{
    title = movieToGetValuesFrom.getTitle();
    genre = movieToGetValuesFrom.getGenre();
    yearOfRelease = movieToGetValuesFrom.getYearOfRelease();
    numberOfLikes = movieToGetValuesFrom.getNumberOfLikes();
    trailer = movieToGetValuesFrom.getTrailer();
}

bool Movie::operator!=(const Movie& movieToCheck) const
{
    return !(movieToCheck == *this);
}

std::ostream& operator<<(std::ostream& outStream, const Movie& movieToOutput)
{
    outStream << movieToOutput.getOutputForm();
    return outStream;
}

std::istream& operator>> (std::istream& inStream, Movie& movie) {
    std::string movieString;
    std::getline(inStream, movieString);

    if (movieString.empty()) {
        return inStream;
    }

    std::vector<std::string> tokens = movie.explode(movieString, ",");

    if (tokens.size() < 5) {
        return inStream;
    }


    std::for_each(tokens.begin(), tokens.end(), [](std::string& token) {
        auto firstNonSpace = std::find_if(token.begin(), token.end(), [](char c) {
            return c != ' ' && c != '\t';
        });
        token.erase(token.begin(), firstNonSpace);

        auto lastNonSpace = std::find_if(token.rbegin(), token.rend(), [](char c) {
            return c != ' ' && c != '\t';
        }).base();

        token.erase(lastNonSpace, token.end());
    });

    movie.title = tokens[0];
    movie.genre = tokens[1];
    try {
        movie.yearOfRelease = std::stoi(tokens[2]);
        movie.numberOfLikes = std::stoi(tokens[3]);
    } catch (const std::exception&) {
        movie.yearOfRelease = 2020;
        movie.numberOfLikes = 0;
    }
    movie.trailer = tokens[4];

    return inStream;
}

const std::vector<std::string> Movie::explode(const std::string& stringToExplode, const std::string& separatorsUsed)
{
    std::vector<std::string> explodedString;
    size_t startPos = 0;

    while (startPos < stringToExplode.length()) {

        size_t endPos = stringToExplode.find_first_of(separatorsUsed, startPos);

        if (endPos == std::string::npos) {
            endPos = stringToExplode.length();
        }

        std::string token = stringToExplode.substr(startPos, endPos - startPos);
        if (!token.empty()) {
            explodedString.push_back(token);
        }

        startPos = endPos + 1;
    }

    return explodedString;
}


//This is how a movie is represented
std::string Movie::getOutputForm() const
{
    std::string finalOutputForm = "Title: " + title + ", Genre: " + genre +
                                  ", Year of Release: " + std::to_string(yearOfRelease) +
                                  ", Likes: " + std::to_string(numberOfLikes) +
                                  ", Trailer: " + trailer + ";";
    return finalOutputForm;
}
