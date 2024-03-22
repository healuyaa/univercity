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

class HospitalDatabase {
public:
    HospitalDatabase(const char* dbPath) : dbPath_(dbPath), db_(nullptr) {}

    ~HospitalDatabase() {
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
            "CREATE TABLE IF NOT EXISTS Patients ("
            "FullName TEXT,"
            "Address TEXT,"
            "PhoneNumber TEXT,"
            "Birthday TEXT,"
            "DateVisit TEXT,"
            "PatientID INTEGER PRIMARY KEY AUTOINCREMENT"
            ");";

        return executeSQL(createTableSQL);
    }

    bool insertPatient(const char* fullName, const char* address, const char* phoneNumber, const char* birthday, const char* dateVisit) {
        const char* insertPatientSQL = "INSERT INTO Patients (FullName, Address, PhoneNumber, Birthday, DateVisit) "
                                    "VALUES (?, ?, ?, ?, ?);";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, insertPatientSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, fullName, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 2, address, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, phoneNumber, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 4, birthday, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 5, dateVisit, -1, SQLITE_STATIC);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }

    bool updatePatientInfo(int patientID, const char* fullName, const char* address, const char* phoneNumber, const char* birthday, const char* dateVisit) {
        const char* updatePatientSQL = "UPDATE Patients SET FullName = ?, Address = ?, PhoneNumber = ?, Birthday = ?, DateVisit = ? WHERE PatientID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, updatePatientSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(statement, 1, fullName, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 2, address, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 3, phoneNumber, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 4, birthday, -1, SQLITE_STATIC);
            sqlite3_bind_text(statement, 5, dateVisit, -1, SQLITE_STATIC);
            sqlite3_bind_int(statement, 6, patientID);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }


    bool deletePatient(int patientID) {
        const char* deletePatientSQL = "DELETE FROM Patients WHERE PatientID = ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, deletePatientSQL, -1, &statement, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(statement, 1, patientID);

            int result = sqlite3_step(statement);
            sqlite3_finalize(statement);

            return result == SQLITE_DONE;
        }

        return false;
    }

    std::vector<std::vector<std::string>> searchPatients(const char* searchTerm) {
        std::vector<std::vector<std::string>> searchResults;

        const char* searchPatientSQL = "SELECT * FROM Patients WHERE FullName LIKE ? OR Address LIKE ? OR PhoneNumber LIKE ? OR Birthday LIKE ? OR DateVisit LIKE ?;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, searchPatientSQL, -1, &statement, nullptr) == SQLITE_OK) {
            std::string searchTermWithWildcard = std::string("%") + searchTerm + "%";
            for (int i = 1; i <= 5; ++i) {
                sqlite3_bind_text(statement, i, searchTermWithWildcard.c_str(), -1, SQLITE_STATIC);
            }

            while (sqlite3_step(statement) == SQLITE_ROW) {
                std::vector<std::string> result;
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 5)));

                searchResults.push_back(result);
            }

            sqlite3_finalize(statement);
        }

        return searchResults;
    }


    std::vector<std::vector<std::string>> selectAllPatients() {
        std::vector<std::vector<std::string>> allPatients;

        const char* selectAllPatientsSQL = "SELECT * FROM Patients;";

        sqlite3_stmt* statement;
        if (sqlite3_prepare_v2(db_, selectAllPatientsSQL, -1, &statement, nullptr) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                std::vector<std::string> result;
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4)));
                result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 5)));

                allPatients.push_back(result);
            }

            sqlite3_finalize(statement);
        }
        return allPatients;
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