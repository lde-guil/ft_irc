#include "../includes/Command.hpp"

void Command::kick()
{
    if (this->_args.size() > 3)
    {
        send(this->_target->getFd(), IRC::Reply::needmoreparams(this->_target->getNickname(), this->_name).c_str(), IRC::Reply::nosuchchannel(this->_target->getNickname(), this->_name).length(), 0);
        return;
    }
    if (this->_args[1].empty())
    {
        send(this->_target->getFd(), IRC::Reply::nosuchchannel(this->_target->getNickname(), "").c_str(), IRC::Reply::nosuchchannel(this->_target->getNickname(), "").length(), 0);
        return;
    }
}
