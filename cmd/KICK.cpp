#include "../includes/Command.hpp"

void Command::kick()
{
    std::cout << "kick launched" << std::endl;
    if (this->_args.size() < 3)
    {
        send(this->_target->getFd(), IRC::Reply::needmoreparams(this->_target->getNickname(), this->_name).c_str(), IRC::Reply::needmoreparams(this->_target->getNickname(), this->_name).length(), 0);
        return;
    }
    Channel *channel = this->getChannel(this->_args[1]);
    Client *client = this->getNClient(this->_args[2]);
    std::cout << "channel found: " << channel->getName() << std::endl;
    std::cout << "client found: " << client->getNickname() << std::endl;
    std::cout << "check 1" << std::endl;
    if (this->_args[1].empty())
    {
        send(this->_target->getFd(), IRC::Reply::nosuchchannel(this->_target->getNickname(), "").c_str(), IRC::Reply::nosuchchannel(this->_target->getNickname(), "").length(), 0);
        return;
    }
    std::cout << "check 2" << std::endl;    
    if (channel == NULL)
    {
        send(this->_target->getFd(), IRC::Reply::nosuchchannel(this->_target->getNickname(), this->_args[1]).c_str(), IRC::Reply::nosuchchannel(this->_target->getNickname(), this->_args[1]).length(), 0);
        return;
    }
    std::cout << "check 3" << std::endl;
    if (this->_args[2].empty())
    {
        send(this->_target->getFd(), IRC::Reply::nosuchnick(this->_target->getNickname(), "").c_str(), IRC::Reply::nosuchnick(this->_target->getNickname(), "").length(), 0);
        return;
    }
    std::cout << "check 4" << std::endl;
    if (client == NULL)
    {
        send(this->_target->getFd(), IRC::Reply::nosuchnick(this->_target->getNickname(), this->_args[2]).c_str(), IRC::Reply::nosuchnick(this->_target->getNickname(), this->_args[2]).length(), 0);
        return;
    }
    send(client->getFd(), IRC::Reply::bannedfromchan(client->getNickname(), channel->getName()).c_str(), IRC::Reply::bannedfromchan(client->getNickname(), channel->getName()).length(), 0);
    channel->removeMember(client);
    client->setChannel(NULL);
    //std::vector<std::string> tempArgs;
    //tempArgs.push_back("PART");
    //tempArgs.push_back(channel->getName());
    //Command temp_cmd("PART", tempArgs, client, this->getServ());
    //temp_cmd.execCmd();
    std::cout << "kick done" << std::endl;
}
