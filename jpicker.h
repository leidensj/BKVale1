#ifndef JPICKER_H
#define JPICKER_H

#include <QFrame>
#include <QString>

class QPushButton;
class JLineEdit;
class JImageView;

class JPicker : public QFrame
{
  Q_OBJECT

public:
  explicit JPicker(int invalidId,
                   const QString& itemText,
                   bool bShowImage,
                   QWidget* parent = 0);
  const QString m_itemText;
  void setId(int id);
  void setText(const QString& text);
  void setImage(const QByteArray& ar);
  int getId() const;
  QString getText() const;
  QByteArray getImage() const;

signals:
  searchSignal();

public slots:
  void clear();

private slots:
  void emitSearchSignal();

private:
  const int m_invalidId;
  int m_id;
  QPushButton* m_btnSearch;
  JLineEdit* m_edText;
  QPushButton* m_btnClear;
  JImageView* m_imageView;
};

#endif // JDATABASEPICKER_H