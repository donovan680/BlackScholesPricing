#pragma once

#include <QtWidgets/QWidget>

#include "Debug/uic/ui_OptionsPricing.h"

class OptionsPricing : public QWidget {
	Q_OBJECT

public:
	OptionsPricing(QWidget* parent = Q_NULLPTR);
	void setS(double S) { underlyingAssetPriceS = S; }
	void setK(double K) { strikePriceK = K; }
	void setR(double r) { riskFreeR = r; }
	void setT(double t) { timeT = t; }
	void setO(double o) { volatilyo = o; }
	double getCall() const;
	double getPut() const;
	double getRiskTime(double volatily, double timeT);
public slots:
	void calculate(); // calculate call & put
private slots:

	void clear();     // clear forms

private:
	Ui::OptionsPricingClass ui;
	double underlyingAssetPriceS, strikePriceK, riskFreeR, timeT, volatilyo,
		riskTime; // vars to be retrieved from the UI
	double d1, d2; // vars to be calculated
	double call, put; // results to be displayed
	void handleInput();
	void updateLCDError();
	double calculateD1() const;
	double calculateD2() const;
};
