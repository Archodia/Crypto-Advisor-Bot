//taken from the code provided in the course, but modifications were made - my added code is highlighted via comments

#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric>


/** constructor, reads the csv data file and initialises vector timeSteps and int timeIterator*/
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
    //my code
    timeSteps = timeStamps();
    timeIterator = 9;
    OrdersByTime = OrdersByTimestamp();
    //end of my code
}

/** return vector of all known products in the dataset*/
std::vector<std::string> OrderBook::products()
{
    std::vector<std::string> products;
    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }
    return products;
}

//Below follows my code, unless stated otherwise:

//the below function follows same logic as products()
/** return vector of all unique time stamps*/ 
std::vector<std::string> OrderBook::timeStamps() 
{
    std::vector<std::string> timeSteps;
    std::map<std::string,bool> timeMap;

    for (OrderBookEntry& e : orders)
    {
        timeMap[e.timestamp] = true;
    }
    // flatten the map to create a vector of strings of all unique timestamps
    for (auto const& e : timeMap)
    {
        timeSteps.push_back(e.first);
    }
    return timeSteps;
}

/** returns string of initial Current time*/
std::string OrderBook::getCurrentTime()
{
    return timeSteps[timeIterator];
}  
// initial current time selected as timeSteps[9], so that there is some historical data (for at least 10 timesteps including current one)
// for commands like avg/predict, even with Initial current time and before the user chooses to move to a further timestep                        


/** returns the next timeframe after the input timestamp
* If there is no next timestamp, wraps around to the start */
std::string OrderBook::nextTimeframe(std::string timestamp)
{
    timeIterator ++;
    if (timeIterator > timeSteps.size() - 1)
    {
        timeIterator = 0;
    }
    return timeSteps[timeIterator];
}

/** return minimum or maximum value of sent product and OrderBookType for current time*/
double OrderBook::minMax(std::string command, 
                         std::string product, 
                         std::string transaction)
{
    if (command == "min")
    {
        OrderBookType type = OrderBookEntry::stringToOrderBookType(transaction); // converts string to OrderBookType
        std::vector<OrderBookEntry> subOrders;
        std::string time = getCurrentTime();
        subOrders = ordersByFilters(time, 
                                    product, 
                                    type); // get subset of orders of current time, sent product and type

        double min = subOrders[0].price;
        for (OrderBookEntry& e : subOrders)
        {
            if (e.price < min) min = e.price;
        }
        std::cout << "The minimum " << transaction << " price for " << product << " in the current time, is: " << min << std::endl;
        return min;
    }
    else // only other option is max so no need to specify it through if condition
    {
        OrderBookType type = OrderBookEntry::stringToOrderBookType(transaction); // converts string to OrderBookType
        std::vector<OrderBookEntry> subOrders;
        std::string time = getCurrentTime();
        subOrders = ordersByFilters(timeSteps[timeIterator], 
                                    product, 
                                    type); // get subset of orders of current time, sent product and type

        double max = subOrders[0].price;
        for (OrderBookEntry& e : subOrders)
        {
            if (e.price > max) max = e.price;
        }
        std::cout << "The maximum " << transaction << " price for " << product << " in the current time, is: " << max << std::endl;
        return max;
    }
    
}

/** return average value for sent product and OrderBookType for the sent number of past time steps, including the current one */
double OrderBook::average(std::string type, 
                      std::string product,
                      int timesteps)
{
    int iterator = timeIterator + 1 - timesteps;
    if (iterator < 0) //not enough historic data to go back to for the number of timesteps requested
    {
        std::cout << "The number of time steps inserted is greater than the number of existing past time steps. " << std::endl;
        std::cout << "Please insert a number of time steps less than or equal to: " << timeIterator + 1 << std::endl;
        std::cout << "Otherwise use command step as many times as required." << std::endl;
    }
    else{
        OrderBookType orderType = OrderBookEntry::stringToOrderBookType(type);
        std::vector<double> prices;
        std::vector<OrderBookEntry> subOrders;
        for (long unsigned int i = iterator; i < iterator+timesteps; ++i)
        {
            subOrders = ordersByFilters(timeSteps[i], 
                                        product, 
                                        orderType); // get subset of orders of timeSteps[i] time, sent product and type
            for (OrderBookEntry& e : subOrders)
            {
                prices.push_back(e.price);
            }
        }
        double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
        double avg =  sum / prices.size();
        std::cout << "The average " << type << " for " << product << " over the " << timesteps << " past time steps is: " << avg << std::endl;
        return avg;
    }   
}

