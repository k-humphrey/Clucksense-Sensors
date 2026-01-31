#pragma once

#include <Arduino.h>

// ---------- Data Structures ----------

// Sent FROM peripheral → central
struct SensorData {
  uint8_t type;
  uint8_t door_status;
  int16_t temp;
  int16_t humidity;
  uint8_t food;
  uint8_t water;
};

// Sent FROM central → peripheral
struct CommandData {
  uint8_t command;
  uint8_t door_status;
  int16_t temperature;
};

// Compile-time size checks
static_assert(sizeof(SensorData) == 8, "SensorData size mismatch");
static_assert(sizeof(CommandData) == 4, "CommandData size mismatch");

// ---------- API ----------

void uart_link_begin(
  int uart_num,
  uint32_t baudrate,
  int rx_pin,
  int tx_pin
);

bool uart_send_sensor(const SensorData& data);
bool uart_read_sensor(SensorData& data);

bool uart_send_command(const CommandData& cmd);
bool uart_read_command(CommandData& cmd);
