#include "../includes/Command.hpp"

void Command::mode(void)
{
    if (_args.size() < 2 || _args[1].empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "MODE");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string target = _args[1];
    std::string modes = "";
    if (_args.size() >= 3)
        modes = _args[2];
    if (modes.empty())
    {
        std::string reply = Reply::needmoreparams(_target->getNickname(), "MODE");
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    if (target[0] == '#')
    {
        Channel *chan = getChannel(target);
        if (!chan)
        {
            std::string reply = Reply::nosuchchannel(_target->getNickname(), target);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }

        if (!chan->isMember(_target))
        {
            std::string reply = Reply::notonchannel(_target->getNickname(), target);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }

        if (!chan->isOperator(_target))
        {
            std::string reply = Reply::chanoprivsneeded(_target->getNickname(), target);
            send(_target->getFd(), reply.c_str(), reply.length(), 0);
            return;
        }

        bool plus = (modes[0] == '+');
        size_t argIndex = 3;
        for (size_t i = 1; i < modes.size(); i++)
        {
            if (modes[i] == 'i')
                chan->setInviteOnly(plus ? 1 : 0);
            else if (modes[i] == 't')
                chan->setRestricted(plus ? 1 : 0);
            else if (modes[i] == 'k')
            {
                if (plus)
                {
                    if (_args.size() > argIndex)
                        chan->setKey(_args[argIndex++]);
                }
                else
                    chan->setKey("");
            }
            else if (modes[i] == 'o')
            {
                if (_args.size() > argIndex)
                {
                    Client *targetClient = getNClient(_args[argIndex++]);
                    if (targetClient && chan->isMember(targetClient))
                    {
                        if (plus)
                            chan->addOperator(targetClient);
                        else
                            chan->removeOperator(targetClient);
                    }
                }
            }
            else if (modes[i] == 'l')
            {
                if (plus)
                {
                    if (_args.size() > argIndex)
                    {
                        int limit = std::atoi(_args[argIndex++].c_str());
                        if (limit > 0)
                            chan->setUserLimit(limit);
                    }
                }
                else
                    chan->setUserLimit(0);
            }
            else
            {
                std::string reply = Reply::unknownmode(_target->getNickname(), std::string(1, modes[i]));
                send(_target->getFd(), reply.c_str(), reply.length(), 0);
                return;
            }
        }
        std::string modeReply = Reply::mode(_target->getPrefix(), target, modes, (_args.size() > 3 ? _args[3] : ""));
        chan->broadcast(modeReply, _target);
        return;
    }
    Client *user = getNClient(target);
    if (!user)
    {
        std::string reply = Reply::nosuchnick(_target->getNickname(), target);
        send(_target->getFd(), reply.c_str(), reply.length(), 0);
        return;
    }
    std::string modeReply = Reply::mode(_target->getPrefix(), target, modes, (_args.size() > 3 ? _args[3] : ""));
    send(_target->getFd(), modeReply.c_str(), modeReply.length(), 0);
}
