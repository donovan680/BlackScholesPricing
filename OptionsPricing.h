#pragma once

#include <QtWidgets/QWidget>

#include "Debug/uic/ui_OptionsPricing.h"

class OptionsPricing : public QWidget {
  Q_OBJECT

 public:
  OptionsPricing(QWidget* parent = Q_NULLPTR);
 private slots:
  void calculate();  // calculate call & put
  void clear();      // clear forms

 private:
  Ui::OptionsPricingClass ui;
};
