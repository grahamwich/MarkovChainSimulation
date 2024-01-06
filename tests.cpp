// COPYRIGHT 2022 Graham Stelzer

#include <iostream>
#include <string>

#include "MarkovSimWriter.hpp" // NOLINT : lint complains about subdirectories

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

// test with gagggagaggcgagaaa

// test constructor:
// also tests orderk
BOOST_AUTO_TEST_CASE(test_constructor) {
    std::string text = "gagggagaggcgagaaa";
    MarkovSimWriter a(text, 3);
    BOOST_CHECK_EQUAL(a.orderK(), 3);
}

// test generate
BOOST_AUTO_TEST_CASE(test_generate) {
    std::string text = "gagggagaggcgagaaa";
    MarkovSimWriter a(text, 1);
    std::string start = text.substr(0, 1);
    BOOST_CHECK_EQUAL(a.generate(start, 1), "g");
}

// test all exceptions in freqs functions
BOOST_AUTO_TEST_CASE(test_freqs_exceptions) {
    std::string text = "gagggagaggcgagaaa";
    MarkovSimWriter a(text, 3);
    BOOST_CHECK_THROW(a.freq("a"), std::invalid_argument);
    BOOST_CHECK_THROW(a.freq("a", 'a'), std::invalid_argument);
}

// test kRand
BOOST_AUTO_TEST_CASE(test_kRand) {
    std::string text = "gagggagaggcgagaaa";
    MarkovSimWriter a(text, 3);
    BOOST_CHECK_THROW(a.kRand("a"), std::invalid_argument);
    BOOST_CHECK_THROW(a.kRand("abc"), std::invalid_argument);
}
