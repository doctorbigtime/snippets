#include <iostream>
#include <cpr/cpr.h>
#include <json.hpp>

using nlohmann::json;


auto main(int argc, char**argv) -> int
{
    std::string url = argc > 1 ? argv[1] : "https://api.gdax.com/products";
    auto req = cpr::Get(cpr::Url{url}, cpr::VerifySsl{false});
    if(req.status_code != 200)
        throw std::runtime_error("Error in request (http status:" + std::to_string(req.status_code)
            + ")" + (req.error.code != cpr::ErrorCode::OK ? " - " + req.error.message : ""));
    auto j = json::parse(req.text);
    assert(j.is_array());
    for(auto const& pj : j)
    {
        std::cout << std::setw(4) << pj << std::endl;
    }
    return 0;
}


