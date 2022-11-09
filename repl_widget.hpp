#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QVector>

class REPLWidget: public QWidget{
Q_OBJECT

public:
  REPLWidget(QWidget * parent = nullptr);

signals:
  void lineEntered(QString entry);

private slots:
  void changed();
  void keyPressEvent(QKeyEvent *event);

private:
  QLineEdit *repl_txtfield;
  struct ReplMemory{
    std::size_t index;
    QVector<QString> *memory;
    ReplMemory() {
      index = 0;
      memory = new QVector<QString>(0);
    };
    void new_entry(QString entry);
    QString prev_entry();
    QString next_entry();
  };
  ReplMemory *repl_memory;
};

#endif
