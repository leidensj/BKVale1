#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "noteview.h"
#include "printutils.h"
#include "productview.h"
#include "categoryview.h"
#include "noteview.h"
#include "reminderview.h"
#include "consumptionwidget.h"
#include "calculatorwidget.h"
#include "usermgtview.h"
#include "imageview.h"
#include "logindialog.h"
#include "personview.h"
#include "shoppinglistview.h"
#include "reservationview.h"
#include "shopview.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QByteArray>
#include <QDir>
#include <QLabel>

BaitaAssistant::BaitaAssistant(const UserLoginSQL& userLogin, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::BaitaAssistant)
  , m_userLogin(userLogin)
  , m_note(nullptr)
  , m_reminder(nullptr)
  , m_consumption(nullptr)
  , m_calculator(nullptr)
  , m_shop(nullptr)
  , m_reservation(nullptr)
  , m_statusDatabasePath(nullptr)
  , m_statusUserName(nullptr)
{
  ui->setupUi(this);
  m_note = new NoteView;
  m_reminder = new ReminderView;
  m_consumption = new ConsumptionWidget;
  m_calculator = new CalculatorWidget;
  m_shop = new ShopView;
  m_reservation = new ReservationView;
  ui->tabNotes->layout()->addWidget(m_note);
  ui->tabReminder->layout()->addWidget(m_reminder);
  ui->tabConsumption->layout()->addWidget(m_consumption);
  ui->tabCalculator->layout()->addWidget(m_calculator);
  ui->tabShop->layout()->addWidget(m_shop);
  ui->tabReservation->layout()->addWidget(m_reservation);

  m_statusDatabasePath = new QLabel();
  m_statusDatabasePath->setAlignment(Qt::AlignRight);
  m_statusDatabasePath->setTextFormat(Qt::RichText);
  m_statusUserName = new QLabel();
  m_statusUserName->setAlignment(Qt::AlignCenter);
  m_statusUserName->setTextFormat(Qt::RichText);
  statusBar()->addWidget(m_statusDatabasePath);
  statusBar()->addWidget(m_statusUserName);

  QObject::connect(ui->actionConnect,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(connect()));

  QObject::connect(ui->actionDisconnect,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(disconnect()));

  QObject::connect(ui->actionPrint,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(print()));

  QObject::connect(ui->actionSettings,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openSettingsDialog()));

  QObject::connect(ui->actionInfo,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(showInfo()));

  QObject::connect(m_note,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(ui->tabWidget,
                   SIGNAL(currentChanged(int)),
                   this,
                   SLOT(updateControls()));

  QObject::connect(m_reminder,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  QObject::connect(ui->actionProducts,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openProductsDialog()));

  QObject::connect(ui->actionCategories,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openCategoriesDialog()));

  QObject::connect(ui->actionUsers,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openUsersDialog()));

  QObject::connect(ui->actionImages,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openImagesDialog()));

  QObject::connect(m_calculator,
                   SIGNAL(printSignal(const QString&)),
                   this,
                   SLOT(print(const QString&)));

  QObject::connect(&m_printerTCP,
                   SIGNAL(connected()),
                   this,
                   SLOT(connectedTCP()));

  QObject::connect(ui->actionLogin,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openLoginDialog()));

  QObject::connect(ui->actionPersons,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openPersonsDialog()));

  QObject::connect(ui->actionShoppingList,
                   SIGNAL(triggered(bool)),
                   this,
                   SLOT(openShoppingListDialog()));

  QObject::connect(m_shop,
                   SIGNAL(changedSignal()),
                   this,
                   SLOT(updateControls()));

  m_settings.load();
  if (!m_settings.m_serialPort.isEmpty() &&
      m_settings.m_bConnectOnStartup)
  {
    connect();
  }
  m_note->setDatabase(m_userLogin.getDatabase());
  m_note->create();
  m_consumption->setDatabase(m_userLogin.getDatabase());
  m_reminder->setDatabase(m_userLogin.getDatabase());
  m_shop->setDatabase(m_userLogin.getDatabase());
  m_reservation->setDatabase(m_userLogin.getDatabase());
  updateControls();
  updateStatusBar();
}

BaitaAssistant::~BaitaAssistant()
{
  delete ui;
}

