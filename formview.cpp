#include "formview.h"
#include "jlineedit.h"
#include "jdatabasepicker.h"
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QDateEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include "jdatabase.h"
#include <QAction>
#include <QGroupBox>
#include "phonetablewidget.h"
#include "addresstablewidget.h"

FormView::FormView(QWidget* parent)
  : JItemView(FORM_SQL_TABLE_NAME, parent)
  , m_rdoPerson(nullptr)
  , m_rdoCompany(nullptr)
  , m_edName(nullptr)
  , m_lblName(nullptr)
  , m_edAlias(nullptr)
  , m_lblAlias(nullptr)
  , m_edEmail(nullptr)
  , m_edCpfCnpj(nullptr)
  , m_lblCpfCnpj(nullptr)
  , m_edRgIE(nullptr)
  , m_lblRgIE(nullptr)
  , m_edDetails(nullptr)
  , m_dtBirthDate(nullptr)
  , m_cbBirthDate(nullptr)
  , m_imagePicker(nullptr)
  , m_dtCreationDate(nullptr)
  , m_tblPhone(nullptr)
  , m_btnPhoneAdd(nullptr)
  , m_btnPhoneRemove(nullptr)
  , m_tblAddress(nullptr)
  , m_btnAddressAdd(nullptr)
  , m_btnAddressRemove(nullptr)
{
  m_rdoPerson = new QRadioButton;
  m_rdoPerson->setText(tr("Fisíca"));
  m_rdoPerson->setChecked(true);
  m_rdoCompany = new QRadioButton;
  m_rdoCompany->setText(tr("Jurídica"));
  m_edName = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                           JLineEdit::st_defaultFlags1);
  m_lblName = new QLabel;
  m_edAlias = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                            JLineEdit::st_defaultFlags1);
  m_lblAlias = new QLabel;
  m_edEmail = new JLineEdit(JLineEdit::Input::All,
                            JLineEdit::st_defaultFlags2);
  m_edCpfCnpj = new JLineEdit(JLineEdit::Input::Numeric,
                              JLineEdit::st_defaultFlags2);
  m_lblCpfCnpj = new QLabel;
  m_edRgIE= new JLineEdit(JLineEdit::Input::Numeric,
                          JLineEdit::st_defaultFlags2);
  m_lblRgIE = new QLabel;
  m_edDetails = new JLineEdit(JLineEdit::Input::AlphanumericAndSpaces,
                              JLineEdit::st_defaultFlags1);
  m_dtBirthDate = new QDateEdit;
  m_dtBirthDate->setCalendarPopup(true);
  m_dtBirthDate->setDisplayFormat("dd/MM/yyyy");
  m_dtBirthDate->setDate(QDate::currentDate());
  m_dtBirthDate->setSpecialValueText(tr("Não informada"));
  m_cbBirthDate = new QCheckBox;
  m_cbBirthDate->setChecked(false);

  m_imagePicker = new JDatabasePicker(IMAGE_SQL_TABLE_NAME);
  m_dtCreationDate = new QDateEdit;
  m_dtCreationDate->setCalendarPopup(true);
  m_dtCreationDate->setDisplayFormat("dd/MM/yyyy");
  m_dtCreationDate->setDate(QDate::currentDate());
  m_dtCreationDate->setReadOnly(true);

  m_tblPhone = new PhoneTableWidget;
  m_btnPhoneAdd = new QPushButton(QIcon(":/icons/res/additem.png"), "");
  m_btnPhoneAdd->setFlat(true);
  m_btnPhoneAdd->setIconSize(QSize(24, 24));
  m_btnPhoneRemove = new QPushButton(QIcon(":/icons/res/removeitem.png"), "");
  m_btnPhoneRemove->setFlat(true);
  m_btnPhoneRemove->setIconSize(QSize(24, 24));

  m_tblAddress = new AddressTableWidget;
  m_btnAddressAdd = new QPushButton;
  m_btnAddressAdd->setFlat(true);
  m_btnAddressAdd->setIconSize(QSize(24, 24));
  m_btnAddressAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAddressAdd->setToolTip(tr("Adicionar endereço"));
  m_btnAddressRemove = new QPushButton;
  m_btnAddressRemove->setFlat(true);
  m_btnAddressRemove->setIconSize(QSize(24, 24));
  m_btnAddressRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnAddressRemove->setToolTip(tr("Remover endereço"));

  QHBoxLayout* ltType = new QHBoxLayout;
  ltType->addWidget(m_rdoPerson);
  ltType->addWidget(m_rdoCompany);
  ltType->addStretch();

  QFormLayout* lt = new QFormLayout;
  lt->addRow(tr("Data de criação:"), m_dtCreationDate);
  lt->addRow(tr("Tipo:"), ltType);
  lt->addRow(m_lblName, m_edName);
  lt->addRow(m_lblAlias, m_edAlias);

  QFormLayout* ltInfo = new QFormLayout;
  ltInfo->addRow(tr("Email:"), m_edEmail);
  ltInfo->addRow(m_lblCpfCnpj, m_edCpfCnpj);
  ltInfo->addRow(m_lblRgIE, m_edRgIE);
  ltInfo->addRow(tr("Detalhes:"), m_edDetails);
  ltInfo->addRow(m_cbBirthDate, m_dtBirthDate);
  ltInfo->addRow(tr("Imagem:"), m_imagePicker);

  QVBoxLayout* ltPhoneBtn = new QVBoxLayout;
  ltPhoneBtn->setContentsMargins(0, 0, 0, 0);
  ltPhoneBtn->setAlignment(Qt::AlignTop);
  ltPhoneBtn->addWidget(m_btnPhoneAdd);
  ltPhoneBtn->addWidget(m_btnPhoneRemove);

  QHBoxLayout* ltPhone = new QHBoxLayout;
  ltPhone->addWidget(m_tblPhone);
  ltPhone->addLayout(ltPhoneBtn);

  QVBoxLayout* ltAddressBtn = new QVBoxLayout;
  ltAddressBtn->setContentsMargins(0, 0, 0, 0);
  ltAddressBtn->setAlignment(Qt::AlignTop);
  ltAddressBtn->addWidget(m_btnAddressAdd);
  ltAddressBtn->addWidget(m_btnAddressRemove);

  QHBoxLayout* ltAddress = new QHBoxLayout;
  ltAddress->addWidget(m_tblAddress);
  ltAddress->addLayout(ltAddressBtn);

  QFrame* fr = new QFrame;
  fr->setLayout(lt);

  QFrame* frInfo = new QFrame;
  frInfo->setLayout(ltInfo);

  QFrame* frPhone = new QFrame;
  frPhone->setLayout(ltPhone);

  QFrame* frAddress = new QFrame;
  frAddress->setLayout(ltAddress);

  m_tab->addTab(fr, QIcon(":/icons/res/resume.png"), tr("Perfil"));
  m_tab->addTab(frInfo, QIcon(":/icons/res/details.png"), tr("Informações"));
  m_tab->addTab(frPhone, QIcon(":/icons/res/phone.png"), tr("Telefone"));
  m_tab->addTab(frAddress, QIcon(":/icons/res/address.png"), tr("Endereço"));

  connect(m_rdoCompany, SIGNAL(toggled(bool)), this, SLOT(switchUserType()));
  connect(m_rdoPerson, SIGNAL(toggled(bool)), this, SLOT(switchUserType()));
  connect(m_cbBirthDate, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_btnPhoneAdd, SIGNAL(clicked(bool)), m_tblPhone, SLOT(addItem()));
  connect(m_btnPhoneRemove, SIGNAL(clicked(bool)), m_tblPhone, SLOT(removeItem()));
  connect(m_tblPhone, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_btnAddressAdd, SIGNAL(clicked(bool)), m_tblAddress, SLOT(addItem()));
  connect(m_btnAddressRemove, SIGNAL(clicked(bool)), m_tblAddress, SLOT(removeItem()));
  connect(m_tblAddress, SIGNAL(changedSignal()), this, SLOT(updateControls()));

  switchUserType();
  m_edName->setFocus();
}

