#include "phonetable.h"
#include <QHeaderView>

PhoneTable::PhoneTable(JAddRemoveButtons* btns, QWidget* parent)
  : JTable(btns, parent)
{
  setColumnCount(4);
  QStringList headers;
  headers << "País" << "Código" << "Número" << "Nome";
  setHorizontalHeaderLabels(headers);

  horizontalHeader()->setSectionResizeMode((int)Column::CountryCode, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Code, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Number, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode((int)Column::Name, QHeaderView::Stretch);
}

void PhoneTable::addRow()
{
  insertRow(rowCount());
  int row = rowCount() - 1;

  auto itCountryCode = new DoubleItem(Data::Type::Integer, DoubleItem::Color::None, false, false, "+");
  auto itCode = new DoubleItem(Data::Type::Integer, DoubleItem::Color::None, false, false, "(", ")");
  auto itNumber = new TextItem(Text::Input::Numeric, false);
  auto itName = new TextItem(Text::Input::ASCII, true);

  setItem(row, (int)Column::CountryCode, itCountryCode);
  setItem(row, (int)Column::Code, itCode);
  setItem(row, (int)Column::Number, itNumber);
  setItem(row, (int)Column::Name, itName);

  Phone o;
  itCountryCode->setValue(o.m_countryCode);
  itCode->setValue(o.m_code);

  setCurrentItem(itNumber);
  setFocus();
}

void PhoneTable::getPhones(QVector<Phone>& v) const
{
  v.clear();
  for (int i = 0; i != rowCount(); ++i)
  {
    Phone o;
    o.m_countryCode = getItem(i, (int)Column::CountryCode)->getValue().toDouble();
    o.m_code = getItem(i, (int)Column::Code)->getValue().toDouble();
    o.m_number = getItem(i, (int)Column::Number)->getValue().toString();
    o.m_name = getItem(i, (int)Column::Name)->getValue().toString();
    v.push_back(o);
  }
}

void PhoneTable::setPhones(const QVector<Phone>& v)
{
  removeAllItems();
  for (int i = 0; i != v.size(); ++i)
  {
    addRow();
    getItem(i, (int)Column::CountryCode)->setValue(v.at(i).m_countryCode);
    getItem(i, (int)Column::Code)->setValue(v.at(i).m_code);
    getItem(i, (int)Column::Number)->setValue(v.at(i).m_number);
    getItem(i, (int)Column::Name)->setValue(v.at(i).m_name);
  }
}