/** return spread between the max bid and the min ask for sent product and specified past or current time step */
double OrderBook::BAspread(std::string product,
                           int timesteps)
{
    int iterator = timeIterator - timesteps;
    if (iterator < 0) // cannot go back to the timestep requested
    {
        std::cout << "Cannot go back this number of time steps." << std::endl;
        std::cout << "Please insert a number less than or equal to: " << timeIterator + 1 << std::endl;
        std::cout << "Otherwise use command step as many times as required." << std::endl;
    }
    else{
        double spread;
        std::vector<OrderBookEntry> ordersByTime = sentTimeOrders(timeSteps[timeIterator]);
        std::vector<OrderBookEntry> ordersByProduct = sentProductOrders(ordersByTime, product); // vector of orders for specified time step and product
        double maxBid = ordersByProduct[0].price;
        double minAsk = ordersByProduct[0].price;
        
        for (OrderBookEntry& e : ordersByProduct)
        {
            if (e.price < minAsk) minAsk = e.price;
            if (e.price > maxBid) maxBid = e.price;
        }
        spread = maxBid - minAsk;
        std::cout << "The bid/ask spread for " << product << ", " << timesteps << " time steps back is: " << spread << std::endl;
        return spread;
    } 
}

// Below are all the helper functions that I created to optimise the code
// These enable the program to search for intended values in selected subsets of orders, 
// instead of searching through the whole OrderBook every time

/** create map of strings and vectors
 *  Each element's key represents a unique timestamp and is mapped with a vector of all Order Book Entries within this time*/
std::map<std::string, std::vector<OrderBookEntry>> OrderBook::OrdersByTimestamp()
{
    std::map<std::string, std::vector<OrderBookEntry>> ordersByTimestamp;
    std::vector<OrderBookEntry> subOrders;
    int iterator = 0;

    for (unsigned int i = 1; i <= orders.size(); ++i)
    {
        if (i == orders.size())
        {
            ordersByTimestamp[timeSteps[iterator]] = subOrders; // maps the last orders_sub vector to last timeStep, vector finalised in previous iteration,
        }                                                        // pushing the last OrderBookEntry of vector orders
        else{                                                        // pushing the last OrderBookEntry of vector orders
            if (orders[i].timestamp == timeSteps[iterator])
            {
                subOrders.push_back(orders[i]);
            }
            if (orders[i].timestamp > timeSteps[iterator]) // if all orders of the same time stamp have been passed and now we are at a new timestamp
            {
                ordersByTimestamp[timeSteps[iterator]] = subOrders; // map the orders_sub vector created till previous iteration to the time step we just passed
                subOrders.erase (subOrders.begin(), subOrders.end()); // erase all elements of orders_sub vector so new OrderBookEntries are pushed from scratch
                subOrders.push_back(orders[i]); //OrderBookEntry of orders[i] is pushed to empty orders_sub vector
                iterator++; //increase iterator to access next element of vector timeSteps in the nect for iteration - should be equal to current time stamp
            }
        }    
    }
    return ordersByTimestamp;
}

/** return vector of all products in the sent timestamp*/
std::vector<OrderBookEntry> OrderBook::sentTimeOrders(std::string timestamp)
{
    std::vector<OrderBookEntry> subOrders;
       subOrders = OrdersByTime.find(timestamp)->second;
    return subOrders;
}

/** return vector of all orders in the sent subset that match the sent product */
std::vector<OrderBookEntry> OrderBook::sentProductOrders(std::vector<OrderBookEntry> ordersSubset,
                                                         std::string product)
{
    std::vector<OrderBookEntry> subOrders;
    for (OrderBookEntry& e : ordersSubset)
    {
        if (e.product == product)
        {
            subOrders.push_back(e);
        }
    }
    return subOrders;
}

/** return vector of all orders in the sent subset that match the sent OrderBookType */
std::vector<OrderBookEntry> OrderBook::sentTypeOrders(std::vector<OrderBookEntry> ordersSubset,
                                                      OrderBookType type)
{
    std::vector<OrderBookEntry> subOrders;
    for (OrderBookEntry& e : ordersSubset)
    {
        if (e.orderType == type)
        {
            subOrders.push_back(e);
        }
    }
    return subOrders;
}

/** return vector of all orders in the sent timestamp, also filtered by sent product and OrderBookType*/
std::vector<OrderBookEntry> OrderBook::ordersByFilters(std::string timestamp, 
                                                       std::string product, 
                                                       OrderBookType type)
{
    std::vector<OrderBookEntry> ordersByTime = sentTimeOrders(timestamp);
    std::vector<OrderBookEntry> ordersByProduct = sentProductOrders(ordersByTime, product);
    std::vector<OrderBookEntry> subOrders = sentTypeOrders(ordersByProduct, type);
    return subOrders;
}