#include "mainwindow.h"
#include "logindialog.h"
#include "databaseutils.h"
#include "settings.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QDir>
#include <QFileDialog>

namespace
{
bool initDatabase(QSqlDatabase db,
                  const QString& filePath,
                  QString& error)
{
  bool bSuccess = BaitaSQL::open(db,
                                 filePath,
                                 error);
  if (bSuccess)
    bSuccess = BaitaSQL::init(db, error);
  else
    db.close();

  return bSuccess;
}

QString getPath()
{
  return QFileDialog::getExistingDirectory(nullptr,
                                           QObject::tr("Selecionar local para salvar seus dados"),
                                           "/home",
                                           QFileDialog::ShowDirsOnly
                                           | QFileDialog::DontResolveSymlinks);
}
}


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Settings settings;
  settings.load();
  if (settings.m_fileDir.isEmpty())
    settings.m_fileDir = getPath();

  if (settings.m_fileDir.isEmpty())
    return 0;

  settings.save();
  QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE"));
  QString error;
  if (!initDatabase(db, settings.filePath(), error))
  {
    QMessageBox msgBox(QMessageBox::Critical,
                       QObject::tr("Erro ao inicializar banco de dados"),
                       error,
                       QMessageBox::Ok);
    msgBox.exec();
  }
  else
  {
    UserLoginSQL userLogin;
    userLogin.setDatabase(db);
    LoginDialog l(userLogin);
    if (l.exec() == QDialog::Accepted)
    {
      BaitaAssistant w(userLogin);
      w.show();
      return a.exec();
    }
  }
  return 0;
}
