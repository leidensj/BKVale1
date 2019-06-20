#include "jlineedit.h"
#include "tinyexpr.h"

namespace
{
QString getRegEx(JLineEdit::Input input)
{
  switch (input)
  {
    case JLineEdit::Input::Alphanumeric:
      return "^[a-zA-Z0-9]*$";
    case JLineEdit::Input::AlphanumericAndSpaces:
      return "^[a-zA-Z0-9 ]*$";
    case JLineEdit::Input::Numeric:
      return "^[0-9]*$";
    case JLineEdit::Input::BasicMath:
      return "^[0-9/\\-\\*\\+\\.][^,]*$";
    case JLineEdit::Input::ASCII:
      return "[ -~]*";
    case JLineEdit::Input::All:
    default:
      return "";
  }
}
}

JRegExpValidator::JRegExpValidator(bool toUpper, const QRegExp & rx,QObject* parent)
  : QRegExpValidator(rx, parent)
  , m_toUpper(toUpper)
{

}

QValidator::State JRegExpValidator::validate(QString& input, int& pos) const
{
  if (m_toUpper)
    input = input.toUpper();
  return QRegExpValidator::validate(input, pos);
}

JLineEdit::JLineEdit(Input input, int flags, QWidget* parent)
  : QLineEdit(parent)
  , m_flags(flags)
{
  if (input != Input::All)
    setValidator(new JRegExpValidator(flags & (int)Flags::ToUpper,
                                      QRegExp(getRegEx(input)), this));
}

void JLineEdit::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
      if (m_flags & (int)Flags::EnterAsTab)
        focusNextChild();
      emit enterSignal();
    } break;
    case Qt::Key_Down:
    {
      if (m_flags & (int)Flags::ArrowsAsTab)
        focusNextChild();
      emit keyDownSignal();
    } break;
    case Qt::Key_Up:
    {
      if (m_flags && (int)Flags::ArrowsAsTab)
        focusPreviousChild();
      emit keyUpSignal();
    } break;
    case Qt::Key_Delete:
    {
      emit deleteSignal();
      QLineEdit::keyPressEvent(event);
    } break;
    default:
      QLineEdit::keyPressEvent(event);
  }
}

void JLineEdit::setTextBlockingSignals(const QString& str)
{
  blockSignals(true);
  setText(str);
  blockSignals(false);
}

JExpLineEdit::JExpLineEdit(JItem::DataType type,
                           int flags,
                           double defaultValue,
                           QWidget* parent)
  : JLineEdit(Input::All, flags, parent)
  , m_dataType(type)
  , m_defaultValue(defaultValue)
  , m_currentValue(defaultValue)
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(evaluate()));
  evaluate();
}

double JExpLineEdit::getValue() const
{
  return m_currentValue;
}

void JExpLineEdit::evaluate()
{
  auto stdExp = QLineEdit::text().toStdString();
  int error = 0;
  double val = te_interp(stdExp.c_str(), &error);
  if (!error)
  {
    m_currentValue = val;
    setTextBlockingSignals(JItem::st_str(val, m_dataType));
    emit valueChanged(m_currentValue);
  }

  if (m_currentValue == m_defaultValue)
    setTextBlockingSignals("");

  QPalette _palette = palette();
  _palette.setColor(QPalette::ColorRole::Text, m_currentValue >= 0 ? Qt::red : Qt::darkGreen);
  setPalette(_palette);
}

void JExpLineEdit::setText(const QString& text)
{
  QLineEdit::setText(text);
  evaluate();
}

void JExpLineEdit::setText(double val)
{
  setText(QString::number(val));
}

QString JExpLineEdit::text() const
{
  return JItem::st_str(getValue(), m_dataType);
}