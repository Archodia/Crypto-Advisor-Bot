#include "AdvisorBot.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "OrderBookEntry.h"
#include "CSVReader.h"

/** constructor */
AdvisorBot::AdvisorBot()
{

}

/** initialises useful variables, puts functions getUserOption and processUserOption in a loop to run the simulation */
void AdvisorBot::init()
{
    std::vector<std::string> input;
    currentTime = orderBook.getCurrentTime();
    products = orderBook.products();

    while(true)
    {
        input = getUserOption();
        processUserOption(input);
    }
}

/** prints help menu - list of all available commands */
void AdvisorBot::printHelp()
{
    std::cout << "The available commands are: " << std::endl;
    std::cout << "help " << std::endl;
    std::cout << "help <cmd> " << std::endl;
    std::cout << "prod " << std::endl;
    std::cout << "min " << std::endl;
    std::cout << "max " << std::endl;
    std::cout << "avg " << std::endl;
    std::cout << "predict " << std::endl;
    std::cout << "time " << std::endl;
    std::cout << "step " << std::endl;
    std::cout << "own command " << std::endl;
}

/** prints all available products */
void AdvisorBot::prod()
{
    std::cout << "The available products are: " << std::endl;
    for (const std::string& p : products)
    {
        std::cout << p << std::endl;
    }
}

/** prints current time */
void AdvisorBot::time()
{
    std::cout << "The current time is: " << currentTime << std:: endl;
}

/** moves Advisor Bot to next time step */
void AdvisorBot::step()
{
    std::cout << "Moving to the next timeframe." << std::endl;
    std::cout << "To view new time, type: time" << std::endl;
    currentTime = orderBook.nextTimeframe(currentTime);
}

/** prints min product ask/bid of current time */
void AdvisorBot::min(std::vector<std::string> userOption)
{
    if ((find(products.begin(), products.end(), userOption[1]) != products.end()))
    {
        if(userOption[2] == "bid" || userOption[2] == "ask")
        {
            orderBook.minMax("min", userOption[1], userOption[2]);
        }
        else{
            std::cout << "Invalid input. Please specify correctly if you want the min value for an ask or a bid!" << std::endl;
            std::cout << "To see how min command works, type: help min" << std::endl;
        }
    }
    else{ // if find function returns products.end(), it means that the 2nd input token is not included in available products
        std::cout << "Invalid input. Please provide a valid product!" << std::endl;
        std::cout << "To see how min command works, type: help min" << std::endl;
        std::cout << "To see the available products, type: prod" << std::endl;
    }
}

/** prints max product ask/bid of current time */
void AdvisorBot::max(std::vector<std::string> userOption)
{
    if ((find(products.begin(), products.end(), userOption[1]) != products.end()))
    {
        if(userOption[2] == "bid" || userOption[2] == "ask")
        {
            orderBook.minMax("max", userOption[1], userOption[2]);
        }
        else{
            std::cout << "Invalid input. Please specify correctly if you want the max value for an ask or a bid!" << std::endl;
            std::cout << "To see how max command works, type: help max" << std::endl;
        }
    }
    else{ // if find function returns products.end(), it means that the 2nd input token is not included in available products
        std::cout << "Invalid input. Please provide a valid product!" << std::endl;
        std::cout << "To see how max command works, type: help max" << std::endl;
        std::cout << "To see the available products, type: prod" << std::endl;
    }
}

/** prints avg bid/ask for sent product of sent numbers of time steps */
void AdvisorBot::avg(std::vector<std::string> userOption)
{
    if ((find(products.begin(), products.end(), userOption[1]) != products.end()))
    {
        if(userOption[2] == "bid" || userOption[2] == "ask")
        {
            try{
                int steps = stoi(userOption[3]);
                orderBook.average(userOption[2], userOption[1], steps);
            }catch(const std::exception& e)
            {
                std::cout << "You have not included a valid number of steps. Please try again inserting an integer. "  << std::endl;
                std::cout << "To see how avg command works, type: help avg" << std::endl;
            }
        }
        else{
            std::cout << "Invalid input. Please specify correctly if you want the max value for an ask or a bid!" << std::endl;
            std::cout << "To see how avg command works, type: help avg" << std::endl;
        }
    }
    else{ // if find function returns products.end(), it means that the 2nd input token is not included in available products
    std::cout << "Invalid input. Please provide a valid product!" << std::endl;
    std::cout << "To see how avg command works, type: help avg" << std::endl;
    std::cout << "To see the available products, type: prod" << std::endl;
    }
}

