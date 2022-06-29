//taken from the code provided in the course, but modifications were made - my additions are highlighted with comments

#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <map>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);
        /** return vector of all known products in the dataset*/
        std::vector<std::string> products();

        //My code unless commented otherwise:
        /** return vector of all unique time stamps*/
        std::vector<std::string> timeStamps();
        /** create map of vectors where each element contains all Order Book Entries of each unique timestamp*/
        std::map<std::string, std::vector<OrderBookEntry>> OrdersByTimestamp();
        /** returns the initial current time in advisor bot*/
        std::string getCurrentTime();
        /** returns the next timeframe*/
        std::string nextTimeframe(std::string timestamp);
        /** return minimum or maximum value of sent product and OrderBookType for current time*/
        double minMax(std::string command, 
                      std::string product, 
                      std::string transaction);
        /** return average value for sent product and OrderBookType for the sent number of past time steps, including the current one */
        double average(std::string type, 
                       std::string product,
                       int timesteps);
        /** return spread between the max bid and the min ask for sent product and specified past or current time step */
        double BAspread(std::string product,
                                   int timesteps);

    private:
        /** variable used in constructor, storing all OrderBook entries of csv file*/
        std::vector<OrderBookEntry> orders; //taken from given code

        /** variable used to store the output of OrdersByTimestamp()*/
        std::map<std::string, std::vector<OrderBookEntry>> OrdersByTime;
        /** vector of all unique timeStamps in order book*/
        std::vector<std::string> timeSteps;
        /** int variable used in nextTimeframe function*/
        int timeIterator;
        /** return vector of all orders in the sent timestamp*/
        std::vector<OrderBookEntry> sentTimeOrders(std::string timestamp);
        /** return vector of all orders in the sent subset that match the sent product */
        std::vector<OrderBookEntry> sentProductOrders(std::vector<OrderBookEntry> ordersSubset,
                                                      std::string product);
        /** return vector of all orders in the sent subset that match the sent OrderBookType */
        std::vector<OrderBookEntry> sentTypeOrders(std::vector<OrderBookEntry> ordersSubset,
                                                   OrderBookType type);
        /** return vector of all orders in the sent timestamp, also filtered by sent product and OrderBookType*/
        std::vector<OrderBookEntry> ordersByFilters(std::string timestamp, 
                                                    std::string product, 
                                                    OrderBookType type);
};