#include "tuple.h"
#include <tuple>
#include "test_tuple.cpp"

int main() {
    Tuple<int, std::string, std::vector<int>> tuple1 = makeTuple(5, std::string("test"), std::vector<int>(2, 3));
    std::cout << get<0>(tuple1) << std::endl;

    get<2>(tuple1)[1] = 2;
    std::cout << get<2>(tuple1)[1] << std::endl;

    auto tuple2 = makeTuple(1, "hello");
    auto test_cat = tupleCat(tuple2, tuple1, makeTuple(5, 10));
    std::cout << get<0>(test_cat) << " " << get<4>(test_cat)[0] << " " << get<5>(test_cat) << std::endl;

    return 0;
}