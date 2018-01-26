rest_cpr: rest_cpr.cpp
	g++ -std=c++1z -g -I../../cpr/include -I../../json/src $< -o $@ -L../../cpr/build/lib -lcpr `curl-config --libs`
restcpp: restcpp.cpp
	g++ -std=c++1z -g -I../../curlpp/include -I../../json/src $< -o $@ -L../../curlpp/build -l:libcurlpp.a `curl-config --libs`
