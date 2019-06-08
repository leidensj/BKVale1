#ifndef SHOPPINGLISTVIEW_H
#define SHOPPINGLISTVIEW_H

#include <QFrame>
#include <QVector>
#include "items/shoppinglist.h"
#include "widgets/jitemview.h"

class JDatabasePicker;
class QCheckBox;
class JLineEdit;
class ShoppingListTable;
class QPlainTextEdit;
class JSpinBox;
class JAddRemoveButtons;

class ShoppingListView : public JItemView
{
  Q_OBJECT

public:
  explicit ShoppingListView(QWidget* parent = nullptr);
  const JItemSQL& getItem() const;
  Id getId() const;

public slots:
  void create();

private slots:
  void updateControls();

private:
  mutable ShoppingList m_ref;

  JLineEdit* m_edTitle;
  JSpinBox* m_snLines;
  QPlainTextEdit* m_teDescription;
  JDatabasePicker* m_supplierPicker;
  JDatabasePicker* m_imagePicker;

  ShoppingListTable* m_table;
  JAddRemoveButtons* m_btns;

  QPushButton* m_vbtnMonth[31];
  QPushButton* m_vbtnWeek[7];
  void setItem(const JItemSQL& o);
};

#endif // SHOPPINGLISTVIEW_H
