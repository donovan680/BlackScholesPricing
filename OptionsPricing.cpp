#include "stdafx.h"

#include "OptionsPricing.h"
#include <QDebug>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <cmath>

double phi(double x); // cumulative normal distribution function kept here for
                      // internal linkage

OptionsPricing::OptionsPricing(QWidget *parent)
    : QWidget(parent), underlyingAssetPriceS(0), strikePriceK(0), riskFreeR(0),
      timeT(0), volatilyo(0) {
  ui.setupUi(this);

  // signals slots

  connect(ui.pushButtonCalculate, SIGNAL(clicked()), this, SLOT(calculate()));
  connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
}

void OptionsPricing::calculate() {
  // we can setup a signal to detect if the spinbox values isn't dirty and ask
  // the user to enter values assuming that the user enters values in the spin
  // boxes
  qDebug() << "calculate called";
  underlyingAssetPriceS = ui.doubleSpinBoxS->value();
  strikePriceK = ui.doubleSpinBoxK->value();
  timeT = ui.doubleSpinBoxT->value();
  volatilyo = ui.doubleSpinBoxO->value();
  riskFreeR = ui.doubleSpinBoxR->value();

  try {
    riskTime = getRiskTime(volatilyo, timeT);

  } catch (std::overflow_error e) {
    qDebug() << "calculating d1 resulted in a divide by zero attempt -> "
             << e.what();
  }
  d1 = calculateD1();
  d2 = calculateD2();
  // Calculate call
  double call;
  QFuture<void> FutureCall = QtConcurrent::run([=, &call]() {
    call = (underlyingAssetPriceS * phi(d1)) -
           (strikePriceK * exp(-riskFreeR * timeT) * phi(d2));
  });

  // Calculate put
  double put;
  QFuture<void> FuturePut = QtConcurrent::run([=, &put]() {
    put = strikePriceK * exp(-riskFreeR * timeT) * phi(-d2) -
          (underlyingAssetPriceS * phi(-d1));
  });

  FutureCall.waitForFinished();
  FuturePut.waitForFinished();

  // Update widgets
  ui.lcdNumberPut->setDigitCount(6);
  ui.lcdNumberCall->setDigitCount(6);
  ui.lcdNumberPut->display(put);
  ui.lcdNumberCall->display(call);
}

void OptionsPricing::clear() {
  ui.doubleSpinBoxS->clear();
  ui.doubleSpinBoxK->clear();
  ui.doubleSpinBoxT->clear();
  ui.doubleSpinBoxO->clear();
  ui.doubleSpinBoxR->clear();
  ui.lcdNumberPut->display(0.0);
  ui.lcdNumberCall->display(0.0);
}

double OptionsPricing::getRiskTime(double o, double t) const {
  double riskTime = o * sqrt(t);
  if (riskTime == 0.0)
    throw std::overflow_error("riskTime equals zero");
  return riskTime;
}

double OptionsPricing::calculateD1() const {
  return (log(underlyingAssetPriceS / strikePriceK) +
          ((riskFreeR + (pow(volatilyo, 2) / 2)) * timeT)) /
         riskTime;
}

double OptionsPricing::calculateD2() const { return (d1 - riskTime); }

// courtesy of John D. Cook: https://www.johndcook.com/blog/cpp_phi/
double phi(double x) {
  // constants
  double a1 = 0.254829592;
  double a2 = -0.284496736;
  double a3 = 1.421413741;
  double a4 = -1.453152027;
  double a5 = 1.061405429;
  double p = 0.3275911;

  // Save the sign of x
  int sign = 1;
  if (x < 0)
    sign = -1;
  x = fabs(x) / sqrt(2.0);

  // A&S formula 7.1.26
  double t = 1.0 / (1.0 + p * x);
  double y =
      1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * exp(-x * x);

  return 0.5 * (1.0 + sign * y);
}
