#include <iostream>
#include <json.hpp>
#include <curl/curl.h>

using JSON = nlohmann::json;

struct JsonReader
{
    static size_t curl_read(void* buffer, size_t size, size_t nmemb, void* reader)
    {
        return static_cast<JsonReader*>(reader)->read(buffer, size*nmemb);
    }

    size_t read(void* buffer, size_t bufsiz)
    {
        std::string chunk = {reinterpret_cast<char*>(buffer), bufsiz};
        std::cout << "Received chunk:" << std::endl
            << "\n*********************\n"
            << chunk
            << "\n*********************\n"
            ;

        data_ += chunk;
        try
        {
            json_ = JSON::parse(data_);
        }
        catch(...)
        {}
        return bufsiz;
    }

    JSON& json() { return json_; }

    private:
    JSON json_;
    std::string data_;
};

auto main(int argc, char**argv) -> int
{
    auto h = curl_easy_init();

    //curl_easy_setopt(h, CURLOPT_VERBOSE, 1);
    //curl_easy_setopt(h, CURLOPT_HEADER, 1);
    curl_easy_setopt(h, CURLOPT_USERAGENT, "A well respected browser/1.0");

    while(--argc)
    {
        std::string url = *++argv;
        JsonReader reader;
        curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, JsonReader::curl_read);
        curl_easy_setopt(h, CURLOPT_WRITEDATA, &reader);
        curl_easy_setopt(h, CURLOPT_URL, url.c_str());
        if(0 == curl_easy_perform(h))
        {
            std::cout << "Success!" << std::endl;
            std::cout << std::setw(4) << reader.json() << std::endl;
        }
        else
        {
            std::cout << "Fail!" << std::endl;
        }
    }
    curl_easy_cleanup(h);
    
    return 0;
}
