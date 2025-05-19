#pragma once
#include <string>
#include <vector>
#include "movie.h"
#include "Exceptions.h"

class FileManager{
protected:
    std::string filePath;

public:
    explicit FileManager(const std::string& filePath) : filePath(filePath) {}
    virtual ~FileManager() = default;

    virtual void saveToFile(const std::vector<Movie>& watchList) = 0;
    virtual void displayFile() const = 0;
    std::string getFilePath() const { return filePath; }
};

class CSVFileManager : public FileManager{
public:
    explicit CSVFileManager(const std::string& filePath);
    void saveToFile(const std::vector<Movie>& watchList) override;
    void displayFile() const override;
};

class HTMLFileManager : public FileManager{
public:
    explicit HTMLFileManager(const std::string& filePath);
    void saveToFile(const std::vector<Movie>& watchList) override;
    void displayFile() const override;
};
