#include "../includes/Command.hpp"

void Command::cap(void)
{
    if (this->_args.size() < 2)
    {
        throw Command::IncorrectArgNumber();
    }
    std::string subcmd = this->_args[1];
    if (subcmd == "LS")
    {
        send(this->_target->getFd(), "CAP * LS :\r\n", sizeof("CAP * LS :\r\n") - 1, 0);
    }
    else if (subcmd == "END")
    {
        return ;
    }
    else if (subcmd == "REQ")
    {
        send(this->_target->getFd(), "CAP * NAK :\r\n", sizeof("CAP * NAK :\r\n") - 1, 0);
    }
    else
    {
        send(this->_target->getFd(), "CAP * NAK :\r\n", sizeof("CAP * NAK :\r\n") - 1, 0);
    }
}
