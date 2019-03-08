#include "noteview.h"
#include "databaseutils.h"
#include "notetablewidget.h"
#include "person.h"
#include "jdatabasepicker.h"
#include "jdatabase.h"
#include "jlineedit.h"
#include "jdateedit.h"
#include "packageeditor.h"
#include "jdatepicker.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QSplitter>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

NoteDetailsDlg::NoteDetailsDlg(QWidget* parent)
  : QDialog(parent)
  , m_teDetails(nullptr)
{
  m_teDetails = new QPlainTextEdit;
  QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
  connect(btn, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout* lt = new QVBoxLayout;
  lt->addWidget(m_teDetails);
  lt->addWidget(btn);

  setWindowTitle(tr("Observações"));
  setWindowIcon(QIcon(":/icons/res/details.png"));

  setLayout(lt);
  m_teDetails->setFocus();
}

void NoteDetailsDlg::setDetails(const QString& str)
{
  m_teDetails->setPlainText(str);
}

QString NoteDetailsDlg::getDetails() const
{
  return m_teDetails->toPlainText();
}

#define DISCCOUNT_LAST_VALUE_PROP "lastValue"

NoteView::NoteView(QWidget *parent)
  : QFrame(parent)
  , m_btnCreate(nullptr)
  , m_btnOpenLast(nullptr)
  , m_btnSearch(nullptr)
  , m_btnAddCode(nullptr)
  , m_btnAdd(nullptr)
  , m_btnRemove(nullptr)
  , m_snNumber(nullptr)
  , m_dtPicker(nullptr)
  , m_edTotal(nullptr)
  , m_supplierPicker(nullptr)
  , m_table(nullptr)
  , m_cbCash(nullptr)
  , m_edDisccount(nullptr)
  , m_btnDetails(nullptr)
  , m_dlgDetails(nullptr)
  , m_lblEntries(nullptr)
  , m_lblSum(nullptr)
{
  m_btnCreate = new QPushButton();
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
  m_btnCreate->setToolTip(tr("Novo vale (Ctrl+N)"));

  m_btnSearch = new QPushButton();
  m_btnSearch->setFlat(true);
  m_btnSearch->setText("");
  m_btnSearch->setIconSize(QSize(24, 24));
  m_btnSearch->setIcon(QIcon(":/icons/res/search.png"));
  m_btnSearch->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
  m_btnSearch->setToolTip(tr("Procurar vales (Ctrl+F)"));
  m_btnSearch->setCheckable(true);

  m_btnOpenLast = new QPushButton();
  m_btnOpenLast->setFlat(true);
  m_btnOpenLast->setText("");
  m_btnOpenLast->setIconSize(QSize(24, 24));
  m_btnOpenLast->setIcon(QIcon(":/icons/res/openlast.png"));
  m_btnOpenLast->setToolTip(tr("Abrir último vale"));

  QFrame* vFrame0 = new QFrame;
  vFrame0->setFrameShape(QFrame::VLine);

  m_btnAddCode = new QPushButton();
  m_btnAddCode->setFlat(true);
  m_btnAddCode->setText("");
  m_btnAddCode->setIconSize(QSize(24, 24));
  m_btnAddCode->setIcon(QIcon(":/icons/res/barcodescan.png"));
  m_btnAddCode->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Asterisk));
  m_btnAddCode->setToolTip(tr("Adicionar item (Alt+*)"));

  m_btnAdd = new QPushButton();
  m_btnAdd->setFlat(true);
  m_btnAdd->setText("");
  m_btnAdd->setIconSize(QSize(24, 24));
  m_btnAdd->setIcon(QIcon(":/icons/res/additem.png"));
  m_btnAdd->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Plus));
  m_btnAdd->setToolTip(tr("Adicionar item (Alt++)"));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/removeitem.png"));
  m_btnRemove->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Minus));
  m_btnRemove->setToolTip(tr("Remover item (Alt+-)"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->setAlignment(Qt::AlignLeft);
  hlayout1->addWidget(m_btnCreate);
  hlayout1->addWidget(m_btnSearch);
  hlayout1->addWidget(m_btnOpenLast);
  hlayout1->addWidget(vFrame0);
  hlayout1->addWidget(m_btnAddCode);
  hlayout1->addWidget(m_btnAdd);
  hlayout1->addWidget(m_btnRemove);

  QLabel* lblNumber = new QLabel();
  lblNumber->setText(tr("Número:"));
  {
    QFont font = lblNumber->font();
    font.setPointSize(12);
    lblNumber->setFont(font);
  }

  m_snNumber = new QSpinBox();
  m_snNumber->setReadOnly(true);
  m_snNumber->setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
  m_snNumber->setMaximum(99999999);
  m_snNumber->setMinimum(0);
  m_snNumber->setSpecialValueText(tr("S/N"));
  m_snNumber->setMinimumSize(90, 0);
  m_snNumber->setAlignment(Qt::AlignRight);
  m_snNumber->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
  {
    QFont font = m_snNumber->font();
    font.setBold(true);
    m_snNumber->setFont(font);
    QPalette palette = m_snNumber->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_snNumber->setPalette(palette);
  }

  m_dtPicker = new JDatePicker;

  QLabel* lblDate = new QLabel();
  lblDate->setText(tr("Data:"));
  {
    QFont font = lblDate->font();
    font.setPointSize(12);
    lblDate->setFont(font);
  }

  m_cbCash = new QCheckBox;
  m_cbCash->setText(tr("À vista"));
  m_cbCash->setIconSize(QSize(24,24));
  m_cbCash->setIcon(QIcon(":/icons/res/cash.png"));
  m_cbCash->setToolTip(tr("Imprime apenas uma via, "
                          "com espaço para assinatura "
                          "do fornecedor"));

  m_dlgDetails = new NoteDetailsDlg(this);

  m_btnDetails = new QPushButton;
  m_btnDetails->setFlat(true);
  m_btnDetails->setText("");
  m_btnDetails->setIconSize(QSize(24, 24));
  m_btnDetails->setIcon(QIcon(":/icons/res/details.png"));
  m_btnDetails->setToolTip(tr("Observações"));

  QFrame* line1 = new QFrame;
  line1->setFrameShape(QFrame::VLine);

  QFrame* line2 = new QFrame;
  line2->setFrameShape(QFrame::VLine);

  QFrame* line3 = new QFrame;
  line3->setFrameShape(QFrame::VLine);

  QHBoxLayout* hlayout2 = new QHBoxLayout();
  hlayout2->setContentsMargins(0, 0, 0, 0);
  hlayout2->setAlignment(Qt::AlignLeft);
  hlayout2->addWidget(lblNumber);
  hlayout2->addWidget(m_snNumber);
  hlayout2->addWidget(line1);
  hlayout2->addWidget(lblDate);
  hlayout2->addWidget(m_dtPicker);
  hlayout2->addWidget(line2);
  hlayout2->addWidget(m_cbCash);
  hlayout2->addWidget(line3);
  hlayout2->addWidget(m_btnDetails);

  m_supplierPicker = new JDatabasePicker(PERSON_SQL_TABLE_NAME,
                                         tr("Fornecedor"),
                                         QIcon(":/icons/res/supplier.png"));
  m_supplierPicker->setPlaceholderText(true);

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout2);
  vlayout1->addWidget(m_supplierPicker);

  QFrame* frame = new QFrame();
  frame->setFrameShape(QFrame::Shape::StyledPanel);
  frame->setFrameShadow(QFrame::Shadow::Plain);
  frame->setLayout(vlayout1);
  frame->setFixedHeight(frame->sizeHint().height());

  m_table = new NoteTableWidget;

  m_edTotal = new QLineEdit();
  m_edTotal->setReadOnly(true);
  m_edTotal->setPlaceholderText(tr("TOTAL"));
  m_edTotal->setAlignment(Qt::AlignRight);
  m_edTotal->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  {
    QFont font = m_edTotal->font();
    font.setBold(true);
    m_edTotal->setFont(font);
    QPalette palette = m_edTotal->palette();
    palette.setColor(QPalette::ColorRole::Text, Qt::red);
    m_edTotal->setPalette(palette);
  }

  m_edDisccount = new JExpLineEdit(JItem::DataType::Money);
  m_edDisccount->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  m_edDisccount->setAlignment(Qt::AlignRight);

  m_lblEntries = new QLabel;
  {
    QFont f = m_lblEntries->font();
    f.setPointSize(8);
    m_lblEntries->setFont(f);
  }
  m_lblSum = new QLabel;
  {
    QFont f = m_lblSum->font();
    f.setPointSize(8);
    m_lblSum->setFont(f);
  }

  QHBoxLayout* totalLayout = new QHBoxLayout;
  totalLayout->setContentsMargins(0, 0, 0, 0);
  totalLayout->addWidget(new QLabel(tr("Descontos/Acréscimos:")));
  totalLayout->addWidget(m_edDisccount);
  totalLayout->addStretch();
  totalLayout->addWidget(m_edTotal);

  QHBoxLayout* headerlayout = new QHBoxLayout;
  headerlayout->setContentsMargins(0, 0, 0, 0);
  headerlayout->addWidget(frame);

  QVBoxLayout* viewLayout = new QVBoxLayout;
  viewLayout->setContentsMargins(9, 0, 0, 0);
  viewLayout->addLayout(hlayout1);
  viewLayout->addLayout(headerlayout);
  viewLayout->addWidget(m_table);
  viewLayout->addLayout(totalLayout);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewLayout);

  m_database = new JDatabase(NOTE_SQL_TABLE_NAME);
  m_database->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

  QVBoxLayout* databaseLayout = new QVBoxLayout;
  databaseLayout->addWidget(m_database);
  databaseLayout->addWidget(m_lblEntries);
  databaseLayout->addWidget(m_lblSum);

  QFrame* databaseFrame = new QFrame;
  databaseFrame->setLayout(databaseLayout);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(viewFrame);
  splitter->addWidget(databaseFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  connect(m_database, SIGNAL(itemSelectedSignal(const JItem&)), this, SLOT(itemSelected(const JItem&)));
  connect(m_database, SIGNAL(itemsRemovedSignal(const QVector<Id>&)), this, SLOT(itemsRemoved(const QVector<Id>&)));
  connect(m_database, SIGNAL(refreshSignal()), this, SLOT(updateControls()));
  connect(m_btnSearch, SIGNAL(clicked(bool)), databaseFrame, SLOT(setVisible(bool)));
  connect(m_btnAdd, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnAddCode, SIGNAL(clicked(bool)), this, SLOT(addProduct()));
  connect(m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));
  connect(m_btnCreate, SIGNAL(clicked(bool)), this, SLOT(create()));
  connect(m_table, SIGNAL(changedSignal()), this, SLOT(updateControls()));
  connect(m_btnOpenLast, SIGNAL(clicked(bool)), this, SLOT(lastItemSelected()));
  connect(m_dtPicker, SIGNAL(dateChangedSignal()), this, SLOT(updateControls()));
  connect(m_cbCash, SIGNAL(clicked(bool)), this, SLOT(updateControls()));
  connect(m_supplierPicker, SIGNAL(changedSignal()), this, SLOT(supplierChanged()));
  connect(m_edDisccount, SIGNAL(editingFinished()), this, SLOT(updateControls()));
  connect(m_edDisccount, SIGNAL(enterSignal()), m_table, SLOT(setFocus()));
  connect(m_btnDetails, SIGNAL(clicked(bool)), this, SLOT(openDetailsDialog()));

  create();
  databaseFrame->hide();
  updateControls();

  m_supplierPicker->getDatabase()->setFixedFilter(PERSON_FILTER_SUPPLIER);
}

