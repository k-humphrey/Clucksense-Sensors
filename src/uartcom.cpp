#include "uartcom.h"

static HardwareSerial* uart = nullptr;

void uart_link_begin(
  int uart_num,
  uint32_t baudrate,
  int rx_pin,
  int tx_pin
) {
  uart = new HardwareSerial(uart_num);
  uart->begin(baudrate, SERIAL_8N1, rx_pin, tx_pin);
}

// ---------- SensorData ----------

bool uart_send_sensor(const SensorData& data) {
  if (!uart) return false;
  uart->write(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
  return true;
}

bool uart_read_sensor(SensorData& data) {
  if (!uart) return false;
  if (uart->available() < sizeof(SensorData)) return false;

  uart->readBytes(reinterpret_cast<uint8_t*>(&data), sizeof(data));
  return true;
}

// ---------- CommandData ----------

bool uart_send_command(const CommandData& cmd) {
  if (!uart) return false;
  uart->write(reinterpret_cast<const uint8_t*>(&cmd), sizeof(cmd));
  return true;
}

bool uart_read_command(CommandData& cmd) {
  if (!uart) return false;
  if (uart->available() < sizeof(CommandData)) return false;

  uart->readBytes(reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd));
  return true;
}


