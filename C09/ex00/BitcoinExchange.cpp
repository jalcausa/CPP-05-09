#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : _database(other._database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        _database = other._database;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::size_t sep = line.find(',');
        if (sep == std::string::npos)
            continue;

        std::string date = line.substr(0, sep);
        std::string rateStr = line.substr(sep + 1);

        float rate = static_cast<float>(std::atof(rateStr.c_str()));
        _database[date] = rate;
    }
    file.close();
}

float BitcoinExchange::getRate(const std::string &date) const
{
    std::map<std::string, float>::const_iterator it = _database.lower_bound(date);

    if (it != _database.end() && it->first == date)
        return it->second;

    if (it == _database.begin())
        return -1.0f; // no earlier date exists

    --it;
    return it->second;
}

bool BitcoinExchange::isValidDate(const std::string &date) const
{
    if (date.size() != 10)
        return false;
    if (date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    int month = std::atoi(date.substr(5, 2).c_str());
    int day   = std::atoi(date.substr(8, 2).c_str());

    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    return true;
}

bool BitcoinExchange::isValidValue(const std::string &valueStr, float &value) const
{
    if (valueStr.empty())
        return false;

    // Check for leading whitespace
    std::size_t start = valueStr.find_first_not_of(" \t");
    if (start == std::string::npos)
        return false;

    std::string trimmed = valueStr.substr(start);

    // Validate characters: optional leading '-', digits, optional single '.'
    std::size_t i = 0;
    bool negative = false;
    if (trimmed[i] == '-')
    {
        negative = true;
        ++i;
    }

    bool hasDot = false;
    bool hasDigit = false;
    for (; i < trimmed.size(); ++i)
    {
        if (trimmed[i] == '.')
        {
            if (hasDot)
                return false;
            hasDot = true;
        }
        else if (trimmed[i] >= '0' && trimmed[i] <= '9')
        {
            hasDigit = true;
        }
        else
        {
            return false;
        }
    }

    if (!hasDigit)
        return false;

    value = static_cast<float>(std::atof(trimmed.c_str()));

    if (negative || value < 0.0f)
        return false; // not a positive number (handled as "not a positive number")

    return true;
}

void BitcoinExchange::processInput(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::size_t sep = line.find(" | ");
        if (sep == std::string::npos)
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date     = line.substr(0, sep);
        std::string valueStr = line.substr(sep + 3);

        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        float value = 0.0f;
        if (!isValidValue(valueStr, value))
        {
            // Determine which error: negative or bad format
            std::size_t start = valueStr.find_first_not_of(" \t");
            if (start != std::string::npos && valueStr[start] == '-')
                std::cout << "Error: not a positive number." << std::endl;
            else
                std::cout << "Error: not a positive number." << std::endl;
            continue;
        }

        if (value > 1000.0f)
        {
            std::cout << "Error: too large a number." << std::endl;
            continue;
        }

        float rate = getRate(date);
        if (rate < 0.0f)
        {
            std::cout << "Error: bad input => " << date << std::endl;
            continue;
        }

        std::cout << date << " => " << value << " = " << value * rate << std::endl;
    }
    file.close();
}
