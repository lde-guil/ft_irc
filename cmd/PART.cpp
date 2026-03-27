#include "../includes/Command.hpp"

void Command::part()
{
    std::string nick = this->_target->getNickname().empty() ? "*" : this->_target->getNickname();

    if (this->_args.size() != 2 || this->_args[1].empty())
    {
        std::string message = Reply::needmoreparams(nick, "PART");
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    Channel *channel = this->getChannel(this->_args[1]);
    if (channel == NULL)
    {
        std::string message = Reply::nosuchchannel(nick, this->_args[1]);
        send(this->_target->getFd(), message.c_str(), message.length(), 0);
        return;
    }
    channel->removeMember(_target);
    _target->setChannel(NULL);
    std::cout << "sent message: " << Reply::part(_target->getPrefix(), channel->getName());
    channel->sendMessage(Reply::part(_target->getPrefix(), channel->getName()), this->_target);
    if (channel->getClientCount() > 0)
        return ;

    std::vector<Channel> *channels = _serv->getChannels();
    std::vector<Channel>::iterator it = channels->begin();
    for (; it != channels->end(); ++it)
    {
        if (it->getName() == channel->getName())
            break;
    }
    if (it == channels->end())
        return;
    channels->erase(it);
}