FormView::~FormView()
{

}

const JItemSQL& FormView::getItem() const
{
  m_ref.clear();
  m_ref.m_id = m_currentId;
  m_ref.m_image.m_id = m_imagePicker->getId();
  m_ref.m_name = m_edName->text();
  m_ref.m_alias = m_edAlias->text();
  m_ref.m_email = m_edEmail->text();
  m_ref.m_CPF_CNPJ = m_edCpfCnpj->text();
  m_ref.m_RG_IE = m_edRgIE->text();
  m_ref.m_details = m_edDetails->text();
  m_ref.m_bBirth = m_cbBirthDate->isChecked() && !m_rdoCompany->isChecked();
  m_ref.m_dtBirth = m_dtBirthDate->date();
  m_ref.m_dtCreation = m_dtCreationDate->date();
  m_ref.m_bCompany = m_rdoCompany->isChecked();

  for (int i = 0; i != m_tblPhone->rowCount(); ++i)
    m_ref.m_vPhone.push_back(dynamic_cast<const Phone&>(m_tblPhone->getItem(i)));

  for (int i = 0; i != m_tblAddress->rowCount(); ++i)
    m_ref.m_vAddress.push_back(dynamic_cast<const Address&>(m_tblAddress->getItem(i)));

  return m_ref;
}

