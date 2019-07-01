#include "spi/spi_testing.hpp"

#include "catch.hpp"


TEST_CASE("spi_testing") {
    hwlib::cout << "Testing spi testbus" << hwlib::endl;
}

TEST_CASE("spi_testing, Empty SPI test bus has empty buffers") {
    spi_testing test_empty_buffer;
    REQUIRE(test_empty_buffer.in_buffer_index == 0);
    REQUIRE(test_empty_buffer.out_buffer_size == 0);
}

TEST_CASE("spi_testing, SPI test bus can test write operations") {
    spi_testing test_write;
    uint8_t data[10] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    test_write.transaction(hwlib::pin_out_dummy).write(10, data);

    REQUIRE(test_write.out_buffer_size == 10);
    for (size_t i = 0; i < 10; i++) {
        REQUIRE(data[i] == test_write.out_buffer[i]);
    }
}

TEST_CASE("spi_testing, SPI test bus can test singe byte write operations") {
    spi_testing test_byte_write;
    test_byte_write.transaction(hwlib::pin_out_dummy)
            .write_byte(4)
            .write_byte(6)
            .write_byte(10);

    REQUIRE(test_byte_write.out_buffer_size == 3);

    REQUIRE(test_byte_write.out_buffer[0] == 4);
    REQUIRE(test_byte_write.out_buffer[1] == 6);
    REQUIRE(test_byte_write.out_buffer[2] == 10);
}

TEST_CASE("spi_testing, SPI test bus can test read operations") {
    spi_testing test_read;
    std::array<uint8_t, 10> prepared_data = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    test_read.append_in_buffer(prepared_data);

    uint8_t read_data[10] = {0};

    REQUIRE(test_read.in_buffer_index == 0);
    test_read.transaction(hwlib::pin_out_dummy).write_read(10, nullptr, read_data);


    REQUIRE(test_read.in_buffer_size == 10);
    REQUIRE(test_read.in_buffer_index == 10);
    for (size_t i = 0; i < 10; i++) {
        REQUIRE(prepared_data[i] == read_data[i]);
    }
}

TEST_CASE("spi_testing, SPI test bus can test single byte read operations") {
    spi_testing test_byte_read;
    std::array<uint8_t, 3> data = {18, 16, 14};
    test_byte_read.append_in_buffer(data);

    REQUIRE(test_byte_read.in_buffer_size == 3);
    REQUIRE(test_byte_read.in_buffer_index == 0);

    REQUIRE(data[0] == test_byte_read.transaction(hwlib::pin_out_dummy).read_byte(nullptr));
    REQUIRE(data[1] == test_byte_read.transaction(hwlib::pin_out_dummy).read_byte(nullptr));
    REQUIRE(data[2] == test_byte_read.transaction(hwlib::pin_out_dummy).read_byte(nullptr));

    REQUIRE(test_byte_read.in_buffer_index == 3);
}

TEST_CASE("spi_testing, SPI test bus read can be cleared") {
    spi_testing test_clear_read;
    std::array<uint8_t, 3> data = {18, 16, 14};
    test_clear_read.append_in_buffer(data);
    test_clear_read.transaction(hwlib::pin_out_dummy).read_byte();

    test_clear_read.clear();
    REQUIRE(test_clear_read.in_buffer_size == 0);
    REQUIRE(test_clear_read.in_buffer_index == 0);
}

TEST_CASE("spi_testing, SPI test bus write can be cleared") {
    spi_testing test_clear_write;
    uint8_t data[3] = {18, 16, 14};
    test_clear_write.transaction(hwlib::pin_out_dummy).write_read(3, data, nullptr);

    test_clear_write.clear();
    REQUIRE(test_clear_write.out_buffer_size == 0);
}





