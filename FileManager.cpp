#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

CSVFileManager::CSVFileManager(const std::string& filePath) : FileManager(filePath + ".csv") {}

void CSVFileManager::saveToFile(const std::vector<Movie>& watchList) {
    std::ofstream fout(filePath);
    if(!fout.is_open()) {
        throw FileException("Could not open file: " + filePath);
    }

    //write csv header
    fout << "Title,Genre,Year,Number of Likes,Trailer\n";

    //write each movie as a csv file
    for(const auto& movie : watchList) {
        fout << movie.getTitle() << ","
             << movie.getGenre() << ","
             << movie.getYearOfRelease() << ","
             << movie.getNumberOfLikes() << ","
             << movie.getTrailer() << "\n";
    }
    fout.close();
}

void CSVFileManager::displayFile() const {
    std::string command = "open \"" + filePath + "\"";
    system(command.c_str());
}

HTMLFileManager::HTMLFileManager(const std::string& filePath) : FileManager(filePath + ".html") {}

void HTMLFileManager::saveToFile(const std::vector<Movie>& watchList) {
    std::ofstream fout(filePath);
    if(!fout.is_open()) {
        throw FileException("Could not open file: " + filePath);
    }

    //write html header
    fout<< "<!DOCTYPE html>\n"
        << "<html>\n"
        << "<head>\n"
        << "    <title>My Watchlist</title>\n"
        << "    <style>\n"
        << "        table { width: 100%; border-collapse: collapse; }\n"
        << "        th, td { padding: 8px; text-align: left; border-bottom: 1px solid #ddd; }\n"
        << "        th { background-color: #f2f2f2; }\n"
        << "        tr:hover { background-color: #f5f5f5; }\n"
        << "    </style>\n"
        << "</head>\n"
        << "<body>\n"
        << "    <h1>My Movie Watchlist</h1>\n"
        << "    <table>\n"
        << "        <tr>\n"
        << "            <th>Title</th>\n"
        << "            <th>Genre</th>\n"
        << "            <th>Year</th>\n"
        << "            <th>Likes</th>\n"
        << "            <th>Trailer</th>\n"
        << "        </tr>\n";

    //write each movie as a table row
    for(const auto& movie : watchList) {
        fout << "        <tr>\n"
             << "            <td>" << movie.getTitle() << "</td>\n"
             << "            <td>" << movie.getGenre() << "</td>\n"
             << "            <td>" << movie.getYearOfRelease() << "</td>\n"
             << "            <td>" << movie.getNumberOfLikes() << "</td>\n"
             << "            <td><a href=\"" << movie.getTrailer() << "\">Watch</a></td>\n"
             << "        </tr>\n";
    }

    //write html footer
    fout << "    </table>\n"
         << "</body>\n"
         << "</html>";
    fout.close();
}

void HTMLFileManager::displayFile() const {
    std::string command = "open \"" + filePath + "\"";
    system(command.c_str());
}