NoteView::~NoteView()
{

}

void NoteView::removeItem()
{
  m_table->removeItem();
  if (!m_table->hasItems())
    m_supplierPicker->setFocus();
  updateControls();
}

Note NoteView::getNote() const
{
  Note note;
  note.m_id = m_currentId;
  note.m_date = m_dtPicker->getDate();
  note.m_supplier.m_id = m_supplierPicker->getId();
  note.m_bCash = m_cbCash->isChecked();
  note.m_observation = m_dlgDetails->getDetails();
  note.m_disccount = m_edDisccount->getValue();
  for (int i = 0; i != m_table->rowCount(); ++i)
    note.m_vNoteItem.push_back(dynamic_cast<const NoteItem&>(m_table->getItem(i)));
  return note;
}

void NoteView::setNote(const Note& note)
{
  m_currentId = note.m_id;
  m_table->removeAllItems();
  m_supplierPicker->clear();
  m_dtPicker->setDate(note.m_date);
  m_snNumber->setValue(note.m_number);
  m_cbCash->setChecked(note.m_bCash);
  for (int i = 0; i != note.m_vNoteItem.size(); ++i)
    m_table->addItem(note.m_vNoteItem.at(i));
  m_supplierPicker->setItem(note.m_supplier);
  m_dlgDetails->setDetails(note.m_observation);
  m_edDisccount->setText(note.m_disccount);
  updateControls();
}

