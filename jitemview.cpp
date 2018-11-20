#include "jitemview.h"
#include "jdatabase.h"
#include <QPushButton>
#include <QLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QSplitter>

JItemView::JItemView(const QString& tableName, QWidget* parent)
  : QFrame(parent)
  , m_tab(nullptr)
  , m_btnCreate(nullptr)
  , m_btnSave(nullptr)
{
  m_btnCreate = new QPushButton;
  m_btnCreate->setFlat(true);
  m_btnCreate->setText("");
  m_btnCreate->setIconSize(QSize(24, 24));
  m_btnCreate->setIcon(QIcon(":/icons/res/file.png"));
  m_btnCreate->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));

  m_btnSave = new QPushButton;
  m_btnSave->setFlat(true);
  m_btnSave->setText("");
  m_btnSave->setIconSize(QSize(24, 24));
  m_btnSave->setIcon(QIcon(":/icons/res/save.png"));
  m_btnSave->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

  QHBoxLayout* buttonlayout = new QHBoxLayout;
  buttonlayout->setContentsMargins(0, 0, 0, 0);
  buttonlayout->addWidget(m_btnCreate);
  buttonlayout->addWidget(m_btnSave);
  buttonlayout->setAlignment(Qt::AlignLeft);

  m_tab = new QTabWidget;

  QVBoxLayout* viewlayout = new QVBoxLayout;
  viewlayout->setContentsMargins(9, 0, 0, 0);
  viewlayout->setAlignment(Qt::AlignTop);
  viewlayout->addLayout(buttonlayout);
  viewlayout->addWidget(m_tab);

  QFrame* viewFrame = new QFrame;
  viewFrame->setLayout(viewlayout);

  m_database = new JDatabase(tableName);
  m_database->layout()->setContentsMargins(0, 0, 9, 0);

  QSplitter* splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(m_database);
  splitter->addWidget(viewFrame);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(splitter);
  setLayout(mainLayout);

  QObject::connect(m_btnCreate,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(create()));
  QObject::connect(m_btnSave,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(save()));
  QObject::connect(m_database,
                   SIGNAL(itemSelectedSignal(const JItem&)),
                   this,
                   SLOT(selectItem(const JItem&)));
  QObject::connect(m_database,
                   SIGNAL(itemsRemovedSignal(const QVector<Id>&)),
                   this,
                   SLOT(itemsRemoved(const QVector<Id>&)));

  setMinimumWidth(600);
}

void JItemView::selectItem(const JItem& o)
{
  setItem(o);
  QString strIcon = o.m_id.isValid()
                    ? ":/icons/res/saveas.png"
                    : ":/icons/res/save.png";
  m_btnSave->setIcon(QIcon(strIcon));
  m_currentId = o.m_id;
}

void JItemView::itemsRemoved(const QVector<Id>& ids)
{
  if (ids.contains(m_currentId))
    create();
}

void JItemView::save()
{
  if (m_database->save(getItem()))
    create();
}
