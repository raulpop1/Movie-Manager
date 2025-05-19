#pragma once
#include "AdminService.h"
#include "UserService.h"
#include <fstream>
#include <string>
#include "FileManager.h"
class UI
{
private:
    AdminService& adminService;
    UserService& userService;
    FileManager* watchlistFileManager;
    void uiSelectFileFormat();
    void uiDisplayWatchlist();

    // Helper function to validate numeric input
    int getValidNumericInput(const std::string& prompt);
public:
    UI(AdminService& adminService, UserService& userService);
    ~UI();
    void runAdmin();
    void runUser();
    void runApp();
    void uiAdminAdd();
    void uiAdminDelete();
    void uiAdminUpdate();
    void uiAdminChangeFile();
    void uiAdminList();
    void uiUserAdd();
    void uiUserWatchList();
    void uiUserNext();
    void uiUserList();
    void uiUserDeleteFromWatchlist();
};
