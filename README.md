# Movie Manager (Qt / C++)

A desktop Movie Manager application written in C++20 using Qt 6 (Widgets). Provides administrator and user interfaces to manage a movie catalog, maintain a watchlist, and persist watchlists in CSV or HTML formats.

## Features
- Administrator
  - Add, update, delete movies
  - Load repository from a file (`movies.txt`)
  - Undo / redo operations
- User
  - Browse and filter movies by genre
  - View movie details and open trailers
  - Add / remove movies from a personal watchlist
  - Save and display watchlist in `CSV` or `HTML`
- Watchlist window with table view and undo/redo buttons

## Architecture
- Core: `Repository`, `AdminService`, `UserService`
- Persistence: `FileManager` implementations (`CSVFileManager`, `HTMLFileManager`)
- GUI: `main.cpp`, `WatchlistWindow.h/.cpp`, `WatchlistModel.h/.cpp`, `AdminWidget` and `UserWidget` in `main.cpp` (or split)
- Model/View: `WatchlistTableModel` backed by repository/watchlist

## Technologies
- C++20
- Qt 6 (Qt Widgets)
- CMake (with `AUTOMOC`)
- Built with CLion on macOS (arm64 / Apple Silicon)

## Prerequisites
- Qt 6 development libraries installed and visible to CMake
- CMake (>= 3.16) and Ninja or another generator
- CLion or another IDE (optional)