/** prints bid-ask spread for sent product of current or specified previous time step */
void AdvisorBot::spread(std::vector<std::string> userOption)
{
    if ((find(products.begin(), products.end(), userOption[1]) != products.end()))
    {
        try{
                int steps = stoi(userOption[2]);
                orderBook.BAspread(userOption[1], steps);
            }catch(const std::exception& e)
            {
                std::cout << "You have not included a valid number of steps. Please try again inserting an integer. "  << std::endl;
                std::cout << "To see how spread command works, type: help spread" << std::endl;
            }
    }
    else{ // if find function returns products.end(), it means that the 2nd input token is not included in available products
    std::cout << "Invalid input. Please provide a valid product!" << std::endl;
    std::cout << "To see how spread command works, type: help spread" << std::endl;
    std::cout << "To see the available products, type: prod" << std::endl;
    }
}

/** gets User's line input */
std::vector<std::string> AdvisorBot::getUserOption()
{
    std::vector<std::string> userOption;
    std::string line;
    std::cout << "-------------------------------------------------------------------------" << std::endl;
    std::cout << "Please enter a command, or enter help to see a list of available commands" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = CSVReader::tokenise(line, ' ');

    }catch(const std::exception& e)
    {
        std::cout << "AdvisorBot::getUserOption - Bad input!" << std::endl;
    }
    return userOption;
}

/** processes User's line input */
void AdvisorBot::processUserOption(std::vector<std::string> userOption)
{
    if (userOption.size() == 1)
    {
        std::cout << "You chose: " << userOption[0] << std::endl;
        if (userOption[0] == "help") 
        {
            printHelp();
        }
        else if (userOption[0] == "prod") 
        {
            prod();
        }
        else if (userOption[0] == "time") 
        {
            time();
        }
        else if (userOption[0] == "step") 
        {
            step();
        }
        else
        {
            std::cout << "Invalid input. Please provide a valid command input by writing a command exactly as shown in the help list." << std::endl;
            std::cout << "To use the help <cmd> command successfully, please type help and then the name of the command you need help for: " << std::endl;
            std::cout << "e.g. help prod, help avg, help time etc." << std::endl;
        }   
    }
    else if(((userOption.size() == 2) || (userOption.size() == 3) ) && userOption[0] == "help")
    {
        AdvisorBot::helpCmd(userOption); // bad input regarding the 2nd and 3rd tokens is handled within helpCmd function
    }
    else if((userOption.size() == 3) && userOption[0] == "min")
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << " " << userOption[2] << std::endl;
        min(userOption); // bad input regarding the 2nd and 3rd tokens is handled within min function
    }
    else if((userOption.size() == 3) && userOption[0] == "max")
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << " " << userOption[2] << std::endl;
        max(userOption); // bad input regarding the 2nd and 3rd tokens is handled within max function
    }
    else if ((userOption.size() == 3) && userOption[0] == "spread") 
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << " " << userOption[2] << std::endl;
        spread(userOption); // bad input regarding the 2nd and 3rd tokens is handled within spread function
    } 
    else if(userOption.size() == 4 && userOption[0] == "avg")
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << " " << userOption[2] << " " << userOption[3]<< std::endl;
        avg(userOption); // bad input regarding the 2nd, 3rd and 4th tokens is handled within avg function
    }
        else if (userOption[0] == "predict") 
    {
        std::cout << "Valid choice, but function was not finished. :( " << std::endl;
    } 
    else // any other variation of bad input
    {
        std::cout << "Invalid input. Please provide a valid command input by writing a command exactly as shown in the help list." << std::endl;
        std::cout << "To use the help <cmd> command successfully, please type help and then the name of the command you need help for: " << std::endl;
        std::cout << "e.g. help prod, help avg, help time etc." << std::endl;
    }   
}

