include(CTest)
if(BUILD_TESTING)
  find_package(GTest REQUIRED)
  find_package(OpenSSL REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system thread date_time)

  add_executable(
    test_ka
      "ark/test_inputiter.cpp"
      "ark/test_mutable.cpp"
      "test_algorithm.cpp"
      "test_base64.cpp"
      "test_conceptpredicate.cpp"
      "test_empty.cpp"
      "test_errorhandling.cpp"
      "test_flatten.cpp"
      "test_functor.cpp"
      "test_functional.cpp"
      "test_functional_common.hpp"
      "test_indexed.cpp"
      "test_integersequence.cpp"
      "test_macro.cpp"
      "test_macroregular.cpp"
      "test_moveoncopy.cpp"
      "test_mutablestore.cpp"
      "test_opt.cpp"
      "test_iterator.cpp"
      "test_parse.cpp"
      "test_productiter.cpp"
      "test_range.cpp"
      "test_relationpredicate.cpp"
      "test_scoped.cpp"
      "test_sha1.cpp"
      "test_src.cpp"
      "test_testutils.cpp"
      "test_typetraits.cpp"
      "test_unit.cpp"
      "test_uri.cpp"
      "test_uri_parsing.cpp"
      "test_utility.cpp"
      "test_zero.cpp"
  )

  target_link_libraries(
    test_ka
      ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY}
      OpenSSL::Crypto OpenSSL::SSL
      GTest::gmock GTest::gmock_main
  )

  add_test(
    NAME test_ka
    COMMAND test_ka
  )
endif()
