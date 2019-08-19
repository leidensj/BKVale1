#include "settings.h"
#include "defines.h"
#include <QSettings>

#define DATABASE_HOST_NAME     "databaseHostName"
#define DATABASE_PORT          "databasePort"
#define IS_ETHERNET            "isethernet"
#define SERIAL_PORT            "serial/port"
#define SERIAL_BAUDRATE        "serial/baudrate"
#define SERIAL_DATABITS        "serial/databits"
#define SERIAL_FLOWCONTROL     "serial/flowcontrol"
#define SERIAL_PARITY          "serial/parity"
#define SERIAL_STOPBITS        "serial/stopbits"
#define ETHERNET_IP            "ethernet/ip"
#define ETHERNET_PORT          "ethernet/port"

#define DEFAULT_DATABASE_PORT 5432

Settings::Settings()
{
  clear();
}

void Settings::clear()
{
  m_databaseHostName.clear();
  m_databasePort = DEFAULT_DATABASE_PORT;
  m_bIsPrinterEthernet = false;
  m_serialPort.clear();
  m_ethernetIP.clear();
  m_ethernetPort = 9100;
  m_serialBaudRate = QSerialPort::Baud9600;
  m_serialDataBits = QSerialPort::Data8;
  m_serialFlowControl = QSerialPort::NoFlowControl;
  m_serialParity = QSerialPort::NoParity;
  m_serialStopBits = QSerialPort::OneStop;
}

void Settings::save() const
{
  QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
  settings.setValue(DATABASE_HOST_NAME, m_databaseHostName);
  settings.setValue(DATABASE_PORT, m_databasePort);
  settings.setValue(IS_ETHERNET, m_bIsPrinterEthernet);
  settings.setValue(SERIAL_PORT, m_serialPort);
  settings.setValue(SERIAL_BAUDRATE, m_serialBaudRate);
  settings.setValue(SERIAL_DATABITS, m_serialDataBits);
  settings.setValue(SERIAL_FLOWCONTROL, m_serialFlowControl);
  settings.setValue(SERIAL_PARITY, m_serialParity);
  settings.setValue(SERIAL_STOPBITS, m_serialStopBits);
  settings.setValue(ETHERNET_IP, m_ethernetIP);
  settings.setValue(ETHERNET_PORT, m_ethernetPort);
}

void Settings::load()
{
  QSettings settings(SETTINGS_COMPANY_NAME, SETTINGS_APP_NAME);
  m_databaseHostName = settings.value(DATABASE_HOST_NAME).toString();
  m_databasePort = settings.value(DATABASE_PORT, DEFAULT_DATABASE_PORT).toInt();
  m_bIsPrinterEthernet = settings.value(IS_ETHERNET, false).toBool();
  m_serialPort = settings.value(SERIAL_PORT).toString();
  m_serialBaudRate = (QSerialPort::BaudRate)settings.value(SERIAL_BAUDRATE).toInt();
  m_serialDataBits = (QSerialPort::DataBits)settings.value(SERIAL_DATABITS).toInt();
  m_serialFlowControl = (QSerialPort::FlowControl)settings.value(SERIAL_FLOWCONTROL).toInt();
  m_serialParity = (QSerialPort::Parity)settings.value(SERIAL_PARITY).toInt();
  m_serialStopBits = (QSerialPort::StopBits)settings.value(SERIAL_STOPBITS).toInt();
  m_ethernetIP = settings.value(ETHERNET_IP).toString();
  m_ethernetPort = settings.value(ETHERNET_PORT, 9100).toInt();
}
