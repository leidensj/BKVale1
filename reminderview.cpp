#include "reminderview.h"
#include "ui_reminderview.h"

ReminderView::ReminderView(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::ReminderView)
{
  ui->setupUi(this);
  ui->buttonFontBig->setChecked(true);
  ui->buttonUppercase->setCheckState(Qt::CheckState::Checked);
  setCapitalization((int)Qt::CheckState::Checked);

  QObject::connect(ui->editTitle,
                   SIGNAL(textEdited(const QString&)),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(ui->editMessage,
                   SIGNAL(textChanged()),
                   this,
                   SLOT(emitChangedSignal()));

  QObject::connect(ui->buttonUppercase,
                   SIGNAL(stateChanged(int)),
                   this,
                   SLOT(setCapitalization(int)));

}

ReminderView::~ReminderView()
{
  delete ui;
}

Reminder ReminderView::reminder() const
{
  Reminder r;
  r.m_title = ui->editTitle->text();
  r.m_message = ui->editMessage->toPlainText();
  r.m_bFontSmall = ui->buttonFontSmall->isChecked();
  switch(ui->buttonUppercase->checkState())
  {
    case Qt::CheckState::Unchecked:
      r.m_capitalization = ReminderCapitalization::Normal; break;
    case Qt::CheckState::PartiallyChecked:
      r.m_capitalization = ReminderCapitalization::AllLowercase; break;
    case Qt::CheckState::Checked:
    default:
      r.m_capitalization = ReminderCapitalization::AllUppercase; break;
  }
  return r;
}

void ReminderView::clear()
{
  ui->buttonFontSmall->setChecked(true);
  ui->editTitle->clear();
  ui->editMessage->clear();
}

bool ReminderView::isValid() const
{
  return !ui->editTitle->text().isEmpty() ||
      !ui->editMessage->toPlainText().isEmpty();
}

void ReminderView::emitChangedSignal()
{
  emit changedSignal();
}

void ReminderView::setCapitalization(int state)
{
  QFont::Capitalization cap = QFont::AllUppercase;
  switch ((Qt::CheckState)state)
  {
    case Qt::CheckState::Unchecked:
      cap = QFont::MixedCase; break;
    case Qt::CheckState::PartiallyChecked:
      cap = QFont::AllLowercase; break;
    default:
      cap = QFont::AllUppercase;
  }

  {
    QFont f = ui->editTitle->font();
    f.setCapitalization(cap);
    ui->editTitle->setFont(f);
  }

  {
    QFont f = ui->editMessage->font();
    f.setCapitalization(cap);
    ui->editMessage->setFont(f);
  }
}

void ReminderView::setReminder(const Reminder r)
{
  ui->editTitle->setText(r.m_title);
  ui->editMessage->setPlainText(r.m_message);
  ui->buttonFontBig->setChecked(!r.m_bFontSmall);
  ui->buttonFontSmall->setChecked(r.m_bFontSmall);
  switch(r.m_capitalization)
  {
    case ReminderCapitalization::AllLowercase:
      ui->buttonUppercase->setCheckState(Qt::PartiallyChecked);
      break;
    case ReminderCapitalization::AllUppercase:
      ui->buttonUppercase->setCheckState(Qt::Checked);
      break;
    case ReminderCapitalization::Normal:
    default:
      ui->buttonUppercase->setCheckState(Qt::Unchecked);
      break;
  }
  //TODO
}