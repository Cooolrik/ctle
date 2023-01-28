#include "gtest/gtest.h"
#include "utils.h"
#include <stdio.h>
using namespace ctle;

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

/*
To test exception blocks
SanboxRunner<ctle::bad_optional_value_access> runner;
EXPECT_TRUE(runner([]{throw std::runtime_error("t");}));
*/
template<typename ExceptionType>
struct SanboxRunner
{
	SanboxRunner() = default;
	bool operator()(std::function<void()> predicate)
	{
		try
		{
			predicate();
		}
		catch(const ExceptionType& e)
		{
			return true;
		}
		catch(const std::exception& se)
		{

		}
		return false;
 		
 	}
 	

};


class uint_string_map_test_class {
    ctle::thread_safe_map<u32, std::string> *object = nullptr;
    u32 index = 0;
    static const u32 num_iters = 10;
    bool done = false;

   public:
    static void uint_string_map_test_thread(void *ptr) {
        std::vector<u32> list(num_iters);
        uint_string_map_test_class *pthis = (uint_string_map_test_class *)ptr;

        // insert values into list
        for (u32 i = 0; i < num_iters; ++i) {
            list[i] = rand() << 16 | pthis->index;
            char num[20];
#ifdef __WIN32__
            sprintf_s(num, "%d", list[i]);
#else
            sprintf(num, "%d", list[i]);
#endif
            std::string str = num;
            pthis->object->insert(
                std::move(std::pair<u32, std::string>(list[i], str)));
        }

        // make sure that the values exist, (do random lookup)
        for (u32 i = 0; i < num_iters; ++i) {
            u32 look_for = rand() % num_iters;
            char num[20];
#ifdef __WIN32__
            sprintf_s(num, "%d", list[look_for]);
#else
            sprintf(num, "%d", list[look_for]);
#endif
            std::string str = num;
            auto val = pthis->object->get(list[look_for]);
            ASSERT_TRUE(val.second);  // make sure we found it
            ASSERT_TRUE(str == val.first);
        }

        // erase all values
        for (u32 i = 0; i < num_iters; ++i) { pthis->object->erase(list[i]); }

        pthis->done = true;
    }

    std::thread run(u32 _index, thread_safe_map<u32, std::string> *_object) {
        this->done = false;
        this->index = _index;
        this->object = _object;
        return std::thread(&uint_string_map_test_thread, (void *)this);
    }

    bool is_done() const { return this->done; }
};

TEST(BasicTest, Test_idx_vector) {
    idx_vector<u64> vec;
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);

    for (size_t i = 0; i < 10; ++i) {
        vec.index().emplace_back((i32)i * 2 + 0);
        vec.index().emplace_back((i32)i * 2 + 1);
        vec.values().emplace_back(random_value<u64>());
    }
    ASSERT_TRUE(vec.index().size() == 20);
    ASSERT_TRUE(vec.values().size() == 10);

    idx_vector<u64> vec2 = std::move(vec);
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec2.index().size() == 20);
    ASSERT_TRUE(vec2.values().size() == 10);
    ASSERT_TRUE(vec.index() != vec2.index());
    ASSERT_TRUE(vec.values() != vec2.values());
    ASSERT_TRUE(vec != vec2);
    ASSERT_TRUE(!(vec == vec2));

    vec = vec2;
    ASSERT_TRUE(vec.index().size() == 20);
    ASSERT_TRUE(vec.values().size() == 10);
    ASSERT_TRUE(vec.index().size() == vec2.index().size());
    ASSERT_TRUE(vec.values().size() == vec2.values().size());
    ASSERT_TRUE(vec.index() == vec2.index());
    ASSERT_TRUE(vec.values() == vec2.values());
    ASSERT_TRUE(vec == vec2);
    ASSERT_TRUE(!(vec != vec2));

    vec.clear();
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec.index() != vec2.index());
    ASSERT_TRUE(vec.values() != vec2.values());

    vec2.clear();
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec2.index().size() == 0);
    ASSERT_TRUE(vec2.values().size() == 0);
    ASSERT_TRUE(vec.index() == vec2.index());
    ASSERT_TRUE(vec.values() == vec2.values());
    ASSERT_TRUE(vec == vec2);
    ASSERT_TRUE(!(vec != vec2));
}