/** provides helpful instructions for each command - including itself */
void AdvisorBot::helpCmd(std::vector<std::string> userOption)
{
    if (userOption.size() == 3)
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << " " << userOption[2] << std::endl;
        if (userOption[1] == "help" &&  userOption[2] == "<cmd>")
        {
            std::cout << "help <cmd> provides a useful description of the command you insert after the word help: " << std::endl;
            std::cout << "e.g. if you type help prod, you will receive a description of what command prod does" << std::endl;
        }
        else // bad input - first token is help and input is of length 3 but the 2nd and 3rd token do not match the above options 
        {
            std::cout << "Invalid input. Please provide a valid command input by writing the command you want to perform exactly as shown in the help list." << std::endl;
            std::cout << "To use the help <cmd> command successfully, please type help and then the name of the command you need help for: " << std::endl;
            std::cout << "e.g. help prod, help avg, help time etc." << std::endl;
        }
    }
    if (userOption.size() == 2)
    {
        std::cout << "You chose: " << userOption[0] << " " << userOption[1] << std::endl;
        if (userOption[1] == "help") 
        {
            std::cout << "help command provides a list of all available commands in the AdvisorBot." << std::endl;
        }
        else if (userOption[1] == "prod") 
        {
            std::cout << "Command prod provides you with a list of all available exchange products" << std::endl;
        }
        else if (userOption[1] == "min") 
        {
            std::cout << "Command min shows you the minimum bid or ask for a given product in the current time step." << std::endl;
            std::cout << "Input should be of the form: min <product> <bid/ask> , e.g. :" << std::endl;
            std::cout << "To find out the minimum ask for ETH/BTC in the current time step, type: min ETH/BTC ask" << std::endl;
            std::cout << "To see which products are available before using the min command, type: prod" << std::endl;
        }
        else if (userOption[1] == "max") 
        {
            std::cout << "Command max shows you the maximum bid or ask for a given product in the current time step." << std::endl;
            std::cout << "Input should be of the form: max <product> <bid/ask> e.g. :" << std::endl;
            std::cout << "To find out the maximum bid for ETH/BTC in the current time step, type: max ETH/BTC bid" << std::endl;
            std::cout << "To see which products are available before using the max command, type: prod" << std::endl;
        }
        else if (userOption[1] == "avg") 
        {
            std::cout << "Command avg computes the average ask or bid for the chosen product over the chosen number of past time steps." << std::endl;
            std::cout << "The input should be of the form: avg <product> <ask/bid> <number of timesteps>." << std::endl;
            std::cout << "For example, if you type: avg ETH/BTC ask 10 , you will receive the average ETH/BTC ask price over the last 10 timesteps." << std::endl;
        }
        else if (userOption[1] == "predict") 
        {
            std::cout << "This command predicts the max or min ask or bid for the chosen product for the next timestep." << std::endl;
            std::cout << "The input should be of the form: predict <max/min> <product> <ask/bid>." << std::endl;
            std::cout << "For example, you can type: predict max ETH/BTC ask , and the AdvisorBot will show you what the max ask for ETH/BTC might be in the next timestep." << std::endl;
            std::cout << "Please note that the outputs of this command are just statistical predictions which may however prove to be incorrect." << std::endl;
        }
        else if (userOption[1] == "time") 
        {
            std::cout << "Command time shows you the current time in the dataset, i.e. which timeframe you are currently looking at." << std::endl;
        }
        else if (userOption[1] == "step") 
        {
            std::cout << "Command step moves you to the next time step." << std::endl;
        }
        else if (userOption[1] == "spread") 
        {
            std::cout << "This command shows you the spread between the max bid and the min ask for the chosen product and specified past or current time step." << std::endl;
            std::cout << "The input should be of the form: spread <product> <number of timesteps>." << std::endl;
            std::cout << "In <number of timesteps> you should enter an integer showing how many time steps back you would like to check the spread for." << std::endl;
            std::cout << "If you want to check the spread for the current time, just insert: 0 " << std::endl;
            std::cout << "For example, if you type: spread ETH/BTC 10 ,the AdvisorBot will show you the spread for ETH/BTC 10 timesteps back." << std::endl;
            std::cout << "For example, if you type: spread ETH/BTC 0 ,the AdvisorBot will show you the spread for ETH/BTC for the current time." << std::endl;
        }
        else // bad input - first token is help and input is of length 2 but the second token does not match any of the above options 
        {
            std::cout << "Invalid input. Please provide a valid command input by writing the command you want to perform exactly as shown in the help list." << std::endl;
            std::cout << "To use the help <cmd> command successfully, please type help and then the name of the command you need help for: " << std::endl;
            std::cout << "e.g. help prod, help avg, help time etc." << std::endl;
        }
        
    }

}