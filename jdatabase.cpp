#include "jdatabase.h"
#include "jlineedit.h"
#include "defines.h"
#include "notefilterdlg.h"
#include "pincodeview.h"
#include "jitemhelper.h"
#include "jmodel.h"
#include <QDate>
#include <QLayout>
#include <QSqlRecord>
#include <QPushButton>
#include <QHeaderView>
#include <QShortcut>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QPixmap>
#include <QIcon>
#include <QCheckBox>
#include <QLabel>
#include <QSortFilterProxyModel>

JTableView::JTableView(QWidget *parent)
  : QTableView(parent)
{

}

void JTableView::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter ||
      event->key() == Qt::Key_Return)
  {
    emit enterKeyPressedSignal();
  }
  QTableView::keyPressEvent(event);
}

JDatabase::JDatabase(const QString& tableName,
                     Mode mode,
                     QWidget *parent)
  : QFrame(parent)
  , m_mode(mode)
  , m_btnOpen(nullptr)
  , m_btnRefresh(nullptr)
  , m_btnRemove(nullptr)
  , m_btnFilter(nullptr)
  , m_btnFilterClear(nullptr)
  , m_edFilterSearch(nullptr)
  , m_cbContains(nullptr)
  , m_table(nullptr)
  , m_proxyModel(nullptr)
  , m_noteFilter(nullptr)
{
  m_btnOpen = new QPushButton();
  m_btnOpen->setFlat(true);
  m_btnOpen->setText("");
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnOpen->setToolTip(tr("Abrir (Ctrl+O)"));
  m_btnOpen->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));

  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setToolTip(tr("Remover (Del)"));
  m_btnRemove->setIcon(QIcon(":/icons/res/remove.png"));
  m_btnOpen->setShortcut(QKeySequence(Qt::Key_Delete));

  m_btnFilter = new QPushButton();
  m_btnFilter->setFlat(true);
  m_btnFilter->setText("");
  m_btnFilter->setIconSize(QSize(24, 24));
  m_btnFilter->setToolTip(tr("Filtro"));
  m_btnFilter->setIcon(QIcon(":/icons/res/filter.png"));

  m_btnFilterClear = new QPushButton();
  m_btnFilterClear->setFlat(true);
  m_btnFilterClear->setText("");
  m_btnFilterClear->setIconSize(QSize(24, 24));
  m_btnFilterClear->setToolTip(tr("Limpar filtro"));
  m_btnFilterClear->setIcon(QIcon(":/icons/res/filterclear.png"));

  m_btnRefresh = new QPushButton();
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setText("");
  m_btnRefresh->setIconSize(QSize(16, 16));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));
  m_btnRefresh->setToolTip(tr("Atualizar (F5)"));
  m_btnRefresh->setShortcut(QKeySequence(Qt::Key_F5));

  QHBoxLayout* hlayout0 = new QHBoxLayout();
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->setAlignment(Qt::AlignLeft);
  hlayout0->addWidget(m_btnOpen);
  hlayout0->addWidget(m_btnRemove);
  hlayout0->addWidget(m_btnFilter);
  hlayout0->addWidget(m_btnFilterClear);

  m_edFilterSearch = new JLineEdit(JLineEdit::Input::All, (int)JLineEdit::Flags::ToUpper);
  m_edFilterSearch->setToolTip(tr("Procurar (Ctrl+F)"));
  m_edFilterSearch->setClearButtonEnabled(true);

  m_cbContains = new QCheckBox;
  m_cbContains->setText(tr("Contendo"));
  m_cbContains->setIconSize(QSize(16, 16));
  m_cbContains->setIcon(QIcon(":/icons/res/center.png"));
  m_cbContains->setToolTip(tr("Procurar por palavras contendo o termo informado"));

  QHBoxLayout* hlayout1 = new QHBoxLayout();
  hlayout1->setContentsMargins(0, 0, 0, 0);
  hlayout1->addWidget(m_btnRefresh);
  hlayout1->addWidget(m_edFilterSearch);
  hlayout1->addWidget(m_cbContains);

  m_table = new JTableView();
  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(m_mode == Mode::Full  ? QAbstractItemView::ExtendedSelection
                                                  : m_mode ==Mode::MultiSelector ? QAbstractItemView::MultiSelection
                                                                                 : QAbstractItemView::SingleSelection);
  m_table->setEditTriggers(QTableView::NoEditTriggers);
  m_table->setSortingEnabled(true);
  m_table->horizontalHeader()->setHighlightSections(false);
  m_table->verticalHeader()->hide();

  QVBoxLayout* vlayout1 = new QVBoxLayout();
  vlayout1->addLayout(hlayout0);
  vlayout1->addLayout(hlayout1);
  vlayout1->addWidget(m_table);
  setLayout(vlayout1);

  JModel* model = JItemHelper::model(tableName, this);
  if (model == nullptr)

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(model);
  m_proxyModel->setSortRole(Qt::EditRole);

  m_tableName = tableName;
  m_table->setModel(m_proxyModel);
  model->select(m_table->horizontalHeader());

  connect(m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(enableControls()));
  connect(m_table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(emitCurrentRowChangedSignal()));
  connect(m_btnRefresh, SIGNAL(clicked(bool)), this, SLOT(refresh()));
  connect(m_edFilterSearch, SIGNAL(textEdited(const QString&)), this, SLOT(filterSearchChanged()));
  connect(m_edFilterSearch, SIGNAL(enterSignal()), this, SLOT(filterSearchEnter()));
  connect(m_edFilterSearch, SIGNAL(keyDownSignal()), this, SLOT(filterSearchEnter()));
  connect(m_cbContains, SIGNAL(clicked(bool)), this, SLOT(containsPressed()));
  connect(m_table->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(filterSearchChanged()));
  connect(m_btnFilter, SIGNAL(clicked(bool)), this, SLOT(showFilter()));
  connect(m_btnFilterClear, SIGNAL(clicked(bool)), this, SLOT(clearFilter()));

  if (m_mode != Mode::ReadOnly)
  {
    connect(m_table, SIGNAL(enterKeyPressedSignal()), this, SLOT(selectItems()));
    connect(m_btnOpen, SIGNAL(clicked(bool)), this, SLOT(selectItems()));
    connect(m_btnRemove, SIGNAL(clicked(bool)), this, SLOT(removeItems()));
    connect(m_table, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(selectItems()));
  }

  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(focusFilterSearch()));

  if (m_table->horizontalHeader()->count() > 1)
    m_table->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);

  if (m_tableName != NOTE_SQL_TABLE_NAME)
  {
    m_btnFilter->hide();
    m_btnFilterClear->hide();
  }

  if (m_mode == Mode::SingleSelector || m_mode == Mode::MultiSelector)
  {
    m_btnOpen->hide();
    m_btnFilter->hide();
    m_btnRemove->setEnabled(false);
    m_btnRemove->hide();
    m_edFilterSearch->setFocus();
  }
  else if (m_mode == Mode::ReadOnly)
  {
    m_btnOpen->hide();
    m_btnRemove->setEnabled(false);
    m_btnRemove->hide();
    m_edFilterSearch->setFocus();
  }

  filterSearchChanged();
}