TEST(BasicTest, Test_optional_idx_vector) {

	
    optional_idx_vector<u64> vec;

	SanboxRunner<ctle::bad_optional_value_access> exceptionSandbox;
	EXPECT_TRUE(exceptionSandbox([vec]{vec.index();}));
	EXPECT_TRUE(exceptionSandbox([vec]{vec.values();}));

    ASSERT_TRUE(vec == optional_idx_vector<u64>());

    vec.set();
    for (size_t i = 0; i < 10; ++i) {
        vec.index().emplace_back((i32)i * 2 + 0);
        vec.index().emplace_back((i32)i * 2 + 1);
        vec.values().emplace_back(random_value<u64>());
    }
    ASSERT_TRUE(vec.index().size() == 20);
    ASSERT_TRUE(vec.values().size() == 10);

    optional_idx_vector<u64> vec2 = std::move(vec);
    ASSERT_TRUE(vec2.index().size() == 20);
    ASSERT_TRUE(vec2.values().size() == 10);
    ASSERT_TRUE(vec != vec2);

    vec = vec2;
    ASSERT_TRUE(vec.index().size() == 20);
    ASSERT_TRUE(vec.values().size() == 10);
    ASSERT_TRUE(vec.index().size() == vec2.index().size());
    ASSERT_TRUE(vec.values().size() == vec2.values().size());
    ASSERT_TRUE(vec.index() == vec2.index());
    ASSERT_TRUE(vec.values() == vec2.values());

    vec.vector().clear();
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec.index() != vec2.index());
    ASSERT_TRUE(vec.values() != vec2.values());

    vec2.vector().clear();
    ASSERT_TRUE(vec.index().size() == 0);
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec2.index().size() == 0);
    ASSERT_TRUE(vec2.values().size() == 0);
    ASSERT_TRUE(vec.index() == vec2.index());
    ASSERT_TRUE(vec.values() == vec2.values());
}

TEST(BasicTest, Test_optional_value) {
    optional_value<int> opt(0x1337);
    ASSERT_TRUE(opt.has_value());
    ASSERT_TRUE(opt == 0x1337);
    ASSERT_FALSE(opt != 0x1337);

    optional_value<int> opt2 = std::move(opt);

	SanboxRunner<ctle::bad_optional_value_access> exceptionSandbox;
	EXPECT_TRUE(exceptionSandbox([opt]{opt.value();}));

    ASSERT_FALSE(opt.has_value());
    ASSERT_TRUE(opt != 0x1337);
    ASSERT_FALSE(opt == 0x1337);
    ASSERT_TRUE(opt != 0);
    ASSERT_TRUE(opt2.has_value());
    ASSERT_TRUE(opt2 == 0x1337);
    ASSERT_TRUE(opt != opt2);
    ASSERT_FALSE(opt == opt2);

    opt.set(opt2);
    ASSERT_TRUE(opt.has_value());
    ASSERT_TRUE(opt2.has_value());
    ASSERT_TRUE(opt == opt2);

    opt2.reset();
    ASSERT_TRUE(opt.has_value());
    ASSERT_FALSE(opt2.has_value());
    ASSERT_TRUE(opt != opt2);

    opt2.set();
    ASSERT_TRUE(opt != opt2);
    opt2.value() = opt.value();
    ASSERT_TRUE(opt == opt2);
    opt.set(0x13337);
    ASSERT_TRUE(opt != opt2);
    ASSERT_FALSE(opt == opt2);
    ASSERT_FALSE(opt == opt2.value());
    ASSERT_TRUE(opt != opt2.value());
    ASSERT_TRUE(opt.value() != opt2.value());
    ASSERT_TRUE(opt.has_value());
    ASSERT_TRUE(opt2.has_value());

    opt.reset();
    opt2.reset();
    ASSERT_TRUE(opt == opt2);
}

TEST(BasicTest, Test_optional_vector) {
    optional_vector<u64> vec;

	SanboxRunner<ctle::bad_optional_value_access> exceptionSandbox;
	EXPECT_TRUE(exceptionSandbox([vec]{vec.vector();}));

    ASSERT_TRUE(vec == optional_vector<u64>());

    vec.set();
    for (size_t i = 0; i < 10; ++i) {
        vec.values().emplace_back(random_value<u64>());
    }
    ASSERT_TRUE(vec.values().size() == 10);

    optional_vector<u64> vec2 = std::move(vec);
    
	EXPECT_TRUE(exceptionSandbox([vec]{vec.values();}));

    ASSERT_TRUE(vec2.values().size() == 10);
    ASSERT_TRUE(vec != vec2);

    vec = vec2;
    ASSERT_TRUE(vec.values().size() == 10);
    ASSERT_TRUE(vec.values().size() == vec2.values().size());
    ASSERT_TRUE(vec.values() == vec2.values());

    vec.vector().clear();
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec.values() != vec2.values());

    vec2.vector().clear();
    ASSERT_TRUE(vec.values().size() == 0);
    ASSERT_TRUE(vec2.values().size() == 0);
    ASSERT_TRUE(vec.values() == vec2.values());
}

