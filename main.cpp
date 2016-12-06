#include <boost/network/protocol/http/client.hpp>
#include <iostream>

#include <boost/network/protocol/http/server.hpp>
#include <boost/network/uri.hpp>

#include <boost/network/protocol/http/server.hpp>
#include <iostream>

namespace http = boost::network::http;

/*<< Defines the server. >>*/
struct hello_world;
typedef http::server<hello_world> server;

/*<< Defines the request handler.  It's a class that defines two
 functions, `operator()` and `log()` >>*/
struct hello_world {
    /*<< This is the function that handles the incoming request. >>*/
    void operator()(server::request const &request, server::connection_ptr connection) {
        server::string_type ip = source(request);
        unsigned int port = request.source_port;
        std::ostringstream data;
        data << "Hello, " << ip << ':' << port << '!';
        connection->write(data.str());
    }
};


namespace http = boost::network::http;
namespace uri = boost::network::uri;
namespace {
    std::string get_filename(const uri::uri &url) {
        std::string path = uri::path(url);
        std::size_t index = path.find_last_of('/');
        std::string filename = path.substr(index + 1);
        return filename.empty()? "index.html" : filename;
    }
} // namespace

void myserver()
{
    try {
        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server::options options(handler);
        server server_(options.address(argv[1]).port(argv[2]));
        /*<< Runs the server. >>*/
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}

void myclient()
{
    using namespace boost::network;
    using namespace boost::network::http;
    
    client::request request_("http://www.baidu.com");
    request_<< header("Connection","close");
    client client_;
    client::response response_= client_.get(request_);
    std::cout << body(response_) << std::endl;
}
int main(int argc, char *argv[]) {
    using namespace boost::network;
    
    myclient();
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [url]" << std::endl;
        return 1;
    }
    
    http::client client;
    http::client::request request(argv[1]);
    request << header("Connection", "close");
    http::client::response response = client.get(request);
    std::cout << body(response) << std::endl;
    
    return 0;
}
