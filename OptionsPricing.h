#pragma once

#include <QtWidgets/QWidget>

#include "Debug/uic/ui_OptionsPricing.h"

class OptionsPricing : public QWidget {
	Q_OBJECT

public:
	OptionsPricing(QWidget* parent = Q_NULLPTR);
private slots:
	void calculate(); // calculate call & put
	void clear();     // clear forms

private:
	Ui::OptionsPricingClass ui;
	double underlyingAssetPriceS, strikePriceK, riskFreeR, timeT, volatilyo,
		riskTime; // vars to be retrieved from the UI
	double d1, d2; // vars to be calculated
	void handleInput();
	void updateLCDError();
	double getRiskTime(double volatily, double timeT);
	double calculateD1() const;
	double calculateD2() const;
};
