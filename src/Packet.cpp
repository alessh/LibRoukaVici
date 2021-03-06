#include <sstream>
#include "Packet.hh"
#include "Debug.hh"

/// This function adds the header to a data packet
static const std::string vibrationHeader(char apiNumber, const std::string& data)
{
      const char buff[3] = {'R', apiNumber, static_cast<char>(data.length())};
      return std::string(buff, 3) + data;
}

const std::string Packet::v0(const char motor, const char intensity)
{
      char msg[2] = {motor, intensity};
      return std::string(msg);
}

const std::string Packet::v1(const char motor, const char intensity)
{
      // The initial buffer
      const char buff[2] = {motor, intensity};
      // The data packet created from the buffer
      std::string data(buff, 2);
      return vibrationHeader(1, data);
}

const std::string Packet::Handshake(char min, char max)
{
      const char buff[3] = {'H', min, max};
      return std::string(buff, 3);
}

char Packet::HandshakeResult(char buffer[], char minApi, char maxApi)
{
      return buffer[0] == 'H' && buffer[1] >= minApi && buffer[1] <= maxApi ? buffer[1] : 0;
}