#include "ManagerFactory.hh"

#include "TextManager.hh"
// If the user wants to use libusb
#ifdef LIBUSB_COMPILE
  #include "USBManager.hh"
#endif

// If the user wants to use bluetooth
#ifdef BT_COMPILE
  #include "BTManager.hh"
#endif

ManagerFactory::ManagerFactory()
{
  // Here we'll add every manager, including those in options
  managers["TextManager"] = TextManager::create;
#ifdef LIBUSB_COMPILE
  managers["USBManager"] = USBManager::create;
#endif

#ifdef BT_COMPILE
  managers["BTManager"] = BTManager::create;
#endif
}


ManagerFactory::~ManagerFactory()
{
}

DeviceManager* ManagerFactory::get(const std::string& name)
{
  // managers is a map containing the create() functions of the proper class
  // TODO: Check if it exists first.
  return managers[name]();
}