JDatabase::~JDatabase()
{
  clearCurrentItems();
}

void JDatabase::clearCurrentItems()
{
  for (int i = 0; i != m_currentItems.size(); ++i)
  {
    JItemSQL* pt = m_currentItems.at(i);
    if (pt != nullptr)
      delete pt;
  }
  m_currentItems.clear();
}

QVector<Id> JDatabase::getSelectedIds() const
{
  JModel* model = dynamic_cast<JModel*>(m_proxyModel->sourceModel());
  QModelIndexList lst = m_table->selectionModel()->selectedRows();

  QVector<Id> ids;
  for (int i = 0; i != lst.size(); ++i)
  {
    QModelIndex idx = m_proxyModel->mapToSource(lst.at(i));
    if (idx.isValid())
      ids.push_back(Id(model->index(idx.row(), SQL_COLID_NUMBER).data(Qt::EditRole).toLongLong()));
  }
  return ids;
}

void JDatabase::selectIds(const QVector<Id>& ids)
{
  for (int i = 0; i != ids.size(); ++i)
  {
    QModelIndexList lst = m_proxyModel->match(m_proxyModel->index(0, 0), Qt::EditRole, ids.at(i).get(), 1, Qt::MatchExactly);
    if (lst.size() != 0)
      m_table->selectRow(lst.at(0).row());
  }

  if (ids.size() != 0)
    m_table->setFocus();
}

