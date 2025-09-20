#include "cli_database.hxx"
//#include <iostream>

//cli_database::cli_database() = default;

//void cli_database::set(const std::string& key, const std::string& value) {
//    data[key] = value;
//    std::cout << "✓ Set '" << key << "' = '" << value << "'" << std::endl;
//}

//std::string cli_database::get(const std::string& key) const {
//    auto it = data.find(key);
//    if (it != data.end()) {
//        return it->second;
//    }
//    return "";
//}

//void cli_database::remove(const std::string& key) {
//    if (data.erase(key) > 0) {
//        std::cout << "✓ Deleted key '" << key << "'" << std::endl;
//    } else {
//        std::cout << "Key '" << key << "' not found." << std::endl;
//    }
//}

//void cli_database::list() const {
//    if (data.empty()) {
//        std::cout << "Database is empty." << std::endl;
//    } else {
//        std::cout << "Database contents:" << std::endl;
//        for (const auto& pair : data) {
//            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
//        }
//    }
//}

//bool cli_database::exists(const std::string& key) const {
//    return data.find(key) != data.end();
//}

//size_t cli_database::size() const {
//    return data.size();
//}

//void cli_database::clear() {
//    data.clear();
//}
