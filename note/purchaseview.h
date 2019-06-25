#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QFrame>
#include <QStringList>
#include "items/note.h"
#include "items/employee.h"
#include "widgets/jitemview.h"

#define MAX_ITEMS 100

class JDateEdit;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;
class NoteTableWidget;
class JDatabasePicker;
class QDockWidget;
class JDatabase;
class QIODevice;
class JExpLineEdit;
class JPlainTextEdit;
class JDatePicker;
class QLabel;
class JLineEdit;
class QTableWidget;
class JAddRemoveButtons;
class JTable;
class QTableWidgetItem;
class QRadioButton;

class PaymentWidget : public QWidget
{
  Q_OBJECT

  QRadioButton* m_rdoCash;
  QRadioButton* m_rdoBonus;
  QRadioButton* m_rdoCredit;
  JTable* m_tbCredit;
  QLabel* m_lblNoteTotal;
  QLabel* m_lblPaymentTotal;
  JAddRemoveButtons* m_btnAddRemove;

  double m_noteTotal;
  QDate m_noteDate;

  enum class Column
  {
    Date,
    Value
  };

public:
  explicit PaymentWidget(QWidget* parent = nullptr);
  Note::PaymentMethod getPaymentMethod() const;
  QVector<PaymentItem> getPaymentItems() const;
  void setPaymentMethod(Note::PaymentMethod o);
  void setPaymentItems(const QVector<PaymentItem>& v);
  QIcon getIcon() const;
  QString getText() const;

public slots:
  void setNoteDate(const QDate& dt);
  void setNoteTotal(double value);

private slots:
  void fillCredit();
  void updateControls();
  void addRow();
  void removeRow();
  void updateTable(QTableWidgetItem* p);
  void emitMethodChangedSignal();

private:
  double computeTotal() const;
  bool isDatesValid() const;

signals:
  void isValidSignal(bool b);
  void methodChangedSignal();
};

class PurchaseView : public JItemView
{
  Q_OBJECT

public:
  explicit PurchaseView(QWidget *parent = 0);
  ~PurchaseView();
  void addNoteItem(const NoteItem& noteItem);
  bool save(Id& id);
  void setDate(const QDate& dt);
  QDate getDate() const;

public slots:
  void selectItem(const JItemSQL& o);
  void getItem(JItemSQL& o) const;

protected slots:
  void itemsRemoved(const QVector<Id>& ids);
  void setItem(const JItemSQL& o);

private:
  Id m_lastId;
  QPushButton* m_btnOpenLast;
  QPushButton* m_btnAddCode;
  JAddRemoveButtons* m_btnAddRemove;
  QSpinBox* m_snNumber;
  JDatePicker* m_dtPicker;
  JExpLineEdit* m_edTotal;
  JDatabasePicker* m_supplierPicker;
  NoteTableWidget* m_table;
  JExpLineEdit* m_edDisccount;
  PaymentWidget* m_wPayment;
  JLineEdit* m_edEntries;
  JLineEdit* m_edSum;
  JPlainTextEdit* m_teObservation;

private slots:
  void removeProduct();
  void supplierChanged();
  void lastItemSelected();
  void addProduct();
  void updateControls();
  void updateStatistics();

signals:
  void changedSignal();
};

#endif // NOTEVIEW_H
