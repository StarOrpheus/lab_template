#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <fstream>
#include <memory>
#include <queue>

#ifdef LOCAL_TESTING
#include "solution.h"
#endif

class solution
{
public:
    using distance_t = int;
    using v_dist_t = std::pair<size_t, distance_t>;
    using graph_t = std::vector<std::vector<v_dist_t>>;

private:
    struct my_comparator
    {
        bool operator()(v_dist_t const& lhs, v_dist_t const& rhs) const
        {
            return lhs.second > rhs.second;
        }
    };

private:
    std::istream& input;
    std::ostream& output;

    size_t n;
    graph_t g;

public:
    solution(std::istream& input, std::ostream& output) : input(input), output(output)
    {}

    void solve()
    {
        size_t m;
        std::vector<distance_t> d;

        input >> n >> m;
        g.resize(n);
        d.resize(n, -1);
        while (m--)
        {
            size_t v, u;
            distance_t d;
            input >> v >> u >> d;
            --v; --u;
            g[v].emplace_back(u, d);
            g[u].emplace_back(v, d);
        }

        std::priority_queue<v_dist_t, std::vector<v_dist_t>, my_comparator> q;
        q.emplace(0, 0);
        d[0] = 0;
        while (!q.empty())
        {
            size_t v = q.top().first;
            distance_t cur_d = q.top().second;
            q.pop();

            if (cur_d < d[v]) continue;

            for (auto&& to: g[v])
            {
                if (d[to.first] == -1 || cur_d + to.second < d[to.first])
                {
                    d[to.first] = cur_d + to.second;
                    q.emplace(to.first, d[to.first]);
                }
            }
        }

        for (auto&& cur_d: d)
            output << cur_d << " ";
    }
};

void solve(std::istream& input, std::ostream& output)
{
    solution(input, output).solve();
}

#ifndef LOCAL_TESTING
int main()
{
//    std::ifstream in("guyaury.in");
//    std::ofstream out("guyaury.out");

//    solve(in, out);
    std::ios_base::sync_with_stdio(false);
    solve(std::cin, std::cout);

    return 0;
}
#endif
