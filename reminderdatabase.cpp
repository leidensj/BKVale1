#include "reminderdatabase.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QHeaderView>

namespace
{
QString friendlyColumnName(ReminderColumn idx)
{
  switch (idx)
  {
    case ReminderColumn::ID:
      return QObject::tr("ID"); break;
    case ReminderColumn::Title:
      return QObject::tr("Descrição"); break;
    case ReminderColumn::Message:
      return QObject::tr("Unidade"); break;
    case ReminderColumn::Favorite:
      return QObject::tr("Fornecedor"); break;
    case ReminderColumn::Capitalization:
      return QObject::tr("Preço"); break;
    case ReminderColumn::Size:
      return QObject::tr("Detalhes"); break;
    default:
      return "";
  }
}
}

ReminderTableModel::ReminderTableModel(QObject *parent, QSqlDatabase db)
  : QSqlTableModel(parent, db)
{

}

QVariant ReminderTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QModelIndex();

  QVariant value = QSqlTableModel::data(index, role);
  if (role == Qt::DisplayRole)
  {
    if (index.column() == (int)ReminderColumn::Favorite)
      value = QVariant::fromValue(QIcon(":/icons/res/favorite.png"));
  }

  return value;
}

ReminderDatabase::ReminderDatabase(QWidget *parent)
  : QFrame(parent)
  , m_table(nullptr)
{
  QVBoxLayout* vlayout = new QVBoxLayout();
  vlayout->setContentsMargins(0, 0, 0, 0);
  m_btnOpen = new QPushButton();
  m_btnOpen->setFlat(true);
  m_btnOpen->setText("");
  m_btnOpen->setIconSize(QSize(24, 24));
  m_btnOpen->setIcon(QIcon(":/icons/res/open.png"));
  m_btnRefresh = new QPushButton();
  m_btnRefresh->setFlat(true);
  m_btnRefresh->setText("");
  m_btnRefresh->setIconSize(QSize(24, 24));
  m_btnRefresh->setIcon(QIcon(":/icons/res/refresh.png"));
  m_btnRemove = new QPushButton();
  m_btnRemove->setFlat(true);
  m_btnRemove->setText("");
  m_btnRemove->setIconSize(QSize(24, 24));
  m_btnRemove->setIcon(QIcon(":/icons/res/trash.png"));
  m_btnFavorite = new QPushButton();
  m_btnFavorite->setFlat(true);
  m_btnFavorite->setText("");
  m_btnFavorite->setIconSize(QSize(24, 24));
  m_btnFavorite->setIcon(QIcon(":/icons/res/favorite.png"));
  m_btnFavorite->setCheckable(true);
  m_btnFavorite->setChecked(false);
  buttons = new QHBoxLayout();
  buttons->setContentsMargins(0, 0, 0, 0);
  buttons->addWidget(m_btnOpen);
  buttons->addWidget(m_btnRefresh);
  buttons->addWidget(m_btnRemove);
  buttons->addWidget(m_btnFavorite);
  buttons->setAlignment(Qt::AlignLeft);

  m_table = new QTableView(this);
  m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);

  vlayout->addLayout(buttons);
  vlayout->addWidget(m_table);
  setLayout(vlayout);
}

void ReminderDatabase::setDatabase(QSqlDatabase db)
{
  if (m_table->model() != nullptr)
    return;

  QSqlTableModel* model = new QSqlTableModel(this, db);
  model->setTable("_REMINDERS");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

  model->setHeaderData((int)ReminderColumn::ID,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderColumn::ID));
  model->setHeaderData((int)ReminderColumn::Title,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderColumn::Title));
  model->setHeaderData((int)ReminderColumn::Message,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderColumn::Message));
  model->setHeaderData((int)ReminderColumn::Capitalization,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderColumn::Capitalization));
  model->setHeaderData((int)ReminderColumn::Size,
                       Qt::Horizontal,
                       friendlyColumnName(ReminderColumn::Size));

  m_table->setModel(model);
  m_table->hideColumn((int)ReminderColumn::ID);
  m_table->hideColumn((int)ReminderColumn::Message);
  m_table->hideColumn((int)ReminderColumn::Capitalization);
  m_table->hideColumn((int)ReminderColumn::Size);
  m_table->horizontalHeader()->setSortIndicator((int)ReminderColumn::Favorite,
                                                  Qt::SortOrder::AscendingOrder);
  m_table->horizontalHeader()->setSectionResizeMode((int)ReminderColumn::Title,
                                                      QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode((int)ReminderColumn::Favorite,
                                                      QHeaderView::ResizeToContents);

  /*QObject::connect(m_table->selectionModel(),
                   SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                   this,
                   SLOT(enableControls()));

  QObject::connect(model,
                   SIGNAL(dataChanged(const QModelIndex&,
                                      const QModelIndex&,
                                      const QVector<int>&)),
                   this,
                   SLOT(enableControls()));
  refresh();
  focusFilter();*/
}

void ReminderDatabase::enableControls()
{
  bool bSelected = m_table->currentIndex().isValid();
  m_btnOpen->setEnabled(bSelected);
  m_btnRemove->setEnabled(bSelected);
  m_btnFavorite->setEnabled(bSelected);

}
