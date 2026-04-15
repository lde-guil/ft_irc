#include "../includes/Command.hpp"

void Command::ping(void)
{
    if (_args.size() < 2 || _args[1].empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "PING");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }

    std::string token = _args[1];
    if (token[0] == ':')
        token = token.substr(1);

    std::string reply = Reply::ping("localhost", token);
    send(_target->getFd(), reply.c_str(), reply.length(), 0);
}
