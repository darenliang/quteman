#ifndef QUTEMAN_UTIL_H
#define QUTEMAN_UTIL_H

#include <QString>
#include <map>


class Util {
public:
    static std::vector<std::pair<std::string, std::string>>
    parseKeyValue(const QString &data) {
        std::vector<std::pair<std::string, std::string>> values;

        std::istringstream stream(data.toStdString());
        std::string line;

        while (getline(stream, line)) {
            auto index = line.find(':');

            // If index not found or is last character
            if (index == std::string::npos || index == line.size() - 1) {
                continue;
            }

            // Trim whitespace
            auto key = line.substr(0, index);
            trim(key);

            auto value = line.substr(index + 1);
            trim(value);

            values.emplace_back(std::move(key), std::move(value));
        }

        return values;
    }

private:
    // Code taken from
    // https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static inline void ltrim(std::string &s) {
        s.erase(
                s.begin(),
                std::find_if(
                        s.begin(), s.end(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))
                )
        );
    }

    static inline void rtrim(std::string &s) {
        s.erase(
                std::find_if(
                        s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(std::isspace))
                ).base(),
                s.end()
        );
    }

    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }
};


#endif //QUTEMAN_UTIL_H
