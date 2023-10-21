
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <numeric>

class StimulatingEBookReading {
public:
    StimulatingEBookReading() {
        page_id.resize(1001, 0);
        id_page.resize(100001, 0);
    }

    void ReadPage(int id, int page) {
        int old_page = id_page[id];
        if (old_page == 0) {
            ++page_id[page];
            id_page[id] = page;
            ++count_id;
        }
        else {
            --page_id[old_page];
            ++page_id[page];
            id_page[id] = page;
           
        }
       
    }

    double GetMotivationalStatistics(int id) {
        int count_page= id_page[id];
        if (count_id == 0 || count_page == 0)return 0;
        if (count_id == 1)return 1;
        int result = std::accumulate(page_id.begin(), page_id.begin( )+ count_page,0);
        return result/ static_cast<double>(count_id - 1);
    }

private:
    std::vector<int> page_id;
    std::vector<int> id_page;
    int count_id=0;
};

int main() {
    //std::ifstream in("test.txt");
    StimulatingEBookReading stimul;
    int numb_requests;
    std::cin >> numb_requests;
    for (int i = 0; i < numb_requests; ++i) {
        std::string request;
        std::cin >> request;
        if (request == "READ") {
            int id, page;
            std::cin >> id >> page;
            stimul.ReadPage(id, page);
        }
        else if (request == "CHEER") {
            int id;
            std::cin >> id;
            std::cout << std::setprecision(6) << stimul.GetMotivationalStatistics(id) << '\n';
        }
    }
}