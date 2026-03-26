#include "../includes/Command.hpp"

static bool isValidNickname(const std::string &nick)
{
    if (nick.empty())
        return false;
    if (!std::isalpha(static_cast<unsigned char>(nick[0])))
        return false;
    for (size_t i = 0; i < nick.size(); ++i)
    {
        char c = nick[i];
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}' || c == '|'))
            return false;
    }
    return true;
}

void Command::nick(void)
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() != 2)
    {
        std::string message = IRC::Reply::needmoreparams(nick, "NICK");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (this->_args[1].empty())
    {
        std::string message = IRC::Reply::nonicknamegiven(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (!isValidNickname(this->_args[1]))
    {
        std::string message = IRC::Reply::erroneusnickname(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (!this->_target->getLogin())
    {
        std::string message = IRC::Reply::notregistered(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    if (!this->_target->getNickname().empty())
    {
        std::string message = IRC::Reply::alreadyregistered(nick);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    std::vector<Client> *clients = this->_serv->getList();
    for (std::vector<Client>::iterator it = clients->begin(); it != clients->end(); ++it)
    {
        if (it->getFd() != this->_target->getFd() && it->getNickname() == this->_args[1] && !it->getNickname().empty())
        {
            std::string message = IRC::Reply::nicknameinuse(this->_args[1]);
            send(this->_target->getFd(), message.c_str(), message.length(), 0);
            return;
        }
    }
    this->_target->setNickname(this->_args[1]);
    this->_target->checkAuth();
}