TEST(BasicTest,Test_thread_safe_map) {
    thread_safe_map<u32, std::string> uint_string_map;
    std::vector<uint_string_map_test_class> test_objects(40);
    std::vector<std::thread> threads(test_objects.size());

    // run all threads
    for (size_t i = 0; i < test_objects.size(); ++i) {
        threads[i] = test_objects[i].run((u32)i, &uint_string_map);
    }

    // wait for all threads to finish
    for (size_t i = 0; i < test_objects.size(); ++i) { threads[i].join(); }
}

status_return<bool, std::string> TestReturnString() {
    return {true, "hej"};
}

enum class TestEnum : uint32_t {
    val0 = 0,
    val15534 = 15534,
    val242398723 = 242398723
};

status_return<TestEnum, std::unique_ptr<int>> TestReturnUniquePtr() {
    auto ptr = std::make_unique<int>(100);
    return {TestEnum::val242398723, std::move(ptr)};
}

status_return<TestEnum, std::unique_ptr<int>> TestDontReturnUniquePtr() {
    return {TestEnum::val0};
}

status_return<TestEnum, void> TestReturnJustStatus() {
    return TestEnum::val15534;
}

TEST(BasicTest,Test_status_return) {
    auto ret = TestReturnString();
    ASSERT_TRUE(ret.status());
    ASSERT_TRUE(ret.value() == "hej");

    auto ret2 = TestReturnUniquePtr();
    ASSERT_TRUE(ret2.status() == TestEnum::val242398723);
    ASSERT_TRUE(*(ret2.value().get()) == 100);

    auto ret3 = TestDontReturnUniquePtr();
    ASSERT_TRUE(ret3.status() == TestEnum::val0);
    ASSERT_TRUE(ret3.value().get() == nullptr);

    auto ret4 = TestReturnJustStatus();
    ASSERT_TRUE(ret4.status() == TestEnum::val15534);
}

TEST(BasicTest, Test_bimap) {
    bimap<u32, std::string> uint_string_bimap;

    // insert two values: 0<->A and 1<->B
    uint_string_bimap.insert(0, "A");
    uint_string_bimap.insert(1, "B");
    ASSERT_TRUE(uint_string_bimap.size() == 2);

    // make sure all keys and values are there
    ASSERT_TRUE(uint_string_bimap.contains_key(0));
    ASSERT_TRUE(uint_string_bimap.contains_key(1));
    ASSERT_TRUE(uint_string_bimap.contains_value("A"));
    ASSERT_TRUE(uint_string_bimap.contains_value("B"));

    // now, insert 1<->A instead, this should remove the previous two
    // mappings, and replace with one mapping
    uint_string_bimap.insert(1, "A");
    ASSERT_TRUE(uint_string_bimap.size() == 1);

    // make the correct values are left
    ASSERT_FALSE(uint_string_bimap.contains_key(0));
    ASSERT_TRUE(uint_string_bimap.contains_key(1));
    ASSERT_TRUE(uint_string_bimap.contains_value("A"));
    ASSERT_FALSE(uint_string_bimap.contains_value("B"));

    // remove the key, there should be no items left
    uint_string_bimap.erase_by_key(1);
    ASSERT_TRUE(uint_string_bimap.size() == 0);

    // insert random uints and strings. cap the size to ensure some
    // conflicts
    for (size_t i = 0; i < 100; ++i) {
        u8 key = random_value<u8>();
        std::string value;
        value += (random_value<u8>() % 25) + 'A';

        uint_string_bimap.insert(key, value);
    }

    // for each key, make sure the value maps back to the key
    for (auto val : uint_string_bimap) {
        ASSERT_TRUE(uint_string_bimap.contains_key(val.first));
        ASSERT_TRUE(uint_string_bimap.contains_value(val.second));

        ASSERT_TRUE(uint_string_bimap.at_key(val.first) == val.second);
        ASSERT_TRUE(uint_string_bimap.at_value(val.second) == val.first);
    }
}

template <class _Ty>
string_span<_Ty> get_span(const std::basic_string<_Ty> &str) {
    return {str.data(), str.data() + str.size()};
}

