#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

bool is_cuphead_directory(const std::filesystem::path& dir) {
    return std::filesystem::is_directory(dir) && std::filesystem::exists(dir / "Cuphead.exe") &&
           std::filesystem::exists(dir / "Cuphead_Data/Managed/");
}

std::filesystem::path open_file(const std::wstring_view& title, const std::wstring_view& filter) {
    wchar_t file[MAX_PATH]{0};

    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.hInstance = nullptr;
    ofn.lpstrFilter = filter.data();
    ofn.lpstrFile = file;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.lpstrTitle = title.data();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    while (!GetOpenFileNameW(&ofn))
        ;

    return std::filesystem::path(file);
}

std::string get_now_time_string() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%F_%H-%M-%S");
    return oss.str();
}

int main(int argc, char* argv[]) {
    std::cout << "正在寻找游戏文件...\n";
    std::filesystem::path game_path = std::filesystem::current_path();
    if (!is_cuphead_directory(game_path)) {
        std::cout << "未寻找到游戏文件，请手动选择\n";
        do {
            game_path = open_file(L"请选择游戏文件", L"游戏文件（Cuphead.exe）\0Cuphead.exe\0")
                            .parent_path();
        } while (!is_cuphead_directory(game_path));
    }

    std::cout << "游戏目录：" << game_path << "\n";

    std::filesystem::path mod_file;
    if (argc > 1) {
        mod_file = argv[1];
    }
    else {
        std::cout << "未拖入mod文件，请手动选择\n";
        mod_file = open_file(L"选择要安装的mod文件", L"MOD文件（*.dll）\0*.dll\0");
    }

    std::cout << "MOD文件：" << mod_file << "\n";
    std::filesystem::path target_file = game_path / "Cuphead_Data/Managed/Assembly-CSharp.dll";
    if (!std::filesystem::exists("backup")) {
        std::filesystem::create_directory("backup");
    }
    std::string backup_file = std::format("backup/Assembly-CSharp-{}.dll", get_now_time_string());
    std::filesystem::copy_file(target_file, backup_file);
    std::filesystem::remove(target_file);
    std::filesystem::copy_file(mod_file, target_file);
    std::cout << "安装完成！\n原本的游戏文件已备份在backup文件夹中\n请按任意键退出...";
    std::getchar();

    return 0;
}