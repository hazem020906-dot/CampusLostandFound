#include <boost/asio.hpp>
#include "Server.hpp"

int main()
{
    boost::asio::io_context io;
    Server server(io, 5555);
    io.run();
    return 0;
}
