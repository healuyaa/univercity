#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <filesystem>
#include <utility>

#include <sqlite3.h>

class VCardSQLite {
public:
    VCardSQLite(const char* dbPath) : dbPath_(dbPath), db_(nullptr) {}

    ~VCardSQLite() {
        closeDatabase();
    }

    bool openDatabase() {
        if (sqlite3_open(dbPath_, &db_) != SQLITE_OK) {
            std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
            return false;
        }
        return true;
    }

    void closeDatabase() {
        if (db_) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
    }

    bool createTable() {
        const char* createTableSQL =
            "CREATE TABLE IF NOT EXISTS Contacts ("
            "FullName TEXT,"
            "PhoneNumber TEXT,"
            "Address TEXT,"
            "Birthday TEXT,"
            "ID INTEGER PRIMARY KEY AUTOINCREMENT"
            ");";

        return executeSQL(createTableSQL);
    }


    bool insertData(const char* fullName, const char* phoneNumber, const char* address, const char* birthday) {
        const char* insertDataSQL = "INSERT INTO Contacts (FullName, PhoneNumber, Address, Birthday) "
                                    "VALUES (?, ?, ?, ?);";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, insertDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, fullName, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 2, phoneNumber, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, address, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 4, birthday, -1, SQLITE_STATIC);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }


    void printVCard(int contactID) {
        const char* selectDataSQL = "SELECT * FROM Contacts WHERE ID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, selectDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(statement, 1, contactID);

            if (sqlite3_step(statement) == SQLITE_ROW) {
                std::cout << "FN:" << sqlite3_column_text(statement, 1) << std::endl;
                std::cout << "TEL:" << sqlite3_column_text(statement, 2) << std::endl;
                std::cout << "ADR:" << sqlite3_column_text(statement, 3) << std::endl;                
                std::cout << "BDAY:" << sqlite3_column_text(statement, 4) << std::endl;
            } else {
                std::cout << "Contact with ID " << contactID << " not found." << std::endl;
            }

            sqlite3_finalize(statement);
        }
    }

    bool updateData(int contactID, const char* fullName, const char* phoneNumber, const char* address, const char* birthday) {
        const char* updateDataSQL = "UPDATE Contacts SET FullName = ?, PhoneNumber = ?, Address = ?, Birthday = ? WHERE ID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, updateDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, fullName, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 2, phoneNumber, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, address, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 4, birthday, -1, SQLITE_STATIC);
            sqlite3_bind_int(statement, 5, contactID);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }

    bool deleteData(int contactID) {
        const char* deleteDataSQL = "DELETE FROM Contacts WHERE ID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, deleteDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(statement, 1, contactID);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }

    std::vector<std::pair<int, std::vector<std::string>>> searchData(const char* searchTerm) {
    std::vector<std::pair<int, std::vector<std::string>>> searchResults;

    const char* searchDataSQL = "SELECT id, * FROM Contacts WHERE FullName LIKE ? OR PhoneNumber LIKE ? OR Address LIKE ? OR Birthday LIKE ?;";

    sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, searchDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, searchTerm, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 2, searchTerm, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, searchTerm, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 4, searchTerm, -1, SQLITE_STATIC);

            while (sqlite3_step(statement) == SQLITE_ROW) {
                int id = sqlite3_column_int(statement, 0);
                std::vector<std::string> result;
                result.push_back(std::to_string(id));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4)));

                searchResults.push_back(std::make_pair(id, result));
            }

            sqlite3_finalize(statement);
        }

        return searchResults;
    }

    std::vector<std::pair<int, std::vector<std::string>>> selectAllData() {
        std::vector<std::pair<int, std::vector<std::string>>> allData;

        const char* selectAllDataSQL = "SELECT id, * FROM Contacts;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, selectAllDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int id = sqlite3_column_int(statement, 0);
                std::vector<std::string> result;
                result.push_back(std::to_string(id));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4)));

                allData.push_back(std::make_pair(id, result));
            }

            sqlite3_finalize(statement);
        }

        return allData;
    }

    std::vector<std::string> selectDataById(int contactID) {
        std::vector<std::string> resultData;

        const char* selectDataSQL = "SELECT * FROM Contacts WHERE ID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, selectDataSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(statement, 1, contactID);

            if (sqlite3_step(statement) == SQLITE_ROW) {
                for (int i = 0; i < sqlite3_column_count(statement) - 1; ++i) {
                    resultData.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, i)));
                }
            } else {
                std::cout << "Contact with ID " << contactID << " not found." << std::endl;
            }

            sqlite3_finalize(statement);
        }

        return resultData;
    }

private:
    const char* dbPath_;
    sqlite3* db_;

    bool executeSQL(const char* sql) {
        if (sqlite3_exec(db_, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
            std::cerr << "SQL execution error: " << sqlite3_errmsg(db_) << std::endl;
            return false;
        }
        return true;
    }
};

struct Contact {
    std::string Fn;
    std::string Tel;
    std::string Adr;
    std::string BDay;
};

class VCardParser {
public:
    static void parseVCard(const std::string& filename, Contact& contact) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::string key, value;            
            size_t pos = line.find(':');

            if (pos != std::string::npos) {
                key = line.substr(0, pos + 1);
                value = line.substr(pos + 1);

                processKeyValue(contact, key, value);
            }
        }
        
        file.close();
    }

    static void writeVCard(const std::string file_name, const std::vector<std::string> answ) {
        std::ofstream vcfFile(file_name);

        if (!vcfFile.is_open()) {
            std::cerr << "Error: Unable to open the VCard file." << std::endl;
            return;
        }

        vcfFile << "BEGIN:VCARD" << std::endl;
        vcfFile << "VERSION:3.0" << std::endl;
        vcfFile << "FN:" << answ[0] << std::endl;
        vcfFile << "ORG:" << std::endl;
        vcfFile << "TITLE:" << std::endl;
        vcfFile << "TEL:" << answ[1] << std::endl;
        vcfFile << "EMAIL:" << std::endl;
        vcfFile << "ADR:" << answ[2] << std::endl;
        vcfFile << "URL:" << std::endl;
        vcfFile << "BDAY:" << answ[3] << std::endl;
        vcfFile << "PHOTO;TYPE=JPEG:" << std::endl;
        vcfFile << "NOTE:" << std::endl;
        vcfFile << "CATEGORY:" << std::endl;
        vcfFile << "LABEL:" << std::endl;
        vcfFile << "END:VCARD" << std::endl;

        vcfFile.close();
        std::cout << "VCard file has been successfully created: " << file_name << std::endl;
    }

private:
    static void processKeyValue(Contact& contact, const std::string& key, const std::string& value) {
        if (key == "FN:") {
            contact.Fn = value;
        } else if (key == "TEL:") {
            contact.Tel = value;
        } else if (key == "ADR:") {
            contact.Adr = value;
        } else if (key == "BDAY:") {
            contact.BDay = value;
        }
    }
};