void NoteView::create()
{
  Note note;
  setNote(note);
  updateControls();
}

void NoteView::supplierChanged()
{
  if (m_supplierPicker->getId().isValid())
  {
    if (m_table->hasItems())
    {
      m_table->setCurrentCell(0, 0);
      m_table->setFocus();
    }
    else if (!m_currentId.isValid())
      m_btnAdd->click();
  }
  updateControls();
}

void NoteView::updateControls()
{
  m_btnRemove->setEnabled(m_table->currentRow() >= 0);
  m_btnOpenLast->setEnabled(m_lastId.isValid());

  double total = m_table->computeTotal() + m_edDisccount->getValue();

  if (m_table->hasItems() || total != 0)
    m_edTotal->setText(JItem::st_strMoney(total));
  else
    m_edTotal->clear();

  QPalette _palette = m_edTotal->palette();
  _palette.setColor(QPalette::ColorRole::Text, total >= 0 ? Qt::red : Qt::darkGreen);
  m_edTotal->setPalette(_palette);

  m_lblEntries->setText(tr("Número de entradas: %1").arg(JItem::st_strInt(m_database->getNumberOfEntries())));
  m_lblSum->setText(tr("Total: %1").arg(JItem::st_strMoney(m_database->getSum(5))));

  emit changedSignal();
}

void NoteView::addProduct()
{
  m_table->addItemAndLoadPrices(m_supplierPicker->getId(), sender() == m_btnAddCode);
}

Note NoteView::save(Person& employee)
{
  Note note = getNote();
  bool bSuccess = m_database->save(note, &employee);
  if (bSuccess)
  {
    m_lastId = note.m_id;
    QString error;
    //TODO quickfix
    NoteSQL::select(note, error);
    create();
  }
  else
    note.clear();
  updateControls();
  return note;
}

void NoteView::lastItemSelected()
{
  if (m_lastId.isValid())
  m_database->selectItem(m_lastId);
}

void NoteView::itemSelected(const JItem& jItem)
{
  const Note& note = dynamic_cast<const Note&>(jItem);
  m_lastId = note.m_id;
  setNote(note);
}

void NoteView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_lastId))
    m_lastId.clear();
  if (ids.contains(m_currentId))
    create();
}

void NoteView::openDetailsDialog()
{
  QString str = m_dlgDetails->getDetails();
  if (!m_dlgDetails->exec())
    m_dlgDetails->setDetails(str);
}