void FormView::setItem(const JItemSQL &o)
{
  auto _o = dynamic_cast<const Form&>(o);
  m_currentId = _o.m_id;
  m_imagePicker->setItem(_o.m_image);
  m_edName->setText(_o.m_name);
  m_edAlias->setText(_o.m_alias);
  m_edEmail->setText(_o.m_email);
  m_edCpfCnpj->setText(_o.m_CPF_CNPJ);
  m_edRgIE->setText(_o.m_RG_IE);
  m_edDetails->setText(_o.m_details);
  m_cbBirthDate->setChecked(_o.m_bBirth && !_o.m_bCompany);
  m_dtBirthDate->setDate(_o.m_dtBirth);
  m_dtCreationDate->setDate(_o.m_dtCreation);
  m_rdoCompany->setChecked(_o.m_bCompany);
  m_rdoPerson->setChecked(!_o.m_bCompany);
  switchUserType();

  m_tblPhone->removeAllItems();
  m_tblAddress->removeAllItems();

  for (int i = 0; i != _o.m_vPhone.size(); ++i)
    m_tblPhone->addItem(_o.m_vPhone.at(i));

  for (int i = 0; i != _o.m_vAddress.size(); ++i)
    m_tblAddress->addItem(_o.m_vAddress.at(i));
}

void FormView::create()
{
  selectItem(Form());
  m_tab->setCurrentIndex(0);
  updateControls();
  m_edName->setFocus();
}

void FormView::updateControls()
{
  m_dtBirthDate->setEnabled(m_cbBirthDate->isChecked());
  if (m_cbBirthDate->isChecked() &&
      m_dtBirthDate->date() == m_dtBirthDate->minimumDate())
    m_dtBirthDate->setDate(QDate::currentDate());

  m_btnPhoneRemove->setEnabled(m_tblPhone->isValidCurrentRow());
  m_btnAddressRemove->setEnabled(m_tblAddress->isValidCurrentRow());
}

void FormView::switchUserType()
{
  if (m_rdoCompany->isChecked())
  {
    m_lblName->setText(tr("Razão social:"));
    m_lblAlias->setText(tr("Nome fantasia:"));
    m_lblCpfCnpj->setText(tr("CNPJ:"));
    m_lblRgIE->setText(tr("IE:"));
    m_cbBirthDate->setText("Fundação:");
    m_edCpfCnpj->setInputMask("99.999.999/9999-99;_");
    m_edCpfCnpj->setText(m_ref.m_bCompany ? m_ref.m_CPF_CNPJ : "");
    m_edRgIE->setInputMask("");
    m_edRgIE->setText(m_ref.m_bCompany ? m_ref.m_RG_IE : "");
  }
  else
  {
    m_lblName->setText(tr("Nome:"));
    m_lblAlias->setText(tr("Apelido:"));
    m_lblCpfCnpj->setText(tr("CPF:"));
    m_lblRgIE->setText(tr("RG:"));
    m_cbBirthDate->setText("Nascimento:");
    m_edCpfCnpj->setInputMask("999.999.999-99;_");
    m_edCpfCnpj->setText(!m_ref.m_bCompany ? m_ref.m_CPF_CNPJ : "");
    m_edRgIE->setInputMask("9999999999;_");
    m_edRgIE->setText(!m_ref.m_bCompany ? m_ref.m_RG_IE : "");
  }
  updateControls();
}
