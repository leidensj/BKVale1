#ifndef ADDRESSTABLEWIDGET_H
#define ADDRESSTABLEWIDGET_H

#include "widgets/jtable.h"
#include "items/address.h"

class AddressTableWidget : public JItemTable
{
  Q_OBJECT

  enum class Column
  {
    Address
  };

public:
  explicit AddressTableWidget(QWidget* parent = nullptr);
  const JItem& getItem(int row) const;

public slots:
  void addItem(const JItem& o);
  void addItem();

private:
  mutable Address m_ref;

protected slots:
  void update(int row, int column);
  void itemActivate(int row, int column);
  void itemDelete(int row, int column);
};

#endif // ADDRESSTABLEWIDGET_H
