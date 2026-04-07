#pragma once
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "config.hpp"

namespace fs = std::filesystem;

class UpdateEngine {
public:
    static bool extract(const std::string& archivePath, const std::string& destination) {
        struct archive* a;
        struct archive* ext;
        struct archive_entry* entry;
        int flags;
        int r;

        flags = ARCHIVE_EXTRACT_TIME;
        flags |= ARCHIVE_EXTRACT_PERM;
        flags |= ARCHIVE_EXTRACT_ACL;
        flags |= ARCHIVE_EXTRACT_FFLAGS;

        a = archive_read_new();
        archive_read_support_format_all(a);
        archive_read_support_filter_all(a);
        ext = archive_write_disk_new();
        archive_write_disk_set_options(ext, flags);
        archive_write_disk_set_standard_lookup(ext);

        if ((r = archive_read_open_filename(a, archivePath.c_str(), 10240)))
            return false;

        std::vector<std::string> ignoreList = loadIgnoreList();
        std::vector<std::string> cleanedFolders;

        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            std::string entryName(archive_entry_pathname(entry));

            size_t firstSlash = entryName.find('/');
            if (firstSlash != std::string::npos) {
                std::string rootFolder = entryName.substr(0, firstSlash);
                if (std::find(cleanedFolders.begin(), cleanedFolders.end(), rootFolder) == cleanedFolders.end()) {
                    if (!isIgnored(rootFolder, ignoreList)) {
                        fs::remove_all(destination + "/" + rootFolder);
                        cleanedFolders.push_back(rootFolder);
                    }
                }
            }

            std::string fullDest = destination + "/" + entryName;
            archive_entry_set_pathname(entry, fullDest.c_str());
            r = archive_read_extract(a, entry, flags);
        }

        archive_read_close(a);
        archive_read_free(a);
        archive_write_close(ext);
        archive_write_free(ext);
        return true;
    }

private:
    static std::vector<std::string> loadIgnoreList() {
        std::vector<std::string> list;
        std::ifstream file("/.updateignore");
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] != '#') list.push_back(line);
        }
        return list;
    }

    static bool isIgnored(const std::string& path, const std::vector<std::string>& ignoreList) {
        if (Config::getInstance().rewrite) return false;
        for (const auto& ignore : ignoreList) {
            if (path == ignore) return true;
        }
        return false;
    }
};
