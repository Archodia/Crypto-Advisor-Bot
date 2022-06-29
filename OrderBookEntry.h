// below code taken from the code provided in the course
// no changes made apart from removing username parameter as it is not needed for Advisor Bot

#pragma once

#include <string>

enum class OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry
{
    public:

        OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType);

        static OrderBookType stringToOrderBookType(std::string s);

        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
};