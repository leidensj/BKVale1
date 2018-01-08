#include "user.h"
#include <QCryptographicHash>
#include <QObject>

#define ADMIN_USERNAME "ADMIN"
#define ADMIN_PASSWORD "a1b2c3d4"

namespace
{
  QString hash(const QString& strPassword)
  {
    return QString(QCryptographicHash::hash(strPassword.toUtf8(),
                                            QCryptographicHash::Md5));
  }
}

User::User()
  : m_bAccessNote(false)
  , m_bAccessReminder(false)
  , m_bAccessCalculator(false)
  , m_bAccessShop(false)
  , m_bAccessConsumption(false)
  , m_bAccessUser(false)
  , m_bAccessItem(false)
  , m_bAccessSettings(false)
{

}

User::User(const QString& strUser,
           const QString& strPassword,
           bool bAccessNote,
           bool bAccessReminder,
           bool bAccessCalculator,
           bool bAccessShop,
           bool bAccessConsumption,
           bool bAccessUser,
           bool bAccessItem,
           bool bAccessSettings)
  : m_strUser(strUser)
  , m_strPassword(strPassword)
  , m_bAccessNote(bAccessNote)
  , m_bAccessReminder(bAccessReminder)
  , m_bAccessCalculator(bAccessCalculator)
  , m_bAccessShop(bAccessShop)
  , m_bAccessConsumption(bAccessConsumption)
  , m_bAccessUser(bAccessUser)
  , m_bAccessItem(bAccessItem)
  , m_bAccessSettings(bAccessSettings)
{

}

UserLogin::UserLogin()
{

}

bool UserLogin::login(const QString& strUser,
           const QString& strPassword,
           QString& error)
{
  if (st_isAdmin(strUser))
    return loginAdmin(strUser, strPassword, error);
  error = QObject::tr("TODO");
  return false;
}

bool UserLogin::st_isAdmin(const QString& strUser)
{
  return strUser.compare(ADMIN_USERNAME, Qt::CaseInsensitive) == 0;
}

bool UserLogin::loginAdmin(const QString& strUser,
                      const QString& strPassword,
                      QString& error)
{
  error.clear();

  if (!st_isAdmin(strUser))
  {
    error = QObject::tr("O usuário '%1' não é administrador.").arg(strUser);
    return false;
  }

  if (strPassword.compare(ADMIN_PASSWORD) != 0)
  {
    error = QObject::tr("Senha do administrador incorreta.");
    return false;
  }

  m_user.m_strUser = strUser;
  m_user.m_strPassword = strPassword;
  enableAll(true);
  return true;
}

void UserLogin::enableAll(bool bEnable)
{
  m_user.m_bAccessNote = bEnable;
  m_user.m_bAccessReminder = bEnable;
  m_user.m_bAccessCalculator = bEnable;
  m_user.m_bAccessShop = bEnable;
  m_user.m_bAccessConsumption = bEnable;
  m_user.m_bAccessUser = bEnable;
  m_user.m_bAccessItem = bEnable;
  m_user.m_bAccessSettings = bEnable;
}

bool UserLogin::isAdmin() const
{
  return st_isAdmin(m_user.m_strUser);
}