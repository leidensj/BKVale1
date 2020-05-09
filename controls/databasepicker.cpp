#include "databasepicker.h"
#include "widgets/jlineedit.h"
#include "widgets/jclicklabel.h"
#include "defines.h"
#include "databaseviewer.h"
#include "items/jitemex.h"
#include "imageviewer.h"
#include "databaseselector.h"
#include <QDialog>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>
#include <QAction>

DatabasePicker::DatabasePicker(const QString& tableName,
                               bool bMultiPicker,
                               QWidget* parent)
 : QFrame(parent)
 , m_bMultiPicker(bMultiPicker)
 , m_selector(nullptr)
 , m_edText(nullptr)
 , m_lblImage(nullptr)
{
  m_edText = new JLineEdit(Text::Input::All, JLineEdit::st_defaultFlags1);
  m_edText->setReadOnly(true);
  QIcon searchIcon = QIcon(JItemEx::icon(tableName));
  QAction* action = m_edText->addAction(searchIcon.isNull() ? QIcon(":/icons/res/binoculars.png") : searchIcon, QLineEdit::LeadingPosition);
  QAction* action2 = m_edText->addAction(QIcon(":/icons/res/remove.png"), QLineEdit::TrailingPosition);
  action->setToolTip(tr("Selecionar ") + JItemEx::text(tableName));
  action2->setToolTip(tr("Remover ") + JItemEx::text(tableName));

  m_selector = new DatabaseSelector(tableName, bMultiPicker, this);

  m_lblImage = new JClickLabel;
  m_lblImage->setMinimumSize(24, 24);
  m_lblImage->setMaximumSize(24, 24);
  m_lblImage->setScaledContents(true);
  m_lblImage->setFrameShape(QFrame::Shape::StyledPanel);
  m_lblImage->setFrameShadow(QFrame::Shadow::Plain);
  m_lblImage->setToolTip(tr("Clique para ampliar"));

  QHBoxLayout* hlayout0 = new QHBoxLayout;
  hlayout0->setContentsMargins(0, 0, 0, 0);
  hlayout0->addWidget(m_edText);
  hlayout0->addWidget(m_lblImage);

  setLayout(hlayout0);

  connect(action, SIGNAL(triggered(bool)), this, SLOT(searchItem()));
  connect(action2, SIGNAL(triggered(bool)), this, SLOT(clear()));
  connect(m_edText, SIGNAL(deleteSignal()), this, SLOT(clear()));
  connect(m_edText, SIGNAL(enterSignal()), this, SLOT(searchItem()));
  connect(m_selector->getViewer(), SIGNAL(itemsSelectedSignal(const QVector<JItemSQL*>&)), this, SLOT(setItems(const QVector<JItemSQL*>&)));
  connect(m_lblImage, SIGNAL(clicked()), this, SLOT(showImage()));
  clear();
}

DatabaseViewer* DatabasePicker::getViewer() const
{
  return m_selector->getViewer();
}

void DatabasePicker::setItems(const QVector<JItemSQL*>& v)
{
  m_selector->close();
  clear();
  bool bChanged = false;
  for (int i = 0; i != v.size(); ++i)
  {
    if (v.at(i) == nullptr)
      continue;
    if (setItem(*v.at(i)))
      bChanged = true;
  }
  if (bChanged)
    emit changedSignal();
}

bool DatabasePicker::setItem(const JItemSQL& o)
{
  return setItem(o.m_id, o.name(), o.image());
}

bool DatabasePicker::setItem(Id id, const QString& name, const QByteArray& arImage)
{
  if (m_bMultiPicker)
  {
    bool bFound = false;
    if (id.isValid())
    {
      for (int i = 0; i != m_ids.size(); ++i)
      {
        if (m_ids.at(i) == id)
        {
          bFound = true;
          break;
        }
      }

      if (!bFound)
      {
        m_ids.push_back(id);
        m_names.push_back(name);
        QString str, strTooltip;
        for (int i = 0; i != m_names.size(); ++i)
        {
          str += m_names.at(i);
          strTooltip += m_names.at(i);
          if (i != (m_names.size() - 1))
          {
            str += "; ";
            strTooltip += "\n";
          }
        }
        m_edText->setText(str);
        m_edText->setToolTip(strTooltip);
      }
    }
    return !bFound;
  }
  else
  {
    Id previousId;
    previousId.set(m_ids.isEmpty() ? INVALID_ID : m_ids.at(0).get());
    m_ids.clear();
    m_names.clear();
    m_ids.push_back(id);
    m_edText->setText(name);
    if (!arImage.isEmpty() && ! arImage.isNull())
    {
      m_lblImage->show();
      QPixmap pixmap(QSize(24, 24));
      pixmap.loadFromData(arImage);
      m_lblImage->setPixmap(pixmap);
    }
    else
      m_lblImage->hide();
    return previousId != id;
  }
}

void DatabasePicker::searchItem()
{
  m_selector->getViewer()->refresh();
  if (m_bMultiPicker && !m_ids.isEmpty())
    m_selector->getViewer()->selectIds(m_ids);
  m_selector->exec();
}

void DatabasePicker::clear()
{
  bool bChanged = !m_edText->text().isEmpty();
  m_edText->clear();
  m_edText->setToolTip("");
  m_ids.clear();
  m_names.clear();
  m_lblImage->setPixmap(QPixmap::fromImage(QImage()));
  m_lblImage->hide();
  if (bChanged)
    emit changedSignal();
}

Id DatabasePicker::getId() const
{
  return Id(m_bMultiPicker ? INVALID_ID : m_ids.isEmpty() ? INVALID_ID : m_ids.at(0));
}

const QVector<Id>& DatabasePicker::getIds() const
{
  return m_ids;
}

const QVector<QString>& DatabasePicker::getNames() const
{
  return m_names;
}

QString DatabasePicker::getTableText() const
{
  return JItemEx::text(m_selector->getViewer()->getTableName());
}

void DatabasePicker::setPlaceholderText(bool bSet)
{
  m_edText->setPlaceholderText(bSet ? getTableText() : "");
}

void DatabasePicker::showImage()
{
  QDialog dlg(this);
  ImageViewer* viewer = new ImageViewer(true);
  viewer->setImage(m_lblImage->pixmap()->toImage());
  QHBoxLayout *layout = new QHBoxLayout;
  dlg.setLayout(layout);
  layout->addWidget(viewer);
  dlg.setWindowFlags(Qt::Window);
  dlg.setWindowTitle(tr("Imagem"));
  dlg.setWindowIcon(QIcon(":/icons/res/icon.png"));
  dlg.setModal(true);
  dlg.exec();
}
