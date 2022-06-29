#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

class AdvisorBot
{
    public:
        AdvisorBot();
        /** Calling this starts the Advisor Bot simulation */
        void init();
        /** string variable for current time */
        std::string currentTime;
    private: 
        /** gets User's line input */
        std::vector<std::string> getUserOption();
        /** processes User's line input */
        void processUserOption(std::vector<std::string> userOption);
        /** prints help menu - list of all available commands */
        void printHelp();
        /** provides helpful instructions for each command - including itself */
        void helpCmd(std::vector<std::string> userOption);
        /** prints all available products */
        void prod();
        /** prints current time */
        void time();
        /** moves Advisor Bot to next time step */
        void step();
        /** prints min product bid/ask of current time step*/
        void min(std::vector<std::string> userOption);
        /** prints max product bid/ask of current time step */
        void max(std::vector<std::string> userOption);
        /** prints avg bid/ask for sent product of sent numbers of time steps */
        void avg(std::vector<std::string> userOption);
        /** prints bid-ask spread for sent product of current or specified previous time step */
        void spread(std::vector<std::string> userOption);

        /** string variable for available products */
        std::vector<std::string> products;
        /** constructs Ordebook by parsing through given file, 20200602.csv */
        OrderBook orderBook{"20200601.csv"};

};