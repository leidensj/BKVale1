#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <databaseutils.h>
#include "activeuser.h"

class JLineEdit;
class QLabel;
class QPushButton;
class QFrame;
class JSpinBox;
class QProgressBar;

class LoginDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LoginDialog(ActiveUser& login,
                       QWidget* parent = 0);

private slots:
  void login();
  void showDatabaseOpt();
  void focusLogin();

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  void updateCapsLock();
  ActiveUser& m_login;
  JLineEdit* m_user;
  JLineEdit* m_password;
  JLineEdit* m_hostName;
  JSpinBox* m_port;
  QLabel* m_capsLock;
  QLabel* m_status;
  QPushButton* m_btnDatabaseOpt;
  QFrame* m_frDatabase;
  QPushButton* m_btnLogin;
};

#endif // LOGINDIALOG_H