void JDatabase::selectItems()
{
  selectItems(getSelectedIds());
}

void JDatabase::selectItem(Id id)
{
  QVector<Id> ids;
  ids.push_back(id);
  selectItems(ids);
}

void JDatabase::selectItems(const QVector<Id> ids)
{
  clearCurrentItems();

  for (int i = 0; i != ids.size(); ++i)
  {
    QString error;
    bool bSuccess = false;
    auto p = JItemHelper::create(m_tableName, ids.at(i));
    if (p != nullptr)
    {
      bSuccess = p->SQL_select(error);
      if (bSuccess)
        m_currentItems.push_back(p);
    }

    if (!bSuccess)
    {
      QMessageBox::critical(this,
                            tr("Erro"),
                            tr("O seguinte erro ocorreu ao selecionar o item com ID "
                               "'%1':\n'%2'").arg(ids.at(i).str(), error));
    }
  }

  if (!m_currentItems.isEmpty())
  {
    if (m_currentItems.at(0) != nullptr)
      emit itemSelectedSignal(*m_currentItems.at(0));
    emit itemsSelectedSignal(m_currentItems);
  }
}

void JDatabase::refresh()
{
  JModel* model = dynamic_cast<JModel*>(m_proxyModel->sourceModel());
  if (m_fixedFilter.isEmpty() && m_filter.isEmpty())
    model->select();
  else
  {
    QString str;
    if (!m_fixedFilter.isEmpty())
    {
      if (!m_filter.isEmpty())
        str = m_fixedFilter + " AND " + m_filter;
      else
        str = m_fixedFilter;
    }
    else
      str = m_filter;
    model->selectFilter(str);
  }

  m_proxyModel->invalidate();
  enableControls();
  if (m_mode != Mode::Full)
    m_edFilterSearch->setFocus();

  emit refreshSignal();
}

void JDatabase::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bSelected);
  m_btnRemove->setEnabled(bSelected);
}

