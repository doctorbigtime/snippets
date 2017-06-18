#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using stringvec = std::vector<std::string>;

std::vector<stringvec> combinations(stringvec const& v, int n)
{
    std::vector<stringvec> combos;
    size_t size = v.size();
    std::vector<bool> selector(size);
    std::fill(selector.end()-n, selector.end(), true);
    do
    {
        for(size_t i = 0; i < selector.size(); ++i)
        {
            std::cout << i << ". " << std::boolalpha << selector[i] << '|';
        }
        std::cout << std::endl;

        combos.push_back({});
        auto& combo = combos.back();
        for(size_t i = 0; i < selector.size(); ++i)
        {
            if(selector[i])
            {
                combo.push_back(v[i]);
            }
        }
        std::cout << "Combo: ";
        for(auto& s : combo)
        {
            std::cout << s << "|";
        }
        std::cout << std::endl;
    } while(std::next_permutation(selector.begin(), selector.end()));
    if(n > 1)
    {
        auto sub_combos = combinations(v, n-1);
        combos.insert(combos.end(), sub_combos.begin(), sub_combos.end());
    }
    return combos;
}

int main(int argc, char** argv)
{
    stringvec v { "foo", "bar", "baz", "qux" };

    auto vecs = combinations(v, 4);
    for(size_t i = 0; i < vecs.size(); ++i)
    {
        std::cout << "Combination #" << i << ". ";
        for(auto& s : vecs[i])
        {
            std::cout << s << "|";
        }
        std::cout << std::endl;
    }
    return 0;
}
