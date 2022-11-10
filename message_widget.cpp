#include "message_widget.hpp"

MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {
  // make a label
  auto message_label = new QLabel("Message:");
  // make a line edit
  message_txtfield = new QLineEdit;
  message_txtfield->setReadOnly(true);
  // layout the widgets
  auto message_layout = new QHBoxLayout(this);
  message_layout->addWidget(message_label);
  message_layout->addWidget(message_txtfield);
  setLayout(message_layout);
}

void MessageWidget::info(QString message) {
  clear();
  message_txtfield->setText(message);
}

void MessageWidget::error(QString message) {
  QPalette palette;
  palette.setColor(QPalette::Base,QColor(Qt::red));
  palette.setColor(QPalette::Text,QColor(Qt::white));
  palette.setColor(QPalette::Highlight,QColor(Qt::red));
  palette.setColor(QPalette::HighlightedText,QColor(Qt::white));
  message_txtfield->setPalette(palette);
  message_txtfield->setText(message);
  message_txtfield->selectAll();
}

void MessageWidget::clear() {
  QPalette palette;
  palette.setColor(QPalette::Base,QColor(Qt::white));
  palette.setColor(QPalette::Text,QColor(Qt::black));
  palette.setColor(QPalette::Highlight,QColor(Qt::white));
  palette.setColor(QPalette::HighlightedText,QColor(Qt::black));
  message_txtfield->setPalette(palette);
  message_txtfield->setText(QString());
}