void BaitaAssistant::connect()
{
  if (m_printerSerial.isOpen())
    m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();

  if (m_settings.m_serialPort.isEmpty() &&
      m_settings.m_ethernetIP.isEmpty())
  {
    QMessageBox::warning(this,
                       tr("Porta ou endereço não informado"),
                       tr("É necessário selecionar uma porta serial "
                          "ou endereço para se conectar à impressora."),
                       QMessageBox::Ok);
    updateControls();
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  QString error;
  bool bSuccess = false;
  if (m_settings.m_interfaceType == InterfaceType::Serial)
  {
    m_printerSerial.setPortName(m_settings.m_serialPort);
    bSuccess = m_printerSerial.open((QIODevice::ReadWrite));
    if (bSuccess)
      bSuccess = Printer::printString(&m_printerSerial,
                                      m_settings.m_interfaceType,
                                      Printer::strCmdInit(),
                                      error);
    else
      error = m_printerSerial.errorString();

  }
  else if (m_settings.m_interfaceType == InterfaceType::Ethernet)
  {
    m_printerTCP.connectToHost(m_settings.m_ethernetIP,
                               (quint16)m_settings.m_ethernetPort);
    bSuccess = m_printerTCP.waitForConnected();
    if (bSuccess)
      bSuccess = Printer::printString(&m_printerTCP,
                                      m_settings.m_interfaceType,
                                      Printer::strCmdInit(),
                                      error);
    else
      error = m_printerTCP.errorString();
  }

  QApplication::restoreOverrideCursor();

  if (!bSuccess)
  {
    QMessageBox::critical(this,
                       tr("Erro"),
                       tr("O seguinte erro ocorreu ao conectar à impressora:\n"
                          "'%1'.").arg(error),
                       QMessageBox::Ok);
  }
  updateControls();
}

void BaitaAssistant::connectedTCP()
{
  setWindowTitle("CONECTOU");
}

void BaitaAssistant::disconnect()
{
  if (m_printerSerial.isOpen())
      m_printerSerial.close();
  if (m_printerTCP.isOpen())
    m_printerTCP.close();
  updateControls();
}

void BaitaAssistant::print()
{
  QIODevice* printer = nullptr;
  if (m_settings.m_interfaceType == InterfaceType::Serial)
    printer = &m_printerSerial;
  else if (m_settings.m_interfaceType == InterfaceType::Ethernet)
    printer = &m_printerTCP;

  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
      m_note->saveAndPrint(printer, m_settings.m_interfaceType);
      break;
    case Functionality::ReminderMode:
      m_reminder->saveAndPrint(printer, m_settings.m_interfaceType);
      break;
    case Functionality::ConsumptionMode:
      print(m_consumption->printContent());
      break;
    case Functionality::CalculatorMode:
      m_calculator->print(printer, m_settings.m_interfaceType);
      break;
    case Functionality::ShopMode:
      m_shop->print(printer, m_settings.m_interfaceType);
      break;
    case Functionality::ReservationMode:
    {
      Reservation res = m_reservation->save();
      if (res.isValidId())
      {
        QString error;
        Printer::printString(printer,
                             m_settings.m_interfaceType,
                             ReservationPrinter::build(res),
                             error);
      }
    } break;
  }
}

void BaitaAssistant::print(const QString& text)
{
  QIODevice* printer = nullptr;
  if (m_settings.m_interfaceType == InterfaceType::Serial)
    printer = &m_printerSerial;
  else if (m_settings.m_interfaceType == InterfaceType::Ethernet)
    printer = &m_printerTCP;

  QString error;
  if (!Printer::printString(printer,
                            m_settings.m_interfaceType,
                            text,
                            error))
  {
    QMessageBox::critical(this,
                          tr("Erro"),
                          tr("Erro '%1' ao imprimir.").arg(error),
                          QMessageBox::Ok);
  }
}

void BaitaAssistant::openSettingsDialog()
{
  if (m_printerTCP.isOpen() || m_printerSerial.isOpen())
  {
    QMessageBox::warning(this,
                         tr("Aviso"),
                         tr("Desconecte a impressora primeiro."),
                         QMessageBox::Ok);
  }
  else
  {
    SettingsDlg dlg(m_settings);
    if (dlg.exec() == QDialog::Accepted)
    {
      m_settings = dlg.getSettings();
      m_settings.save();
    }
  }
}

void BaitaAssistant::updateStatusBar()
{
  // para adicionar um ícone:
  // "<img src=':/icons/res/16user.png'> " + ...

  m_statusUserName->setText(tr("Usuário: ") + m_userLogin.strUser());
  m_statusDatabasePath->setText(tr("Banco de dados: ") + m_settings.databasePath());
}