TEST(BasicTest, Test_string_funcs) {
    const char *testString1_start = "123456789";
    const char *testString1_end = &testString1_start[strlen(testString1_start)];

    const char *testString2_start = " A BCD EFF GH 123 ";
    const char *testString2_end = &testString2_start[strlen(testString2_start)];

    const char *testString3_start = "-4332827     ";
    const char *testString3_end = &testString3_start[strlen(testString3_start)];

    const char *testString4_start = "   679873244321     ";
    const char *testString4_end = &testString4_start[strlen(testString4_start)];

    //generate some random values 
	const size_t random_vals_count = 30000;
    std::vector<i64> random_vals(random_vals_count);
    for (size_t i = 0; i < random_vals_count; ++i) {
        random_vals[i] = random_value<i64>();
    }

    // generate some random deliminator strings
    const std::string source_delims = ",afgejsbdASDGes_ \t\r\n";
    const size_t random_delims_count = 300;
    std::vector<std::string> random_delims(random_delims_count);
    for (size_t i = 0; i < random_delims_count; ++i) {
        std::string delim_string;
        const size_t len = (rand() % 10) + 1;
        for (size_t q = 0; q < len; ++q) {
            delim_string += source_delims[rand() % source_delims.size()];
        }
        random_delims[i] = delim_string;
    }

    std::vector<string_span<char>> lexed_tokens;

    // we should never hit the char, so span all string
    ASSERT_TRUE(strcspn_t(testString1_start, testString1_end, "abcdeFGH, ") ==
                9);

    // we don't span anything with this string
    ASSERT_TRUE(strspn_t(testString1_start, testString1_end, "abcdeFGH, ") ==
                0);

    // we should hit character '3' at 2 steps in
    ASSERT_TRUE(strcspn_t(testString1_start, testString1_end, "3") == 2);

    // we should hit any of 9, 8, 7, e, H or ',' at 6 steps in (7)
    ASSERT_TRUE(strcspn_t(testString1_start, testString1_end, "987eH,") == 6);

    // initial span of 123456_89 should be until the 6, so 6 steps in
    ASSERT_TRUE(strspn_t(testString1_start, testString1_end, "123456_89") == 6);

    // second span should be 89, 2 steps in after the 7 at index 6 (so
    // beginning at index 7)
    ASSERT_TRUE(strspn_t(&testString1_start[7], testString1_end, "123456_89") ==
                2);

    // test string and wstring
    ASSERT_TRUE(strspn_t(get_span<char>("qwerTY"), "ytrewq") == 4);
    ASSERT_TRUE(strcspn_t(get_span<char>("qwerrrrrrtyTY"), "YTy") == 10);
    ASSERT_TRUE(strspn_t(get_span<wchar_t>(L"qwertyTY"), L"ytrewq") == 6);
    ASSERT_TRUE(strcspn_t(get_span<wchar_t>(L"qwertyTY"), L"YTy") == 5);

    // get the tokens of the string
    auto tok = strtok_t(testString2_start, testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "A");
    tok = strtok_t(&testString2_start[3], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "BCD");
    tok = strtok_t(&testString2_start[6], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "EFF");
    tok = strtok_t(&testString2_start[11], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "GH");
    tok = strtok_t(&testString2_start[12], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "H");
    tok = strtok_t(&testString2_start[13], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end) == "123");
    tok = strtok_t(&testString2_start[18], testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end).empty());

    // test invalid & empty strings
    tok = strtok_t(testString2_end, testString2_start, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end).empty());
    tok = strtok_t(testString2_start, testString2_start, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end).empty());
    tok = strtok_t(testString2_end, testString2_end, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end).empty());
    tok = strtok_t<char>(nullptr, nullptr, " ");
    ASSERT_TRUE(std::string(tok.start, tok.end).empty());

    // test atou64
    ASSERT_TRUE(stou64_t(testString1_start, testString1_end) == 123456789);
    ASSERT_TRUE(stou64_t(testString2_start, testString2_end) == 0);
    ASSERT_TRUE(stou64_t(testString3_start, testString3_end) == 0);
    ASSERT_TRUE(stou64_t(testString4_start, testString4_end) == 0);
    ASSERT_TRUE(stou64_t(&testString4_start[strspn_t(testString4_start, testString4_end, " \t")],
                                testString4_end) == 679873244321);

    // test atoi64
    ASSERT_TRUE(stoi64_t(testString1_start, testString1_end) == 123456789);
    ASSERT_TRUE(stoi64_t(testString2_start, testString2_end) == 0);
    ASSERT_TRUE(stoi64_t(testString3_start, testString3_end) == -4332827);
    ASSERT_TRUE(stoi64_t(get_span<wchar_t>(L"678876")) == 678876);
    ASSERT_TRUE(stoi64_t(get_span<wchar_t>(L"-35678876")) == -35678876);

    // empty and invalid strings
    ASSERT_TRUE(stou64_t<char>(nullptr, nullptr) == 0);
    ASSERT_TRUE(stou64_t(testString1_start, testString1_start) == 0);
    ASSERT_TRUE(stou64_t(testString1_end, testString1_start) == 0);

    // write random vals to string
    std::stringstream ss;
    for (size_t i = 0; i < random_vals_count; ++i) {
        ss << random_vals[i];
        ss << random_delims[i % random_delims_count];
        }
        const std::string tokens = ss.str();

        // tokenize and read back
        auto current_span = get_span(tokens);
        size_t token_index = 0;
        while (current_span.start < current_span.end) {
        // get the next token in the remaining span
        auto token = strtok_t(current_span, source_delims.c_str());
        if (token.start == token.end) break;  // no more tokens

        // get value, make sure it matches
        ASSERT_TRUE(stoi64_t(token) == random_vals[token_index]);

        // move the span to beyond the token
        current_span.start = token.end;
        ++token_index;
        }
        ASSERT_TRUE(token_index == random_vals_count);

        // test simple lexing
        std::string lexTest1 = " \thej,salvan hur mfds det ?! ";
        lexed_tokens.clear();
        ASSERT_TRUE(lex_t(&lexed_tokens, get_span<char>(lexTest1), ",?"));
        ASSERT_TRUE(lexed_tokens.size() == 8);
        ASSERT_TRUE(std::string(lexed_tokens[0]) == "hej");
        ASSERT_TRUE(std::string(lexed_tokens[1]) == ",");
        ASSERT_TRUE(std::string(lexed_tokens[2]) == "salvan");
        ASSERT_TRUE(std::string(lexed_tokens[3]) == "hur");
        ASSERT_TRUE(std::string(lexed_tokens[4]) == "mfds");
        ASSERT_TRUE(std::string(lexed_tokens[5]) == "det");
        ASSERT_TRUE(std::string(lexed_tokens[6]) == "?");
        ASSERT_TRUE(std::string(lexed_tokens[7]) == "!");

        // test some more complex lexing
        std::string lexTest2 = "1/2/3 4//5 //6";
        lexed_tokens.clear();
        ASSERT_TRUE(lex_t(&lexed_tokens, get_span<char>(lexTest2), "/"));
        ASSERT_TRUE(lexed_tokens.size() == 12);

        // test string lexing as well
        std::string lexTest3 = "foo/'bar'/\"bur\" @ my = \"123\" end";
        lexed_tokens.clear();
        ASSERT_TRUE(lex_t(&lexed_tokens, get_span<char>(lexTest3), "/="));
        ASSERT_TRUE(lexed_tokens.size() == 10);

        // test separators
        std::string lexTest4 = "float val = 1.f; auto val2 = {{ 1, 2, 3 }};"; lexed_tokens.clear(); ASSERT_TRUE(lex_t(&lexed_tokens,get_span<char>(lexTest4), ";{},"));
        ASSERT_TRUE(lexed_tokens.size() == 18);
        ASSERT_TRUE(std::string(lexed_tokens[12]) == "2");
        ASSERT_TRUE(std::string(lexed_tokens[15]) == "}");

        // test fail, where string is not correctly ended
        std::string lexTest5 = " str = \"this string won't end well ";
        lexed_tokens.clear();
        ASSERT_FALSE(lex_t(&lexed_tokens, get_span<char>(lexTest5)));
        ASSERT_TRUE(lexed_tokens.size() == 2);

        // test success, where string IS correctly ended (and single quotes)
        std::string lexTest6 = " str = 'this string does end well' ";
        lexed_tokens.clear();
        ASSERT_TRUE(lex_t(&lexed_tokens, get_span<char>(lexTest6)));
        ASSERT_TRUE(lexed_tokens.size() == 3);
        ASSERT_TRUE(std::string(lexed_tokens[2]) ==
                    "this string does end well");
}

//     TEST_METHOD(Test_string_funcs) {
//         



//     }
// };
// }  // namespace ctletests
