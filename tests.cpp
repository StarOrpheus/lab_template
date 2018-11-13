#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <random>

#include "gtest/gtest.h"
#include "solution.h"
#include "graphs.h"

using token_t = std::string;
using test_result_t = std::pair<bool, std::string>;

std::vector<token_t> istokenize(std::istream& in)
{
    auto begin_iter = std::istream_iterator<std::string>(in);
    auto end_iter = std::istream_iterator<std::string>();

    std::vector<token_t > result(begin_iter, end_iter);

    return result;
}

test_result_t match_str_against_str(std::string const& input, std::string const& answer)
{
    static const std::string mesg_not_eq = "Answer differs in token";
    static const std::string mesg_len_fail = "Token count differs:";

    std::istringstream input_ss(input);
    std::ostringstream output_ss;

    solve(input_ss, output_ss);

    std::stringstream raw_out_ss = std::stringstream(output_ss.str());
    std::stringstream raw_ans_ss = std::stringstream(answer);

    std::vector<token_t> tokens_from_sol = istokenize(raw_out_ss);
    std::vector<token_t> tokens_from_ans = istokenize(raw_ans_ss);

    if (tokens_from_ans.size() != tokens_from_sol.size())
    {
        auto mesg = mesg_len_fail + "\r\n\r\n" + output_ss.str() + "\r\n\r\n" + answer;

        return {false, std::move(mesg)};
    }

    auto mism_iters = std::mismatch(tokens_from_sol.begin(), tokens_from_sol.end(), tokens_from_ans.begin());
    if (mism_iters.first != tokens_from_sol.end())
    {
        auto mesg = mesg_not_eq + std::to_string((mism_iters.first - tokens_from_sol.begin() + 1))
                    + "\n" + output_ss.str() + "\n" + answer;

        return {false, std::move(mesg)};
    }

    return {true, ""};
}
test_result_t run_test_on_file(std::string const& fname, std::function<test_result_t(std::string const& input, std::string const& answer)> const& test_f)
{
    std::ifstream test_file(fname);

    std::string input_buf, answer_buf;
    std::string ibuf;

    size_t test_id = 0;

    while (true)
    {
        input_buf.clear(); answer_buf.clear();

        while (std::getline(test_file, ibuf))
        {
            if (ibuf.empty()) { break; }

            input_buf += ibuf + '\n';
            ibuf.clear();
        }

        if (test_file.bad() || test_file.eof()) { break; }

        while (std::getline(test_file, ibuf))
        {
            if (ibuf.empty()) { break; }

            answer_buf += ibuf + '\n';
            ibuf.clear();
        }

        if (test_file.bad() || test_file.eof())
        {
            break;
        } else
        {
            std::getline(test_file, ibuf);
        }

        assert(!answer_buf.empty());
        ++test_id;

        test_result_t test = test_f(input_buf, answer_buf);

        if (!test.first)
        {
            return test;
        }

        input_buf.clear();
        answer_buf.clear();
    }

    std::cerr << test_id << " tests done!" << std::endl;

    return {true, ""};
}

TEST(basic, t1)
{
    std::string inp = "4 5\n"
                      "1 2 1\n"
                      "1 3 5\n"
                      "2 4 8\n"
                      "3 4 1\n"
                      "2 3 3";

    std::string outp = "0 1 4 5";

    test_result_t test = match_str_against_str(inp, outp);

    if (!test.first)
    {
        std::cerr << test.second << std::endl;
        ASSERT_TRUE(test.first);
    }
}

TEST(performance, randomized_maxtest)
{
    static size_t const salt = 0x02b37af1;

    size_t n = 30000, m = 400000;
    lgraph_t generated = gen_undir_linked_lgraph(n, m, salt);

    std::stringstream buf;
    buf << n << " " << m << '\n';
    for (size_t v = 0; v < n; ++v)
        for (auto&& to: generated[v])
            if (v < to.first)
                buf << (v+1) << " " << (to.first + 1) << " " << to.second << "\n";
    std::stringstream input(buf.str());
    std::stringstream output;

    auto start = clock();
    solve(input, output);
    auto exec_time = (clock() - start) / static_cast<double>(CLOCKS_PER_SEC);

    std::cerr << exec_time << std::endl;
}

#include <stdio.h>

GTEST_API_
int main(int argc, char **argv) {
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
//  testing::GTEST_FLAG(filter) = "constructors*";
    return RUN_ALL_TESTS();
}