void JDatabase::removeItems()
{
  QVector<Id> ids = getSelectedIds();

  if (ids.size() == 0)
    return;

  if (QMessageBox::question(this,
                            tr("Remover itens"),
                            tr("Tem certeza que deseja remover os itens selecionados?"),
                            QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
  {
    return;
  }

  if (JItemHelper::authenticationToRemove(m_tableName))
  {
    PinCodeView w(this);
    if (!w.exec())
      return;

    Employee e = w.getEmployee();
    QString error;
    if (!e.m_id.isValid())
      error = tr("Pincode informado não encontrado.");
    else if (e.hasPermissionToRemove(m_tableName))
      error = tr("Funcionário não possui permissão.");

    if (!error.isEmpty())
    {
      QMessageBox::warning(this, tr("Erro"), error, QMessageBox::Ok);
      return;
    }
  }

  for (int i = 0; i != ids.size(); ++i)
  {
    Id id = ids.at(i);
    auto p = JItemHelper::create(m_tableName, id);
    if (p != nullptr)
    {
      QString error;
      p->SQL_remove(error);
      delete p;
    }
  }

  emit itemsRemovedSignal(ids);
  refresh();
}

void JDatabase::filterSearchChanged()
{
  int column = m_table->horizontalHeader()->sortIndicatorSection();
  m_proxyModel->setFilterKeyColumn(column);
  QString filter = m_edFilterSearch->text();
  if (!m_cbContains->isChecked() && !filter.isEmpty())
    filter.prepend("\\b");
  QRegExp regExp(filter, Qt::CaseInsensitive, QRegExp::RegExp);
  m_proxyModel->setFilterRegExp(regExp);

  if (filter.isEmpty())
  {
    QString columnName;
    if (column > 0)
      columnName = m_proxyModel->headerData(column, Qt::Horizontal).toString().toLower();
    columnName = tr("Procurar por: ") + columnName;
    m_edFilterSearch->setPlaceholderText(columnName);
  }

  enableControls();
}

void JDatabase::filterSearchEnter()
{
  m_table->setFocus();
  if (m_proxyModel->rowCount() != 0)
  {
    m_table->selectRow(0);
  }
  else
  {
    m_edFilterSearch->setFocus();
    m_edFilterSearch->selectAll();
  }
}

void JDatabase::clearFilterSearch()
{
  m_edFilterSearch->clear();
  filterSearchChanged();
}

void JDatabase::containsPressed()
{
  filterSearchChanged();
  m_edFilterSearch->setFocus();
}

void JDatabase::focusFilterSearch()
{
  m_edFilterSearch->selectAll();
  m_edFilterSearch->setFocus();
}

void JDatabase::setFixedFilter(const QString& fixedFilter)
{
  m_fixedFilter = fixedFilter;
  refresh();
}

QString JDatabase::getTableName() const
{
  return m_tableName;
}

JItemSQL* JDatabase::getCurrentItem() const
{
  return m_currentItems.size() != 0 ? m_currentItems.at(0) : nullptr;
}

bool JDatabase::save(const JItemSQL& o, Employee* pEmployee)
{
  if (JItemHelper::authenticationToInsertUpdate(m_tableName))
  {
    PinCodeView w(this);
    if (!w.exec())
      return false;

    Employee e = w.getEmployee();
    if (pEmployee != nullptr)
      *pEmployee = e;
    QString error;
    if (!e.m_id.isValid())
      error = tr("Pincode informado não encontrado.");
    else if (e.hasPermissionToEdit(m_tableName))
      error = tr("Funcionário não possui permissão.");

    if (!error.isEmpty())
    {
      QMessageBox::warning(this, tr("Erro"), error, QMessageBox::Ok);
      return false;
    }
  }

  QString error;
  bool bSuccess = false;
  bSuccess = o.SQL_insert_update(error);

  if (bSuccess)
    refresh();
  else
    QMessageBox::critical(this, tr("Erro"), tr("Erro '%1' ao salvar o item.").arg(error), QMessageBox::Ok);
  return bSuccess;
}

void JDatabase::emitCurrentRowChangedSignal()
{
  emit currentRowChangedSignal(m_table->currentIndex().row());
}

void JDatabase::showFilter()
{
  if (m_tableName == NOTE_SQL_TABLE_NAME)
  {
    if (m_noteFilter == nullptr)
      m_noteFilter = new NoteFilterDlg(this);
    m_noteFilter->exec();
    m_filter = m_noteFilter->getFilter();
    refresh();
  }
}

void JDatabase::clearFilter()
{
  if (m_tableName == NOTE_SQL_TABLE_NAME && m_noteFilter != nullptr)
  {
    m_noteFilter->clearFilter();
    m_filter = "";
    refresh();
  }
}

int JDatabase::getNumberOfEntries() const
{
  return m_proxyModel->rowCount();
}

double JDatabase::getSum(int column) const
{
  double sum = 0.0;
  for (int row = 0; row != m_proxyModel->rowCount(); ++row)
    sum += m_proxyModel->data(m_proxyModel->index(row, column), Qt::EditRole).toDouble();
  return sum;
}

JDatabaseSelector::JDatabaseSelector(const QString& tableName,
                                     bool bMultiSelector,
                                     QWidget* parent)
  : QDialog(parent)
  , m_database(nullptr)
{
  m_database = new JDatabase(tableName, bMultiSelector ? JDatabase::Mode::MultiSelector : JDatabase::Mode::SingleSelector);
  m_database->layout()->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout* vlayout0 = new QVBoxLayout;
  if (bMultiSelector)
    vlayout0->addWidget(new QLabel(tr("Selecione os itens desejados e pressione Enter para confirmar.")));
  vlayout0->addWidget(m_database);
  setLayout(vlayout0);

  resize(500, 400);

  QString title = "Selecionar " + JItemHelper::text(tableName);
  QString icon = JItemHelper::icon(tableName);

  setWindowTitle(title);
  if (!icon.isEmpty())
    setWindowIcon(QIcon(icon));

  QObject::connect(m_database, SIGNAL(itemsSelectedSignal(const QVector<JItemSQL*>&)), this, SLOT(itemsSelected(const QVector<JItemSQL*>&)));
}

void JDatabaseSelector::itemsSelected(const QVector<JItemSQL*>& /*v*/)
{
  accept();
}

JDatabase* JDatabaseSelector::getDatabase() const
{
  return m_database;
}

void JDatabaseSelector::closeEvent(QCloseEvent * e)
{
  m_database->clearFilterSearch();
  m_database->refresh();
  QDialog::closeEvent(e);
}