void BaitaAssistant::updateControls()
{
  const bool bIsSQLOk = m_userLogin.getDatabase().isOpen();
  const bool bIsOpen = m_printerSerial.isOpen() || m_printerTCP.isOpen();
  ui->actionConnect->setEnabled(!bIsOpen && bIsSQLOk);
  ui->actionDisconnect->setEnabled(bIsOpen && bIsSQLOk);
  ui->actionSettings->setEnabled(bIsSQLOk && !bIsOpen && m_userLogin.hasAccessToSettings());
  ui->actionLogin->setEnabled(bIsSQLOk);
  ui->actionUsers->setEnabled(bIsSQLOk && m_userLogin.hasAccessToUsers());
  ui->actionProducts->setEnabled(bIsSQLOk && m_userLogin.hasAccessToProducts());
  ui->actionPersons->setEnabled(bIsSQLOk && m_userLogin.hasAccessToPersons());
  ui->actionCategories->setEnabled(bIsSQLOk && m_userLogin.hasAccessToCategories());
  ui->actionImages->setEnabled(bIsSQLOk && m_userLogin.hasAccessToImages());

  ui->tabWidget->setTabEnabled((int)Functionality::NoteMode,
                               bIsSQLOk && m_userLogin.hasAccessToNote());
  ui->tabWidget->setTabEnabled((int)Functionality::ReminderMode,
                               bIsSQLOk && m_userLogin.hasAccessToReminder());
  ui->tabWidget->setTabEnabled((int)Functionality::CalculatorMode,
                               bIsSQLOk && m_userLogin.hasAccessToCalculator());
  ui->tabWidget->setTabEnabled((int)Functionality::ShopMode,
                               bIsSQLOk && m_userLogin.hasAccessToShop());
  ui->tabWidget->setTabEnabled((int)Functionality::ConsumptionMode,
                               bIsSQLOk && m_userLogin.hasAccessToConsumption());

  m_calculator->enablePrinter(bIsOpen);

  switch ((Functionality)ui->tabWidget->currentIndex())
  {
    case Functionality::NoteMode:
      ui->actionPrint->setEnabled(m_note->getNote().isValid());
      break;
    case Functionality::ReminderMode:
      ui->actionPrint->setEnabled(m_reminder->getReminder().isValid() && bIsOpen);
      break;
      case Functionality::CalculatorMode:
      ui->actionPrint->setEnabled(bIsOpen);
      break;
    case Functionality::ConsumptionMode:
      ui->actionPrint->setEnabled(m_consumption->isValid() && bIsOpen);
      break;
    case Functionality::ShopMode:
      ui->actionPrint->setEnabled(m_shop->getShoppingList().isValidId() && bIsOpen);
      break;
    case Functionality::ReservationMode:
      ui->actionPrint->setEnabled(m_reservation->getReservation().isValid() && bIsOpen);
      break;
    default:
      break;
  }
}

void BaitaAssistant::showInfo()
{

}

void BaitaAssistant::openProductsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ProductView* w = new ProductView(this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Produtos"));
  dlg.setWindowIcon(QIcon(":/icons/res/item.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openCategoriesDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  CategoryView* w = new CategoryView(this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Categorias"));
  dlg.setWindowIcon(QIcon(":/icons/res/category.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openUsersDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  UserMgtView* w = new UserMgtView(m_userLogin.getId(), this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Usuários"));
  dlg.setWindowIcon(QIcon(":/icons/res/user.png"));
  dlg.setModal(true);
  dlg.exec();

  if (w->hasLoggedUserChanged())
    openLoginDialog();
}

void BaitaAssistant::openImagesDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ImageView* w = new ImageView(this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Imagens"));
  dlg.setWindowIcon(QIcon(":/icons/res/icon.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openLoginDialog()
{
  hide();
  LoginDialog l(m_userLogin);
  if (!l.exec())
  {
    QMessageBox::critical(this,
                          tr("Login não realizado"),
                          tr("A aplicação será encerrada."),
                          QMessageBox::Ok);
    close();
  }
  else
  {
    show();
    updateControls();
    updateStatusBar();
  }
}

void BaitaAssistant::openPersonsDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  PersonView* w = new PersonView(this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Pessoas"));
  dlg.setWindowIcon(QIcon(":/icons/res/person.png"));
  dlg.setModal(true);
  dlg.exec();
}

void BaitaAssistant::openShoppingListDialog()
{
  QDialog dlg(this);
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  ShoppingListView* w = new ShoppingListView(this);
  w->setDatabase(m_userLogin.getDatabase());
  layout->addWidget(w);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Gerenciar Listas de Compras"));
  dlg.setWindowIcon(QIcon(":/icons/res/shopmgt.png"));
  dlg.setModal(true);
  dlg.exec();
}
