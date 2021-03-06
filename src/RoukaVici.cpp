#include <sstream>
#include "RoukaVici.hh"
#include "ManagerFactory.hh"
#include "DeviceManager.hh"
#include "GroupManager.hh"
#include "Debug.hh"
#include "RoukaLock.hh"

RoukaVici::RoukaVici()
{
  mf = new ManagerFactory();
  dm = mf->get("TextManager");
  grps = new GroupManager();
  lock  = new RoukaLock();
  std::stringstream ss;
  ss <<  "[" << this << "] RoukaVici successfully initiated(dm=" << dm << ")";
  Debug::Log(ss.str());
}

RoukaVici::~RoukaVici()
{
  std::stringstream ss;
  ss << "[" << this << "] Closing library. Obama out.";
  Debug::Log(ss.str());
  delete grps;
  delete dm;
  delete mf;
  delete lock;
}

int RoukaVici::Status()
{
  std::stringstream ss;
  ss << "[" << this << "] Checking RoukaVici status";
  Debug::Log(ss.str());
  if (!dm->HasDevice())
    return 1;
  return 0;
}

int RoukaVici::FindDevice()
{
  this->lock->lock();
  std::stringstream ss;
  ss << "[" << this << "] Finding device";
  Debug::Log(ss.str());
  dm->FindDevice();
  int ret = dm->HasDevice() ? 0 : 1;
  this->lock->unlock();
  return ret;
}

int RoukaVici::Write(const std::string& msg) const
{
  this->lock->lock();
  int ret = dm->Write(msg);
  this->lock->unlock();
  return ret;
}

int RoukaVici::Vibrate(char motor, char intensity) const
{
  this->lock->lock();
  std::stringstream ss;
  ss << "[" << this << "] Vibration order received: " << static_cast<int>(motor) << ":" << static_cast<int>(static_cast<unsigned char>(intensity));
  Debug::Log(ss.str());
  int ret = dm->Vibrate(motor, intensity);
  this->lock->unlock();
  return ret;
}

int RoukaVici::NewGroup(const std::string& name)
{
  this->lock->lock();
  int ret = grps->NewGroup(name);
  this->lock->unlock();
  return ret;
}

int RoukaVici::AddToGroup(const std::string& name, char motor)
{
  this->lock->lock();
  int ret = grps->AddToGroup(name, motor);
  this->lock->unlock();
  return ret;
}

int RoukaVici::RmFromGroup(const std::string& name, char motor)
{
  this->lock->lock();
  int ret = grps->RmFromGroup(name, motor);
  this->lock->unlock();
  return ret;
}

int RoukaVici::VibrateGroup(const std::string& name, char intensity) const
{
  this->lock->lock();
  int ret = grps->VibrateGroup(name, intensity, dm);
  this->lock->unlock();
  return ret;
}

int RoukaVici::ChangeDeviceManager(const std::string& name)
{
  this->lock->lock();
  {
    std::stringstream ss;
    ss << "[" << this << "] Changing DeviceManager";
    Debug::Log(ss.str());
  }
  DeviceManager* temp = mf->get(name);
  if (temp == nullptr) {
    this->lock->unlock();
    return 1;
  }
  {
    std::stringstream ss;
    ss << "Old dm=" << dm << "\tNew dm=" << temp;
    Debug::Log(ss.str());
  }
  delete dm;
  dm = temp;
  this->lock->unlock();
  return 0;
}

int RoukaVici::GetLogMode()
{
  return Debug::GetLogMode();
}

void RoukaVici::SetLogMode(const int mode)
{
  Debug::SetLogMode(mode);
}

void RoukaVici::SetLogFile(const std::string& name)
{
  Debug::SetLogFile(name);
}

void RoukaVici::RegisterDebugCallback(DebugCallback callback)
{
  Debug::RegisterCallback(callback);
}

void RoukaVici::RegisterUnityDebugCallback(DebugCallback callback)
{
  Debug::Log("UnityDebugCallback is deprecated and will be removed in later versions. Please use RegisterCallback instead", true);
  Debug::RegisterCallback(callback);
}